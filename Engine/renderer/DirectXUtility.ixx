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

export namespace mt::renderer
{
	struct Vertex {
		DirectX::XMFLOAT3 position;
		DirectX::XMFLOAT4 color;
	};

	class DxException;

	struct ObjectConstants;

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

	Microsoft::WRL::ComPtr<ID3D12Resource> createDefaultBuffer(
		ID3D12Device* device,
		ID3D12GraphicsCommandList* cmdList,
		const void* initData,
		UINT64 byteSize,
		Microsoft::WRL::ComPtr<ID3D12Resource>& uploadBuffer
	);

	DirectX::XMVECTOR SphericalToCartesian(float radius, float theta, float phi);

	DirectX::XMMATRIX IverseTranspose(DirectX::CXMMATRIX M);

	DirectX::XMVECTOR RandomUnitVector3();

	DirectX::XMVECTOR RandomHemisphereUnitVector3(DirectX::XMVECTOR n);
}

namespace mt::renderer 
{
	class DxException
	{
	public:
		HRESULT error_code = S_OK;
		std::wstring function_name;
		std::wstring filename;
		int line_number = -1;

		DxException() = default;
		DxException(HRESULT hr, const std::wstring& functionName, const std::wstring& filename, int lineNumber)
			:error_code(hr),
			function_name(functionName),
			filename(filename),
			line_number(lineNumber)
		{
		}

		std::wstring ToString() const
		{
			// Get the string description of the error code.
			_com_error err(error_code);
			
			//std::wstring msg = std::wstring(err.ErrorMessage());

			return function_name + L" failed in " + filename + L"; line " + std::to_wstring(line_number) + L"; error: ";// +msg;
		}
	};

	struct ObjectConstants {
		DirectX::XMFLOAT4X4 world_view_projection = Identity4x4();
	};

	export inline void throwIfFailed(const HRESULT result, const std::string& function, const std::string& file, int line)
	{
		if (result < 0) { throw DxException(result, AnsiToWString(function), AnsiToWString(file), line); }
	}
}

module : private;

namespace mt::renderer
{
	

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

		throwIfFailed(hr, __FUNCTION__, __FILE__, __LINE__);

