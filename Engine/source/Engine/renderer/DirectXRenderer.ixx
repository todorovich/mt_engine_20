// Copyright 2018 Micho Todorovich, all rights reserved.
module;

#include <wrl.h>
#include <windowsx.h>
#include <windows.h>

#include <d3d12.h>
#include <DirectXMath.h>
#include <dxgi.h>
#include <dxgi1_4.h>
#include <DirectXCollision.h>

#include "d3dx12.h"


// Link necessary d3d12 libraries.
#pragma comment(lib, "d3dcompiler.lib")
#pragma comment(lib, "D3D12.lib")
#pragma comment(lib, "dxgi.lib")
#pragma comment(lib, "dxguid.lib")

export module DirectXRenderer;

//import UploadBuffer;
import Camera;
import MathHelper;
import std.core;
import DirectXUtility;

using Microsoft::WRL::ComPtr;
using namespace std;
using namespace DirectX;

export template<typename T> class UploadBuffer
{
public:
    UploadBuffer(ID3D12Device* device, UINT elementCount, bool isConstantBuffer) :
        mIsConstantBuffer(isConstantBuffer)
    {
        mElementByteSize = sizeof(T);

        // Constant buffer elements need to be multiples of 256 bytes.
        // This is because the hardware can only view constant data 
        // at m*256 byte offsets and of n*256 byte lengths. 
        // typedef struct D3D12_CONSTANT_BUFFER_VIEW_DESC {
        // UINT64 OffsetInBytes; // multiple of 256
        // UINT   SizeInBytes;   // multiple of 256
        // } D3D12_CONSTANT_BUFFER_VIEW_DESC;
        if (isConstantBuffer) mElementByteSize = mt::renderer::CalcConstantBufferByteSize(sizeof(T));

        auto heap_properties = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD);
        auto resource_desc = CD3DX12_RESOURCE_DESC::Buffer(mElementByteSize * elementCount);
        mt::renderer::ThrowIfFailed(
            device->CreateCommittedResource(
                &heap_properties,
                D3D12_HEAP_FLAG_NONE,
                &resource_desc,
                D3D12_RESOURCE_STATE_GENERIC_READ,
                nullptr,
                IID_PPV_ARGS(&mUploadBuffer)
            ),
            "",
            __FILE__,
            __LINE__
        );

        mt::renderer::ThrowIfFailed(
            mUploadBuffer->Map(0, nullptr, reinterpret_cast<void**>(&mMappedData)),
            "",
            __FILE__,
            __LINE__
        );

        // We do not need to unmap until we are done with the resource.  However, we must not write to
        // the resource while it is in use by the GPU (so we must use synchronization techniques).
    }

    UploadBuffer(const UploadBuffer& rhs) = delete;
    UploadBuffer& operator=(const UploadBuffer& rhs) = delete;
    ~UploadBuffer()
    {
        if (mUploadBuffer != nullptr)
            mUploadBuffer->Unmap(0, nullptr);

        mMappedData = nullptr;
    }

    ID3D12Resource* Resource()const
    {
        return mUploadBuffer.Get();
    }

    void CopyData(int elementIndex, const T& data)
    {
        memcpy(&mMappedData[elementIndex * mElementByteSize], &data, sizeof(T));
    }

private:
    Microsoft::WRL::ComPtr<ID3D12Resource> mUploadBuffer;
    BYTE* mMappedData = nullptr;

    UINT mElementByteSize = 0;
    bool mIsConstantBuffer = false;
};

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

export namespace mt::renderer
{
    struct Vertex {
        XMFLOAT3 Pos;
        XMFLOAT4 Color;
    };

    struct ObjectConstants {
        XMFLOAT4X4 WorldViewProj = MathHelper::Identity4x4();
    };

    class DirectXRenderer {
    public:
        DirectXRenderer() {
            //auto string = L"sizeof vector: " + std::to_wstring(sizeof(camera)) + L" bytes \n";

            //OutputDebugStringW(string.c_str());
        }

        DirectXRenderer(const DirectXRenderer&) = delete;

        ~DirectXRenderer() {
            if (dx_device != nullptr)
                flush_command_queue();
        }

        // Accessors
        camera::Camera& GetCurrentCamera() { return camera; } // NOT CONST!!!!

        bool Get4xMsaaState() const { return _4x_msaa_state; };

        bool GetIsInitialized() const { return _is_initialized; };

        int GetSwapChainBufferCount() const { return _swap_chain_buffer_count; };

        void Set4xMsaaState(bool value);

