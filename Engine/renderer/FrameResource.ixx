// Copyright 2023 Micho Todorovich, all rights reserved.
module;

#include <wrl.h>
#include <DirectXMath.h>
#include <d3d12.h>

export module FrameResource;

import <cstdint>;
import <ctime>;
import <memory>;
import <stdexcept>;

import MathUtility;
import UploadBuffer;
import Error;

using namespace std::literals;

export namespace mt::renderer 
{
	struct ObjectConstants
	{
        // Used to go from Object space to World space
		DirectX::XMFLOAT4X4 world_matrix = mt::math::Identity4x4();
	};

    struct PassConstants
    {
        // Used to go from World space to View space
        DirectX::XMFLOAT4X4 view_matrix = mt::math::Identity4x4();
        // Used to go from View space to World Space
        DirectX::XMFLOAT4X4 inverse_view_matrix = mt::math::Identity4x4();
        // Used to go from View space to (homogeneous) clip space
        DirectX::XMFLOAT4X4 projection_matrix = mt::math::Identity4x4();
        // Used to go from Clip Space to View Space
        DirectX::XMFLOAT4X4 inverse_projection_matrix = mt::math::Identity4x4();
        // Used to go from Object Space to Clip Space
        DirectX::XMFLOAT4X4 view_and_projection_matrix = mt::math::Identity4x4();
        // Used to go from Clip space to Object space
        DirectX::XMFLOAT4X4 inverse_view_and_projection_matrix = mt::math::Identity4x4();
        
        DirectX::XMFLOAT3 eye_position_world_space = { 0.0f, 0.0f, 0.0f };

        float cbPerObjectPad1 = 0.0f;
        
        DirectX::XMFLOAT2 render_target_size = { 0.0f, 0.0f };
        DirectX::XMFLOAT2 inverse_render_target_size = { 0.0f, 0.0f };
        
        float near_z_clipping_distance = 0.0f;
        float far_z_clipping_distance= 0.0f;
        float total_time_ns = 0.0f;
        float delta_time_ns = 0.0f;
    };

    struct FrameResource 
    {
        FrameResource(ID3D12Device* device, std::uint32_t passCount, std::uint32_t objectCount);
        FrameResource(const FrameResource& frameResource) = delete;
        FrameResource(FrameResource&& frameResource) = delete;
        FrameResource& operator=(const FrameResource& frameResource) = delete;
        FrameResource& operator=(FrameResource&& frameResource) = delete;
        ~FrameResource() = default;

        Microsoft::WRL::ComPtr<ID3D12CommandAllocator> command_list_allocator;

        std::unique_ptr<UploadBuffer<PassConstants>> pass_constants_upload_buffer = nullptr;
        std::unique_ptr<UploadBuffer<ObjectConstants>> object_constants_upload_buffer = nullptr;

        std::size_t fence = std::size_t{ 0 };
    };
}

module :private;

mt::renderer::FrameResource::FrameResource(
    ID3D12Device* dx_device, std::uint32_t pass_count, std::uint32_t object_count
)
{
	if (FAILED(dx_device->CreateCommandAllocator(
		D3D12_COMMAND_LIST_TYPE_DIRECT,
		IID_PPV_ARGS(command_list_allocator.GetAddressOf())
	)))
	{
		throw new std::runtime_error("Unable to present the swap chain (swap front/back buffers).");
	}

    pass_constants_upload_buffer = std::make_unique<UploadBuffer<PassConstants>>(dx_device, pass_count, true);
    object_constants_upload_buffer = std::make_unique<UploadBuffer<ObjectConstants>>(dx_device, object_count, true);
}
