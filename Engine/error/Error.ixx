// Copyright 2023 Micho Todorovich, all rights reserved.
export module Error;

export import <expected>;
export import <string_view>;

using namespace std::literals;

export namespace mt::error
{
	enum struct ErrorCode : std::uint32_t {
		ERROR_UNINITIALIZED = 0,
		FAILED = 1,

		ONE_ENGINE_RULE_VIOLATED = 10,

		MEMORY_FAILURE  = 100,
		BAD_ALLOCATION = 110,
		OUT_OF_MEMORY = 120,

		GRAPHICS_FAILURE = 200,
		CREATE_COMMITTED_RESOURCE_FAILED = 210,
		MAPPING_RESOURCE_FAILED = 215,
		CREATE_COMMAND_ALLOCATOR_FAILED = 230,

		WINDOW_MANAGER_FAILURE = 300,

		INPUT_MANAGER_FAILURE = 400,
		UNABLE_TO_ACCEPT_INPUT = 410,
		CALLED_WHILE_NOT_ACCEPTING_INPUT = 420,

		INVALID_GAME_PROVIDED = 1000,
	};

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
		constexpr Error(
			std::wstring_view message = L""sv,
			ErrorCode error_code = ErrorCode::ERROR_UNINITIALIZED,
			std::string_view function_name = ""sv,
			std::string_view file_name= ""sv,
			int line_number = -1,
			Error* cause = nullptr
		) noexcept
			: _message(message)
			, _error_code(error_code)
			, _function_name(function_name)
			, _file_name(file_name)
			, _line_number(line_number)
			, _cause(cause)
		{}

		constexpr Error(const Error& other) noexcept = default;
		constexpr Error(Error&& other) noexcept = default;
		constexpr Error& operator=(const Error& other) noexcept = default;
		constexpr Error& operator=(Error&& other) noexcept = default;
		constexpr ~Error() noexcept = default;

		[[nodiscard]] const std::wstring_view	getMessage() const { return _message; }
		[[nodiscard]] const std::string_view 	getFunctionName() const { return _function_name; }
		[[nodiscard]] const std::string_view 	getFileName() const { return _file_name; }
		[[nodiscard]] const int 				getLineNumber() const { return _line_number; }
		[[nodiscard]] const ErrorCode 			getErrorCode() const { return _error_code; }
		[[nodiscard]] const Error* 				getCause() const { return _cause; }
	};
}
