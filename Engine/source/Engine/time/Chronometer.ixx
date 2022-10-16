// Copyright 2022 Micho Todorovich, all rights reserved.
export module Chronometer;

export import Time;

export using namespace std::literals::chrono_literals;

export namespace mt::time
{
    class TimeManager;

    export class Chronometer 
    {
    private:

        TimePoint _start_time;
        TimePoint _stop_time;

        TimePoint _time_paused;
        TimePoint _time_continued;

        Duration _duration_since_started;
        Duration _duration_active;
        Duration _total_duration_paused;
        Duration _duration_paused;

        Duration _average_active_duration;
        Duration _average_paused_duration;
        Duration _average_total_duration;

        std::string _name;

        Duration *_active_samples;
        Duration *_paused_samples;
        Duration *_total_samples;

        std::function<void(Chronometer &, const TimePoint &, const TimePoint &,
                           const Duration &)> _current_tick_function = &Chronometer::Inactive;

        __int64 _timer_ID;
        int _current_index;
        int _sample_size;

        bool _can_pause;
        bool _is_paused;
        bool _is_running;

        static __int64 _next_timer_id;

        void Tick(const TimePoint &current_tick_time, const TimePoint &previous_tick_time, const Duration &delta_time);

        void _CollectSample();

        void
        Inactive(const TimePoint &current_tick_time, const TimePoint &previous_tick_time, const Duration &delta_time) {
            _is_running = false;
        }

        void
        Started(const TimePoint &current_tick_time, const TimePoint &previous_tick_time, const Duration &delta_time) {
            _duration_since_started = current_tick_time - _start_time;

            _duration_active += current_tick_time - _start_time;
            _current_tick_function = &Chronometer::Active;
        }

        void
        Active(const TimePoint &current_tick_time, const TimePoint &previous_tick_time, const Duration &delta_time) {
            _duration_since_started = current_tick_time - _start_time;

            _duration_active += delta_time;
        }

        void
        Paused(const TimePoint &current_tick_time, const TimePoint &previous_tick_time, const Duration &delta_time) {
            _duration_since_started = current_tick_time - _start_time;

            if (_time_paused < previous_tick_time) {
                auto duration_retro_paused = previous_tick_time - _time_paused;

                _duration_active -= duration_retro_paused;
                _duration_paused = duration_retro_paused + delta_time;
            } else if (_time_paused < current_tick_time) {
                _duration_active += _time_paused - previous_tick_time;
                _duration_paused = current_tick_time - _time_paused;
            } else {
                return;
            }

            _current_tick_function = &Chronometer::FullPause;
        }

        void
        FullPause(const TimePoint &current_tick_time, const TimePoint &previous_tick_time, const Duration &delta_time) {
            _duration_since_started = current_tick_time - _start_time;

            _duration_paused += delta_time;
        }

        void
        Continued(const TimePoint &current_tick_time, const TimePoint &previous_tick_time, const Duration &delta_time) {
            _duration_since_started = current_tick_time - _start_time;

            if (_time_continued < previous_tick_time) {
                auto duration_retro_continued = previous_tick_time - _time_continued;

                _duration_paused -= duration_retro_continued;
                _duration_active += duration_retro_continued + delta_time;
            } else if (_time_continued < current_tick_time) {
                _duration_paused += _time_continued - previous_tick_time;
                _duration_active += current_tick_time - _time_continued;
            } else {
                return;
            }

            _duration_paused = Duration();

            _time_paused = TimePoint();
            _time_continued = TimePoint();

            _current_tick_function = &Chronometer::Active;
        }

        void
        Stopped(const TimePoint &current_tick_time, const TimePoint &previous_tick_time, const Duration &delta_time) {

            _current_tick_function = &Chronometer::Inactive;
        }

    public:

        friend TimeManager;

        // Big 6

