export module StopWatch;

import <chrono>;
import <string_view>;
import <vector>;

export using namespace std::literals;

import Engine;

export namespace mt::time
{
    class TimeManager;

    class StopWatch
    {
        static const size_t _number_of_samples = 128;

        TimeManager& _time_manager;

        const std::string_view _name;

        const std::chrono::steady_clock::time_point _created;
        
        std::chrono::steady_clock::time_point _task_started;
        std::chrono::steady_clock::time_point _task_finished;
        std::chrono::steady_clock::time_point _task_paused;
        
        std::chrono::steady_clock::duration _paused;

        // The total amount of time since the StopWatch was created;
        std::chrono::steady_clock::duration _total;

        // The total amount of time spent doing the Task
        std::chrono::steady_clock::duration _total_active;

        // The total amount spent not doing the task
        std::chrono::steady_clock::duration _total_idle;
        
        std::vector<std::chrono::steady_clock::duration> _task_intervals{ _number_of_samples, 1ns };
        std::chrono::steady_clock::duration _average_task_interval;

        int sample_index = 0;

        bool _isActive = false;

    public:

        StopWatch(
			TimeManager& _time_manager,
			std::string_view name,
			std::chrono::steady_clock::time_point created = std::chrono::steady_clock::now()
		) noexcept
			: _time_manager(_time_manager)
            , _name(name)
            , _created(created)
            , _task_finished(created)
            , _task_started(created)
            , _task_paused(std::chrono::steady_clock::time_point::min())
            , _paused(std::chrono::steady_clock::duration::min())
            , _average_task_interval(1ns)
            , _total(0ns)
            , _total_active(0ns)
            , _total_idle(0ns)
        {}

        void startTask(std::chrono::steady_clock::time_point start_time = std::chrono::steady_clock::now()) noexcept
		{
            if (!_isActive)
            {
                _isActive = true;
                _task_started = start_time;
                _paused = 0ns;
                _total_idle += _task_started - _task_finished;
            }
        }

        void pauseTask(std::chrono::steady_clock::time_point pause_time = std::chrono::steady_clock::now()) noexcept
        {  
            if (_task_paused == std::chrono::steady_clock::time_point::min())
            {
                _task_paused = pause_time;
                _total = _task_paused - _task_started;
            }
        }

        void continueTask(std::chrono::steady_clock::time_point start_time = std::chrono::steady_clock::now()) noexcept
        { 
            if (!(_task_paused == std::chrono::steady_clock::time_point::min()))
            {
                _paused += start_time - _task_paused;
                _total = _task_paused - _task_started;
                _task_paused = std::chrono::steady_clock::time_point::min();
            }
        }

        void finishTask(std::chrono::steady_clock::time_point finish_time = std::chrono::steady_clock::now()) noexcept
        {
            if (_isActive)
            {
                _isActive = false;
                
                _task_finished = finish_time;

                auto index = sample_index % _number_of_samples;
                
                // Subtract out the previous sample from the average
                _average_task_interval -= _task_intervals[index] / _number_of_samples;

                _task_intervals[index] = _task_finished - _task_started - _paused;
                
                _total_active += _task_intervals[index];

                _total_idle += _paused;

                // Add the current sample to the average
                _average_task_interval += _task_intervals[index] / _number_of_samples;
                
                sample_index++;
            }
        }

        void doTask(mt::Task* doTask) noexcept;

        std::chrono::steady_clock::duration getActive() const { return _total_active; }
        std::chrono::steady_clock::duration getPaused() const { return _total_idle; }
        std::chrono::steady_clock::duration getAverageTaskInterval() const { return _average_task_interval; }

        std::vector<std::chrono::steady_clock::duration> get() const { return _task_intervals; }
    };
}
