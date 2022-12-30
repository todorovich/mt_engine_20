module Geometry;

import <vector>;
import <cstdint>;

namespace mt::geometry
{
	constexpr MeshData createBoxGeometry(float width, float height, float depth)
	{
		float half_width = width / 2.0f;
		float half_height = height / 2.0f;
		float half_depth = depth / 2.0f;

		return MeshData{
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
				Vertex(-half_width, +half_height, -half_depth, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f),
				Vertex(-half_width, +half_height, +half_depth, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f),
				Vertex(+half_width, +half_height, +half_depth, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f),
				Vertex(+half_width, +half_height, -half_depth, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f),
				// bottom face
				Vertex(-half_width, -half_height, -half_depth, 0.0f, -1.0f, 0.0f, -1.0f, 0.0f, 0.0f, 0.0f, 1.0f),
				Vertex(+half_width, -half_height, -half_depth, 0.0f, -1.0f, 0.0f, -1.0f, 0.0f, 0.0f, 0.0f, 0.0f),
				Vertex(+half_width, -half_height, +half_depth, 0.0f, -1.0f, 0.0f, -1.0f, 0.0f, 0.0f, 1.0f, 0.0f),
				Vertex(-half_width, -half_height, +half_depth, 0.0f, -1.0f, 0.0f, -1.0f, 0.0f, 0.0f, 1.0f, 1.0f),
				// left face
				Vertex(-half_width, -half_height, +half_depth, -1.0f, 0.0f, 0.0f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f),
				Vertex(-half_width, +half_height, +half_depth, -1.0f, 0.0f, 0.0f, 0.0f, 0.0f, -1.0f, 1.0f, 0.0f),
				Vertex(-half_width, +half_height, -half_depth, -1.0f, 0.0f, 0.0f, 0.0f, 0.0f, -1.0f, 1.0f, 1.0f),
				Vertex(-half_width, -half_height, -half_depth, -1.0f, 0.0f, 0.0f, 0.0f, 0.0f, -1.0f, 0.0f, 1.0f),
				// right face
				Vertex(+half_width, -half_height, -half_depth, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f),
				Vertex(+half_width, +half_height, -half_depth, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f),
				Vertex(+half_width, +half_height, +half_depth, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f),
				Vertex(+half_width, -half_height, +half_depth, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f),
			},
			std::vector < std::uint16_t > {
				// front face
				0, 1, 2,
				0, 2, 3,

				// back face
				4, 5, 6,
				4, 6, 7,

				// top face
				8, 9, 10,
				8, 10, 11,

				// bottom face
				12, 13, 14,
				12, 14, 15,

				// left face
				16, 17, 18,
				16, 18, 19,

				// right face
				20, 21, 22,
				20, 22, 23
			}
		};
	}
}