// File: EngineTests/input/InputManagerTests.ixx

module;

#include <catch.hpp>

export module InputManagerTests;

import std;
import Windows;

import Engine;
import InputManagerInterface;
import BasicInputManager;
import MakeUnique;
import InputTestEngine;
import MicrosoftVirtualKeyCode;

namespace mt::input::test
{
    class MockTask : public mt::task::Task
    {
    public:
        std::expected<void, std::error_condition> operator()() override
        {
            // Simulate task execution
            return {};
        }
    } task{};

    class MockOneDimensionalInputTask : public mt::task::OneDimensionalInputTask
    {
    public:
        std::expected<void, std::error_condition> operator()(int value) override
        {
            // Simulate task execution with 1D input
            return {};
        }
    } one_dimensional_input;

    class MockTwoDimensionalInputTask : public mt::task::TwoDimensionalInputTask
    {
    public:
        std::expected<void, std::error_condition> operator()(int x, int y) override
        {
            // Simulate task execution with 2D input
            return {};
        }
    } two_dimensional_input;

    class MockThreeDimensionalInputTask : public mt::task::ThreeDimensionalInputTask
    {
    public:
        std::expected<void, std::error_condition> operator()(int x, int y, int z) override
        {
            // Simulate task execution with 3D input
            return {};
        }
    } three_dimensional_input;

    // Helper function to create and initialize the engine
    std::unique_ptr<mt::Engine> createAndInitializeEngine() {
        std::error_condition error;
        auto engine = mt::memory::make_unique_nothrow<mt::input::test::InputTestEngine>(error);
        
        if (engine == nullptr)
            FAIL("Unable to initialize an mt::Engine. Preconditions not met. Test not executed.");
        
        return engine;
    }

    TEST_CASE("InputManager Initialization", "[InputManager]")
    {
        std::error_condition error;
        auto engine = mt::memory::make_unique_nothrow<mt::input::test::InputTestEngine>(error);
        auto inputManager = engine->getInputManager();

        REQUIRE(inputManager->isAcceptingInput() == true);
    }

    TEST_CASE("InputManager Accept Input", "[InputManager]") 
    {
        std::error_condition error;
        auto engine = createAndInitializeEngine();

        auto inputManager = engine->getInputManager();

        mt::input::model::InputType inputType{
            mt::input::model::InputDataType::BUTTON_PRESSED,
            mt::input::model::InputContext::CHIRAL,
            mt::input::model::MicrosoftVirtualKeyCode::A
        };
        mt::input::model::InputData1D inputData{ 1 };

        inputManager->acceptInput(inputType, inputData);

        REQUIRE(inputManager->getInputQueueSize() == 1);
    }

    TEST_CASE("InputManager Toggle Mouse Relative", "[InputManager]")
    {
        auto engine = createAndInitializeEngine();
        auto inputManager = engine->getInputManager();

        bool initialState = inputManager->isMouseRelative();
        inputManager->toggleIsMouseRelative();
        REQUIRE(inputManager->isMouseRelative() == !initialState);

        inputManager->toggleIsMouseRelative();
        REQUIRE(inputManager->isMouseRelative() == initialState);
    }

    TEST_CASE("InputManager Register Input Handler", "[InputManager]")
    {
        auto engine = createAndInitializeEngine();
        auto inputManager = engine->getInputManager();

        auto mockTask = std::make_unique<mt::input::test::MockTask>();
        mt::task::Task* taskPtr = mockTask.get();
        std::vector<std::unique_ptr<mt::input::test::MockTask>> taskStorage;
        taskStorage.emplace_back(std::move(mockTask));

        mt::input::model::InputHandler handler = taskPtr;
        mt::input::model::InputType inputType{
            mt::input::model::InputDataType::BUTTON_RELEASED,
            mt::input::model::InputContext::CHIRAL,
            mt::input::model::MicrosoftVirtualKeyCode::A
        };

        inputManager->registerInputHandler(handler, inputType);

        REQUIRE(inputManager->getHandlerMapSize() == 1);
    }

    TEST_CASE("InputManager Accept and Process Input", "[InputManager]")
    {
        std::error_condition error;
        auto engine = createAndInitializeEngine();
        auto inputManager = engine->getInputManager();

        mt::input::model::InputType inputType{
            mt::input::model::InputDataType::BUTTON_PRESSED,
            mt::input::model::InputContext::CHIRAL,
            mt::input::model::MicrosoftVirtualKeyCode::A
        };
        mt::input::model::InputData1D inputData{ 1 };

        inputManager->acceptInput(inputType, inputData);
        REQUIRE(inputManager->getInputQueueSize() == 1);

        inputManager->processInput();
        REQUIRE(inputManager->getInputQueueSize() == 0);
    }

