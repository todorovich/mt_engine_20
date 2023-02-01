// Copyright 2023 Micho Todorovich, all rights reserved.
export module Geometry;

export import <memory>;

export import MeshData;
export import MeshGeometry;
export import SubmeshGeometry;

export namespace mt::geometry 
{
	constexpr MeshData createBoxGeometry(float width, float height, float depth) noexcept;
}
