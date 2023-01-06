module StopWatch;

import TimeManager;

void mt::time::StopWatch::doTask(Task* doTask) noexcept
{
    startTask();
    doTask(_time_manager.getEngine());
    finishTask();
}