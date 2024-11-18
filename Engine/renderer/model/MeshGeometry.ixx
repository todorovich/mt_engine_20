// Copyright 2023 Micho Todorovich, all rights reserved.
module;

#include <wrl.h>
#include <d3d12.h>

export module MeshGeometry;

import std;

export import gsl;
export import Error;
export import SubmeshGeometry;
export import MakeUnique;

using namespace gsl;

using namespace mt::error;
using namespace mt::memory;

using namespace std::literals;

export namespace mt::renderer
{
	class MeshGeometry
	{
		// Give it a _name so we can look it up by _name.
		std::string _name;

		// TODO: improve this
		MeshGeometry(std::string&& name) noexcept
			: _name(name)
		{

		}

	public:
		friend std::unique_ptr<MeshGeometry> mt::memory::make_unique_nothrow<MeshGeometry>(
			std::string&& name
		) noexcept;

		~MeshGeometry() noexcept = default;

		MeshGeometry(const MeshGeometry&) noexcept = default;

		MeshGeometry(MeshGeometry&&) noexcept = default;

		MeshGeometry& operator=(const MeshGeometry&) noexcept = default;

		MeshGeometry& operator=(MeshGeometry&&) noexcept = default;

		// System memory copies.  Use Blobs because the vertex/index format can be generic.
		// It is up to the client to cast appropriately.
		Microsoft::WRL::ComPtr<ID3DBlob> vertex_buffer_cpu;
		Microsoft::WRL::ComPtr<ID3DBlob> index_buffer_cpu;

		Microsoft::WRL::ComPtr<ID3D12Resource> vertex_buffer_gpu;
		Microsoft::WRL::ComPtr<ID3D12Resource> index_buffer_gpu;

		Microsoft::WRL::ComPtr<ID3D12Resource> vertex_buffer_uploader;
		Microsoft::WRL::ComPtr<ID3D12Resource> index_buffer_uploader;

		// Data about the buffers.
		std::uint32_t vertex_byte_stride = 0;
		std::uint32_t vertex_buffer_byte_size = 0;
		std::uint32_t index_buffer_byte_size = 0;
		DXGI_FORMAT index_format = DXGI_FORMAT_R16_UINT;

		// A MeshGeometry may store multiple geometries in one vertex/index buffer.
		// Use this container to define the Submesh geometries so we can draw
		// the Submeshes individually.
		std::unordered_map<std::string, SubmeshGeometry> draw_arguments;


		[[nodiscard]] inline D3D12_VERTEX_BUFFER_VIEW vertexBufferView() const noexcept
		{
			return D3D12_VERTEX_BUFFER_VIEW{
				vertex_buffer_gpu->GetGPUVirtualAddress(), vertex_buffer_byte_size, vertex_byte_stride
			};
		}

		[[nodiscard]] inline D3D12_INDEX_BUFFER_VIEW indexBufferView() const noexcept
		{
			return D3D12_INDEX_BUFFER_VIEW{
				index_buffer_gpu->GetGPUVirtualAddress(), index_buffer_byte_size, index_format
			};
		}

		[[nodiscard]] std::string_view getName() const noexcept
		{
			return _name;
		}

		// We can free this memory after we finish upload to the GPU.
		void disposeUploaders() noexcept
		{
			vertex_buffer_uploader = Microsoft::WRL::ComPtr<ID3D12Resource>{};
			index_buffer_uploader = Microsoft::WRL::ComPtr<ID3D12Resource>{};
		}
	};
};

export namespace mt::memory::factory
{
	std::expected<std::unique_ptr<mt::renderer::MeshGeometry>, std::error_condition> MeshGeometry(
		std::string&& name
	) noexcept
	{
		auto ptr = make_unique_nothrow<mt::renderer::MeshGeometry>(std::forward<std::string>(name));

		if (ptr != nullptr)
		{
			return ptr;
		}
		else
		{
			return std::unexpected(MakeErrorCondition(mt::error::ErrorCode::BAD_ALLOCATION));
		}
	}
};