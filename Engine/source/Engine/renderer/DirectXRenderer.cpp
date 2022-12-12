// Copyright 2022 Micho Todorovich, all rights reserved.
module;

#include <windows.h>
#include <wrl.h>
#include <combaseapi.h>
#include <assert.h>
#include <d3d12.h>
#include <d3dcompiler.h>
#include <dxgi.h>
#include <DirectXMath.h>
#include <DirectXColors.h>
#include "d3dx12.h"

module DirectXRenderer;

import Engine;
import DirectXUtility;

import std.core;
import std.filesystem;	

using Microsoft::WRL::ComPtr;
using namespace mt::renderer;

void DirectXRenderer::render()
{
	_is_rendering = true;

	_createCommandList();

	// Add the command list to the queue for execution.
	ID3D12CommandList* cmdsLists[] = { _dx_command_list.Get() };
	_dx_command_queue->ExecuteCommandLists(_countof(cmdsLists), cmdsLists);

	// swap the back and front buffers_
	ThrowIfFailed(
		_dx_swap_chain->Present(0, 0),
		__FUNCTION__,
		__FILE__,
		__LINE__
	);
	_current_back_buffer = (_current_back_buffer + 1) % _swap_chain_buffer_count;

	// Wait until frame commands are complete.  This waiting is inefficient and is
	// done for simplicity.  Later we will show how to organize our rendering code
	// so we do not have to wait per frame.
	//flush_command_queue();
	
	// NOTE: Moved this to happen outside of the render as it ends up syncing the render time_manager_
	// to the framerate. Instead I moved it to flush just before the idle is reset, this should
	// force the game to actually stay synced with the gpu without screwing up the stats. Given
	// the note above I imagine a real fix is incoming.

	_is_rendering = false;

	_frames_rendered++;
}

bool DirectXRenderer::initializeDirect3d(HWND main_window_handle)
{
	_main_window_handle = main_window_handle;

#if defined(DEBUG) || defined(_DEBUG) 
	// Enable the D3D12 debug layer.
	ComPtr<ID3D12Debug> debugController;

	ThrowIfFailed(
		D3D12GetDebugInterface(IID_PPV_ARGS(&debugController)),
		__FUNCTION__,
		__FILE__,
		__LINE__
	);

	debugController->EnableDebugLayer();
#endif

	// Create DirectX Graphics Infrastructure 1.1 factory that you can use to generate other DXGI objects
	ThrowIfFailed(
		CreateDXGIFactory1(IID_PPV_ARGS(&_dx_dxgi_factory)),
		__FUNCTION__,
		__FILE__,
		__LINE__
	);

	// Try to create hardware device.
	// param 1 is the adapter to use, nullptr for default
	HRESULT hardwareResult = D3D12CreateDevice(nullptr,	D3D_FEATURE_LEVEL_11_0,	IID_PPV_ARGS(&_dx_device));

	// If we failed to create the D3D12 Device then try to fall back to Windows Advanced Rasterization Platform (WARP)(software) device
	if (FAILED(hardwareResult))
	{
		ComPtr<IDXGIAdapter> pWarpAdapter;
	
		ThrowIfFailed(
			_dx_dxgi_factory->EnumWarpAdapter(IID_PPV_ARGS(&pWarpAdapter)),
			__FUNCTION__,
			__FILE__,
			__LINE__
		);

		ThrowIfFailed(
			D3D12CreateDevice(pWarpAdapter.Get(),	D3D_FEATURE_LEVEL_11_0,	IID_PPV_ARGS(&_dx_device)),
			__FUNCTION__,
			__FILE__,
			__LINE__
		); 
	}

	// create a fence
	ThrowIfFailed(
		_dx_device->CreateFence(0, D3D12_FENCE_FLAG_NONE,	IID_PPV_ARGS(&_fence)),
		__FUNCTION__,
		__FILE__,
		__LINE__
	);

	_rtv_descriptor_size		 = _dx_device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);
	_dsv_descriptor_size		 = _dx_device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_DSV);
	_cbv_srv_uav_descriptor_size = _dx_device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);

	// Check 4X MSAA quality support for our back buffer format.
	// All Direct3D 11 capable devices support 4X MSAA for all render 
	// target formats, so we only need to check quality support.

	D3D12_FEATURE_DATA_MULTISAMPLE_QUALITY_LEVELS msQualityLevels;

	msQualityLevels.Format			 = _back_buffer_format;
	msQualityLevels.SampleCount		 = 4;
	msQualityLevels.Flags			 = D3D12_MULTISAMPLE_QUALITY_LEVELS_FLAG_NONE;
	msQualityLevels.NumQualityLevels = 0;
	
	ThrowIfFailed(
		_dx_device->CheckFeatureSupport(
			D3D12_FEATURE_MULTISAMPLE_QUALITY_LEVELS,
			&msQualityLevels,
			sizeof(msQualityLevels)
		),
		__FUNCTION__,
		__FILE__,
		__LINE__
	);

	_4x_msaa_quality = msQualityLevels.NumQualityLevels;
	
	assert(_4x_msaa_quality > 0 && "Unexpected MSAA quality level.");

