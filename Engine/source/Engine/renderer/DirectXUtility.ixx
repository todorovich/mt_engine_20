module;

#include <windows.h>
#include <wrl.h>
#include <comdef.h>
#include <d3d12.h>
#include <d3dcompiler.h>
#include <dxgi.h>
#include <DirectXMath.h>
#include "d3dx12.h"

export module DirectXUtility;

export import MathHelper;

export import std.core;

export namespace mt::renderer
{
	struct Vertex {
		DirectX::XMFLOAT3 position;
		DirectX::XMFLOAT4 color;
	};

	class DxException;

	struct ObjectConstants;

	void ThrowIfFailed(HRESULT result, std::string function, std::string file, int line);

	std::wstring AnsiToWString(const std::string& str);

	constexpr UINT CalcConstantBufferByteSize(UINT byteSize)
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

	constexpr DirectX::XMFLOAT4X4 Identity4x4()
	{
		return DirectX::XMFLOAT4X4(
			1.0f, 0.0f, 0.0f, 0.0f,
			0.0f, 1.0f, 0.0f, 0.0f,
			0.0f, 0.0f, 1.0f, 0.0f,
			0.0f, 0.0f, 0.0f, 1.0f
		);
	}

	Microsoft::WRL::ComPtr<ID3DBlob> CompileShader(
		const std::wstring& filename,
		const D3D_SHADER_MACRO* defines,
		const std::string& entrypoint,
		const std::string& target
	);

	Microsoft::WRL::ComPtr<ID3D12Resource> CreateDefaultBuffer(
		ID3D12Device* device,
		ID3D12GraphicsCommandList* cmdList,
		const void* initData,
		UINT64 byteSize,
		Microsoft::WRL::ComPtr<ID3D12Resource>& uploadBuffer
	);

	DirectX::XMVECTOR SphericalToCartesian(float radius, float theta, float phi);

	DirectX::XMMATRIX InverseTranspose(DirectX::CXMMATRIX M);

	DirectX::XMVECTOR RandUnitVec3();

	DirectX::XMVECTOR RandHemisphereUnitVec3(DirectX::XMVECTOR n);
}

namespace mt::renderer 
{
	class DxException
	{
	public:
		HRESULT ErrorCode = S_OK;
		std::wstring FunctionName;
		std::wstring Filename;
		int LineNumber = -1;

		DxException() = default;
		DxException(HRESULT hr, const std::wstring& functionName, const std::wstring& filename, int lineNumber)
			:ErrorCode(hr),
			FunctionName(functionName),
			Filename(filename),
			LineNumber(lineNumber)
		{
		}

		std::wstring ToString() const
		{
			// Get the string description of the error code.
			_com_error err(ErrorCode);
			std::wstring msg = err.ErrorMessage();

			return FunctionName + L" failed in " + Filename + L"; line " + std::to_wstring(LineNumber) + L"; error: " + msg;
		}
	};

	struct ObjectConstants {
		DirectX::XMFLOAT4X4 world_view_projection = Identity4x4();
	};
}

module : private;

namespace mt::renderer
{
	void ThrowIfFailed(HRESULT result, std::string function, std::string file, int line)
	{
		if (result < 0) { throw DxException(result, AnsiToWString(""), AnsiToWString(file), line); }
	}

	Microsoft::WRL::ComPtr<ID3DBlob> CompileShader(
		const std::wstring& filename,
		const D3D_SHADER_MACRO* defines,
		const std::string& entrypoint,
		const std::string& target
	)
	{
		UINT compileFlags = 0;
#if defined(DEBUG) || defined(_DEBUG)  
		compileFlags = D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION;
#endif

		HRESULT hr = S_OK;

		Microsoft::WRL::ComPtr<ID3DBlob> byteCode = nullptr;
		Microsoft::WRL::ComPtr<ID3DBlob> errors;

		hr = D3DCompileFromFile(
			filename.c_str(),
			defines, 
			D3D_COMPILE_STANDARD_FILE_INCLUDE,
			entrypoint.c_str(),
			target.c_str(), 
			compileFlags, 
			0, 
			&byteCode, 
			&errors
		);

		if (errors != nullptr)
			OutputDebugStringA((char*)errors->GetBufferPointer());

		ThrowIfFailed(hr, __FUNCTION__, __FILE__, __LINE__);

		return byteCode;
	}

	std::wstring AnsiToWString(const std::string& str)
	{
		WCHAR buffer[512];
		MultiByteToWideChar(CP_ACP, 0, str.c_str(), -1, buffer, 512);
		return std::wstring(buffer);
	}

	Microsoft::WRL::ComPtr<ID3D12Resource> CreateDefaultBuffer(
		ID3D12Device* device,
		ID3D12GraphicsCommandList* cmdList,
		const void* initData,
		UINT64 byteSize,
		Microsoft::WRL::ComPtr<ID3D12Resource>& uploadBuffer
	)
	{
		Microsoft::WRL::ComPtr<ID3D12Resource> defaultBuffer;

		auto heapProperties = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_DEFAULT);
		auto buffer = CD3DX12_RESOURCE_DESC::Buffer(byteSize);
		// Create the actual default buffer resource.
		ThrowIfFailed(
			device->CreateCommittedResource(
				&heapProperties,
				D3D12_HEAP_FLAG_NONE,
				&buffer,
				D3D12_RESOURCE_STATE_COMMON,
				nullptr,
				IID_PPV_ARGS(defaultBuffer.GetAddressOf())
			),
			__FUNCTION__,
			__FILE__,
			__LINE__
		);

