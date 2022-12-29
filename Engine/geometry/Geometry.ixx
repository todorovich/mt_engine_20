module;

#include <wrl.h>
#include <d3d12.h>

export module Geometry;

import <memory>;

export import MeshData;
export import MeshGeometry;
export import SubmeshGeometry;

using Microsoft::WRL::ComPtr;

export namespace mt::geometry 
{
	std::unique_ptr<MeshData> createBoxGeometry(float width, float height, float depth);

 	// TODO: anything related to generating geometry should be in the Geometry module.
    std::unique_ptr<MeshGeometry> createBoxGeometry(
        const ComPtr<ID3D12Device>& dx_device, const ComPtr <ID3D12GraphicsCommandList>& dx_command_list
    );

}
