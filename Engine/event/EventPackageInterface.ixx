module;

export module EventPackageInterface;

export import <cstdlib>;

export namespace mt::event
{
	class EventPackageInterface
	{
		const std::size_t _size{};

	protected:
		explicit EventPackageInterface(std::size_t size) noexcept
			: _size(size)
		{}

	public:
		virtual ~EventPackageInterface() noexcept = default;
		EventPackageInterface(EventPackageInterface&&) noexcept = default;
		EventPackageInterface(const EventPackageInterface&) noexcept = default;
		EventPackageInterface& operator=(EventPackageInterface&&) noexcept = delete;
		EventPackageInterface& operator=(EventPackageInterface&) noexcept = delete;

		[[nodiscard]] std::size_t size() const noexcept { return _size; };

		virtual void operator()() noexcept = 0;
	};
}