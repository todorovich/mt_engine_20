// Copyright 2023 Micho Todorovich, all rights reserved.
module;

#include <wrl.h>
#include <comdef.h>
#include <d3d12.h>
#include <d3dcompiler.h>
#include <DirectXMath.h>
#include "d3dx12.h"

export module DirectXUtility;

import std;
import Windows;

export import Constants;
export import Error;

using namespace windows;
using namespace mt::error;

export namespace mt::renderer
{
	constexpr UINT CalcConstantBufferByteSize(UINT byteSize) noexcept
	{
		// Constant buffers must be a multiple of the minimum hardware
		// allocation size (usually 256 bytes).  So round up to nearest
		// multiple of 256.  We do this by adding 255 and then masking off
		// the lower 2 bytes which store all bits < 256.
		// Example: Suppose byteSize = 300.
		// (300 + 255) & ~255
		// 555 & ~255
		// 0x022B & ~0x00ff
		// 0x022B & 0xff00
		// 0x0200
		// 512
		return (byteSize + 255) & ~255;
	}

	std::expected<Microsoft::WRL::ComPtr<ID3DBlob>, std::error_condition> CompileShader(
		const std::wstring& filename,
		const D3D_SHADER_MACRO* defines,
		const std::string& entrypoint,
		const std::string& target
	) noexcept;

	std::expected<Microsoft::WRL::ComPtr<ID3D12Resource>, std::error_condition> createDefaultBuffer(
		ID3D12Device* device,
		ID3D12GraphicsCommandList* cmdList,
		const void* initData,
		UINT64 byteSize,
		Microsoft::WRL::ComPtr<ID3D12Resource>& uploadBuffer
	) noexcept;
}

using namespace std::literals;

module : private;

namespace mt::renderer
{
	std::expected<Microsoft::WRL::ComPtr<ID3DBlob>, std::error_condition> CompileShader(
		const std::wstring& filename,
		const D3D_SHADER_MACRO* defines,
		const std::string& entrypoint,
		const std::string& target
	) noexcept
	{
		UINT compileFlags = 0;

		if constexpr (mt::IS_DEBUG) compileFlags = D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION;

		Microsoft::WRL::ComPtr<ID3DBlob> byteCode = nullptr;
		Microsoft::WRL::ComPtr<ID3DBlob> errors;

		if (FAILED(D3DCompileFromFile(
			filename.c_str(),
			defines,
			D3D_COMPILE_STANDARD_FILE_INCLUDE,
			entrypoint.c_str(),
			target.c_str(),
			compileFlags,
			0,
			&byteCode,
			&errors
		)))
		{
			if (errors != nullptr)
				OutputDebugStringA((char*)errors->GetBufferPointer());

			return std::unexpected(MakeErrorCondition(ErrorCode::COMPILE_SHADER_FAILED));
		}

		return byteCode;
	}

	std::expected<Microsoft::WRL::ComPtr<ID3D12Resource>, std::error_condition> createDefaultBuffer(
		ID3D12Device* device,
		ID3D12GraphicsCommandList* command_lists,
		const void* initialization_data,
		UINT64 byte_size,
		Microsoft::WRL::ComPtr<ID3D12Resource>& upload_buffer
	) noexcept
	{
		Microsoft::WRL::ComPtr<ID3D12Resource> default_buffer;

		auto heap_properties = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_DEFAULT);
		auto buffer = CD3DX12_RESOURCE_DESC::Buffer(byte_size);

		// Create the actual default buffer resource.
		if (FAILED(device->CreateCommittedResource(
			&heap_properties,
			D3D12_HEAP_FLAG_NONE,
			&buffer,
			D3D12_RESOURCE_STATE_COMMON,
			nullptr,
			IID_PPV_ARGS(default_buffer.GetAddressOf())
		)))
		{
			return std::unexpected(MakeErrorCondition(ErrorCode::CREATE_COMMITTED_RESOURCE_FAILED));
		}

		auto heap_properties_2 = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD);
		auto buffer_2 = CD3DX12_RESOURCE_DESC::Buffer(byte_size);

		// In order to copy CPU memory data into our default buffer, we need to create
		// an intermediate upload heap. 
		if (FAILED(device->CreateCommittedResource(
			&heap_properties_2,
			D3D12_HEAP_FLAG_NONE,
			&buffer_2,
			D3D12_RESOURCE_STATE_GENERIC_READ,
			nullptr,
			IID_PPV_ARGS(upload_buffer.GetAddressOf())
		)))
		{
			return std::unexpected(MakeErrorCondition(ErrorCode::CREATE_COMMITTED_RESOURCE_FAILED));
		}

		// Describe the data we want to copy into the default buffer.
		D3D12_SUBRESOURCE_DATA sub_resource_data = {};
		sub_resource_data.pData = initialization_data;
		sub_resource_data.RowPitch = (LONG_PTR)byte_size;
		sub_resource_data.SlicePitch = sub_resource_data.RowPitch;

		auto resourceBarrier = CD3DX12_RESOURCE_BARRIER::Transition(
			default_buffer.Get(),
			D3D12_RESOURCE_STATE_COMMON,
			D3D12_RESOURCE_STATE_COPY_DEST
		);
		// Schedule to copy the data to the default buffer resource.  At a high level, the helper function UpdateSubresources
		// will copy the CPU memory into the intermediate upload heap.  Then, using ID3D12CommandList::CopySubresourceRegion,
		// the intermediate upload heap data will be copied to default_buffer.
		command_lists->ResourceBarrier(1, &resourceBarrier);
		UpdateSubresources<1>(command_lists, default_buffer.Get(), upload_buffer.Get(), 0, 0, 1, &sub_resource_data);

		resourceBarrier = CD3DX12_RESOURCE_BARRIER::Transition(
			default_buffer.Get(),
			D3D12_RESOURCE_STATE_COPY_DEST,
			D3D12_RESOURCE_STATE_GENERIC_READ
		);
		command_lists->ResourceBarrier(1, &resourceBarrier);

		// Note: uploadBuffer has to be kept alive after the above function calls because
		// the command list has not been executed yet that performs the actual copy.
		// The caller can Release the uploadBuffer after it knows the copy has been executed.

		return default_buffer;
	}
}
