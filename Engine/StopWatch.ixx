export module StopWatch;

export import Time;

export import std.core;

export using namespace std::literals::chrono_literals;

export namespace mt::time
{
    class TimeManager;

    class StopWatch
    {
        static const size_t _number_of_samples = 128;

        TimeManager& _time_manager;

        const std::string_view _name;

        const TimePoint _created;
        
        TimePoint _task_started;
        TimePoint _task_finished;
        TimePoint _task_paused;
        
        Duration _paused;

        // The total amount of time since the StopWatch was created;
        Duration _total; 

        // The total amount of time spent doing the Task
        Duration _total_active;

        // The total amount spent not doing the task
        Duration _total_idle;
        
        std::vector<Duration> _task_intervals{ _number_of_samples, 0ns };
        Duration _average_task_interval;

        int sample_index = 0;

        bool _isActive;

    public:

        StopWatch(TimeManager& _time_manager, std::string_view name, TimePoint created = Clock::now())
            : _time_manager(_time_manager)
            , _name(name)
            , _created(created)
            , _task_finished(created)
            , _task_started(created)
        {
            _total = Duration();
            _total_active = Duration();
            _total_idle = Duration();

            _task_started = TimePoint::min();
            _task_finished = TimePoint::min();
        }

        void reset() {}

        void start_task(TimePoint start_time = Clock::now())
        {
            if (!_isActive)
            {
                _isActive = true;
                _task_started = start_time;
                _paused = 0ns;
                _total_idle += _task_started - _task_finished;
            }
        }

        void pause_task(TimePoint start_time = Clock::now()) {};

        void continue_task(TimePoint start_time = Clock::now()) {};

        void finish_task(TimePoint finish_time = Clock::now())
        {
            if (_isActive)
            {
                _isActive = false;
                
                _task_finished = finish_time;

                auto index = sample_index % _number_of_samples;
                
                // Subtract out the previous sample from the average
                _average_task_interval -= _task_intervals[index] / _number_of_samples;

                _task_intervals[index] = _task_finished - _task_started;
                
                _total_active += _task_intervals[index];

                // Add the current sample to the average
                _average_task_interval += _task_intervals[index] / _number_of_samples;
                
                sample_index++;
            }
        }

        void doTask(std::function<void(void)> doTask)
        {
            start_task();
            doTask();
            finish_task();
        }

        Duration getActive() { return _total_active; }
        Duration getPaused() { return _total_idle; }
        Duration getAverageTaskInterval() { return _average_task_interval; }
        std::vector<Duration> get() { return _task_intervals; }
    };
}
