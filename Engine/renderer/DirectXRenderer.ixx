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
export import FrameResource;
export import Geometry;
export import MathUtility;
export import UploadBuffer;
export import RenderItem;
export import renderer.Vertex;

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

		ComPtr<ID3D12CommandAllocator> _dx_command_list_allocator;
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

		[[nodiscard]] std::expected<void, std::error_condition> _flushCommandQueue() noexcept;

		[[nodiscard]] std::expected<void, std::error_condition> _createCommandList() noexcept;

		[[nodiscard]] std::expected<void, std::error_condition> _createDxCommandObjects() noexcept;

		void _drawRenderItems(
			ID3D12GraphicsCommandList* command_list, const std::vector<std::unique_ptr<RenderItem>>& render_items
		) noexcept;

		[[nodiscard]] std::expected<void, std::error_condition> _createSwapChain() noexcept;

		[[nodiscard]] std::expected<void, std::error_condition> _createDescriptorHeaps() noexcept;

		[[nodiscard]] std::expected<void, std::error_condition> _createRootSignature() noexcept;

		[[nodiscard]] std::expected<void, std::error_condition> _createShadersAndInputLayout() noexcept;

		[[nodiscard]] std::expected<void, std::error_condition> _createGeometry() noexcept;

		void _createRenderItems() noexcept;

		[[nodiscard]] std::expected<void, std::error_condition> _createFrameResources() noexcept;

        void _createConstantBufferViews() noexcept;

		[[nodiscard]] std::expected<void, std::error_condition> _createPipelineStateObject() noexcept;

		void _updateObjectConstants() noexcept;

		void _updatePassConstants() noexcept;

    public:
        DirectXRenderer(Engine& engine) noexcept
            : RendererInterface(105.0f, engine.getWindowManager()->getWindowAspectRatio())
			, _engine(engine)
        {}

        virtual ~DirectXRenderer() noexcept = default;
        DirectXRenderer(const DirectXRenderer&) noexcept = delete;
        DirectXRenderer(DirectXRenderer&&) noexcept = default;
        DirectXRenderer& operator=(const DirectXRenderer&) noexcept = delete;
        DirectXRenderer& operator=(DirectXRenderer&&) noexcept = default;

        // Accessors

		[[nodiscard]] virtual std::expected<void, std::error_condition> shutdown() noexcept override
		{
			if constexpr (mt::IS_DEBUG) OutputDebugStringW(L"Renderer::shutdown()\n");
			return _flushCommandQueue();
		}

		[[nodiscard]] bool isCurrentFenceComplete() noexcept { return _fence->GetCompletedValue() >= _fence_index; }

		// Mutators

		[[nodiscard]] std::expected<void, std::error_condition> set4xMsaaState(bool value) noexcept override;

		[[nodiscard]] std::expected<void, std::error_condition> onResize() noexcept override;

		[[nodiscard]] std::expected<void, std::error_condition> render() noexcept override;

		[[nodiscard]] std::expected<void, std::error_condition> update() noexcept override;

		[[nodiscard]] std::expected<void, std::error_condition> initialize() noexcept override;
    };
}