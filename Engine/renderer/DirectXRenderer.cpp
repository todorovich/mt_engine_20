// Copyright 2022 Micho Todorovich, all rights reserved.
module;

#include <windows.h>
#include <windowsx.h>
#include <wrl.h>
#include <combaseapi.h>
#include <assert.h>

#include <d3d12.h>
#include <dxgi.h>
#include <dxgi1_4.h>
#include <DirectXCollision.h>
#include <DirectXMath.h>
#include <DirectXColors.h>
#include <d3dcompiler.h>

#include "d3dx12.h"

module DirectXRenderer;

import <array>;
import <filesystem>;
import <numbers>;

import Engine;
import FrameResource;
import DirectXUtility;
import WindowManager;
import MeshGeometry;

using namespace mt::renderer;
using namespace std::numbers;
using Microsoft::WRL::ComPtr;

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
			assert(_frame_resources[_frame_resource_index]->command_list_allocator);

			// Flush before changing any resources.
			_flushCommandQueue();

			throwIfFailed(
				_dx_command_list->Reset(_frame_resources[_frame_resource_index]->command_list_allocator.Get(), nullptr),
				__FUNCTION__, __FILE__, __LINE__
			);

			// Release the previous resources we will be recreating.
			for (int i = 0; i < _swap_chain_buffer_count; ++i)
			{
				_swap_chain_buffer[i].Reset();
			}
			_depth_stencil_buffer.Reset();

			// Resize the swap chain.
			throwIfFailed(
				_dx_swap_chain->ResizeBuffers(
					_swap_chain_buffer_count,
					_window_width, _window_height,
					_back_buffer_format,
					DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH
				),
				__FUNCTION__, __FILE__, __LINE__
			);

			_current_back_buffer = 0;

			CD3DX12_CPU_DESCRIPTOR_HANDLE rtvHeapHandle(_dx_rtv_heap->GetCPUDescriptorHandleForHeapStart());
			for (UINT i = 0; i < _swap_chain_buffer_count; i++)
			{
				throwIfFailed(
					_dx_swap_chain->GetBuffer(i, IID_PPV_ARGS(&_swap_chain_buffer[i])),
					__FUNCTION__, __FILE__, __LINE__
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

			throwIfFailed(
				_dx_device->CreateCommittedResource(
					&heap_properties,
					D3D12_HEAP_FLAG_NONE,
					&depthStencilDesc,
					D3D12_RESOURCE_STATE_COMMON,
					&optClear,
					IID_PPV_ARGS(_depth_stencil_buffer.GetAddressOf())
				),
				__FUNCTION__, __FILE__, __LINE__
			);

			// Create descriptor to mip level 0 of entire resource using the format of the resource.
			_dx_device->CreateDepthStencilView(_depth_stencil_buffer.Get(), nullptr, _getDepthStencilView());


			auto resource_barrier = CD3DX12_RESOURCE_BARRIER::Transition(
				_depth_stencil_buffer.Get(),
				D3D12_RESOURCE_STATE_COMMON,
				D3D12_RESOURCE_STATE_DEPTH_WRITE
			);

			// Transition the resource from its initial state to be used as a depth buffer.
			_dx_command_list->ResourceBarrier(1, &resource_barrier);

			// Execute the Resize commands.
			throwIfFailed(
				_dx_command_list->Close(),
				__FUNCTION__, __FILE__, __LINE__
			);

			ID3D12CommandList* command_lists[] = { _dx_command_list.Get() };
			_dx_command_queue->ExecuteCommandLists(_countof(command_lists), command_lists);

			// Wait until Resize is complete.
			_flushCommandQueue();

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
			getCurrentCamera().setLens(0.25f * pi_v<float>, getWindowAspectRatio(), 1.0f, 1000.0f);
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

	if (_frame_resources[_frame_resource_index]->fence != 0 && _fence->GetCompletedValue() < _frame_resources[_frame_resource_index]->fence)
	{
		// second parameter was false, compiler claims its being converted to nullptr so i made it explicit.
		HANDLE eventHandle = CreateEventEx(nullptr, nullptr, false, EVENT_ALL_ACCESS);

		// Fire event when GPU hits current fence.
		throwIfFailed(
			_fence->SetEventOnCompletion(_frame_resources[_frame_resource_index]->fence, eventHandle),
			__FUNCTION__, __FILE__, __LINE__
		);

		// Wait until the GPU hits current fence event is fired.
		WaitForSingleObject(eventHandle, INFINITE);

		CloseHandle(eventHandle);
	}

	// Update the constant buffer with the latest worldViewProj matrix.
	ObjectConstants object_constants;
	// The transpose is necessary because we are switching from row major (DirectXMath) to column major (HLSL)
	XMStoreFloat4x4(&object_constants.world_view_projection, XMMatrixTranspose(world_view_projection));
	_frame_resources[_frame_resource_index]->object_constants_upload_buffer->CopyData(0, object_constants);
}

void mt::renderer::DirectXRenderer::render()
{
	_is_rendering = true;

	_createCommandList();

	// Add the command list to the queue for execution.
	ID3D12CommandList* command_lists[] = { _dx_command_list.Get() };
	_dx_command_queue->ExecuteCommandLists(_countof(command_lists), command_lists);

	// swap the back and front buffers_
	throwIfFailed(
		_dx_swap_chain->Present(0, 0), __FUNCTION__, __FILE__, __LINE__
	);

	_current_back_buffer = (_current_back_buffer + 1) % _swap_chain_buffer_count;

	_frame_resources[_frame_resource_index]->fence = ++_fence_index;

	_dx_command_queue->Signal(_fence.Get(), _frame_resources[_frame_resource_index]->fence);

	_frame_resource_index = (_frame_resource_index + 1) % _number_of_frame_resources;

	_is_rendering = false;

	_frames_rendered++;
}

void DirectXRenderer::_createCommandList()
{
	throwIfFailed(
		_frame_resources[_frame_resource_index]->command_list_allocator->Reset(),
		__FUNCTION__, __FILE__, __LINE__
	);

	// A command list can be reset after it has been added to the command queue via ExecuteCommandList.
	// Reusing the command list reuses memory.
	throwIfFailed(
		_dx_command_list->Reset(_frame_resources[_frame_resource_index]->command_list_allocator.Get(), _pipeline_state_objects[0].Get()),
		__FUNCTION__, __FILE__, __LINE__
	);

	_dx_command_list->RSSetViewports(1, &_screen_viewport);
	_dx_command_list->RSSetScissorRects(1, &_scissor_rectangle);

	auto resource_barrier = CD3DX12_RESOURCE_BARRIER::Transition(
		_getCurrentBackBuffer(),
		D3D12_RESOURCE_STATE_PRESENT,
		D3D12_RESOURCE_STATE_RENDER_TARGET
	);

	// Indicate a state transition on the resource usage.
	_dx_command_list->ResourceBarrier(1, &resource_barrier);

	// Clear the back buffer and depth buffer.
	_dx_command_list->ClearRenderTargetView(_getCurrentBackBufferView(), DirectX::Colors::LightSteelBlue, 0, nullptr);
	_dx_command_list->ClearDepthStencilView(
		_getDepthStencilView(), D3D12_CLEAR_FLAG_DEPTH | D3D12_CLEAR_FLAG_STENCIL, 1.0f, 0, 0, nullptr
	);

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
		1, 0, 0, 0
	);

	resource_barrier = CD3DX12_RESOURCE_BARRIER::Transition(
		_getCurrentBackBuffer(),
		D3D12_RESOURCE_STATE_RENDER_TARGET,
		D3D12_RESOURCE_STATE_PRESENT
	);
	// Indicate a state transition on the resource usage.
	_dx_command_list->ResourceBarrier(1, &resource_barrier);

	// Done recording commands.
	throwIfFailed(
		_dx_command_list->Close(),
		__FUNCTION__, __FILE__, __LINE__
	);
}

