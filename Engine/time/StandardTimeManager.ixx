module;

#include <windows.h>

export module StandardTimeManager;

export import <chrono>;
export import <memory>;

export import AlarmManager;

export import gsl;
export import Engine;
export import TimeManagerTasks;
//export import StopWatch;

using namespace gsl;
using namespace std::literals;
using namespace mt::error;
using namespace mt::task;
using namespace mt::time::model;

export namespace mt::time
{

	class ShuttingDownTickFunction : public TickFunction
	{
		Engine* _engine = nullptr;
		StopWatch* _input_time = nullptr;

	public:
		ShuttingDownTickFunction() = default;

		ShuttingDownTickFunction(gsl::not_null<Engine*> engine, gsl::not_null<mt::time::model::StopWatch*> input_time)
			: _engine(engine)
			, _input_time(input_time)
		{}

		virtual std::expected<void, Error> operator()() noexcept {
			// It is necessary to keep processing input in order to receive and process WM_DESTROY
			_input_time->startTask();
			_engine->getInputManager()->processInput();
			_input_time->finishTask();
			return {};
		}
	};

	/**
	 * have to flush commands on gpu before destroying the window,
	 * destroying the windows causes PostQuitMessage via WM_Destroy
	 * The quit message terminates the windows message loop.
	 * would like to use raii and handle most of this shutdown stuff in the destructor.
	 * the problem is we won't get WM_Destroy, unless we destroy the window.
	 * We can't destroy the window while the renderer is using it.
	 *
	 * If we destroy in the destructor then we will never receive the WM_Destroy and therefore never PostQuitMessage
	 *
	 *  If we just delay quiting the message loop, then it'll eventually call to an non-existant WindowMessageManager
	 *
	 */
	class InitiateShutDownTickFunction : public TickFunction
	{
		Engine* _engine = nullptr;
		ShuttingDownTickFunction _shutting_down_tick_function;

	public:
		InitiateShutDownTickFunction() = default;

		InitiateShutDownTickFunction(
			gsl::not_null<Engine*> engine,
			ShuttingDownTickFunction& shutting_down_tick_function
		)
			: _engine(engine)
			, _shutting_down_tick_function(shutting_down_tick_function)
		{}

		virtual std::expected<void, Error> operator()() noexcept
		{
			_engine->setIsShuttingDown();

			if constexpr (IS_DEBUG) OutputDebugStringW(L"Engine Shutdown Initiated\n");

			// This can fail... but we're shutting down either way right?
			//ExitProcess(0); // may need exit process if this fails
			auto expected1 = _engine->getRenderer()->shutdown();
			// Destroy the windows.

			auto expected2 = _engine->getWindowManager()->destroyMainWindow();

			// This will eventually be called by WM_Destroy after it receives the message from windows that the window was
			// destroyed.
			//PostQuitMessage(0);

			_engine->getTimeManager()->setTickFunction(&_shutting_down_tick_function);

			return {};
		}
	};

	class StandardTickFunction : public TickFunction
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

		virtual std::expected<void, mt::error::Error> operator()() noexcept override
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

			if (_engine->shouldShutDown())
			{
				_engine->getTimeManager()->shutdown();
			}

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

		StandardTickFunction _standard_tick_function;
		ShuttingDownTickFunction _shutting_down_tick_function;
		InitiateShutDownTickFunction _initiate_shut_down_tick_function;

		std::map<std::string_view, std::unique_ptr<StopWatch>> _getStopWatches()
		{
			std::map<std::string_view, std::unique_ptr<StopWatch>> stop_watch;

			auto run_time = std::make_unique<StopWatch>(DefaultTimers::RUN_TIME);
			auto windows_message_time = std::make_unique<StopWatch>(DefaultTimers::WINDOWS_MESSAGE_TIME);
			auto tick_time = std::make_unique<StopWatch>(DefaultTimers::TICK_TIME);
			auto update_time = std::make_unique<StopWatch>(DefaultTimers::UPDATE_TIME);
			auto input_time = std::make_unique<StopWatch>(DefaultTimers::INPUT_TIME);
			auto render_time = std::make_unique<StopWatch>(DefaultTimers::RENDER_TIME);
			auto frame_time = std::make_unique<StopWatch>(DefaultTimers::FRAME_TIME);

			_stop_watches.emplace(std::make_pair(DefaultTimers::RUN_TIME,				std::move(run_time)));
			_stop_watches.emplace(std::make_pair(DefaultTimers::WINDOWS_MESSAGE_TIME,	std::move(windows_message_time)));
			_stop_watches.emplace(std::make_pair(DefaultTimers::TICK_TIME,				std::move(tick_time)));
			_stop_watches.emplace(std::make_pair(DefaultTimers::UPDATE_TIME,			std::move(update_time)));
			_stop_watches.emplace(std::make_pair(DefaultTimers::INPUT_TIME,				std::move(input_time)));
			_stop_watches.emplace(std::make_pair(DefaultTimers::RENDER_TIME,			std::move(render_time)));
			_stop_watches.emplace(std::make_pair(DefaultTimers::FRAME_TIME,				std::move(frame_time)));

			return stop_watch;
		}

	public:
		StandardTimeManager(mt::Engine& engine, Error& _alarm_manager_error) noexcept;

		virtual void resume() noexcept override;		// Call to unpaused.
		virtual void pause() noexcept override;			// Call to pause.
		virtual void tick() noexcept override;			// Call every frame.

		virtual void shutdown() noexcept override {
			pause();
			setTickFunction(&_initiate_shut_down_tick_function);
		};

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
