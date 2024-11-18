// Copyright 2023 Micho Todorovich, all rights reserved.
module;

export module Error;

import std;

using namespace std::literals;

export namespace mt::error
{
	enum struct ErrorCode : std::uint32_t
	{
		ERROR_UNINITIALIZED = 0,
		FAILED = 1,

		ONE_ENGINE_RULE_VIOLATED = 10,

		MEMORY_FAILURE = 1000, // MEMORY
		BAD_ALLOCATION,
		OUT_OF_MEMORY,

		GRAPHICS_FAILURE = 2000,  // GRAPHICS
		CREATE_COMMITTED_RESOURCE_FAILED,
		MAPPING_RESOURCE_FAILED,

		CREATE_COMMAND_ALLOCATOR_FAILED,
		RESET_COMMAND_LIST_ALLOCATOR_FAILED,

		CREATE_COMMAND_LIST_FAILED,
		CLOSE_COMMAND_LIST_FAILED,
		RESET_COMMAND_LIST_FAILED,

		RESIZE_BEFORE_INITIALIZED,

		FAILED_TO_CREATE_FENCE,
		FAILED_TO_WAIT_FOR_FENCE, //L"Unable to set the event to wait on the fence the."sv,

		COMPILE_SHADER_FAILED,
		CREATE_COMMAND_QUEUE_FAILED,
		FAILED_TO_SIGNAL_COMMAND_QUEUE_TO_WAIT_FOR_FENCE,

		CREATE_SWAP_CHAIN_FAILED,
		FAILED_TO_RETRIEVE_SWAP_CHAIN_BUFFER,
		RESIZE_SWAP_CHAIN_FAILED,
		FAILED_TO_PRESENT_SWAP_CHAIN,

		SERIALIZE_ROOT_SIGNATURE_FAILED,
		CREATE_ROOT_SIGNATURE_FAILED,

		CREATE_BLOB_FAILED,

		CREATE_DESCRIPTOR_HEAP_FAILED,

		CREATE_PIPELINE_STATE_OBJECT_FAILED,

		RETRIEVE_DEBUG_INTERFACE_FAILED,

		FAILED_TO_CREATE_DXGI_FACTORY_2,
		FAILED_TO_RETRIEVE_WARP_ADAPTER,
		FAILED_TO_CREATE_DX_DEVICE_FROM_WARP_ADAPTER,

		FAILED_TO_GET_MSAA_QUALITY_LEVELS,

		WINDOW_MANAGER_FAILURE = 3000, // WINDOWS MANAGER
		CREATE_MAIN_WINDOW_FAILED,

		WINDOWS_MESSAGE_MANAGER_FAILURE,
		ONE_WINDOWS_MESSAGE_MANAGER_RULE,

		INPUT_MANAGER_FAILURE = 4000, // INPUT MANAGER
		UNABLE_TO_ACCEPT_INPUT,
		CALLED_WHILE_NOT_ACCEPTING_INPUT,
		INVALID_INPUT_DATA_TYPE,

		EVENT_MANAGER_FAILURE = 5000, // EVENT MANAGER
		EVENT_QUEUE_ALREADY_EXISTS,


		INVALID_GAME_PROVIDED = 9999, // INVALID GAME
	};

	class EngineErrorCategory : public std::error_category
	{
	public:
		constexpr EngineErrorCategory() noexcept
			: error_category(std::uintptr_t{ 47 })
		{}

		[[nodiscard]] const char* name() const noexcept override
		{
			return "engine";
		}

