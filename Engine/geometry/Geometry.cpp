module;

#include <DirectXMath.h>

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

		DirectX::XMFLOAT4 yellow  { 1.0f, 1.0f, 0.0f, 1.0f };
		DirectX::XMFLOAT4 green   { 0.0f, 1.0f, 0.0f, 1.0f };
		DirectX::XMFLOAT4 red     { 1.0f, 0.0f, 0.0f, 1.0f };
		DirectX::XMFLOAT4 black   { 0.0f, 0.0f, 0.0f, 1.0f };
		DirectX::XMFLOAT4 white   { 1.0f, 1.0f, 1.0f, 1.0f };
		DirectX::XMFLOAT4 cyan    { 0.0f, 1.0f, 1.0f, 1.0f };
		DirectX::XMFLOAT4 magenta { 1.0f, 0.0f, 1.0f, 1.0f };
		DirectX::XMFLOAT4 blue    { 0.0f, 0.0f, 1.0f, 1.0f };

		DirectX::XMFLOAT3 left_top_back      { -half_width, +half_height, +half_depth };
		DirectX::XMFLOAT3 right_top_back     { +half_width, +half_height, +half_depth };
		DirectX::XMFLOAT3 left_bottom_back   { -half_width, -half_height, +half_depth };
		DirectX::XMFLOAT3 right_bottom_back  { +half_width, -half_height, +half_depth };
		DirectX::XMFLOAT3 left_top_front     { -half_width, +half_height, -half_depth };
		DirectX::XMFLOAT3 right_top_front    { +half_width, +half_height, -half_depth };
		DirectX::XMFLOAT3 left_bottom_front  { -half_width, -half_height, -half_depth };
		DirectX::XMFLOAT3 right_bottom_front { +half_width, -half_height, -half_depth };

		DirectX::XMFLOAT3 right  { +1.0f, +0.0f, +0.0 };
		DirectX::XMFLOAT3 left   { -1.0f, +0.0f, +0.0 };
		DirectX::XMFLOAT3 top    { +0.0f, +1.0f, +0.0 };
		DirectX::XMFLOAT3 bottom { +0.0f, -1.0f, +0.0 };
		DirectX::XMFLOAT3 back   { +0.0f, +0.0f, +1.0 };
		DirectX::XMFLOAT3 front  { +0.0f, +0.0f, -1.0 };

		DirectX::XMFLOAT2 left_bottom  { 0.0f, 0.0f };
		DirectX::XMFLOAT2 left_top     { 0.0f, 1.0f };
		DirectX::XMFLOAT2 right_top    { 1.0f, 1.0f };
		DirectX::XMFLOAT2 right_bottom { 1.0f, 0.0f };

		return MeshData {
			std::vector<Vertex> {
				Vertex(left_bottom_front,	front,	right,	magenta,	left_bottom),
				Vertex(left_top_front,		front,	right,	white,		left_top),
				Vertex(right_top_front,		front,	right,	cyan,		right_top),
				Vertex(right_bottom_front,	front,	right,	blue,		right_bottom),
				Vertex(right_bottom_back,	back,	left,	black,		left_bottom),
				Vertex(right_top_back,		back,	left,	green,		left_top),
				Vertex(left_top_back,		back,	left,	yellow,		right_top),
				Vertex(left_bottom_back,	back,	left,	red,		right_bottom),
				Vertex(left_top_front,		top,	right,	white,		left_bottom),
				Vertex(left_top_back,		top,	right,	yellow,		left_top),
				Vertex(right_top_back,		top,	right,	green,		right_top),
				Vertex(right_top_front,		top,	right,	cyan,		right_bottom),
				Vertex(right_bottom_front,	bottom,	left,	blue,		left_bottom),
				Vertex(right_bottom_back,	bottom,	left,	black,		left_top),
				Vertex(left_bottom_back,	bottom,	left,	red,		right_top),
				Vertex(left_bottom_front,	bottom,	left,	magenta,	right_bottom),
				Vertex(left_bottom_back,	left,	front,	red,		left_bottom),
				Vertex(left_top_back,		left,	front,	yellow,		left_top),
				Vertex(left_top_front,		left,	front,	white,		right_top),
				Vertex(left_bottom_front,	left,	front,	magenta,	right_bottom),
				Vertex(right_bottom_front,	right,	back,	blue,		left_bottom),
				Vertex(right_top_front,		right,	back,	cyan,		left_top),
				Vertex(right_top_back,		right,	back,	green,		right_top),
				Vertex(right_bottom_back,	right,	back,	black,		right_bottom),
			},
			std::vector<std::uint16_t> {
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