#ifdef _DEBUG
	// This is just annoying, but could be useful in the future when logs are better.
	//log_adapters();
#endif

	_createDxCommandObjects();
	
	_createSwapChain();
	
	_createDescriptorHeaps();

	// Reset the command list to prep for initialization commands.
	ThrowIfFailed(
		_dx_command_list->Reset(_dx_command_list_allocator.Get(), nullptr),
		__FUNCTION__,
		__FILE__,
		__LINE__
	);

	_createConstantBuffers();
	
	_createRootSignature();
	
	_createShadersAndInputLayout();
	
	_createBoxGeometry();
	
	_createPipelineStateObject();

	// Execute the initialization commands.
	ThrowIfFailed(
		_dx_command_list->Close(),
		__FUNCTION__,
		__FILE__,
		__LINE__
	);

	ID3D12CommandList* cmdsLists[] = { _dx_command_list.Get() };
	_dx_command_queue->ExecuteCommandLists(_countof(cmdsLists), cmdsLists);

	// Wait until initialization is complete.
	flush_command_queue();

	_is_initialized = true;

	return true;
}

void DirectXRenderer::_createCommandList()
{
	ThrowIfFailed(
		_dx_command_list_allocator->Reset(),
		__FUNCTION__,
		__FILE__,
		__LINE__
	);

	// A command list can be reset after it has been added to the command queue via ExecuteCommandList.
	// Reusing the command list reuses memory.
	ThrowIfFailed(
		_dx_command_list->Reset(_dx_command_list_allocator.Get(), _pso.Get()),
		__FUNCTION__,
		__FILE__,
		__LINE__
	);

	_dx_command_list->RSSetViewports(1, &_screen_viewport);
	_dx_command_list->RSSetScissorRects(1, &_scissor_rectangle);

	auto resource_barrier = CD3DX12_RESOURCE_BARRIER::Transition(_getCurrentBackBuffer(),
                                                                 D3D12_RESOURCE_STATE_PRESENT,
                                                                 D3D12_RESOURCE_STATE_RENDER_TARGET);

	// Indicate a state transition on the resource usage.
	_dx_command_list->ResourceBarrier(1, &resource_barrier);

	// Clear the back buffer and depth buffer.
	_dx_command_list->ClearRenderTargetView(_getCurrentBackBufferView(), DirectX::Colors::LightSteelBlue, 0, nullptr);
	_dx_command_list->ClearDepthStencilView(_getDepthStencilView(), D3D12_CLEAR_FLAG_DEPTH | D3D12_CLEAR_FLAG_STENCIL, 1.0f, 0, 0, nullptr);

	auto current_back_buffer_view = _getCurrentBackBufferView();
    auto depth_stencil_view = _getDepthStencilView();
	// Specify the buffers we are going to render to.
	_dx_command_list->OMSetRenderTargets(1, &current_back_buffer_view, true, &depth_stencil_view);

	ID3D12DescriptorHeap* descriptorHeaps[] = { _dx_cbv_heap.Get() };
	_dx_command_list->SetDescriptorHeaps(_countof(descriptorHeaps), descriptorHeaps);

	_dx_command_list->SetGraphicsRootSignature(_dx_root_signature.Get());


	auto vertex_buffer_view = _box_mesh_geometry->vertexBufferView();
	_dx_command_list->IASetVertexBuffers(0, 1, &vertex_buffer_view);

    auto index_buffer_view = _box_mesh_geometry->indexBufferView();
	_dx_command_list->IASetIndexBuffer(&index_buffer_view);
	_dx_command_list->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	_dx_command_list->SetGraphicsRootDescriptorTable(0, _dx_cbv_heap->GetGPUDescriptorHandleForHeapStart());

	_dx_command_list->DrawIndexedInstanced(
		_box_mesh_geometry->draw_arguments["box"].index_count,
		1, 0, 0, 0);

	resource_barrier = CD3DX12_RESOURCE_BARRIER::Transition(_getCurrentBackBuffer(),
	                                                        D3D12_RESOURCE_STATE_RENDER_TARGET,
	                                                        D3D12_RESOURCE_STATE_PRESENT);
	// Indicate a state transition on the resource usage.
	_dx_command_list->ResourceBarrier(1, &resource_barrier);

	// Done recording commands.
	ThrowIfFailed(
		_dx_command_list->Close(),
		__FUNCTION__,
		__FILE__,
		__LINE__
	);
}

