module;

#include <DirectXMath.h>

export module MeshData;

export import <cstdint>;
export import <vector>;

export import MeshGeometry;

export namespace mt::geometry
{
	struct Vertex
	{
		DirectX::XMFLOAT3 position;
		DirectX::XMFLOAT3 normal;
		DirectX::XMFLOAT3 tangent;
		DirectX::XMFLOAT2 texture_coordinate;

		Vertex() {}

		Vertex(
			const DirectX::XMFLOAT3& position,
			const DirectX::XMFLOAT3& normal,
			const DirectX::XMFLOAT3& tangent,
			const DirectX::XMFLOAT2& texture_coordinate
		)
			: position(position)
			, normal(normal)
			, tangent(tangent)
			, texture_coordinate(texture_coordinate)
		{}

		Vertex(
			float px, float py, float pz,
			float nx, float ny, float nz,
			float tx, float ty, float tz,
			float u, float v
		)
			: position(px, py, pz)
			, normal(nx, ny, nz)
			, tangent(tx, ty, tz)
			, texture_coordinate(u, v)
		{}
	};

	struct MeshData {
		std::vector<Vertex> vertices;
		std::vector<std::uint16_t> indices;
	};
}