		[[nodiscard]] std::string message(int error_code) const override
		{
			switch (static_cast<ErrorCode>(error_code))
			{
				case ErrorCode::ERROR_UNINITIALIZED:
					return "Error Uninitialized.";
				case ErrorCode::FAILED:
					return "Failed.";
				case ErrorCode::BAD_ALLOCATION:
					return "Bad Allocation.";
				case ErrorCode::ONE_ENGINE_RULE_VIOLATED:
					return "One Engine Rule Violated, only one engine may exist at a time.";
				case ErrorCode::MEMORY_FAILURE:
					return "Memory Failure.";
				case ErrorCode::OUT_OF_MEMORY:
					return "Out of Memory.";
				case ErrorCode::GRAPHICS_FAILURE:
					return "Unable to compile shader.";
				case ErrorCode::CREATE_COMMITTED_RESOURCE_FAILED:
					return "Failed to create a committed resource";
				case ErrorCode::MAPPING_RESOURCE_FAILED:
					return "Failed to map a resource to the GPU.";
				case ErrorCode::CREATE_COMMAND_ALLOCATOR_FAILED:
					return "Failed to create a command allocator.";
				case ErrorCode::WINDOW_MANAGER_FAILURE:
					return "Window Manager Failure.";
				case ErrorCode::INPUT_MANAGER_FAILURE:
					return "Input Manager Failure";
				case ErrorCode::UNABLE_TO_ACCEPT_INPUT:
					return "Failed to accept input, message queue is full.";
				case ErrorCode::CALLED_WHILE_NOT_ACCEPTING_INPUT:
					return "The message queue was called while not accepting input.";
				case ErrorCode::INVALID_INPUT_DATA_TYPE:
					return "Attempted to register InputType with InputDataType::NO_DATA_TYPE which is invalid.";
				case ErrorCode::INVALID_GAME_PROVIDED:
					return "The game provided to Engine::run must not be null";
				case ErrorCode::EVENT_MANAGER_FAILURE:
					return "Event Manager Failure.";
				case ErrorCode::EVENT_QUEUE_ALREADY_EXISTS:
					return "An event queue already exists for supplied name.";
				case ErrorCode::WINDOWS_MESSAGE_MANAGER_FAILURE:
					return "Windows Message Manager Failure.";
				case ErrorCode::ONE_WINDOWS_MESSAGE_MANAGER_RULE:
					return "Only one WindowsMessageManagerInterface may exist at a time.";
				default:
					return "Unknown Error";
			}
		}
	};

	EngineErrorCategory& engineErrorCategory()
	{
		static EngineErrorCategory engine_error_category{};
		return engine_error_category;
	}

	void Assign(std::error_condition& error_condition, const ErrorCode& error)
	{
		error_condition.assign(static_cast<int>(error), engineErrorCategory());
	}

	[[nodiscard]] std::error_condition MakeErrorCondition(mt::error::ErrorCode error_code) noexcept
	{
		static EngineErrorCategory engine_error_category;
		return std::error_condition(static_cast<int>(error_code), engine_error_category);
	}

	// Should use an object pool (error pool) allocated on the engine at creation.
	//

	class Error
	{
		std::wstring_view _message;
		std::string_view _function_name;
		std::string_view _file_name;
		int _line_number;
		ErrorCode _error_code;
		Error* _cause;

	protected:
		void setMessage(std::wstring_view message)
		{
			_message = message;
		}

		void setFunctionName(std::string_view function_name)
		{
			_function_name = function_name;
		}

		void setFileName(std::string_view file_name)
		{
			_file_name = file_name;
		}

		void setLineNumber(int line_number)
		{
			_line_number = line_number;
		}

		void setErrorCode(ErrorCode error_code)
		{
			_error_code = error_code;
		}

		void setCause(Error* cause)
		{
			_cause = cause;
		}

	public:
		[[deprecated("Use mt::error::makeErrorCondition(ErrorCode) instead.")]]
		constexpr Error(
			std::wstring_view message = L""sv,
			ErrorCode error_code = ErrorCode::ERROR_UNINITIALIZED,
			std::string_view function_name = ""sv,
			std::string_view file_name = ""sv,
			int line_number = -1,
			Error* cause = nullptr
		) noexcept
			: _message(message)
			, _error_code(error_code)
			, _function_name(function_name)
			, _file_name(file_name)
			, _line_number(line_number)
			, _cause(cause) {}

		constexpr Error(const Error& other) noexcept = default;

		constexpr Error(Error&& other) noexcept = default;

		constexpr Error& operator=(const Error& other) noexcept = default;

		constexpr Error& operator=(Error&& other) noexcept = default;

		constexpr ~Error() noexcept = default;

		[[nodiscard]] const std::wstring_view getMessage() const { return _message; }

		[[nodiscard]] const std::string_view getFunctionName() const { return _function_name; }

		[[nodiscard]] const std::string_view getFileName() const { return _file_name; }

		[[nodiscard]] const int getLineNumber() const { return _line_number; }

		[[nodiscard]] const ErrorCode getErrorCode() const { return _error_code; }

		[[nodiscard]] const Error* getCause() const { return _cause; }
	};
}