void DirectXRenderer::_createDxCommandObjects()
{
	D3D12_COMMAND_QUEUE_DESC command_queue_description = {};
	command_queue_description.Type = D3D12_COMMAND_LIST_TYPE_DIRECT;
	command_queue_description.Flags = D3D12_COMMAND_QUEUE_FLAG_NONE;

	// Create a command queue
	ThrowIfFailed(
		_dx_device->CreateCommandQueue(&command_queue_description, IID_PPV_ARGS(&_dx_command_queue)),
		__FUNCTION__,
		__FILE__,
		__LINE__
	);

	// Create a command allocator
	ThrowIfFailed(
		_dx_device->CreateCommandAllocator(
			D3D12_COMMAND_LIST_TYPE_DIRECT,
			IID_PPV_ARGS(_dx_command_list_allocator.GetAddressOf())
		),
		__FUNCTION__,
		__FILE__,
		__LINE__
	);

	// Create a command list
	ThrowIfFailed(
		_dx_device->CreateCommandList(
			0,
			D3D12_COMMAND_LIST_TYPE_DIRECT,
			_dx_command_list_allocator.Get(),				// Associated command allocator
			nullptr,										// Initial PipelineStateObject
			IID_PPV_ARGS(_dx_command_list.GetAddressOf())
		),
		__FUNCTION__,
		__FILE__,
		__LINE__
	);

	// Start off in a closed state.  This is because the first time_manager_ we refer
	// to the command list we will Reset it, and it needs to be closed before
	// calling Reset.
	_dx_command_list->Close();
}

void DirectXRenderer::_createSwapChain()
{
	// Releases all references for the pointer to the interface that is associated with this ComPtr.
	// Release the previous swapchain we will be recreating.
	_dx_swap_chain.Reset();

	DXGI_SWAP_CHAIN_DESC swap_chain_description;
	swap_chain_description.BufferDesc.Width						= _window_width;
	swap_chain_description.BufferDesc.Height					= _window_height;
	swap_chain_description.BufferDesc.RefreshRate.Numerator		= 60;
	swap_chain_description.BufferDesc.RefreshRate.Denominator	= 1;
	swap_chain_description.BufferDesc.Format					= _back_buffer_format;
	swap_chain_description.BufferDesc.ScanlineOrdering			= DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	swap_chain_description.BufferDesc.Scaling					= DXGI_MODE_SCALING_UNSPECIFIED;
	swap_chain_description.SampleDesc.Count						= _4x_msaa_state ? 4 : 1;
	swap_chain_description.SampleDesc.Quality					= _4x_msaa_state ? (_4x_msaa_quality - 1) : 0;
	swap_chain_description.BufferUsage							= DXGI_USAGE_RENDER_TARGET_OUTPUT;
	swap_chain_description.BufferCount							= _swap_chain_buffer_count;
	swap_chain_description.OutputWindow							= _main_window_handle;
	swap_chain_description.Windowed								= true;
	swap_chain_description.SwapEffect							= DXGI_SWAP_EFFECT_FLIP_DISCARD;
	swap_chain_description.Flags								= DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;

	// Note: Swap chain uses queue to perform flush.
	ThrowIfFailed(
		_dx_dxgi_factory->CreateSwapChain(
			_dx_command_queue.Get(),
			&swap_chain_description,
			_dx_swap_chain.GetAddressOf()
		),
		__FUNCTION__,
		__FILE__,
		__LINE__
	);
}

