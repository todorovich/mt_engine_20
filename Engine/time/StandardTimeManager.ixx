export module StandardTimeManager;

export import <chrono>;
export import <memory>;

export import AlarmManager;

export import gsl;
export import Engine;
export import TimeManagerTasks;

using namespace gsl;
using namespace std::literals;
using namespace mt::error;
using namespace mt::task;
using namespace mt::time::model;

export namespace mt::time
{
	class StandardTickFunction : public Task
	{
		StopWatch* 	_tick_time 		= nullptr;
		StopWatch* 	_update_time 	= nullptr;
		StopWatch* 	_render_time 	= nullptr;
		StopWatch* 	_frame_time 	= nullptr;
		StopWatch* 	_input_time 	= nullptr;
		Engine* 	_engine 		= nullptr;

	public:
		StandardTickFunction() = default;

		StandardTickFunction(
			gsl::not_null<mt::time::model::StopWatch*> tick_time,
			gsl::not_null<mt::time::model::StopWatch*> update_time,
			gsl::not_null<mt::time::model::StopWatch*> render_time,
			gsl::not_null<mt::time::model::StopWatch*> frame_time,
			gsl::not_null<mt::time::model::StopWatch*> input_time,
			gsl::not_null<Engine*> engine
		)
			: _tick_time(tick_time)
			, _update_time(update_time)
			, _render_time(render_time)
			, _frame_time(frame_time)
			, _input_time(input_time)
			, _engine(engine)
		{}

		virtual std::expected<void, mt::error::Error> operator()() override
		{
			_tick_time->startTask();

			auto time_manager = _engine->getTimeManager();

			_update_time->startTask();
			if (time_manager->getShouldUpdate())
			{
				_engine->getGame()->physicsUpdate();

				time_manager->updateComplete();
			}
			_update_time->finishTask();

			_render_time->startTask();
			// Render whenever you can, but don't wait.
			if (time_manager->getShouldRender())
			{
				// TODO: figure out how to unlink this.
				_input_time->startTask();
				_engine->getInputManager()->processInput();
				_engine->getGame()->inputUpdate();
				_input_time->finishTask();

				// Processing input could result in a shutdown.
				if (!_engine->isShuttingDown())
				{
					_engine->getGame()->renderUpdate();
					auto renderer = _engine->getRenderer();
					if (auto expected = renderer->update(); !expected) return std::unexpected(expected.error());
					if (auto expected = renderer->render(); !expected) return std::unexpected(expected.error());
					time_manager->renderComplete();
				}

				_frame_time->finishTask();
				_frame_time->startTask();
			}
			_render_time->finishTask();

			_tick_time->finishTask();

			return {};
		};
	};

	class StandardTimeManager : public TimeManagerInterface
	{
		std::unique_ptr<AlarmManagerInterface> _alarm_manager;

		std::map<std::string_view, std::unique_ptr<StopWatch>>	_stop_watches;

		mt::Engine& _engine;

		void _addEngineAlarms() noexcept;

		mt::time::TimeManagerSetShouldUpdate _set_should_update;
		mt::time::TimeManagerSetShouldRender _set_should_render;
		mt::time::TimeManagerSetEndOfFrame _set_end_of_frame;

		StandardTickFunction _standardTickFunction;

	public:
		StandardTimeManager(mt::Engine& engine, Error& _alarm_manager_error) noexcept;

		virtual void resume() noexcept override;		// Call to unpaused.
		virtual void pause() noexcept override;			// Call to pause.
		virtual void tick() noexcept override;			// Call every frame.
		virtual void shutdown() noexcept override;

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
