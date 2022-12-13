export module StopWatch;

#pragma warning( push )
#pragma warning( disable : 5050 )
export import std.core;
#pragma warning( pop )

export import Time;

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
            , _task_paused(TimePoint::min())
            , _paused(0ns)
            , _total(0ns)
            , _total_active(0ns)
            , _total_idle(0ns)
        {}

        void reset() {}

        void startTask(TimePoint start_time = Clock::now())
        {
            if (!_isActive)
            {
                _isActive = true;
                _task_started = start_time;
                _paused = 0ns;
                _total_idle += _task_started - _task_finished;
            }
        }

        void pauseTask(TimePoint pause_time = Clock::now()) 
        {  
            if (_task_paused == TimePoint::min())
            {
                _task_paused = pause_time;
                _total = _task_paused - _task_started;
            }
        }

        void continueTask(TimePoint start_time = Clock::now()) 
        { 
            if (_task_paused != TimePoint::min())
            {
                _paused += start_time - _task_paused;
                _total = _task_paused - _task_started;
                _task_paused = TimePoint::min();
            }
        }

        void finishTask(TimePoint finish_time = Clock::now())
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

        void doTask(Task* doTask);

        Duration getActive() const { return _total_active; }
        Duration getPaused() const { return _total_idle; }
        Duration getAverageTaskInterval() const { return _average_task_interval; }

        std::vector<Duration> get() const { return _task_intervals; }
    };
}
