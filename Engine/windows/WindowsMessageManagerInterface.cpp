module WindowsMessageManagerInterface;

import Error;

using namespace mt::error;
using namespace mt::windows;

using namespace std::literals;

std::unique_ptr<WindowsMessageManagerInterface> WindowsMessageManagerInterface::_defaultWindowsMessageHandler =
	std::make_unique<NullWindowsMessageManager>();

WindowsMessageManagerInterface* WindowsMessageManagerInterface::_instance = _defaultWindowsMessageHandler.get();

WindowsMessageManagerInterface::WindowsMessageManagerInterface(std::error_condition& error) noexcept
{
	if (_instance == _defaultWindowsMessageHandler.get())
		_instance = this;
	else
		Assign(error, ErrorCode::ONE_WINDOWS_MESSAGE_MANAGER_RULE);
}