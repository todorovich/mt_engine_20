export module EventHandlerInterface;

export namespace mt::event
{
	template <typename ... Parameters>
	struct EventHandler
	{
		virtual ~EventHandler() noexcept = default;
		virtual void operator()(Parameters ...) noexcept = 0;
	};
}