		auto heapProperties2 = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD);
		auto buffer2 = CD3DX12_RESOURCE_DESC::Buffer(byteSize);

		// In order to copy CPU memory data into our default buffer, we need to create
		// an intermediate upload heap. 
		ThrowIfFailed(
			device->CreateCommittedResource(
				&heapProperties2,
				D3D12_HEAP_FLAG_NONE,
				&buffer2,
				D3D12_RESOURCE_STATE_GENERIC_READ,
				nullptr,
				IID_PPV_ARGS(uploadBuffer.GetAddressOf())
			),
			__FUNCTION__,
			__FILE__,
			__LINE__
		);


		// Describe the data we want to copy into the default buffer.
		D3D12_SUBRESOURCE_DATA subResourceData = {};
		subResourceData.pData = initData;
		subResourceData.RowPitch = (LONG_PTR)byteSize;
		subResourceData.SlicePitch = subResourceData.RowPitch;

		auto resourceBarrier = CD3DX12_RESOURCE_BARRIER::Transition(
			defaultBuffer.Get(),
			D3D12_RESOURCE_STATE_COMMON,
			D3D12_RESOURCE_STATE_COPY_DEST
		);
		// Schedule to copy the data to the default buffer resource.  At a high level, the helper function UpdateSubresources
		// will copy the CPU memory into the intermediate upload heap.  Then, using ID3D12CommandList::CopySubresourceRegion,
		// the intermediate upload heap data will be copied to mBuffer.
		cmdList->ResourceBarrier(1, &resourceBarrier);
		UpdateSubresources<1>(cmdList, defaultBuffer.Get(), uploadBuffer.Get(), 0, 0, 1, &subResourceData);

		resourceBarrier = CD3DX12_RESOURCE_BARRIER::Transition(defaultBuffer.Get(),
			D3D12_RESOURCE_STATE_COPY_DEST,
			D3D12_RESOURCE_STATE_GENERIC_READ);
		cmdList->ResourceBarrier(1, &resourceBarrier);

		// Note: uploadBuffer has to be kept alive after the above function calls because
		// the command list has not been executed yet that performs the actual copy.
		// The caller can Release the uploadBuffer after it knows the copy has been executed.

		return defaultBuffer;
	}

	DirectX::XMVECTOR SphericalToCartesian(float radius, float theta, float phi)
	{
		return DirectX::XMVectorSet(
			radius * sinf(phi) * cosf(theta),
			radius * cosf(phi),
			radius * sinf(phi) * sinf(theta),
			1.0f);
	}

	DirectX::XMMATRIX InverseTranspose(DirectX::CXMMATRIX M)
	{
		// Inverse-transpose is just applied to normals.  So zero out 
		// translation row so that it doesn't get into our inverse-transpose
		// calculation--we don't want the inverse-transpose of the translation.
		DirectX::XMMATRIX A = M;
		A.r[3] = DirectX::XMVectorSet(0.0f, 0.0f, 0.0f, 1.0f);

		DirectX::XMVECTOR det = DirectX::XMMatrixDeterminant(A);
		return DirectX::XMMatrixTranspose(DirectX::XMMatrixInverse(&det, A));
	}

	DirectX::XMVECTOR RandUnitVec3()
	{
		DirectX::XMVECTOR One = DirectX::XMVectorSet(1.0f, 1.0f, 1.0f, 1.0f);
		DirectX::XMVECTOR Zero = DirectX::XMVectorZero();

		// Keep trying until we get a point on/in the hemisphere.
		while (true)
		{
			// Generate random point in the cube [-1,1]^3.
			DirectX::XMVECTOR v = DirectX::XMVectorSet(
				MathHelper::RandF(-1.0f, 1.0f),
				MathHelper::RandF(-1.0f, 1.0f),
				MathHelper::RandF(-1.0f, 1.0f),
				0.0f
			);

			// Ignore points outside the unit sphere in order to get an even distribution 
			// over the unit sphere.  Otherwise points will clump more on the sphere near 
			// the corners of the cube.

			if (DirectX::XMVector3Greater(DirectX::XMVector3LengthSq(v), One))
				continue;

			return DirectX::XMVector3Normalize(v);
		}
	}

	DirectX::XMVECTOR RandHemisphereUnitVec3(DirectX::XMVECTOR n)
	{
		DirectX::XMVECTOR One = DirectX::XMVectorSet(1.0f, 1.0f, 1.0f, 1.0f);
		DirectX::XMVECTOR Zero = DirectX::XMVectorZero();

		// Keep trying until we get a point on/in the hemisphere.
		while (true)
		{
			// Generate random point in the cube [-1,1]^3.
			DirectX::XMVECTOR v = DirectX::XMVectorSet(
				MathHelper::RandF(-1.0f, 1.0f),
				MathHelper::RandF(-1.0f, 1.0f),
				MathHelper::RandF(-1.0f, 1.0f),
				0.0f
			);

			// Ignore points outside the unit sphere in order to get an even distribution 
			// over the unit sphere.  Otherwise points will clump more on the sphere near 
			// the corners of the cube.

			if (DirectX::XMVector3Greater(DirectX::XMVector3LengthSq(v), One))
				continue;

			// Ignore points in the bottom hemisphere.
			if (DirectX::XMVector3Less(DirectX::XMVector3Dot(n, v), Zero))
				continue;

			return DirectX::XMVector3Normalize(v);
		}
	}
}