void DirectXRenderer::_createDescriptorHeaps()
{
	// Create the Render-Target-View (RTV) Descriptor-Heap Description
	D3D12_DESCRIPTOR_HEAP_DESC rtv_heap_description;
	rtv_heap_description.NumDescriptors = getSwapChainBufferCount();
	rtv_heap_description.Type			= D3D12_DESCRIPTOR_HEAP_TYPE_RTV;
	rtv_heap_description.Flags			= D3D12_DESCRIPTOR_HEAP_FLAG_NONE;
	rtv_heap_description.NodeMask		= 0;

	// Create the Render-Target-View (RTV) Descriptor-Heap from the provided description
	ThrowIfFailed(
		_dx_device->CreateDescriptorHeap(&rtv_heap_description, IID_PPV_ARGS(_dx_rtv_heap.GetAddressOf())),
		__FUNCTION__,
		__FILE__,
		__LINE__
	);

	// Create the Depth-Stencil-View (DSV) Descriptor-Heap Description
	D3D12_DESCRIPTOR_HEAP_DESC dsv_heap_description;
	dsv_heap_description.NumDescriptors		= 1;
	dsv_heap_description.Type				= D3D12_DESCRIPTOR_HEAP_TYPE_DSV;
	dsv_heap_description.Flags				= D3D12_DESCRIPTOR_HEAP_FLAG_NONE;
	dsv_heap_description.NodeMask			= 0;

	// Create the Depth-Stencil-View (DSV) Descriptor-Heap from the provided description
	ThrowIfFailed(
		_dx_device->CreateDescriptorHeap(&dsv_heap_description, IID_PPV_ARGS(_dx_dsv_heap.GetAddressOf())),
		__FUNCTION__,
		__FILE__,
		__LINE__
	);

	// Create Constant-Buffer-View/Shader-Resource-View/Unordered-Access-View Descriptor-Heap Description
	D3D12_DESCRIPTOR_HEAP_DESC cbv_heap_description;
	cbv_heap_description.NumDescriptors	= 1;
	cbv_heap_description.Type			= D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
	cbv_heap_description.Flags			= D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
	cbv_heap_description.NodeMask		= 0;

	// Create other CBV/SRV/UAV Descriptor-Heap from the descriptor
	ThrowIfFailed(
		_dx_device->CreateDescriptorHeap(&cbv_heap_description,IID_PPV_ARGS(&_dx_cbv_heap)),
		__FUNCTION__,
		__FILE__,
		__LINE__
	);

}

void DirectXRenderer::_createConstantBuffers()
{
	_object_constants_upload_buffer = std::make_unique<UploadBuffer<ObjectConstants>>(_dx_device.Get(), 1, true);

	//constexpr UINT object_constant_buffer_size_bytes = CalcConstantBufferByteSize(sizeof(ObjectConstants));

	D3D12_GPU_VIRTUAL_ADDRESS constant_buffer_address = _object_constants_upload_buffer->Resource()->GetGPUVirtualAddress();
	// Offset to the ith object constant buffer in the buffer.
	// int boxCBufIndex = 0;
	// cbAddress += boxCBufIndex*object_constant_buffer_size_bytes;

	D3D12_CONSTANT_BUFFER_VIEW_DESC cbv_description;
	cbv_description.BufferLocation	= constant_buffer_address;
	cbv_description.SizeInBytes		= CalcConstantBufferByteSize(sizeof(ObjectConstants));

	_dx_device->CreateConstantBufferView(
		&cbv_description,
		_dx_cbv_heap->GetCPUDescriptorHandleForHeapStart()
	);
}

void DirectXRenderer::_createRootSignature()
{
	// Shader programs typically require resources as input (constant buffers,
	// textures, samplers).  The root signature defines the resources the shader
	// programs expect.  If we think of the shader programs as a function, and
	// the input resources as function parameters, then the root signature can be
	// thought of as defining the function signature.  

	// Root parameter can be a table, root descriptor or root constants.
	CD3DX12_ROOT_PARAMETER slotRootParameter[1]{};

	// Create a single descriptor table of CBVs.
	CD3DX12_DESCRIPTOR_RANGE cbvTable;
	cbvTable.Init(D3D12_DESCRIPTOR_RANGE_TYPE_CBV, 1, 0);
	slotRootParameter[0].InitAsDescriptorTable(1, &cbvTable);

	// A root signature is an array of root parameters.
	CD3DX12_ROOT_SIGNATURE_DESC rootSigDesc(1, slotRootParameter, 0, nullptr,
		D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT);

	// create a root signature with a single slot which points to a descriptor range consisting of a single constant buffer
	ComPtr<ID3DBlob> serializedRootSig	= nullptr;
	ComPtr<ID3DBlob> errorBlob			= nullptr;
	HRESULT hr = D3D12SerializeRootSignature(&rootSigDesc, D3D_ROOT_SIGNATURE_VERSION_1,
		serializedRootSig.GetAddressOf(), errorBlob.GetAddressOf());

	if (errorBlob != nullptr)
	{
		::OutputDebugStringA((char*)errorBlob->GetBufferPointer());
	}

	ThrowIfFailed(hr, __FUNCTION__, __FILE__, __LINE__);

	ThrowIfFailed(
		_dx_device->CreateRootSignature(
			0,
			serializedRootSig->GetBufferPointer(),
			serializedRootSig->GetBufferSize(),
			IID_PPV_ARGS(&_dx_root_signature)
		),
		__FUNCTION__,
		__FILE__,
		__LINE__
	);
}

