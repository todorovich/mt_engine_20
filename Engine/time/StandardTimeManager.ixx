export module StandardTimeManager;

export import <chrono>;
export import <memory>;

export import AlarmManager;

export import gsl;
export import Engine;
export import TimeManagerTasks;

using namespace gsl;
using namespace std::literals;
using namespace mt::time::model;
using namespace mt::error;

export namespace mt::time
{
	class StandardTimeManager : public TimeManagerInterface
	{
		std::unique_ptr<AlarmManagerInterface> _alarm_manager;

		std::map<std::string_view, std::unique_ptr<StopWatch>>	_stop_watches;

		mt::Engine& _engine;

		void _addEngineAlarms() noexcept;

		mt::time::TimeManagerSetShouldUpdate _set_should_update;
		mt::time::TimeManagerSetShouldRender _set_should_render;
		mt::time::TimeManagerSetEndOfFrame _set_end_of_frame;

	public:
		StandardTimeManager(mt::Engine& engine, Error& _alarm_manager_error) noexcept;

		virtual void resume() noexcept override;		// Call to unpaused.
		virtual void pause() noexcept override;			// Call to pause.
		virtual void tick() noexcept override;			// Call every frame.

		virtual StopWatch* findStopWatch(std::string_view name) override
		{
			auto find = _stop_watches.find(name);
			if (find == _stop_watches.end())
			{

				return nullptr;
			}
			else
			{
				return find->second.get();
			}
		}
	};
}
