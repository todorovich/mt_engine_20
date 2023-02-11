export module TimeManagerTasks;

export import <expected>;

export import Engine;
export import gsl;

using namespace gsl;

export namespace mt::time
{
	class TimeManagerSetShouldUpdate : public mt::task::Task
	{
		mt::Engine& _engine;
	public:
		TimeManagerSetShouldUpdate(mt::Engine& engine)
			: _engine(engine) {}

		~TimeManagerSetShouldUpdate() = default;
		TimeManagerSetShouldUpdate(const TimeManagerSetShouldUpdate&) = default;
		TimeManagerSetShouldUpdate(TimeManagerSetShouldUpdate&&) = default;
		TimeManagerSetShouldUpdate& operator=(const TimeManagerSetShouldUpdate&) = default;
		TimeManagerSetShouldUpdate& operator=(TimeManagerSetShouldUpdate&&) = default;

		std::expected<void, mt::error::Error> operator()() noexcept;
	};

	class TimeManagerSetShouldRender : public mt::task::Task
	{
		mt::Engine& _engine;
	public:
		TimeManagerSetShouldRender(mt::Engine& engine)
			: _engine(engine) {}

		~TimeManagerSetShouldRender() = default;
		TimeManagerSetShouldRender(const TimeManagerSetShouldRender&) = default;
		TimeManagerSetShouldRender(TimeManagerSetShouldRender&&) = default;
		TimeManagerSetShouldRender& operator=(const TimeManagerSetShouldRender&) = default;
		TimeManagerSetShouldRender& operator=(TimeManagerSetShouldRender&&) = default;
			
		std::expected<void, mt::error::Error> operator()() noexcept;
	};

	class TimeManagerSetEndOfFrame : public mt::task::Task
	{
		mt::Engine& _engine;
	public:
		TimeManagerSetEndOfFrame(mt::Engine& engine)
			: _engine(engine) {}

		~TimeManagerSetEndOfFrame() = default;
		TimeManagerSetEndOfFrame(const TimeManagerSetEndOfFrame&) = default;
		TimeManagerSetEndOfFrame(TimeManagerSetEndOfFrame&&) = default;
		TimeManagerSetEndOfFrame& operator=(const TimeManagerSetEndOfFrame&) = default;
		TimeManagerSetEndOfFrame& operator=(TimeManagerSetEndOfFrame&&) = default;
			
		std::expected<void, mt::error::Error> operator()() noexcept;
	};
}