void DirectXRenderer::_createShadersAndInputLayout()
{
	namespace fs = std::filesystem;

	//HRESULT hr = S_OK;

	OutputDebugStringW(std::wstring(L"Current Path: " + fs::current_path().wstring() + L'\n' + fs::current_path().parent_path().root_path().wstring() + L'\n').c_str());

	// TODO: this is garbage, make this better.
	fs::path p;
	for (auto& e : fs::current_path())
	{
		p /= e;

		if (wcsncmp(e.c_str(), L"mt_engine_20", sizeof(L"mt_engine_20")) == 0)
		{
			break;
		}
	}

	OutputDebugStringW((L"P = " + p.wstring()).c_str());

	_mvs_byte_code = mt::renderer::CompileShader(p.wstring() + L"\\Engine\\source\\Shaders\\color.hlsl", nullptr, "VS", "vs_5_0");
	_mps_byte_code = mt::renderer::CompileShader(p.wstring() + L"\\Engine\\source\\Shaders\\color.hlsl", nullptr, "PS", "ps_5_0");

	// D3D_INPUT_ELEMENT_DESC
	mInputLayout =
	{
		// Semantic Name,									Input Slot,											Instanced Data Step Rate						 
		//              Semantic Index,							Aligned Byte Offset,
		//					Format,									Input Slot Class, 
		{ "POSITION",	0,	DXGI_FORMAT_R32G32B32_FLOAT,	0,	0,	D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA,	0 },
		{ "COLOR",		0,	DXGI_FORMAT_R32G32B32A32_FLOAT,	0,	12,	D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA,	0 }
	};
}

void DirectXRenderer::_createBoxGeometry()
{
	std::array<Vertex, 8> vertices
	{
		Vertex({ DirectX::XMFLOAT3(-1.0f, -1.0f, -1.0f), DirectX::XMFLOAT4(DirectX::Colors::White) }),
		Vertex({ DirectX::XMFLOAT3(-1.0f, +1.0f, -1.0f), DirectX::XMFLOAT4(DirectX::Colors::Black) }),
		Vertex({ DirectX::XMFLOAT3(+1.0f, +1.0f, -1.0f), DirectX::XMFLOAT4(DirectX::Colors::Red) }),
		Vertex({ DirectX::XMFLOAT3(+1.0f, -1.0f, -1.0f), DirectX::XMFLOAT4(DirectX::Colors::Green) }),
		Vertex({ DirectX::XMFLOAT3(-1.0f, -1.0f, +1.0f), DirectX::XMFLOAT4(DirectX::Colors::Blue) }),
		Vertex({ DirectX::XMFLOAT3(-1.0f, +1.0f, +1.0f), DirectX::XMFLOAT4(DirectX::Colors::Yellow) }),
		Vertex({ DirectX::XMFLOAT3(+1.0f, +1.0f, +1.0f), DirectX::XMFLOAT4(DirectX::Colors::Cyan) }),
		Vertex({ DirectX::XMFLOAT3(+1.0f, -1.0f, +1.0f), DirectX::XMFLOAT4(DirectX::Colors::Magenta) })
	};

	std::array<std::uint16_t, 36> indices =
	{
		// front face
		0, 1, 2,
		0, 2, 3,

		// back face
		4, 6, 5,
		4, 7, 6,

		// left face
		4, 5, 1,
		4, 1, 0,

		// right face
		3, 2, 6,
		3, 6, 7,

		// top face
		1, 5, 6,
		1, 6, 2,

		// bottom face
		4, 0, 3,
		4, 3, 7
	};

	const UINT vbByteSize = (UINT)vertices.size() * sizeof(Vertex);
	const UINT ibByteSize = (UINT)indices.size() * sizeof(std::uint16_t);

	_box_mesh_geometry = std::make_unique<MeshGeometry>();
	_box_mesh_geometry->name = "boxGeo";

	ThrowIfFailed(
		D3DCreateBlob(vbByteSize, &_box_mesh_geometry->vertex_buffer_cpu),
		__FUNCTION__,
		__FILE__,
		__LINE__
	);
	CopyMemory(_box_mesh_geometry->vertex_buffer_cpu->GetBufferPointer(), vertices.data(), vbByteSize);

	ThrowIfFailed(
		D3DCreateBlob(ibByteSize, &_box_mesh_geometry->index_buffer_cpu),
		__FUNCTION__,
		__FILE__,
		__LINE__
	);
	CopyMemory(_box_mesh_geometry->index_buffer_cpu->GetBufferPointer(), indices.data(), ibByteSize);

	_box_mesh_geometry->vertex_buffer_gpu = CreateDefaultBuffer(
		_dx_device.Get(),
		_dx_command_list.Get(), 
		vertices.data(), 
		vbByteSize, 
		_box_mesh_geometry->vertex_buffer_uploader
	);

	_box_mesh_geometry->index_buffer_gpu = CreateDefaultBuffer(
		_dx_device.Get(),
		_dx_command_list.Get(),
		indices.data(),
		ibByteSize, 
		_box_mesh_geometry->index_buffer_uploader
	);

	_box_mesh_geometry->vertex_byte_stride = sizeof(Vertex);
	_box_mesh_geometry->vertex_buffer_byte_size = vbByteSize;
	_box_mesh_geometry->index_format = DXGI_FORMAT_R16_UINT;
	_box_mesh_geometry->index_buffer_byte_size = ibByteSize;

	SubmeshGeometry submesh;
	submesh.index_count = (UINT)indices.size();
	submesh.start_index_location = 0;
	submesh.base_vertex_location = 0;

	_box_mesh_geometry->draw_arguments["box"] = submesh;
}

