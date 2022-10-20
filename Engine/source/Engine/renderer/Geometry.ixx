module;

#include <wrl.h>
//#include "d3dx12.h"
#include <d3dcompiler.h>
#include <d3d12.h>
#include <DirectXCollision.h>
//#include <DirectXMath.h>

export module Geometry;

import std.core;

namespace mt::renderer 
{
    // Defines a subrange of geometry in a MeshGeometry.  This is for when multiple
    // geometries are stored in one vertex and index buffer.  It provides the offsets
    // and data needed to draw a subset of geometry stores in the vertex and index 
    // buffers so that we can implement the technique described by Figure 6.3.
    export struct SubmeshGeometry
    {
        UINT IndexCount = 0;
        UINT StartIndexLocation = 0;
        INT BaseVertexLocation = 0;

        // Bounding box of the geometry defined by this submesh. 
        // This is used in later chapters of the book.
        DirectX::BoundingBox Bounds;
    };

    export struct MeshGeometry
    {
        // Give it a _name so we can look it up by _name.
        std::string Name;

        // System memory copies.  Use Blobs because the vertex/index format can be generic.
        // It is up to the client to cast appropriately.  
        Microsoft::WRL::ComPtr<ID3DBlob> VertexBufferCPU;
        Microsoft::WRL::ComPtr<ID3DBlob> IndexBufferCPU;

        Microsoft::WRL::ComPtr<ID3D12Resource> VertexBufferGPU;
        Microsoft::WRL::ComPtr<ID3D12Resource> IndexBufferGPU;

        Microsoft::WRL::ComPtr<ID3D12Resource> VertexBufferUploader;
        Microsoft::WRL::ComPtr<ID3D12Resource> IndexBufferUploader;

        // Data about the buffers.
        UINT VertexByteStride = 0;
        UINT VertexBufferByteSize = 0;
        DXGI_FORMAT IndexFormat = DXGI_FORMAT_R16_UINT;
        UINT IndexBufferByteSize = 0;

        // A MeshGeometry may store multiple geometries in one vertex/index buffer.
        // Use this container to define the Submesh geometries so we can draw
        // the Submeshes individually.
        std::unordered_map<std::string, SubmeshGeometry> DrawArgs;

        D3D12_VERTEX_BUFFER_VIEW VertexBufferView() const
        {
            D3D12_VERTEX_BUFFER_VIEW vbv;
            vbv.BufferLocation = VertexBufferGPU->GetGPUVirtualAddress();
            vbv.StrideInBytes = VertexByteStride;
            vbv.SizeInBytes = VertexBufferByteSize;

            return vbv;
        }

        D3D12_INDEX_BUFFER_VIEW IndexBufferView() const
        {
            D3D12_INDEX_BUFFER_VIEW ibv;
            ibv.BufferLocation = IndexBufferGPU->GetGPUVirtualAddress();
            ibv.Format = IndexFormat;
            ibv.SizeInBytes = IndexBufferByteSize;

            return ibv;
        }

        // We can free this memory after we finish upload to the GPU.
        void DisposeUploaders()
        {
            VertexBufferUploader = Microsoft::WRL::ComPtr<ID3D12Resource>{};
            IndexBufferUploader = Microsoft::WRL::ComPtr<ID3D12Resource>{};
        }
    };
}