// INITIALIZATION CODE

bool DirectXRenderer::initializeDirect3d(HWND main_window_handle)
{
	_main_window_handle = main_window_handle;

#if defined(DEBUG) || defined(_DEBUG)
	// Enable the D3D12 debug layer.
	ComPtr<ID3D12Debug> debugController;

	throwIfFailed(
		D3D12GetDebugInterface(IID_PPV_ARGS(&debugController)), __FUNCTION__, __FILE__, __LINE__
	);

	debugController->EnableDebugLayer();
#endif

	// Create DirectX Graphics Infrastructure 1.1 factory that you can use to generate other DXGI objects
	throwIfFailed(
		CreateDXGIFactory1(IID_PPV_ARGS(&_dx_dxgi_factory)), __FUNCTION__, __FILE__, __LINE__
	);

	// Try to create hardware device.
	// param 1 is the adapter to use, nullptr for default
	HRESULT hardwareResult = D3D12CreateDevice(nullptr, D3D_FEATURE_LEVEL_11_0, IID_PPV_ARGS(&_dx_device));

	// If we failed to create the D3D12 Device then try to fall back to Windows Advanced Rasterization Platform (WARP)(software) device
	if (FAILED(hardwareResult))
	{
		ComPtr<IDXGIAdapter> pWarpAdapter;

		throwIfFailed(
			_dx_dxgi_factory->EnumWarpAdapter(IID_PPV_ARGS(&pWarpAdapter)), __FUNCTION__, __FILE__, __LINE__
		);

		throwIfFailed(
			D3D12CreateDevice(pWarpAdapter.Get(), D3D_FEATURE_LEVEL_11_0, IID_PPV_ARGS(&_dx_device)),
			__FUNCTION__, __FILE__, __LINE__
		);
	}

	// create a fence
	throwIfFailed(
		_dx_device->CreateFence(0, D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(&_fence)), __FUNCTION__, __FILE__, __LINE__
	);

	_rtv_descriptor_size = _dx_device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);
	_dsv_descriptor_size = _dx_device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_DSV);
	_cbv_srv_uav_descriptor_size = _dx_device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);

	// Check 4X MSAA quality support for our back buffer format.
	// All Direct3D 11 capable devices support 4X MSAA for all render
	// target formats, so we only need to check quality support.

	D3D12_FEATURE_DATA_MULTISAMPLE_QUALITY_LEVELS msQualityLevels;

	msQualityLevels.Format = _back_buffer_format;
	msQualityLevels.SampleCount = 4;
	msQualityLevels.Flags = D3D12_MULTISAMPLE_QUALITY_LEVELS_FLAG_NONE;
	msQualityLevels.NumQualityLevels = 0;

	throwIfFailed(
		_dx_device->CheckFeatureSupport(
			D3D12_FEATURE_MULTISAMPLE_QUALITY_LEVELS,
			&msQualityLevels,
			sizeof(msQualityLevels)
		),
		__FUNCTION__, __FILE__, __LINE__
	);

	_4x_msaa_quality = msQualityLevels.NumQualityLevels;

	assert(_4x_msaa_quality > 0 && "Unexpected MSAA quality level.");

	_createFrameResources();

	_createDxCommandObjects();

	_createSwapChain();

	_createDescriptorHeaps();

	// Reset the command list to prep for initialization commands.
	throwIfFailed(
		_dx_command_list->Reset(_frame_resources[_frame_resource_index]->command_list_allocator.Get(), nullptr), __FUNCTION__, __FILE__, __LINE__
	);

	_createRootSignature();

	_createShadersAndInputLayout();

	_createGeometry();

	// RENDER ITEMS

	// ANOTHER DESCRIPTOR HEAP

	_createConstantBufferViews();

	_createPipelineStateObject();

	// Execute the initialization commands.
	throwIfFailed(
		_dx_command_list->Close(), 	__FUNCTION__, __FILE__, __LINE__
	);

	ID3D12CommandList* command_lists[] = { _dx_command_list.Get() };
	_dx_command_queue->ExecuteCommandLists(_countof(command_lists), command_lists);

	// Wait until initialization is complete.
	_flushCommandQueue();

	_is_initialized = true;

	return true;
}

