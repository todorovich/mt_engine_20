module;

#include <catch.hpp>

export module PriorityQueueTests;

import std;

import Error;
import PriorityQueue;

namespace mt {
    TEST_CASE("Push and Top", "[PriorityQueue]") {
        queue::PriorityQueue<10> pq;

        REQUIRE(pq.push(10));
        REQUIRE(pq.push(20));
        REQUIRE(pq.push(5));

        auto topValue = pq.top();
        REQUIRE(topValue);
        REQUIRE(topValue.value() == 20);
    }

    TEST_CASE("Pop and Empty", "[PriorityQueue]") {
        queue::PriorityQueue<10> pq;

        REQUIRE(pq.push(10));
        REQUIRE(pq.push(20));
        REQUIRE(pq.push(5));

        pq.pop();

        auto topValue = pq.top();
        REQUIRE(topValue);
        REQUIRE(topValue.value() == 10);

        pq.pop();
        pq.pop();

        REQUIRE(pq.empty());
    }

    TEST_CASE("Error Conditions", "[PriorityQueue]") {
        queue::PriorityQueue<2> pq;

        auto result1 = pq.push(10);
        REQUIRE(result1);

        auto result2 = pq.push(20);
        REQUIRE(result2);

        auto result3 = pq.push(30);  // Exceed capacity
        REQUIRE_FALSE(result3);
        REQUIRE(result3.error().value() == (int)mt::error::ErrorCode::OUT_OF_MEMORY);

        pq.pop();
        pq.pop();

        auto result4 = pq.pop();  // Pop empty queue
        REQUIRE_FALSE(result4);
        REQUIRE(result4.error().value() == (int)mt::error::ErrorCode::NO_SUCH_ELEMENT);
    }

    TEST_CASE("Order Preservation", "[PriorityQueue]") {
        queue::PriorityQueue<10> pq;

        REQUIRE(pq.push(10));
        REQUIRE(pq.push(20));
        REQUIRE(pq.push(5));
        REQUIRE(pq.push(30));
        REQUIRE(pq.push(15));

        int previous = pq.top().value();
        pq.pop();

        while (!pq.empty()) {
            int current = pq.top().value();
            REQUIRE(previous >= current);  // Ensure the heap property is maintained
            previous = current;
            pq.pop();
        }
    }

    TEST_CASE("Stress Test with Max Capacity", "[PriorityQueue]") {
        queue::PriorityQueue<100> pq;

        for (int i = 0; i < 100; ++i) {
            REQUIRE(pq.push(i));
        }

        auto result = pq.push(101);  // Should fail as capacity is 100
        REQUIRE_FALSE(result);
        REQUIRE(result.error().value() == (int)mt::error::ErrorCode::OUT_OF_MEMORY);

        int previous = pq.top().value();
        pq.pop();

        while (!pq.empty()) {
            int current = pq.top().value();
            REQUIRE(previous >= current);  // Ensure the heap property is maintained
            previous = current;
            pq.pop();
        }
    }

    TEST_CASE("Edge Case - Single Element", "[PriorityQueue]") {
        queue::PriorityQueue<10> pq;

        REQUIRE(pq.push(42));

        auto topValue = pq.top();
        REQUIRE(topValue);
        REQUIRE(topValue.value() == 42);

        pq.pop();
        REQUIRE(pq.empty());
    }


    TEST_CASE("Semi-Random Fill and Correct Removal Order", "[PriorityQueue]") {
        constexpr size_t capacity = 50;
        queue::PriorityQueue<capacity> pq;

        // Create a random number generator
        std::mt19937 rng(42);  // Seed with a fixed value for reproducibility
        std::uniform_int_distribution<int> dist(1, 100);

        // Fill the queue with semi-random values
        for (size_t i = 0; i < capacity; ++i) {
            int value = dist(rng);
            REQUIRE(pq.push(value));
        }

        // Ensure the elements are removed in the correct order (max-heap property)
        int previous = pq.top().value();
        pq.pop();

        while (!pq.empty()) {
            int current = pq.top().value();
            REQUIRE(previous >= current);  // Ensure the heap property is maintained
            previous = current;
            pq.pop();
        }
    }

    TEST_CASE("Min-Heap and Max-Heap Output", "[PriorityQueue]") {
        mt::queue::MaxPriorityQueue<10> maxHeap;
        mt::queue::MinPriorityQueue<10> minHeap;

        // Insert elements
        int elements[] = { 10, 5, 20, 15 };
        for (int value : elements) {
            REQUIRE(maxHeap.push(value));
            REQUIRE(minHeap.push(value));
        }

        // Max-Heap output
        std::vector<int> maxHeapOutput;
        while (!maxHeap.empty()) {
            auto topValue = maxHeap.top();
            REQUIRE(topValue);
            maxHeapOutput.push_back(topValue.value());
            maxHeap.pop();
        }

        // Min-Heap output
        std::vector<int> minHeapOutput;
        while (!minHeap.empty()) {
            auto topValue = minHeap.top();
            REQUIRE(topValue);
            minHeapOutput.push_back(topValue.value());
            minHeap.pop();
        }

        // Validate Max-Heap output
        REQUIRE(maxHeapOutput == std::vector<int>{20, 15, 10, 5});

        // Validate Min-Heap output
        REQUIRE(minHeapOutput == std::vector<int>{5, 10, 15, 20});
    }
}
