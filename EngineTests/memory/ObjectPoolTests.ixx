module;

#include <catch.hpp>
#include <thread>

export module ObjectPool.Tests;

import ObjectPool;
import Error;
import Engine;

using namespace mt::memory;
using namespace mt::error;

class TestObject {
public:
    int value;
    TestObject(int v) : value(v) {}
    ~TestObject() = default;
};

TEST_CASE("ObjectPool Allocate and Deallocate", "[ObjectPool]") {
	mt::Engine engine;
    std::error_condition error;
    ObjectPool<TestObject, 10> pool(engine, error);

    REQUIRE_FALSE(error);

    auto obj1 = pool.allocate(1);
    REQUIRE(obj1.has_value());
    REQUIRE(obj1.value()->value == 1);

    auto obj2 = pool.allocate(2);
    REQUIRE(obj2.has_value());
    REQUIRE(obj2.value()->value == 2);

    obj1.value().reset();
    obj2.value().reset();

    REQUIRE(pool.size() == 0);
}

TEST_CASE("ObjectPool Out of Memory", "[ObjectPool]") {
    mt::Engine engine;
    std::error_condition error;
    ObjectPool<TestObject, 2> pool(engine, error);

    REQUIRE_FALSE(error);

    auto obj1 = pool.allocate(1);
    auto obj2 = pool.allocate(2);
    auto obj3 = pool.allocate(3);

    REQUIRE(obj1.has_value());
    REQUIRE(obj2.has_value());
    REQUIRE_FALSE(obj3.has_value());
    REQUIRE(obj3.error() == MakeErrorCondition(ErrorCode::OUT_OF_MEMORY));
}

TEST_CASE("ObjectPool Bad Allocation", "[ObjectPool]") {
    mt::Engine engine;
    std::error_condition error;
    ObjectPool<TestObject, 1> pool(engine, error);

    REQUIRE_FALSE(error);

    auto obj1 = pool.allocate(1);
    REQUIRE(obj1.has_value());

    // Simulate bad allocation by throwing an exception
    auto obj2 = pool.allocate(2);
    REQUIRE_FALSE(obj2.has_value());
    REQUIRE(obj2.error() == MakeErrorCondition(ErrorCode::OUT_OF_MEMORY));
}

TEST_CASE("ObjectPool Repeated Allocation and Deallocation", "[ObjectPool]") {
    mt::Engine engine;
    std::error_condition error;
    ObjectPool<TestObject, 5> pool(engine, error);

    REQUIRE_FALSE(error);

    for (int i = 0; i < 5; ++i) {
        auto obj = pool.allocate(i);
        REQUIRE(obj.has_value());
        REQUIRE(obj.value()->value == i);
        obj.value().reset();
    }

    REQUIRE(pool.size() == 0);
}

TEST_CASE("ObjectPool Deallocate Null Pointer", "[ObjectPool]") {
    mt::Engine engine;
    std::error_condition error;
    ObjectPool<TestObject, 5> pool(engine, error);

    REQUIRE_FALSE(error);

    ObjectPool<TestObject, 5>::unique_ptr_t null_ptr(nullptr, pool.get_deleter());
    null_ptr.reset(); // Should handle gracefully

    REQUIRE(pool.size() == 0);
}

TEST_CASE("ObjectPool Deallocate Non-Pool Object", "[ObjectPool]") {
    mt::Engine engine;
    std::error_condition error;
    ObjectPool<TestObject, 5> pool(engine, error);

    REQUIRE_FALSE(error);

    TestObject non_pool_object(42);
    ObjectPool<TestObject, 5>::unique_ptr_t non_pool_ptr(&non_pool_object, pool.get_deleter());
    non_pool_ptr.reset(); // Should handle gracefully

    REQUIRE(pool.size() == 0);
}

TEST_CASE("ObjectPool Exceeding Capacity", "[ObjectPool]") {
    mt::Engine engine;
    std::error_condition error;
    ObjectPool<TestObject, 3> pool(engine, error);

    REQUIRE_FALSE(error);

    auto obj1 = pool.allocate(1);
    auto obj2 = pool.allocate(2);
    auto obj3 = pool.allocate(3);
    auto obj4 = pool.allocate(4);

    REQUIRE(obj1.has_value());
    REQUIRE(obj2.has_value());
    REQUIRE(obj3.has_value());
    REQUIRE_FALSE(obj4.has_value());
    REQUIRE(obj4.error() == MakeErrorCondition(ErrorCode::OUT_OF_MEMORY));
}

TEST_CASE("ObjectPool Concurrent Access", "[ObjectPool]") {
    std::error_condition error;
    mt::Engine engine;
    ObjectPool<TestObject, 10> pool(engine, error);

    REQUIRE_FALSE(error);

    auto allocate_and_deallocate = [&pool]() {
        for (int i = 0; i < 5; ++i) {
            auto obj = pool.allocate(i);
            if (obj.has_value()) {
                REQUIRE(obj.value()->value == i);
                obj.value().reset();
            }
        }
    };

    std::thread t1(allocate_and_deallocate);
    std::thread t2(allocate_and_deallocate);

    t1.join();
    t2.join();

    REQUIRE(pool.size() == 0);
}