void DirectXRenderer::_flushCommandQueue()
{
	// Advance the fence value to mark commands up to this fence point.
	_fence_index++;

	// Add an instruction to the command queue to set a new fence point.  Because we
	// are on the GPU timeline, the new fence point won't be set until the GPU finishes
	// processing all the commands prior to this Signal().
	throwIfFailed(
		_dx_command_queue->Signal(_fence.Get(), _fence_index),
		__FUNCTION__, __FILE__, __LINE__
	);

	// Wait until the GPU has completed commands up to this fence point.
	if (_fence->GetCompletedValue() < _fence_index)
	{
		// second parameter was false, compiler claims its being converted to nullptr so i made it explicit.
		HANDLE eventHandle = CreateEventEx(nullptr, nullptr, false, EVENT_ALL_ACCESS);

		// Fire event when GPU hits current fence.
		throwIfFailed(
			_fence->SetEventOnCompletion(_fence_index, eventHandle),
			__FUNCTION__, __FILE__, __LINE__
		);

		// Wait until the GPU hits current fence event is fired.
		WaitForSingleObject(eventHandle, INFINITE);

		CloseHandle(eventHandle);
	}
}

void DirectXRenderer::_createDxCommandObjects()
{
	D3D12_COMMAND_QUEUE_DESC command_queue_description = {};
	command_queue_description.Type = D3D12_COMMAND_LIST_TYPE_DIRECT;
	command_queue_description.Flags = D3D12_COMMAND_QUEUE_FLAG_NONE;

	// Create a command queue
	throwIfFailed(
		_dx_device->CreateCommandQueue(&command_queue_description, IID_PPV_ARGS(&_dx_command_queue)),
		__FUNCTION__, __FILE__, __LINE__
	);

	// Create a command allocator
	throwIfFailed(
		_dx_device->CreateCommandAllocator(
			D3D12_COMMAND_LIST_TYPE_DIRECT,
			IID_PPV_ARGS(_frame_resources[_frame_resource_index]->command_list_allocator.GetAddressOf())
		),
		__FUNCTION__, __FILE__, __LINE__
	);

	// Create a command list
	throwIfFailed(
		_dx_device->CreateCommandList(
			0,
			D3D12_COMMAND_LIST_TYPE_DIRECT,
			_frame_resources[_frame_resource_index]->command_list_allocator.Get(),                // Associated command allocator
			nullptr,                                        // Initial PipelineStateObject
			IID_PPV_ARGS(_dx_command_list.GetAddressOf())
		),
		__FUNCTION__, __FILE__, __LINE__
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
	swap_chain_description.BufferDesc.Width = _window_width;
	swap_chain_description.BufferDesc.Height = _window_height;
	swap_chain_description.BufferDesc.RefreshRate.Numerator = 60;
	swap_chain_description.BufferDesc.RefreshRate.Denominator = 1;
	swap_chain_description.BufferDesc.Format = _back_buffer_format;
	swap_chain_description.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	swap_chain_description.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
	swap_chain_description.SampleDesc.Count = _4x_msaa_state ? 4 : 1;
	swap_chain_description.SampleDesc.Quality = _4x_msaa_state ? (_4x_msaa_quality - 1) : 0;
	swap_chain_description.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	swap_chain_description.BufferCount = _swap_chain_buffer_count;
	swap_chain_description.OutputWindow = _main_window_handle;
	swap_chain_description.Windowed = true;
	swap_chain_description.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;
	swap_chain_description.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;

	// Note: Swap chain uses queue to perform flush.
	throwIfFailed(
		_dx_dxgi_factory->CreateSwapChain(
			_dx_command_queue.Get(),
			&swap_chain_description,
			_dx_swap_chain.GetAddressOf()
		),
		__FUNCTION__, __FILE__, __LINE__
	);
}

void DirectXRenderer::_createDescriptorHeaps()
{
	// Create the Render-Target-View (RTV) Descriptor-Heap Description
	D3D12_DESCRIPTOR_HEAP_DESC rtv_heap_description;
	rtv_heap_description.NumDescriptors = getSwapChainBufferCount();
	rtv_heap_description.Type = D3D12_DESCRIPTOR_HEAP_TYPE_RTV;
	rtv_heap_description.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;
	rtv_heap_description.NodeMask = 0;

	// Create the Render-Target-View (RTV) Descriptor-Heap from the provided description
	throwIfFailed(
		_dx_device->CreateDescriptorHeap(&rtv_heap_description, IID_PPV_ARGS(_dx_rtv_heap.GetAddressOf())),
		__FUNCTION__, __FILE__, __LINE__
	);

	// Create the Depth-Stencil-View (DSV) Descriptor-Heap Description
	D3D12_DESCRIPTOR_HEAP_DESC dsv_heap_description;
	dsv_heap_description.NumDescriptors = 1;
	dsv_heap_description.Type = D3D12_DESCRIPTOR_HEAP_TYPE_DSV;
	dsv_heap_description.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;
	dsv_heap_description.NodeMask = 0;

	// Create the Depth-Stencil-View (DSV) Descriptor-Heap from the provided description
	throwIfFailed(
		_dx_device->CreateDescriptorHeap(&dsv_heap_description, IID_PPV_ARGS(_dx_dsv_heap.GetAddressOf())),
		__FUNCTION__, __FILE__, __LINE__
	);

	// Create Constant-Buffer-View/Shader-Resource-View/Unordered-Access-View Descriptor-Heap Description
	D3D12_DESCRIPTOR_HEAP_DESC cbv_heap_description;
	cbv_heap_description.NumDescriptors = 1;
	cbv_heap_description.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
	cbv_heap_description.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
	cbv_heap_description.NodeMask = 0;

	// Create other CBV/SRV/UAV Descriptor-Heap from the descriptor
	throwIfFailed(
		_dx_device->CreateDescriptorHeap(&cbv_heap_description, IID_PPV_ARGS(&_dx_cbv_heap)),
		__FUNCTION__, __FILE__, __LINE__
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
	CD3DX12_ROOT_SIGNATURE_DESC rootSigDesc(
		1, slotRootParameter, 0, nullptr,
		D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT
	);

	// create a root signature with a single slot which points to a descriptor range consisting of a single constant buffer
	ComPtr<ID3DBlob> serializedRootSig = nullptr;
	ComPtr<ID3DBlob> errorBlob = nullptr;
	HRESULT hr = D3D12SerializeRootSignature(
		&rootSigDesc, D3D_ROOT_SIGNATURE_VERSION_1,
		serializedRootSig.GetAddressOf(), errorBlob.GetAddressOf());

	if (errorBlob != nullptr)
	{
		::OutputDebugStringA((char*) errorBlob->GetBufferPointer());
	}

	throwIfFailed(hr, __FUNCTION__, __FILE__, __LINE__);

	throwIfFailed(
		_dx_device->CreateRootSignature(
			0,
			serializedRootSig->GetBufferPointer(),
			serializedRootSig->GetBufferSize(),
			IID_PPV_ARGS(&_dx_root_signature)
		),
		__FUNCTION__, __FILE__, __LINE__
	);
}

void DirectXRenderer::_createShadersAndInputLayout()
{
	namespace fs = std::filesystem;

	//HRESULT hr = S_OK;

	OutputDebugStringW(
		std::wstring(
			L"Current Path: " + fs::current_path().wstring() + L'\n' +
			fs::current_path().parent_path().root_path().wstring() + L'\n'
		).c_str());

	// TODO: this is garbage, make this better.
	fs::path p;
	for (auto& e: fs::current_path())
	{
		p /= e;

		if (wcsncmp(e.c_str(), L"mt_engine_20", sizeof(L"mt_engine_20")) == 0)
		{
			break;
		}
	}

	OutputDebugStringW((L"P = " + p.wstring()).c_str());

	_mvs_byte_code = mt::renderer::CompileShader(p.wstring() + L"\\Shaders\\color.hlsl", nullptr, "VS", "vs_5_0");
	_mps_byte_code = mt::renderer::CompileShader(p.wstring() + L"\\Shaders\\color.hlsl", nullptr, "PS", "ps_5_0");

	// D3D_INPUT_ELEMENT_DESC
	mInputLayout =
		{
			// Semantic Name,									Input Slot,											Instanced Data Step Rate
			//              Semantic Index,							Aligned Byte Offset,
			//					Format,									Input Slot Class,
			{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT,    0, 0,  D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
			{ "COLOR",    0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 12, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 }
		};
}

void DirectXRenderer::_createGeometry()
{
	auto box = mt::geometry::createBoxGeometry(1.0f, 1.0f, 1.0f);

	std::vector<mt::renderer::Vertex> vertices(box.vertices.size());
	for (std::size_t index = 0; index < box.vertices.size(); ++index)
	{
		vertices[index].position = box.vertices[index].position;
		vertices[index].color = box.vertices[index].color;
	}

	const UINT vbByteSize = (UINT) vertices.size() * sizeof(mt::renderer::Vertex);
	const UINT ibByteSize = (UINT) box.indices.size() * sizeof(uint16_t);

	_box_mesh_geometry = std::make_unique<MeshGeometry>();
	_box_mesh_geometry->name = "box";

	throwIfFailed(
		D3DCreateBlob(vbByteSize, &_box_mesh_geometry->vertex_buffer_cpu),
		__FUNCTION__, __FILE__, __LINE__
	);
	memcpy(_box_mesh_geometry->vertex_buffer_cpu->GetBufferPointer(), vertices.data(), vbByteSize);

	throwIfFailed(
		D3DCreateBlob(ibByteSize, &_box_mesh_geometry->index_buffer_cpu),
		__FUNCTION__, __FILE__, __LINE__
	);
	memcpy(_box_mesh_geometry->index_buffer_cpu->GetBufferPointer(), box.indices.data(), ibByteSize);

	_box_mesh_geometry->vertex_buffer_gpu = createDefaultBuffer(
		_dx_device.Get(),
		_dx_command_list.Get(),
		vertices.data(),
		vbByteSize,
		_box_mesh_geometry->vertex_buffer_uploader
	);

	_box_mesh_geometry->index_buffer_gpu = createDefaultBuffer(
		_dx_device.Get(),
		_dx_command_list.Get(),
		box.indices.data(),
		ibByteSize,
		_box_mesh_geometry->index_buffer_uploader
	);

	_box_mesh_geometry->vertex_byte_stride = sizeof(Vertex);
	_box_mesh_geometry->vertex_buffer_byte_size = vbByteSize;
	_box_mesh_geometry->index_format = DXGI_FORMAT_R16_UINT;
	_box_mesh_geometry->index_buffer_byte_size = ibByteSize;

	_box_mesh_geometry->draw_arguments.emplace(
		std::make_pair(
			"box",
			SubmeshGeometry{ static_cast<uint32_t>(box.indices.size()), 0, 0, DirectX::BoundingBox() }
		)
	);
}

void DirectXRenderer::_createFrameResources()
{
	for (std::size_t index = 0; index < _frame_resources.size(); ++index)
	{
		_frame_resources[index] = std::make_unique<FrameResource>(_dx_device.Get(), 1, 1);
	}
}

void DirectXRenderer::_createConstantBufferViews()
{
	auto& _object_constants_upload_buffer = _frame_resources[_frame_resource_index]->object_constants_upload_buffer;
	(_dx_device.Get(), 1, true);

	//constexpr UINT object_constant_buffer_size_bytes = CalcConstantBufferByteSize(sizeof(ObjectConstants));

	D3D12_GPU_VIRTUAL_ADDRESS
		constant_buffer_address = _object_constants_upload_buffer->Resource()->GetGPUVirtualAddress();
	// Offset to the ith object constant buffer in the buffer.
	// int boxCBufIndex = 0;
	// cbAddress += boxCBufIndex*object_constant_buffer_size_bytes;

	D3D12_CONSTANT_BUFFER_VIEW_DESC cbv_description;
	cbv_description.BufferLocation = constant_buffer_address;
	cbv_description.SizeInBytes = CalcConstantBufferByteSize(sizeof(ObjectConstants));

	_dx_device->CreateConstantBufferView(
		&cbv_description,
		_dx_cbv_heap->GetCPUDescriptorHandleForHeapStart()
	);
}

void DirectXRenderer::_createPipelineStateObject()
{
	D3D12_GRAPHICS_PIPELINE_STATE_DESC standard_pipeline_state_object;
	ZeroMemory(&standard_pipeline_state_object, sizeof(D3D12_GRAPHICS_PIPELINE_STATE_DESC));
	standard_pipeline_state_object.InputLayout = { mInputLayout.data(), (UINT) mInputLayout.size() };
	standard_pipeline_state_object.pRootSignature = _dx_root_signature.Get();
	standard_pipeline_state_object.VS = {
		reinterpret_cast<BYTE*>(_mvs_byte_code->GetBufferPointer()),
		_mvs_byte_code->GetBufferSize()
	};
	standard_pipeline_state_object.PS = {
		reinterpret_cast<BYTE*>(_mps_byte_code->GetBufferPointer()),
		_mps_byte_code->GetBufferSize()
	};
	standard_pipeline_state_object.RasterizerState = CD3DX12_RASTERIZER_DESC(D3D12_DEFAULT);
	standard_pipeline_state_object.BlendState = CD3DX12_BLEND_DESC(D3D12_DEFAULT);
	standard_pipeline_state_object.DepthStencilState = CD3DX12_DEPTH_STENCIL_DESC(D3D12_DEFAULT);
	standard_pipeline_state_object.SampleMask = UINT_MAX;
	standard_pipeline_state_object.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;
	standard_pipeline_state_object.NumRenderTargets = 1;
	standard_pipeline_state_object.RTVFormats[0] = _back_buffer_format;
	standard_pipeline_state_object.SampleDesc.Count = get4xMsaaState() ? 4 : 1;
	standard_pipeline_state_object.SampleDesc.Quality = get4xMsaaState() ? (_4x_msaa_quality - 1) : 0;
	standard_pipeline_state_object.DSVFormat = _depth_stencil_format;

	throwIfFailed(
		_dx_device->CreateGraphicsPipelineState(
			&standard_pipeline_state_object, IID_PPV_ARGS(&_pipeline_state_objects[0])
		),
		__FUNCTION__, __FILE__, __LINE__
	);

	D3D12_GRAPHICS_PIPELINE_STATE_DESC wireframe_pipeline_state_object = standard_pipeline_state_object;
	wireframe_pipeline_state_object.RasterizerState.FillMode = D3D12_FILL_MODE_WIREFRAME;

	throwIfFailed(
		_dx_device->CreateGraphicsPipelineState(
			&wireframe_pipeline_state_object, IID_PPV_ARGS(&_pipeline_state_objects[1])
		),
		__FUNCTION__, __FILE__, __LINE__
	);
}

// ACCESSORS

ID3D12Resource* DirectXRenderer::_getCurrentBackBuffer() const
{
	return _swap_chain_buffer[_current_back_buffer].Get();
}

D3D12_CPU_DESCRIPTOR_HANDLE DirectXRenderer::_getCurrentBackBufferView() const
{
	return CD3DX12_CPU_DESCRIPTOR_HANDLE(
		_dx_rtv_heap->GetCPUDescriptorHandleForHeapStart(),
		_current_back_buffer,
		_rtv_descriptor_size
	);
}

D3D12_CPU_DESCRIPTOR_HANDLE DirectXRenderer::_getDepthStencilView() const
{
	return _dx_dsv_heap->GetCPUDescriptorHandleForHeapStart();
}
