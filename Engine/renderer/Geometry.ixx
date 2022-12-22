module;

#include <wrl.h>
//#include "d3dx12.h"
#include <d3dcompiler.h>
#include <d3d12.h>
#include <DirectXCollision.h>
//#include <DirectXMath.h>

export module Geometry;

import <string>;
import <unordered_map>;

namespace mt::renderer 
{
    // Defines a subrange of geometry in a MeshGeometry.  This is for when multiple
    // geometries are stored in one vertex and index buffer.  It provides the offsets
    // and data needed to draw a subset of geometry stores in the vertex and index 
    // buffers so that we can implement the technique described by Figure 6.3.
    export struct SubmeshGeometry
    {
        UINT index_count = 0;
        UINT start_index_location = 0;
        INT base_vertex_location = 0;

        // Bounding box of the geometry defined by this submesh. 
        // This is used in later chapters of the book.
        DirectX::BoundingBox bounds;
    };

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
        UINT vertex_byte_stride = 0;
        UINT vertex_buffer_byte_size = 0;
        DXGI_FORMAT index_format = DXGI_FORMAT_R16_UINT;
        UINT index_buffer_byte_size = 0;

        // A MeshGeometry may store multiple geometries in one vertex/index buffer.
        // Use this container to define the Submesh geometries so we can draw
        // the Submeshes individually.
        std::unordered_map<std::string, SubmeshGeometry> draw_arguments;

        D3D12_VERTEX_BUFFER_VIEW vertexBufferView() const
        {
            D3D12_VERTEX_BUFFER_VIEW vbv;
            vbv.BufferLocation = vertex_buffer_gpu->GetGPUVirtualAddress();
            vbv.StrideInBytes = vertex_byte_stride;
            vbv.SizeInBytes = vertex_buffer_byte_size;

            return vbv;
        }

        D3D12_INDEX_BUFFER_VIEW indexBufferView() const
        {
            D3D12_INDEX_BUFFER_VIEW ibv;
            ibv.BufferLocation = index_buffer_gpu->GetGPUVirtualAddress();
            ibv.Format = index_format;
            ibv.SizeInBytes = index_buffer_byte_size;

            return ibv;
        }

        // We can free this memory after we finish upload to the GPU.
        void disposeUploaders()
        {
            vertex_buffer_uploader = Microsoft::WRL::ComPtr<ID3D12Resource>{};
            index_buffer_uploader = Microsoft::WRL::ComPtr<ID3D12Resource>{};
        }
    };
}