void DirectXRenderer::_createPipelineStateObject()
{
	D3D12_GRAPHICS_PIPELINE_STATE_DESC psoDesc;
	ZeroMemory(&psoDesc, sizeof(D3D12_GRAPHICS_PIPELINE_STATE_DESC));
	psoDesc.InputLayout = { mInputLayout.data(), (UINT)mInputLayout.size() };
	psoDesc.pRootSignature = _dx_root_signature.Get();
	psoDesc.VS =
	{
		reinterpret_cast<BYTE*>(_mvs_byte_code->GetBufferPointer()),
		_mvs_byte_code->GetBufferSize()
	};
	psoDesc.PS =
	{
		reinterpret_cast<BYTE*>(_mps_byte_code->GetBufferPointer()),
		_mps_byte_code->GetBufferSize()
	};
	psoDesc.RasterizerState = CD3DX12_RASTERIZER_DESC(D3D12_DEFAULT);
	psoDesc.BlendState = CD3DX12_BLEND_DESC(D3D12_DEFAULT);
	psoDesc.DepthStencilState = CD3DX12_DEPTH_STENCIL_DESC(D3D12_DEFAULT);
	psoDesc.SampleMask = UINT_MAX;
	psoDesc.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;
	psoDesc.NumRenderTargets = 1;
	psoDesc.RTVFormats[0] = _back_buffer_format;
	psoDesc.SampleDesc.Count = get4xMsaaState() ? 4 : 1;
	psoDesc.SampleDesc.Quality = get4xMsaaState() ? (_4x_msaa_quality - 1) : 0;
	psoDesc.DSVFormat = _depth_stencil_format;
	
	ThrowIfFailed(
		_dx_device->CreateGraphicsPipelineState(&psoDesc, IID_PPV_ARGS(&_pso)),
		__FUNCTION__,
		__FILE__,
		__LINE__
	);
}

void DirectXRenderer::flush_command_queue()
{
	incrementFence();

	waitForFence();
}

void DirectXRenderer::incrementFence()
{
	// Advance the fence value to mark commands up to this fence point.
	_current_fence_index++;

	// Add an instruction to the command queue to set a new fence point.  Because we 
	// are on the GPU timeline, the new fence point won't be set until the GPU finishes
	// processing all the commands prior to this Signal().
	ThrowIfFailed(
		_dx_command_queue->Signal(_fence.Get(), _current_fence_index),
		__FUNCTION__,
		__FILE__,
		__LINE__
	);
}

// TODO: Can I not wait and do other things instead?
void DirectXRenderer::waitForFence()
{
	// Wait until the GPU has completed commands up to this fence point.
	if (_fence->GetCompletedValue() < _current_fence_index)
	{
		// second parameter was false, compiler claims its being converted to nullptr so i made it explicit.
		HANDLE eventHandle = CreateEventEx(nullptr, nullptr, false, EVENT_ALL_ACCESS);

		// Fire event when GPU hits current fence.  
		ThrowIfFailed(
			_fence->SetEventOnCompletion(_current_fence_index, eventHandle),
			__FUNCTION__,
			__FILE__,
			__LINE__
		);

		// Wait until the GPU hits current fence event is fired.
		WaitForSingleObject(eventHandle, INFINITE);

		CloseHandle(eventHandle);
	}
}

