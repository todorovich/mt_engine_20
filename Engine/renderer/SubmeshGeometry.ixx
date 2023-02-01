// Copyright 2023 Micho Todorovich, all rights reserved.
module;

#include <DirectXCollision.h>

export module SubmeshGeometry;

import <cstdint>;

namespace mt::renderer
{
    // Defines a subrange of geometry in a MeshGeometry.  This is for when multiple
    // geometries are stored in one vertex and index buffer.  It provides the offsets
    // and data needed to draw a subset of geometry stores in the vertex and index 
    // buffers so that we can implement the technique described by Figure 6.3.
    export struct SubmeshGeometry
    {
        std::uint32_t index_count;
        std::uint32_t start_index_location;
        std::uint32_t base_vertex_location;

        // Bounding box of the geometry defined by this submesh. 
        // This is used in later chapters of the book.
        DirectX::BoundingBox bounds;
    };
}