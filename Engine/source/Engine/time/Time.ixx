export module Time;

export import std.core;

export namespace mt::time
{
    using Clock = std::chrono::steady_clock;
    using Period = Clock::period;
    using Duration = Clock::duration;
    using TimePoint = Clock::time_point;
}