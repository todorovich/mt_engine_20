// Copyright 2023 Micho Todorovich, all rights reserved.
module;

#include <catch.hpp>
#include <windows.h>

export module MicrosoftTests;

import <cstdint>;

TEST_CASE("UNIT is a uint32_t", "[Aliases]")
{
    bool is_same = std::is_same<std::uint32_t, UINT>();
    REQUIRE(is_same);
}