export module StandardTimeManager;

export import <chrono>;
export import <memory>;

export import AlarmManager;
export import TimeManagerInterface;
export import TimeManagerTasks;

using namespace std::literals;

export import Engine;

export namespace mt::time
{
	class StandardTimeManager : public TimeManagerInterface
	{
		std::unique_ptr<AlarmManagerInterface> _alarm_manager;

		std::map<std::string_view, std::unique_ptr<StopWatch>>	_stop_watches;

		mt::Engine* _engine;

		void _addEngineAlarms() noexcept;

		mt::time::TimeManagerSetShouldUpdate _set_should_update;
		mt::time::TimeManagerSetShouldRender _set_should_render;
		mt::time::TimeManagerSetEndOfFrame _set_end_of_frame;

	public:
		explicit StandardTimeManager(mt::Engine* engine) noexcept;

		virtual void resume() noexcept override;		// Call to unpaused.
		virtual void pause() noexcept override;			// Call to pause.
		virtual void tick() noexcept override;			// Call every frame.

		Engine* getEngine() noexcept
		{
			return _engine;
		}

		virtual StopWatch* findStopWatch(std::string_view name) override
		{
			auto find = _stop_watches.find(name);
			if (find == _stop_watches.end())
			{
				throw;
			}
			else
			{
				return find->second.get();
			}
		}
	};
}