		return byteCode;
	}

	std::wstring AnsiToWString(const std::string& str)
	{
		WCHAR buffer[512];
		MultiByteToWideChar(CP_ACP, 0, str.c_str(), -1, buffer, 512);
		return std::wstring(buffer);
	}

	Microsoft::WRL::ComPtr<ID3D12Resource> createDefaultBuffer(
		ID3D12Device* device,
		ID3D12GraphicsCommandList* command_list,
		const void* initialization_data,
		UINT64 byte_size,
		Microsoft::WRL::ComPtr<ID3D12Resource>& upload_buffer
	)
	{
		Microsoft::WRL::ComPtr<ID3D12Resource> default_buffer;

		auto heap_properties = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_DEFAULT);
		auto buffer = CD3DX12_RESOURCE_DESC::Buffer(byte_size);
		// Create the actual default buffer resource.
		throwIfFailed(
			device->CreateCommittedResource(
				&heap_properties,
				D3D12_HEAP_FLAG_NONE,
				&buffer,
				D3D12_RESOURCE_STATE_COMMON,
				nullptr,
				IID_PPV_ARGS(default_buffer.GetAddressOf())
			),
			__FUNCTION__,
			__FILE__,
			__LINE__
		);

		auto heap_properties_2 = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD);
		auto buffer_2 = CD3DX12_RESOURCE_DESC::Buffer(byte_size);

		// In order to copy CPU memory data into our default buffer, we need to create
		// an intermediate upload heap. 
		throwIfFailed(
			device->CreateCommittedResource(
				&heap_properties_2,
				D3D12_HEAP_FLAG_NONE,
				&buffer_2,
				D3D12_RESOURCE_STATE_GENERIC_READ,
				nullptr,
				IID_PPV_ARGS(upload_buffer.GetAddressOf())
			),
			__FUNCTION__,
			__FILE__,
			__LINE__
		);


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
		// the intermediate upload heap data will be copied to mBuffer.
		command_list->ResourceBarrier(1, &resourceBarrier);
		UpdateSubresources<1>(command_list, default_buffer.Get(), upload_buffer.Get(), 0, 0, 1, &sub_resource_data);

		resourceBarrier = CD3DX12_RESOURCE_BARRIER::Transition(default_buffer.Get(),
			D3D12_RESOURCE_STATE_COPY_DEST,
			D3D12_RESOURCE_STATE_GENERIC_READ);
		command_list->ResourceBarrier(1, &resourceBarrier);

		// Note: uploadBuffer has to be kept alive after the above function calls because
		// the command list has not been executed yet that performs the actual copy.
		// The caller can Release the uploadBuffer after it knows the copy has been executed.

		return default_buffer;
	}

	DirectX::XMVECTOR SphericalToCartesian(float radius, float theta, float phi)
	{
		return DirectX::XMVectorSet(
			radius * sinf(phi) * cosf(theta),
			radius * cosf(phi),
			radius * sinf(phi) * sinf(theta),
			1.0f);
	}

	DirectX::XMMATRIX IverseTranspose(DirectX::CXMMATRIX M)
	{
		// Inverse-transpose is just applied to normals.  So zero out 
		// translation row so that it doesn't get into our inverse-transpose
		// calculation--we don't want the inverse-transpose of the translation.
		DirectX::XMMATRIX A = M;
		A.r[3] = DirectX::XMVectorSet(0.0f, 0.0f, 0.0f, 1.0f);

		DirectX::XMVECTOR determinant = DirectX::XMMatrixDeterminant(A);
		return DirectX::XMMatrixTranspose(DirectX::XMMatrixInverse(&determinant, A));
	}

	DirectX::XMVECTOR RandomUnitVector3()
	{
		DirectX::XMVECTOR one = DirectX::XMVectorSet(1.0f, 1.0f, 1.0f, 1.0f);
		DirectX::XMVECTOR zero = DirectX::XMVectorZero();

		// Keep trying until we get a point on/in the hemisphere.
		while (true)
		{
			// Generate random point in the cube [-1,1]^3.
			DirectX::XMVECTOR vector = DirectX::XMVectorSet(
				MathHelper::RandF(-1.0f, 1.0f),
				MathHelper::RandF(-1.0f, 1.0f),
				MathHelper::RandF(-1.0f, 1.0f),
				0.0f
			);

			// Ignore points outside the unit sphere in order to get an even distribution 
			// over the unit sphere.  Otherwise points will clump more on the sphere near 
			// the corners of the cube.

			if (DirectX::XMVector3Greater(DirectX::XMVector3LengthSq(vector), one))
				continue;

			return DirectX::XMVector3Normalize(vector);
		}
	}

	DirectX::XMVECTOR RandomHemisphereUnitVector3(DirectX::XMVECTOR n)
	{
		DirectX::XMVECTOR one = DirectX::XMVectorSet(1.0f, 1.0f, 1.0f, 1.0f);
		DirectX::XMVECTOR zero = DirectX::XMVectorZero();

		// Keep trying until we get a point on/in the hemisphere.
		while (true)
		{
			// Generate random point in the cube [-1,1]^3.
			DirectX::XMVECTOR vector = DirectX::XMVectorSet(
				MathHelper::RandF(-1.0f, 1.0f),
				MathHelper::RandF(-1.0f, 1.0f),
				MathHelper::RandF(-1.0f, 1.0f),
				0.0f
			);

			// Ignore points outside the unit sphere in order to get an even distribution 
			// over the unit sphere.  Otherwise points will clump more on the sphere near 
			// the corners of the cube.

			if (DirectX::XMVector3Greater(DirectX::XMVector3LengthSq(vector), one))
				continue;

			// Ignore points in the bottom hemisphere.
			if (DirectX::XMVector3Less(DirectX::XMVector3Dot(n, vector), zero))
				continue;

			return DirectX::XMVector3Normalize(vector);
		}
	}
}
