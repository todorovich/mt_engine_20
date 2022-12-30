// Copyright 2022 Micho Todorovich, all rights reserved.
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

export import Camera;
export import MathHelper;
export import DirectXUtility;
export import UploadBuffer;
export import Geometry;

import Engine;

using Microsoft::WRL::ComPtr;
using namespace std;
using namespace DirectX;
using mt::Engine;

export namespace mt::renderer
{
    class DirectXRenderer 
    {
    protected:
        // Data
        static const int _swap_chain_buffer_count = 2;

        Engine& _engine;

        Camera _camera; // 204 bytes (getting kind of bloated)

        // 64 byte types
        XMFLOAT4X4 _world_transform = Identity4x4(); // Transformation from Local Space to World Space
        XMFLOAT4X4 _view_tranform = Identity4x4();    // Transformation from World Space to the camera's View Space
        XMFLOAT4X4 _projection_transform = Identity4x4();    // Transformation from View Space to Projection Space (Normalized Device Coordinates (GPU Space?))

        // 32 byte type
        vector<D3D12_INPUT_ELEMENT_DESC> mInputLayout; // 32 bytes according to sizeof

        // 24 byte type
        D3D12_VIEWPORT _screen_viewport;

        // 16 byte types
        D3D12_RECT _scissor_rectangle;

        // Pointers - 8 byte each
        HWND _main_window_handle;

        unique_ptr<UploadBuffer<ObjectConstants>> _object_constants_upload_buffer = nullptr;
        unique_ptr<MeshGeometry> _box_mesh_geometry = nullptr;

        ComPtr<IDXGIFactory4> _dx_dxgi_factory;
        ComPtr<IDXGISwapChain> _dx_swap_chain;
        ComPtr<ID3D12Device> _dx_device;

        ComPtr<ID3D12Fence> _fence;

        ComPtr<ID3D12CommandQueue> _dx_command_queue;
        ComPtr<ID3D12CommandAllocator> _dx_command_list_allocator;
        ComPtr<ID3D12GraphicsCommandList> _dx_command_list;

        ComPtr<ID3D12Resource> _swap_chain_buffer[_swap_chain_buffer_count];
        ComPtr<ID3D12Resource> _depth_stencil_buffer;

        ComPtr<ID3D12DescriptorHeap> _dx_rtv_heap;
        ComPtr<ID3D12DescriptorHeap> _dx_dsv_heap;

        ComPtr<ID3D12RootSignature> _dx_root_signature;
        ComPtr<ID3D12DescriptorHeap> _dx_cbv_heap;

        ComPtr<ID3DBlob> _mvs_byte_code;
        ComPtr<ID3DBlob> _mps_byte_code;
        ComPtr<ID3D12PipelineState> _pso;

        // 8 byte types
        UINT64 _current_fence_index = 0;

        POINT _last_mouse_position;

        // 4 byte types
        UINT _rtv_descriptor_size = 0;
        UINT _dsv_descriptor_size = 0;
        UINT _cbv_srv_uav_descriptor_size = 0;
        UINT _4x_msaa_quality = 0;      // quality level of 4X MSAA

        D3D_DRIVER_TYPE _d3d_driver_type = D3D_DRIVER_TYPE_HARDWARE;
        DXGI_FORMAT _back_buffer_format = DXGI_FORMAT_R8G8B8A8_UNORM;
        DXGI_FORMAT _depth_stencil_format = DXGI_FORMAT_D24_UNORM_S8_UINT;

        long long _frames_rendered = 0;

        int _current_back_buffer = 0;
        int _window_width = 0;
        int _window_height = 0;

        float _window_aspect_ratio = 0.0f;

        // Single byte each
        bool _4x_msaa_state = false;
        bool _is_initialized = false;
        bool _is_rendering = false;

        // Accessors
        ID3D12Resource* _getCurrentBackBuffer() const;

        D3D12_CPU_DESCRIPTOR_HANDLE _getCurrentBackBufferView() const;

        D3D12_CPU_DESCRIPTOR_HANDLE _getDepthStencilView() const;

        // Mutators

        void _createCommandList();

        void _createDxCommandObjects();

        void _createSwapChain();

        virtual void _createDescriptorHeaps();

        void _createConstantBuffers();

        void _createRootSignature();

        void _createShadersAndInputLayout();

        void _createPipelineStateObject();

		void _createGeometry();

    public:
        DirectXRenderer(Engine& engine)
            : _engine(engine)
        {}

        ~DirectXRenderer() {
            if (_dx_device != nullptr)
                flush_command_queue();
        }
        
        DirectXRenderer(const DirectXRenderer&) = delete;
        DirectXRenderer(DirectXRenderer&&) = default;
        DirectXRenderer& operator=(const DirectXRenderer&) = delete;
        DirectXRenderer& operator=(DirectXRenderer&&) = default;

        // Accessors
        Camera& getCurrentCamera() { return _camera; } // NOT CONST!!!!

        bool get4xMsaaState() const { return _4x_msaa_state; };

        bool getIsInitialized() const { return _is_initialized; };

        int getSwapChainBufferCount() const { return _swap_chain_buffer_count; };
              
        bool getIsRendering() const { return _is_rendering; }

        float getWindowAspectRatio() const { return _window_aspect_ratio; }

        // TODO: doesn't this belong in the window manager?
        int getWindowWidth() const { return _window_width; }

        int getWindowHeight() const { return _window_height; }

        long long getFramesRendered() const { return _frames_rendered; }

        // Mutators
        bool initializeDirect3d(HWND main_window_handle);
        
        void set4xMsaaState(bool value);

        void render();

        void resize(int client_width, int client_height);

        void update();

        void flush_command_queue();

        // Fence Stuff

        void incrementFence();

        void waitForFence();

        bool isCurrentFenceComplete() { return _fence->GetCompletedValue() >= _current_fence_index; }
    };
}