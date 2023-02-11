// Copyright 2023 Micho Todorovich, all rights reserved.
module;

#include <DirectXMath.h>

export module renderer.Vertex;

export namespace mt::renderer
{
	// TODO: Find me a better home.
	struct Vertex {
		DirectX::XMFLOAT3 position;
		DirectX::XMFLOAT4 color;
	};
}