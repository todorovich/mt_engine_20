export module MakeUnique;

import std;

export namespace mt::memory
{
	template <typename T, typename... Args>
	std::unique_ptr<T> make_unique_nothrow(Args&&... args) noexcept
	{
		return std::unique_ptr<T>(new (std::nothrow) T(std::forward<Args>(args)...));
	}
}