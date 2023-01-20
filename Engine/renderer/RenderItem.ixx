module;

#include <DirectXMath.h>
#include <d3d12.h>

export module RenderItem;

import <cstdint>;

import MathUtility;
import MeshGeometry;

using namespace mt::math;

export namespace mt::renderer
{
	struct RenderItem
	{
		DirectX::XMFLOAT4X4 world_matrix = Identity4x4();

		// Dirty flag indicating the object data has changed and we need to update the constant buffer.
		// Because we have an object cbuffer for each FrameResource, we have to apply the
		// update to each FrameResource.  Thus, when we modify obect data we should set 
		// NumFramesDirty = gNumFrameResources so that each frame resource gets the update.
		int number_of_frames_requiring_update;

		// Index into GPU constant buffer corresponding to the ObjectCB for this render item.
		std::int32_t object_constant_buffer_index = -1;

		MeshGeometry* geometry = nullptr;

		D3D12_PRIMITIVE_TOPOLOGY primitive_topology= D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST;

		// DrawIndexedInstanced parameters.
		std::uint32_t index_count = 0;
		std::uint32_t start_index_location = 0;
		std::uint32_t base_vertex_location = 0;

		RenderItem(int number_of_frame_resources = 1)
			: number_of_frames_requiring_update(number_of_frame_resources)
		{}

		RenderItem(const RenderItem&) = default;
		RenderItem(RenderItem&&) = default;
		RenderItem& operator=(const RenderItem&) = default;
		RenderItem& operator=(RenderItem&&) = default;

		bool requiresUpdate() const noexcept
		{
			return number_of_frames_requiring_update > 0;
		}

		void objectConstantsUpdated() noexcept
		{
			--number_of_frames_requiring_update;
		}
	};
}