#pragma warning( push )
#pragma warning( disable : 5050 )
module StopWatch;

import std.core;
#pragma warning( pop )

import TimeManager;

void mt::time::StopWatch::doTask(Task* doTask)
{
    startTask();
    doTask(_time_manager.getEngine());
    finishTask();
}