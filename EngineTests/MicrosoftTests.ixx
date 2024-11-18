// Copyright 2023 Micho Todorovich, all rights reserved.
module;

#include <catch.hpp>

export module MicrosoftTests;

import std;
import Handle;
import Windows;

using namespace windows;

TEST_CASE("UNIT is a uint32_t", "[Aliases]")
{
    bool is_same = std::is_same<std::uint32_t, UINT>();
    REQUIRE(is_same);
}

TEST_CASE("Handle and mt::memory::Handle are the same type", "[Aliases]")
{
	bool is_same = std::is_same<HANDLE, mt::memory::Handle>();
	REQUIRE(is_same);
}