    TEST_CASE("InputManager Duplicate Input Handler Registration", "[InputManager]")
    {
        std::error_condition error;
        auto engine = createAndInitializeEngine();
        auto inputManager = engine->getInputManager();

        auto mockTask = std::make_unique<mt::input::test::MockTask>();
        mt::task::Task* taskPtr = mockTask.get();
        std::vector<std::unique_ptr<mt::input::test::MockTask>> taskStorage;
        taskStorage.emplace_back(std::move(mockTask));

        mt::input::model::InputHandler handler = taskPtr;
        mt::input::model::InputType inputType{
            mt::input::model::InputDataType::BUTTON_RELEASED,
            mt::input::model::InputContext::CHIRAL,
            mt::input::model::MicrosoftVirtualKeyCode::A
        };

        inputManager->registerInputHandler(handler, inputType);
        inputManager->registerInputHandler(handler, inputType);

        REQUIRE(inputManager->getHandlerMapSize() == 1);
    }

    TEST_CASE("InputManager Unregister Input Handler", "[InputManager]")
    {
        std::error_condition error;
        auto engine = createAndInitializeEngine();
        auto inputManager = engine->getInputManager();

        auto mockTask = std::make_unique<mt::input::test::MockTask>();
        mt::task::Task* taskPtr = mockTask.get();
        std::vector<std::unique_ptr<mt::input::test::MockTask>> taskStorage;
        taskStorage.emplace_back(std::move(mockTask));

        mt::input::model::InputHandler handler = taskPtr;
        mt::input::model::InputType inputType{
            mt::input::model::InputDataType::BUTTON_RELEASED,
            mt::input::model::InputContext::CHIRAL,
            mt::input::model::MicrosoftVirtualKeyCode::A
        };

        inputManager->registerInputHandler(handler, inputType);
        inputManager->unregisterInputHandler(handler, inputType);

        REQUIRE(inputManager->getHandlerMapSize() == 0);
    }

    TEST_CASE("InputManager Empty Input Queue Processing", "[InputManager]")
    {
        std::error_condition error;
        auto engine = createAndInitializeEngine();
        auto inputManager = engine->getInputManager();

        REQUIRE_NOTHROW(inputManager->processInput());
        REQUIRE(inputManager->getInputQueueSize() == 0);
    }

    TEST_CASE("InputManager Rapid Input Toggling", "[InputManager]")
    {
        std::error_condition error;
        auto engine = createAndInitializeEngine();
        auto inputManager = engine->getInputManager();

        for (int i = 0; i < 100; ++i) {
            inputManager->toggleIsMouseRelative();
        }

        // Ensure the final state is consistent
        REQUIRE(inputManager->isMouseRelative() == (100 % 2 == 1));
    }

    TEST_CASE("InputManager Simultaneous Inputs", "[InputManager]")
    {
        std::error_condition error;
        auto engine = createAndInitializeEngine();
        auto inputManager = engine->getInputManager();

        mt::input::model::InputType inputType1{
            mt::input::model::InputDataType::BUTTON_PRESSED,
            mt::input::model::InputContext::CHIRAL,
            mt::input::model::MicrosoftVirtualKeyCode::A
        };
        mt::input::model::InputData1D inputData1{ 1 };

        mt::input::model::InputType inputType2{
            mt::input::model::InputDataType::BUTTON_PRESSED,
            mt::input::model::InputContext::CHIRAL,
            mt::input::model::MicrosoftVirtualKeyCode::B
        };
        mt::input::model::InputData1D inputData2{ 1 };

        inputManager->acceptInput(inputType1);
        inputManager->acceptInput(inputType2);

        REQUIRE(inputManager->getInputQueueSize() == 2);
    }

    TEST_CASE("InputManager Boundary Values for Input Data", "[InputManager]")
    {
        std::error_condition error;
        auto engine = createAndInitializeEngine();
        auto inputManager = engine->getInputManager();

        mt::input::model::InputType inputType{
            mt::input::model::InputDataType::BUTTON_PRESSED,
            mt::input::model::InputContext::CHIRAL,
            mt::input::model::MicrosoftVirtualKeyCode::A
        };
        mt::input::model::InputData1D inputData{ std::numeric_limits<int>::max() };

        inputManager->acceptInput(inputType, inputData);
        REQUIRE(inputManager->getInputQueueSize() == 1);
    }

    TEST_CASE("InputManager Stress Test with Large Input Queue", "[InputManager]")
    {
        std::error_condition error;
        auto engine = createAndInitializeEngine();
        auto inputManager = engine->getInputManager();

        mt::input::model::InputType inputType{
            mt::input::model::InputDataType::BUTTON_PRESSED,
            mt::input::model::InputContext::CHIRAL,
            mt::input::model::MicrosoftVirtualKeyCode::A
        };
        mt::input::model::InputData1D inputData{ 1 };

        for (int i = 0; i < 10000; ++i) {
            inputManager->acceptInput(inputType, inputData);
        }

        REQUIRE(inputManager->getInputQueueSize() == BasicInputManager::POOL_SIZE);

        inputManager->processInput();
        REQUIRE(inputManager->getInputQueueSize() == 0);
    }
}