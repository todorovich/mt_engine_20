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
		DirectX::XMFLOAT4 color;
		DirectX::XMFLOAT3 position;
		DirectX::XMFLOAT3 normal;
		DirectX::XMFLOAT3 right;
		DirectX::XMFLOAT2 texture_coordinate;

		Vertex() {}

		Vertex(
			const DirectX::XMFLOAT3& position,
			const DirectX::XMFLOAT3& normal,
			const DirectX::XMFLOAT3& right,
			const DirectX::XMFLOAT4& color,
			const DirectX::XMFLOAT2& texture_coordinate
		) noexcept
			: position(position)
			, normal(normal)
			, right(right)
			, color(color)
			, texture_coordinate(texture_coordinate)
		{}
	};

	struct MeshData {
		std::vector<Vertex> vertices;
		std::vector<std::uint16_t> indices;
	};
}