ID3D12Resource* DirectXRenderer::_getCurrentBackBuffer() const
{
	return _swap_chain_buffer[_current_back_buffer].Get();
}

D3D12_CPU_DESCRIPTOR_HANDLE DirectXRenderer::_getCurrentBackBufferView() const
{
	return CD3DX12_CPU_DESCRIPTOR_HANDLE(
		_dx_rtv_heap->GetCPUDescriptorHandleForHeapStart(),
		_current_back_buffer,
		_rtv_descriptor_size);
}

D3D12_CPU_DESCRIPTOR_HANDLE DirectXRenderer::_getDepthStencilView() const
{
	return _dx_dsv_heap->GetCPUDescriptorHandleForHeapStart();
}

void DirectXRenderer::_logAdapters()
{
	IDXGIAdapter* adapter = nullptr;
	std::vector<IDXGIAdapter*> adapterList;
	for (UINT i = 0; _dx_dxgi_factory->EnumAdapters(i, &adapter) != DXGI_ERROR_NOT_FOUND; ++i)
	{
		DXGI_ADAPTER_DESC desc;
		adapter->GetDesc(&desc);

		std::wstring text = L"***Adapter: ";
		text += desc.Description;
		text += L"\n";

		OutputDebugStringW(text.c_str());

		adapterList.push_back(adapter);
	}

	for (size_t i = 0; i < adapterList.size(); ++i)
	{
		_logAdapterOutputs(adapterList[i]);
		adapterList[i]->Release();
		adapterList[i] = nullptr;
	}
}

void DirectXRenderer::_logAdapterOutputs(IDXGIAdapter* adapter)
{
	UINT i = 0;
	IDXGIOutput* output = nullptr;
	while (adapter->EnumOutputs(i, &output) != DXGI_ERROR_NOT_FOUND)
	{
		DXGI_OUTPUT_DESC desc;
		output->GetDesc(&desc);

		std::wstring text = L"***Output: ";
		text += desc.DeviceName;
		text += L"\n";
		OutputDebugStringW(text.c_str());

		_logOutputDisplayModes(output, _back_buffer_format);

		output->Release();
		output = nullptr;

		++i;
	}
}

void DirectXRenderer::_logOutputDisplayModes(IDXGIOutput* output, DXGI_FORMAT format)
{
	UINT count = 0;
	UINT flags = 0;

	// Call with nullptr to get list count.
	output->GetDisplayModeList(format, flags, &count, nullptr);

	std::vector<DXGI_MODE_DESC> modeList(count);
	output->GetDisplayModeList(format, flags, &count, &modeList[0]);

	for (auto& x : modeList)
	{
		UINT n = x.RefreshRate.Numerator;
		UINT d = x.RefreshRate.Denominator;
		std::wstring text =
			L"Width = " + std::to_wstring(x.Width) + L" " +
			L"Height = " + std::to_wstring(x.Height) + L" " +
			L"Refresh = " + std::to_wstring(n) + L"/" + std::to_wstring(d) +
			L"\n";

		::OutputDebugStringW(text.c_str());
	}
}

