module;

#include <DirectXMath.h>

export module Vertex;

export namespace mt::geometry 
{
	struct Vertex {
		DirectX::XMFLOAT3 position;
		DirectX::XMFLOAT4 color;
	};
}