// Copyright 2023 Micho Todorovich, all rights reserved.
module;

#include <expected>

export module Task;

export import Error;

export namespace mt::task
{
	class Task
	{
	public:
		Task() noexcept = default;
		virtual ~Task() noexcept = default;
		Task(const Task&) noexcept = default;
		Task(Task&&) noexcept = default;
		Task& operator=(const Task&) noexcept = default;
		Task& operator=(Task&&) noexcept = default;
		
		virtual std::expected<void, std::error_condition> operator()() = 0;
	};

	class OneDimensionalInputTask
	{
	public:
		OneDimensionalInputTask() noexcept = default;
		virtual ~OneDimensionalInputTask() noexcept = default;
		OneDimensionalInputTask(const OneDimensionalInputTask&) noexcept = default;
		OneDimensionalInputTask(OneDimensionalInputTask&&) noexcept = default;
		OneDimensionalInputTask& operator=(const OneDimensionalInputTask&) noexcept = default;
		OneDimensionalInputTask& operator=(OneDimensionalInputTask&&) noexcept = default;

		virtual std::expected<void, std::error_condition> operator()(int) = 0;
	};

	class TwoDimensionalInputTask
	{
	public:
		TwoDimensionalInputTask() noexcept = default;
		virtual ~TwoDimensionalInputTask() noexcept = default;
		TwoDimensionalInputTask(const TwoDimensionalInputTask&) noexcept = default;
		TwoDimensionalInputTask(TwoDimensionalInputTask&&) noexcept = default;
		TwoDimensionalInputTask& operator=(const TwoDimensionalInputTask&) noexcept = default;
		TwoDimensionalInputTask& operator=(TwoDimensionalInputTask&&) noexcept = default;
		
		virtual std::expected<void, std::error_condition> operator()(int, int) = 0;
	};

	class ThreeDimensionalInputTask
	{
	public:
		ThreeDimensionalInputTask() noexcept = default;
		virtual ~ThreeDimensionalInputTask() noexcept = default;
		ThreeDimensionalInputTask(const ThreeDimensionalInputTask&) noexcept = default;
		ThreeDimensionalInputTask(ThreeDimensionalInputTask&&) noexcept = default;
		ThreeDimensionalInputTask& operator=(const ThreeDimensionalInputTask&) noexcept = default;
		ThreeDimensionalInputTask& operator=(ThreeDimensionalInputTask&&) noexcept = default;

		virtual std::expected<void, std::error_condition> operator()(int, int, int) = 0;
	};
}