module;

#include <string.h>

module Chronometer;

import Engine;
import Time;
import TimeManager;

import std.core;

using namespace std::literals::chrono_literals;

__int64 mt::time::Chronometer::_next_timer_id = 1;

void mt::time::Chronometer::Reset()
{
    *this = Chronometer(_time_manager, _name.c_str(), _can_pause);
}

void mt::time::Chronometer::Start(mt::time::TimePoint start_time)
{
    if (!_is_running)
    {
        _start_time = start_time;

        _duration_since_started = Duration();
        _duration_active = Duration();
        _duration_paused = Duration();

        _is_running = true;

        if (_start_paused)
            _current_tick_function = &Chronometer::Paused;
        else
            _current_tick_function = &Chronometer::Active;
        
    }
}

void mt::time::Chronometer::Stop(mt::time::TimePoint stop_time)
{
    if (_is_running)
    {
        _is_running = false;

        _stop_time = stop_time;

        _duration_since_started = _stop_time - _start_time;

        if (_is_paused)
        {
            _duration_paused += _stop_time - _time_manager.GetCurrentTickTime();
        }
        else
        {
            _duration_active += _stop_time - _time_manager.GetCurrentTickTime();
        }

        _CollectSample();

        _current_tick_function = &Chronometer::Stopped;
    }
}

void mt::time::Chronometer::Pause(TimePoint time_paused)
{
    if (_can_pause && _is_running && !_is_paused)
    {
        _is_paused = true;
        _time_paused = time_paused;
        _current_tick_function = &Chronometer::Paused;
    }
}

void mt::time::Chronometer::Continue(TimePoint time_continued)
{
    if (_can_pause && _is_running && _is_paused)
    {
        _time_continued = time_continued;
        _is_paused = false;
        _current_tick_function = &Chronometer::Continued;
    }
}

// PRIVATE FUNCTIONS

void mt::time::Chronometer::Tick(const TimePoint& current_tick_time, const TimePoint& previous_tick_time, const Duration& delta_time)
{
    if (_is_running)
    {
        _current_tick_function(*this, current_tick_time, previous_tick_time, delta_time);
    }
}

void mt::time::Chronometer::_CollectSample()
{
    _average_active_duration -= _active_samples[_current_index] / _sample_size;
    _active_samples[_current_index] = _duration_active;
    _average_active_duration += _active_samples[_current_index] / _sample_size;

    _average_paused_duration -= _paused_samples[_current_index] / _sample_size;
    _paused_samples[_current_index] = _duration_paused;
    _average_paused_duration += _paused_samples[_current_index] / _sample_size;

    _average_total_duration -= _total_samples[_current_index] / _sample_size;
    _total_samples[_current_index] = _duration_since_started;
    _average_total_duration += _total_samples[_current_index] / _sample_size;

    _current_index++;
    _current_index %= _sample_size;
}

// ACCESSORS

void mt::time::Chronometer::SetNumberOfSamples(int new_sample_size)
{
    Duration* _temp_sample = new Duration[_sample_size];

    auto previous_size = sizeof(Duration) * _sample_size;

    auto new_size = sizeof(Duration) * new_sample_size;

    memcpy_s(_temp_sample, sizeof(Duration) * _sample_size, _active_samples, sizeof(Duration));
    std::swap(_temp_sample, _active_samples);
    delete[] _temp_sample;

    memcpy_s(_temp_sample, sizeof(Duration) * _sample_size, _paused_samples, sizeof(Duration));
    std::swap(_temp_sample, _paused_samples);
    delete[] _temp_sample;

    memcpy_s(_temp_sample, sizeof(Duration) * _sample_size, _total_samples, sizeof(Duration));
    std::swap(_temp_sample, _total_samples);
    delete[] _temp_sample;
}

mt::time::Duration mt::time::Chronometer::GetLastActiveDuration() const
{
    return (_current_index > 0) ? _active_samples[(_current_index - 1) % _sample_size] : 0ns;
}

mt::time::Duration mt::time::Chronometer::GetLastPausedDuration() const
{
    return (_current_index > 0) ? _paused_samples[(_current_index - 1) % _sample_size] : 0ns;
}

mt::time::Duration mt::time::Chronometer::GetLastTotalDuration() const
{
    return (_current_index > 0) ? _total_samples[(_current_index - 1) % _sample_size] : 0ns;
}

mt::time::Duration mt::time::Chronometer::GetAverageActiveDuration() const
{
    return _average_active_duration;
}

mt::time::Duration mt::time::Chronometer::GetAveragePausedDuration() const
{
    return _average_paused_duration;
}

mt::time::Duration mt::time::Chronometer::GetAverageTotalDuration() const
{
    return _average_total_duration;
}

mt::time::Duration mt::time::Chronometer::GetDurationActive() const
{
    return _duration_active;
}

mt::time::Duration mt::time::Chronometer::GetDurationPaused() const
{
    return _total_duration_paused + _duration_paused;
}

mt::time::Duration mt::time::Chronometer::GetDurationSinceStarted() const
{
    return _duration_since_started;
}