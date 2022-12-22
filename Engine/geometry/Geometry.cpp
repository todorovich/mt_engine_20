module;

#include <wrl.h>
#include <d3dcompiler.h>
#include <d3d12.h>
#include <DirectXMath.h>
#include <DirectXColors.h>

//#include <DirectXCollision.h>

module Geometry;

import <array>;
import <memory>;
import <cstdint>;

import DirectXUtility;
import Geometry;

using namespace mt::geometry;

using mt::renderer::throwIfFailed;
using mt::renderer::createDefaultBuffer;
using Microsoft::WRL::ComPtr;

namespace mt::geometry
{
	std::unique_ptr<MeshGeometry> createBoxGeometry(
		const ComPtr<ID3D12Device>& dx_device, const ComPtr <ID3D12GraphicsCommandList>& dx_command_list
	)
	{
		std::array<Vertex, 8> vertices
		{
			Vertex({ DirectX::XMFLOAT3(-1.0f, -1.0f, -1.0f), DirectX::XMFLOAT4(DirectX::Colors::White) }),
			Vertex({ DirectX::XMFLOAT3(-1.0f, +1.0f, -1.0f), DirectX::XMFLOAT4(DirectX::Colors::Black) }),
			Vertex({ DirectX::XMFLOAT3(+1.0f, +1.0f, -1.0f), DirectX::XMFLOAT4(DirectX::Colors::Red) }),
			Vertex({ DirectX::XMFLOAT3(+1.0f, -1.0f, -1.0f), DirectX::XMFLOAT4(DirectX::Colors::Green) }),
			Vertex({ DirectX::XMFLOAT3(-1.0f, -1.0f, +1.0f), DirectX::XMFLOAT4(DirectX::Colors::Blue) }),
			Vertex({ DirectX::XMFLOAT3(-1.0f, +1.0f, +1.0f), DirectX::XMFLOAT4(DirectX::Colors::Yellow) }),
			Vertex({ DirectX::XMFLOAT3(+1.0f, +1.0f, +1.0f), DirectX::XMFLOAT4(DirectX::Colors::Cyan) }),
			Vertex({ DirectX::XMFLOAT3(+1.0f, -1.0f, +1.0f), DirectX::XMFLOAT4(DirectX::Colors::Magenta) })
		};

		std::array<std::uint16_t, 36> indices =
		{
			// front face
			0, 1, 2,
			0, 2, 3,

			// back face
			4, 6, 5,
			4, 7, 6,

			// left face
			4, 5, 1,
			4, 1, 0,

			// right face
			3, 2, 6,
			3, 6, 7,

			// top face
			1, 5, 6,
			1, 6, 2,

			// bottom face
			4, 0, 3,
			4, 3, 7
		};

		const UINT vbByteSize = (UINT)vertices.size() * sizeof(Vertex);
		const UINT ibByteSize = (UINT)indices.size() * sizeof(std::uint16_t);

		std::unique_ptr<mt::geometry::MeshGeometry> _box_mesh_geometry = std::make_unique<MeshGeometry>();
		_box_mesh_geometry->name = "boxGeo";

		throwIfFailed(
			D3DCreateBlob(vbByteSize, &_box_mesh_geometry->vertex_buffer_cpu),
			__FUNCTION__,
			__FILE__,
			__LINE__
		);
		memcpy(_box_mesh_geometry->vertex_buffer_cpu->GetBufferPointer(), vertices.data(), vbByteSize);

		throwIfFailed(
			D3DCreateBlob(ibByteSize, &_box_mesh_geometry->index_buffer_cpu),
			__FUNCTION__,
			__FILE__,
			__LINE__
		);
		memcpy(_box_mesh_geometry->index_buffer_cpu->GetBufferPointer(), indices.data(), ibByteSize);

		_box_mesh_geometry->vertex_buffer_gpu = createDefaultBuffer(
			dx_device.Get(),
			dx_command_list.Get(),
			vertices.data(),
			vbByteSize,
			_box_mesh_geometry->vertex_buffer_uploader
		);

		_box_mesh_geometry->index_buffer_gpu = createDefaultBuffer(
			dx_device.Get(),
			dx_command_list.Get(),
			indices.data(),
			ibByteSize,
			_box_mesh_geometry->index_buffer_uploader
		);

		_box_mesh_geometry->vertex_byte_stride = sizeof(Vertex);
		_box_mesh_geometry->vertex_buffer_byte_size = vbByteSize;
		_box_mesh_geometry->index_format = DXGI_FORMAT_R16_UINT;
		_box_mesh_geometry->index_buffer_byte_size = ibByteSize;

		_box_mesh_geometry->draw_arguments.emplace(
			std::make_pair("box", SubmeshGeometry{ static_cast<std::uint32_t>(indices.size()) , 0 , 0, DirectX::BoundingBox()})
		);

		return std::move(_box_mesh_geometry);
	}
}