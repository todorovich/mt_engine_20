module TimeManagerTasks;

import Engine;

std::expected<void, mt::Error> mt::time::TimeManagerSetShouldUpdate::operator()() noexcept
{
	_engine->getTimeManager()->_setShouldUpdate();
	return {};
}

std::expected<void, mt::Error> mt::time::TimeManagerSetShouldRender::operator()() noexcept
{
	_engine->getTimeManager()->_setShouldRender();
	return {};
}

std::expected<void, mt::Error> mt::time::TimeManagerSetEndOfFrame::operator()() noexcept
{
	_engine->getTimeManager()->_setEndOfFrame();
	return {};
}