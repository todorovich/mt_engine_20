module;

#include <wrl.h>
#include <d3d12.h>

export module MeshGeometry;

import <cstdint>;
import <string>;
import <unordered_map>;

export import SubmeshGeometry;

namespace mt::renderer
{
    export struct MeshGeometry
    {
        // Give it a _name so we can look it up by _name.
        std::string name;

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

        D3D12_VERTEX_BUFFER_VIEW vertexBufferView() const noexcept
        {
            return D3D12_VERTEX_BUFFER_VIEW{ vertex_buffer_gpu->GetGPUVirtualAddress(), vertex_buffer_byte_size, vertex_byte_stride };
        }

        D3D12_INDEX_BUFFER_VIEW indexBufferView() const noexcept
		{
            return D3D12_INDEX_BUFFER_VIEW{ index_buffer_gpu->GetGPUVirtualAddress(), index_buffer_byte_size, index_format };
        }

        // We can free this memory after we finish upload to the GPU.
        void disposeUploaders() noexcept
		{
            vertex_buffer_uploader = Microsoft::WRL::ComPtr<ID3D12Resource>{};
            index_buffer_uploader = Microsoft::WRL::ComPtr<ID3D12Resource>{};
        }
    };
}