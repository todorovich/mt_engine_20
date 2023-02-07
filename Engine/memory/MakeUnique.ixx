export module MakeUnique;

export import <memory>;

export namespace mt::memory
{
	template <class T, class... Args>
	std::unique_ptr<T> make_unique_nothrow(Args&&... args) noexcept
	{
		return std::unique_ptr<T>(new (std::nothrow) T(std::forward<Args>(args)...));
	}
}