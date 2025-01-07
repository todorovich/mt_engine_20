module;

#include <catch.hpp>

export module MakeUniqueTests;

import MakeUnique;
import Engine;
import MeshGeometry;
import InputTestEngine;
import StandardEngine;

namespace mt::memory::test
{
    class ErrorConstructibleTestClass {
    public:
        ErrorConstructibleTestClass(std::error_condition error, int code) noexcept
            : error_(error), code_(code)
        {
        }

    private:
        std::error_condition error_;
        int code_;
    };

    class NonErrorConstructibleTestClass {
    public:
        NonErrorConstructibleTestClass(/* No std::error_condition */ int code) noexcept
            : code_(code)
        {
        }

    private:
        int code_;
    };

    class TestEngine : public Engine
    {
	public:
		TestEngine(std::error_condition) noexcept : Engine(){}
    };


    TEST_CASE("ErrorConditionConstructible concept") {
        SECTION("ErrorConstructibleClass satisfies ErrorConditionConstructible") {
            STATIC_REQUIRE(ErrorConditionConstructible<ErrorConstructibleTestClass, int>);
        }

        SECTION("NonErrorConstructibleClass does not satisfy ErrorConditionConstructible") {
            STATIC_REQUIRE_FALSE(ErrorConditionConstructible<NonErrorConstructibleTestClass, int>);
        }
    }

    TEST_CASE("mt::Engine can be created using ExpectMakeUnique", "[Engine]") {
        auto expected = mt::memory::ExpectMakeUnique<TestEngine>();
        REQUIRE(expected);

        auto engine = std::move(expected.value());
        REQUIRE(engine != nullptr);
    }

    TEST_CASE("mt::StandardEngine can be created using ExpectMakeUnique", "[Engine]") {
        auto expected = mt::memory::ExpectMakeUnique<mt::StandardEngine>();
        REQUIRE(expected);

        auto engine = std::move(expected.value());
        REQUIRE(engine != nullptr);
    }
}
