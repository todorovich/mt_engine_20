export module Time;

#pragma warning( push )
#pragma warning( disable : 5050 )
export import std.core;
#pragma warning( pop )

export namespace mt { class Engine; }

export namespace mt::time
{
    using Clock = std::chrono::steady_clock;
    using Period = Clock::period;
    using Duration = Clock::duration;
    using TimePoint = Clock::time_point;

    using Task = void(mt::Engine&);
}