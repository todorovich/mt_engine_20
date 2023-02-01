// Copyright 2023 Micho Todorovich, all rights reserved.
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

#include <dxgidebug.h>

// Link necessary d3d12 libraries.
#pragma comment(lib, "d3dcompiler.lib")
#pragma comment(lib, "D3D12.lib")
#pragma comment(lib, "dxgi.lib")
#pragma comment(lib, "dxguid.lib")

export module DirectXRenderer;

import <ctime>;

export import Engine;
export import Error;
export import FrameResource;
export import Geometry;
export import MathUtility;
export import UploadBuffer;
export import RenderItem;

using Microsoft::WRL::ComPtr;
using namespace std;
using namespace DirectX;

using mt::error::Error;

export namespace mt::renderer
{
	class DirectXRenderer : public RendererInterface
	{
    protected:
        // Data
		static const std::size_t _number_of_frame_resources = 2;

        Engine& _engine;

        // 32 byte type
        vector<D3D12_INPUT_ELEMENT_DESC> mInputLayout; // 32 bytes according to sizeof

        // 24 byte type
        D3D12_VIEWPORT _screen_viewport;

        // 16 byte types
        D3D12_RECT _scissor_rectangle;

        // Pointers - 8 byte each
        HWND _main_window_handle;

		ComPtr<IDXGIFactory4> _dx_dxgi_factory;
		ComPtr<IDXGISwapChain> _dx_swap_chain;
        ComPtr<ID3D12Device> _dx_device;

		ComPtr<ID3D12Fence> _fence;

        ComPtr<ID3D12CommandQueue> _dx_command_queue;

		Microsoft::WRL::ComPtr<ID3D12CommandAllocator> _dx_command_list_allocator;
		ComPtr<ID3D12GraphicsCommandList> _dx_command_list;

        ComPtr<ID3D12Resource> _swap_chain_buffer[getSwapChainBufferCount()];
        ComPtr<ID3D12Resource> _depth_stencil_buffer;

        ComPtr<ID3D12DescriptorHeap> _dx_rtv_heap;
        ComPtr<ID3D12DescriptorHeap> _dx_dsv_heap;

		vector<std::unique_ptr<RenderItem>> _render_items;

        ComPtr<ID3D12RootSignature> _dx_root_signature;
        ComPtr<ID3D12DescriptorHeap> _dx_cbv_heap;

		unique_ptr<MeshGeometry> _box_mesh_geometry = nullptr;

		ComPtr<ID3DBlob> _mvs_byte_code;
		ComPtr<ID3DBlob> _mps_byte_code;

		vector<ComPtr<ID3D12PipelineState>> _pipeline_state_objects{std::size_t{2}};

		vector<unique_ptr<FrameResource>> _frame_resources =
			vector<unique_ptr<FrameResource>>(_number_of_frame_resources);

        // 8 byte types
        UINT64 _fence_index = 0;
		std::size_t _frame_resource_index = 0;
		std::size_t _current_back_buffer = 0;

        // 4 byte types
        UINT _rtv_descriptor_size = 0;
        UINT _dsv_descriptor_size = 0;
        UINT _cbv_srv_uav_descriptor_size = 0;
        UINT _4x_msaa_quality = 0;      // quality level of 4X MSAA
		UINT _pass_constant_buffer_offset = 0;

        DXGI_FORMAT _back_buffer_format = DXGI_FORMAT_R8G8B8A8_UNORM;
        DXGI_FORMAT _depth_stencil_format = DXGI_FORMAT_D24_UNORM_S8_UINT;

        int v = 0;

		FrameResource* _getCurrentFrameResource() const noexcept
		{
			return _frame_resources[_frame_resource_index].get();
		};
        // Accessors
        ID3D12Resource* _getCurrentBackBuffer() const noexcept
		{
			return _swap_chain_buffer[_current_back_buffer].Get();
		};

        D3D12_CPU_DESCRIPTOR_HANDLE _getCurrentBackBufferView() const noexcept;

        D3D12_CPU_DESCRIPTOR_HANDLE _getDepthStencilView() const noexcept;

        // Mutators

		[[nodiscard]] std::expected<void, Error> _flushCommandQueue(std::size_t fence_index) noexcept;

		[[nodiscard]] std::expected<void, Error> _createCommandList() noexcept;

		[[nodiscard]] std::expected<void, Error> _createDxCommandObjects() noexcept;

		void _drawRenderItems(
			ID3D12GraphicsCommandList* command_list, const std::vector<std::unique_ptr<RenderItem>>& render_items
		) noexcept;

		[[nodiscard]] std::expected<void, Error> _createSwapChain() noexcept;

		[[nodiscard]] std::expected<void, Error> _createDescriptorHeaps() noexcept;

		[[nodiscard]] std::expected<void, Error> _createRootSignature() noexcept;

		[[nodiscard]] std::expected<void, Error> _createShadersAndInputLayout() noexcept;

		[[nodiscard]] std::expected<void, Error> _createGeometry() noexcept;

		void _createRenderItems() noexcept;

		void _createFrameResources() noexcept;

        void _createConstantBufferViews() noexcept;

		[[nodiscard]] std::expected<void, Error> _createPipelineStateObject() noexcept;

		void _updateObjectConstants();

		void _updatePassConstants();

    public:
        DirectXRenderer(Engine& engine)
            : _engine(engine)
        {}

        virtual ~DirectXRenderer() = default;
        
        DirectXRenderer(const DirectXRenderer&) = delete;
        DirectXRenderer(DirectXRenderer&&) = default;
        DirectXRenderer& operator=(const DirectXRenderer&) = delete;
        DirectXRenderer& operator=(DirectXRenderer&&) = default;

        // Accessors

		[[nodiscard]] virtual std::expected<void, Error> shutdown() noexcept override
		{
			return _flushCommandQueue(_fence_index);
		}

		[[nodiscard]] bool isCurrentFenceComplete() noexcept { return _fence->GetCompletedValue() >= _fence_index; }

		// Mutators

		[[nodiscard]] virtual std::expected<void, Error> set4xMsaaState(bool value) noexcept override;

		[[nodiscard]] std::expected<void, Error> resize(int client_width, int client_height) noexcept override;

		[[nodiscard]] std::expected<void, Error> render() noexcept override;

        void update() override;

		[[nodiscard]] std::expected<void, Error> initialize() noexcept override;
    };
}