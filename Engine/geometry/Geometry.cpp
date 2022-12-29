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
	std::unique_ptr<MeshData> createBoxGeometry(float width, float height, float depth)
	{
		float half_width = width / 2.0f;
		float half_height = height / 2.0f;
		float half_depth = depth / 2.0f;

		return std::make_unique<MeshData>(
			std::vector < Vertex > {
				// front face
				Vertex(-half_width, -half_height, -half_depth, 0.0f, 0.0f, -1.0f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f),
				Vertex(-half_width, +half_height, -half_depth, 0.0f, 0.0f, -1.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f),
				Vertex(+half_width, +half_height, -half_depth, 0.0f, 0.0f, -1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f),
				Vertex(+half_width, -half_height, -half_depth, 0.0f, 0.0f, -1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f),
				// back face
				Vertex(-half_width, -half_height, +half_depth, 0.0f, 0.0f, -1.0f, -1.0f, 0.0f, 0.0f, 0.0f, 1.0f),
				Vertex(+half_width, -half_height, +half_depth, 0.0f, 0.0f, -1.0f, -1.0f, 0.0f, 0.0f, 0.0f, 0.0f),
				Vertex(+half_width, +half_height, +half_depth, 0.0f, 0.0f, -1.0f, -1.0f, 0.0f, 0.0f, 1.0f, 0.0f),
				Vertex(-half_width, +half_height, +half_depth, 0.0f, 0.0f, -1.0f, -1.0f, 0.0f, 0.0f, 1.0f, 1.0f),
				// top face
				Vertex(-half_width, +half_height, -half_depth, 0.0f, 1.0f, -1.0f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f),
				Vertex(-half_width, +half_height, +half_depth, 0.0f, 1.0f, -1.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f),
				Vertex(+half_width, +half_height, +half_depth, 0.0f, 1.0f, -1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f),
				Vertex(+half_width, +half_height, -half_depth, 0.0f, 1.0f, -1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f),
				// bottom face
				Vertex(-half_width, -half_height, -half_depth, 0.0f, -1.0f, -1.0f, -1.0f, 0.0f, 0.0f, 0.0f, 1.0f),
				Vertex(-half_width, -half_height, -half_depth, 0.0f, -1.0f, -1.0f, -1.0f, 0.0f, 0.0f, 0.0f, 0.0f),
				Vertex(+half_width, -half_height, +half_depth, 0.0f, -1.0f, -1.0f, -1.0f, 0.0f, 0.0f, 1.0f, 0.0f),
				Vertex(+half_width, -half_height, +half_depth, 0.0f, -1.0f, -1.0f, -1.0f, 0.0f, 0.0f, 1.0f, 1.0f),
				// left face
				Vertex(-half_width, -half_height, +half_depth, -1.0f, 0.0f, -1.0f, 0.0f, 0.0f, -1.0f, 0.0f, 1.0f),
				Vertex(+half_width, +half_height, +half_depth, -1.0f, 0.0f, -1.0f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f),
				Vertex(+half_width, +half_height, -half_depth, -1.0f, 0.0f, -1.0f, 0.0f, 0.0f, -1.0f, 1.0f, 0.0f),
				Vertex(-half_width, -half_height, -half_depth, -1.0f, 0.0f, -1.0f, 0.0f, 0.0f, -1.0f, 1.0f, 1.0f),
				// right face
				Vertex(+half_width, -half_height, -half_depth, 1.0f, 0.0f, -1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f),
				Vertex(+half_width, +half_height, -half_depth, 1.0f, 0.0f, -1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f),
				Vertex(+half_width, +half_height, +half_depth, 1.0f, 0.0f, -1.0f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f),
				Vertex(+half_width, -half_height, +half_depth, 1.0f, 0.0f, -1.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f),
			},
			std::vector<std::uint16_t> {
				// front face
				0, 1, 2,
				0, 2, 3,

				// back face
				4, 5, 6,
				5, 6, 7,

				// left face
				8, 9, 10,
				8, 10, 11,

				// right face
				12, 13, 14,
				12, 14, 15,

				// top face
				16, 17, 18,
				16, 18, 19,

				// bottom face
				20, 21, 22,
				20, 22, 23
			}
		);
	}

	std::unique_ptr<MeshGeometry> createBoxGeometry(
		const ComPtr<ID3D12Device>& dx_device, const ComPtr <ID3D12GraphicsCommandList>& dx_command_list
	)
	{
		std::array<mt::renderer::Vertex, 8> vertices
		{
			mt::renderer::Vertex({ DirectX::XMFLOAT3(-1.0f, -1.0f, -1.0f), DirectX::XMFLOAT4(DirectX::Colors::White) }),
			mt::renderer::Vertex({ DirectX::XMFLOAT3(-1.0f, +1.0f, -1.0f), DirectX::XMFLOAT4(DirectX::Colors::Black) }),
			mt::renderer::Vertex({ DirectX::XMFLOAT3(+1.0f, +1.0f, -1.0f), DirectX::XMFLOAT4(DirectX::Colors::Red) }),
			mt::renderer::Vertex({ DirectX::XMFLOAT3(+1.0f, -1.0f, -1.0f), DirectX::XMFLOAT4(DirectX::Colors::Green) }),
			mt::renderer::Vertex({ DirectX::XMFLOAT3(-1.0f, -1.0f, +1.0f), DirectX::XMFLOAT4(DirectX::Colors::Blue) }),
			mt::renderer::Vertex({ DirectX::XMFLOAT3(-1.0f, +1.0f, +1.0f), DirectX::XMFLOAT4(DirectX::Colors::Yellow) }),
			mt::renderer::Vertex({ DirectX::XMFLOAT3(+1.0f, +1.0f, +1.0f), DirectX::XMFLOAT4(DirectX::Colors::Cyan) }),
			mt::renderer::Vertex({ DirectX::XMFLOAT3(+1.0f, -1.0f, +1.0f), DirectX::XMFLOAT4(DirectX::Colors::Magenta) })
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