void DirectXRenderer::resize(int client_width, int client_height)
{
	if (client_width != _window_width || client_height != _window_height)
	{
		_window_width = client_width;
		_window_height = client_height;
		_window_aspect_ratio = static_cast<float>(_window_width) / _window_height;

		if (_dx_device) 
		{
			assert(_dx_device);
			assert(_dx_swap_chain);
			assert(_dx_command_list_allocator);

			// Flush before changing any resources.
			flush_command_queue();

			ThrowIfFailed(
				_dx_command_list->Reset(_dx_command_list_allocator.Get(), nullptr),
				__FUNCTION__,
				__FILE__,
				__LINE__
			);

			// Release the previous resources we will be recreating.
			for (int i = 0; i < _swap_chain_buffer_count; ++i)
				_swap_chain_buffer[i].Reset();
			_depth_stencil_buffer.Reset();

			// Resize the swap chain.
			ThrowIfFailed(
				_dx_swap_chain->ResizeBuffers(
					_swap_chain_buffer_count,
					_window_width, _window_height,
					_back_buffer_format,
					DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH
				),
				__FUNCTION__,
				__FILE__,
				__LINE__
			);

			_current_back_buffer = 0;

			CD3DX12_CPU_DESCRIPTOR_HANDLE rtvHeapHandle(_dx_rtv_heap->GetCPUDescriptorHandleForHeapStart());
			for (UINT i = 0; i < _swap_chain_buffer_count; i++)
			{
				ThrowIfFailed(
					_dx_swap_chain->GetBuffer(i, IID_PPV_ARGS(&_swap_chain_buffer[i])),
					__FUNCTION__,
					__FILE__,
					__LINE__
				);

				_dx_device->CreateRenderTargetView(_swap_chain_buffer[i].Get(), nullptr, rtvHeapHandle);
				rtvHeapHandle.Offset(1, _rtv_descriptor_size);
			}

			// Create the depth/stencil buffer and view.
			D3D12_RESOURCE_DESC depthStencilDesc;
			depthStencilDesc.Dimension = D3D12_RESOURCE_DIMENSION_TEXTURE2D;
			depthStencilDesc.Alignment = 0;
			depthStencilDesc.Width = _window_width;
			depthStencilDesc.Height = _window_height;
			depthStencilDesc.DepthOrArraySize = 1;
			depthStencilDesc.MipLevels = 1;
			depthStencilDesc.Format = _depth_stencil_format;
			depthStencilDesc.SampleDesc.Count = _4x_msaa_state ? 4 : 1;
			depthStencilDesc.SampleDesc.Quality = _4x_msaa_state ? (_4x_msaa_quality - 1) : 0;
			depthStencilDesc.Layout = D3D12_TEXTURE_LAYOUT_UNKNOWN;
			depthStencilDesc.Flags = D3D12_RESOURCE_FLAG_ALLOW_DEPTH_STENCIL;

			D3D12_CLEAR_VALUE optClear;
			optClear.Format = _depth_stencil_format;
			optClear.DepthStencil.Depth = 1.0f;
			optClear.DepthStencil.Stencil = 0;

			auto heap_properties = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_DEFAULT);

			ThrowIfFailed(
				_dx_device->CreateCommittedResource(
					&heap_properties,
					D3D12_HEAP_FLAG_NONE,
					&depthStencilDesc,
					D3D12_RESOURCE_STATE_COMMON,
					&optClear,
					IID_PPV_ARGS(_depth_stencil_buffer.GetAddressOf())
				),
				__FUNCTION__,
				__FILE__,
				__LINE__
			);

			// Create descriptor to mip level 0 of entire resource using the format of the resource.
			_dx_device->CreateDepthStencilView(_depth_stencil_buffer.Get(), nullptr, _getDepthStencilView());


			auto resource_barrier = CD3DX12_RESOURCE_BARRIER::Transition(_depth_stencil_buffer.Get(),
				D3D12_RESOURCE_STATE_COMMON,
				D3D12_RESOURCE_STATE_DEPTH_WRITE);

			// Transition the resource from its initial state to be used as a depth buffer.
			_dx_command_list->ResourceBarrier(1, &resource_barrier);

			// Execute the Resize commands.
			ThrowIfFailed(
				_dx_command_list->Close(),
				__FUNCTION__,
				__FILE__,
				__LINE__
			);

			ID3D12CommandList* cmdsLists[] = { _dx_command_list.Get() };
			_dx_command_queue->ExecuteCommandLists(_countof(cmdsLists), cmdsLists);

			// Wait until Resize is complete.
			flush_command_queue();

			// Update the viewport transform to cover the client area.
			_screen_viewport.TopLeftX = 0;
			_screen_viewport.TopLeftY = 0;
			_screen_viewport.Width = static_cast<float>(_window_width);
			_screen_viewport.Height = static_cast<float>(_window_height);
			_screen_viewport.MinDepth = 0.0f;
			_screen_viewport.MaxDepth = 1.0f;

			_scissor_rectangle = { 0, 0, _window_width, _window_height };

			_window_aspect_ratio = static_cast<float>(_window_width) / _window_height;

			// The window resized, so update the aspect ratio and recompute the projection matrix.
			getCurrentCamera().setLens(0.25f * MathHelper::Pi, getWindowAspectRatio(), 1.0f, 1000.0f);
			//XMStoreFloat4x4(&_projection_transform, P);
		}
	}
}

void DirectXRenderer::set4xMsaaState(bool value)
{
	if (_4x_msaa_state != value)
	{
		_4x_msaa_state = value;

		// Recreate the swapchain and buffers with new multisample settings.
		_createSwapChain();
		resize(_window_width, _window_height);
	}
}

void DirectXRenderer::update()
{
	_camera.updateViewMatrix();

	DirectX::XMMATRIX world_view_projection = _camera.getViewMatrix() * _camera.getProjectionMatrix();

	// Update the constant buffer with the latest worldViewProj matrix.
	ObjectConstants object_constants;
	XMStoreFloat4x4(&object_constants.world_view_projection, XMMatrixTranspose(world_view_projection));
	_object_constants_upload_buffer->CopyData(0, object_constants);

}