        bool GetIsRendering() const { return _is_rendering; }

        // Mutators
        bool InitializeDirect3d(HWND main_window_handle);

        void Render();

        void Resize(int client_width, int client_height);

        void Update();

        void flush_command_queue();

        // Fence Stuff

        void IncrementFence();

        void WaitForFence();

        bool IsCurrentFenceComplete() { return fence->GetCompletedValue() >= current_fence_index; }

    protected:

        // Accessors
        ID3D12Resource* GetCurrentBackBuffer() const;

        D3D12_CPU_DESCRIPTOR_HANDLE GetCurrentBackBufferView() const;

        D3D12_CPU_DESCRIPTOR_HANDLE GetDepthStencilView() const;

        // Mutators

        void CreateCommandList();

        void CreateDxCommandObjects();

        void CreateSwapChain();

        virtual void CreateDescriptorHeaps();

        void CreateConstantBuffers();

        void CreateRootSignature();

        void CreateShadersAndInputLayout();

        void CreateBoxGeometry();

        void CreatePipelineStateObject();

        void LogAdapters();

        void LogAdapterOutputs(IDXGIAdapter* adapter);

        void LogOutputDisplayModes(IDXGIOutput* output, DXGI_FORMAT format);

        // Data
        static const int _swap_chain_buffer_count = 2;

        camera::Camera camera; // 204 bytes (getting kind of bloated)

        // 64 byte types
        XMFLOAT4X4 mWorld = MathHelper::Identity4x4(); // Transformation from Local Space to World Space
        XMFLOAT4X4 mView = MathHelper::Identity4x4();    // Transformation from World Space to the camera's View Space
        XMFLOAT4X4 mProj = MathHelper::Identity4x4();    // Transformation from View Space to Projection Space (Normalized Device Coordinates (GPU Space?))

        // 32 byte type
        vector<D3D12_INPUT_ELEMENT_DESC> mInputLayout; // 32 bytes according to sizeof

        // 24 byte type
        D3D12_VIEWPORT mScreenViewport;

        // 16 byte types
        D3D12_RECT mScissorRect;

        // Pointers - 8 byte each
        HWND _main_window_handle;

        unique_ptr<UploadBuffer<ObjectConstants>> object_constants_upload_buffer = nullptr;
        unique_ptr<MeshGeometry> box_mesh_geometry = nullptr;

        ComPtr<IDXGIFactory4> dx_dxgi_factory;
        ComPtr<IDXGISwapChain> dx_swap_chain;
        ComPtr<ID3D12Device> dx_device;

        ComPtr<ID3D12Fence> fence;

        ComPtr<ID3D12CommandQueue> dx_command_queue;
        ComPtr<ID3D12CommandAllocator> dx_command_list_allocator;
        ComPtr<ID3D12GraphicsCommandList> dx_command_list;

        ComPtr<ID3D12Resource> swap_chain_buffer[_swap_chain_buffer_count];
        ComPtr<ID3D12Resource> depth_stencil_buffer;

        ComPtr<ID3D12DescriptorHeap> dx_rtv_heap;
        ComPtr<ID3D12DescriptorHeap> dx_dsv_heap;

        ComPtr<ID3D12RootSignature> dx_root_signature;
        ComPtr<ID3D12DescriptorHeap> dx_cbv_heap;

        ComPtr<ID3DBlob> mvsByteCode;
        ComPtr<ID3DBlob> mpsByteCode;
        ComPtr<ID3D12PipelineState> mPSO;

        // 8 byte types
        UINT64 current_fence_index = 0;

        POINT mLastMousePos;

        // 4 byte types
        UINT rtv_descriptor_size = 0;
        UINT dsv_descriptor_size = 0;
        UINT cbv_srv_uav_descriptor_size = 0;
        UINT _4x_msaa_quality = 0;      // quality level of 4X MSAA

        D3D_DRIVER_TYPE md3dDriverType = D3D_DRIVER_TYPE_HARDWARE;
        DXGI_FORMAT mBackBufferFormat = DXGI_FORMAT_R8G8B8A8_UNORM;
        DXGI_FORMAT mDepthStencilFormat = DXGI_FORMAT_D24_UNORM_S8_UINT;

        int current_back_buffer = 0;
        int _window_width = 0;
        int _window_height = 0;

        float _window_aspect_ratio = 0.0f;

        // Single byte each
        bool _4x_msaa_state = false;
        bool _is_initialized = false;
        bool _is_rendering = false;
    };
}