        Chronometer(const char *timer_name, bool can_pause = true)
                : _start_time(0ns), _stop_time(0ns), _time_paused(0ns), _time_continued(0ns),
                  _total_duration_paused(0ns), _duration_active(0ns), _duration_since_started(0ns),
                  _average_active_duration(0ns), _average_paused_duration(0ns), _average_total_duration(0ns),
                  _name(timer_name), _active_samples(nullptr), _paused_samples(nullptr), _total_samples(nullptr),
                  _current_index(0), _timer_ID(-1), _sample_size(100), _can_pause(can_pause), _is_paused(false) {
            // Atomic Compare and Swap
            _timer_ID = _next_timer_id;
            _next_timer_id++;

            // this is bad data ordering
            _active_samples = new Duration[_sample_size];
            _paused_samples = new Duration[_sample_size];
            _total_samples = new Duration[_sample_size];

            for (auto x = 0; x < _sample_size; x++) {
                _active_samples[x] = 0ns;
                _paused_samples[x] = 0ns;
                _total_samples[x] = 0ns;
            }
        }

        ~Chronometer() {
            if (_active_samples != nullptr) {
                delete[] _active_samples;
                delete[] _paused_samples;
                delete[] _total_samples;
                _active_samples = nullptr;
                _paused_samples = nullptr;
                _total_samples = nullptr;
            }
        };

        Chronometer(const Chronometer &other) = delete;

        Chronometer(Chronometer &&other)
                : _start_time(std::move(other._start_time)), _stop_time(std::move(other._stop_time)),
                  _time_paused(std::move(other._time_paused)), _time_continued(std::move(other._time_continued)),
                  _total_duration_paused(std::move(other._total_duration_paused)),
                  _duration_active(std::move(other._duration_active)),
                  _duration_since_started(std::move(other._duration_since_started)),
                  _average_active_duration(std::move(other._average_active_duration)), _name(std::move(other._name)),
                  _active_samples(std::move(other._active_samples)), _paused_samples(std::move(other._active_samples)),
                  _total_samples(std::move(other._active_samples)), _current_index(std::move(other._current_index)),
                  _timer_ID(std::move(other._timer_ID)), _sample_size(std::move(other._sample_size)),
                  _can_pause(std::move(other._can_pause)), _is_paused(std::move(other._is_paused)) {

        }

        Chronometer &operator=(const Chronometer &other) = delete;

        Chronometer &operator=(Chronometer &&other) {
            _start_time = std::move(other._start_time);
            _stop_time = std::move(other._stop_time);
            _time_paused = std::move(other._time_paused);
            _time_continued = std::move(other._time_continued);
            _total_duration_paused = std::move(other._total_duration_paused);
            _duration_active = std::move(other._duration_active);
            _duration_since_started = std::move(other._duration_since_started);
            _average_active_duration = std::move(other._average_active_duration);
            _name = std::move(other._name);
            _active_samples = std::move(other._active_samples);
            _paused_samples = std::move(other._paused_samples);
            _total_samples = std::move(other._total_samples);
            _current_index = std::move(other._current_index);
            _timer_ID = std::move(other._timer_ID);
            _sample_size = std::move(other._sample_size);
            _can_pause = std::move(other._can_pause);
            _is_paused = std::move(other._is_paused);

            return *this;
        }

        // Comparison

        friend bool operator<(const Chronometer &lhs, const Chronometer &rhs) {
            return lhs._timer_ID < rhs._timer_ID;
        }

        // Methods

        void Start(TimePoint start_time = Clock::now());

        void Stop(TimePoint stop_time = Clock::now());

        void Lap(TimePoint time = Clock::now()) {
            Stop(time);
            Start(time);
        }

        void Reset();

        void Pause(TimePoint time_paused = Clock::now());

        void Continue(TimePoint time_continued = Clock::now());

        void SetNumberOfSamples(int samples);

        Duration GetLastActiveDuration() const;

        Duration GetLastPausedDuration() const;

        Duration GetLastTotalDuration() const;

        Duration GetAverageActiveDuration() const;

        Duration GetAveragePausedDuration() const;

        Duration GetAverageTotalDuration() const;

        Duration GetDurationActive() const;

        Duration GetDurationPaused() const;

        Duration GetDurationSinceStarted() const;

        std::string &GetName() { return _name; };

    };
}

