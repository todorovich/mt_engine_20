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
		BAD_ALLOCATION = 100,
		OUT_OF_MEMORY = 110,
		GRAPHICS_FAILURE = 200,
		WINDOW_MANAGER_FAILURE = 300
	};

	// Should use an object pool (error pool) allocated on the engine at creation.
	//
	struct Error
	{
		const std::wstring_view message;
		const std::string_view function_name;
		const std::string_view file_name;
		const int line_number;
		const ErrorCode error_code;
		const Error* cause;

		constexpr Error(
			std::wstring_view message = L""sv,
			ErrorCode error_code = ErrorCode::ERROR_UNINITIALIZED,
			std::string_view function_name = ""sv,
			std::string_view file_name= ""sv,
			int line_number = -1,
			Error* cause = nullptr
		)
			: message(message)
			, error_code(error_code)
			, function_name(function_name)
			, file_name(file_name)
			, line_number(line_number)
			, cause(cause)
		{}

		constexpr Error(const Error& other) noexcept = default;
		constexpr Error(Error&& other) noexcept = default;
		constexpr Error& operator=(const Error& other) noexcept = delete;
		constexpr Error& operator=(Error&& other) noexcept = delete;
		constexpr ~Error() noexcept = default;
	};
}