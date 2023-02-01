export module Error;

export import <expected>;
export import <string_view>;

export namespace mt
{
	enum struct ErrorCode : std::uint32_t {
		FAILED = 1,
		OUT_OF_MEMORY = 100,
		GRAPHICS_FAILURE = 200,
		WINDOW_MANAGER_FAILURE = 300
	};

	struct Error
	{
		const std::wstring_view message;
		const std::string_view function_name;
		const std::string_view file_name;
		const int line_number;
		const ErrorCode error_code;

		constexpr Error(
			std::wstring_view message,
			ErrorCode error_code,
			std::string_view function_name,
			std::string_view file_name,
			int line_number
		)
			: message(message)
			, error_code(error_code)
			, function_name(function_name)
			, file_name(file_name)
			, line_number(line_number)
		{}

		constexpr Error(const Error& other) = default;
		constexpr Error(Error&& other) = default;
		constexpr Error& operator=(const Error& other) = delete;
		constexpr Error& operator=(Error&& other) = delete;
		constexpr ~Error() = default;
	};
}