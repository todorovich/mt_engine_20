export module MakeUnique;

import std;
import Error;

export namespace mt::memory
{
	template <typename T, typename... Args>
	concept ErrorConditionConstructible =
		requires(std::error_condition & ec, Args... args) {
			{ T(ec, args...) } noexcept;
	};


	template <typename T, typename... Args>
	std::unique_ptr<T> make_unique_nothrow(Args&&... args) noexcept
	{
		return std::unique_ptr<T>(new (std::nothrow) T(std::forward<Args>(args)...));
	}

	template <typename T, typename... Args> requires ErrorConditionConstructible<T, Args...>
	std::expected<std::unique_ptr<T>, std::error_condition> ExpectMakeUnique(Args&&... args) noexcept
	{
		std::error_condition error = MakeErrorCondition(mt::error::ErrorCode::ERROR_UNINITIALIZED); // Set error condition to mt::Error's 0 value
		auto ptr = std::unique_ptr<T>(new (std::nothrow) T(error, std::forward<Args>(args)...));
		
		if (ptr == nullptr) {
			return std::unexpected{error}; // Return unexpected if allocation failed
		}
		
		if (error.value() != static_cast<int>(mt::error::ErrorCode::ERROR_UNINITIALIZED)) {
			return std::unexpected{error}; // Return unexpected if error condition is set after creation
		}
		
		return ptr;
	}

	template <typename T, typename... Args>
	std::shared_ptr<T> make_shared_nothrow(Args&&... args) noexcept
	{
		return std::shared_ptr<T>(new (std::nothrow) T(std::forward<Args>(args)...));
	}
}