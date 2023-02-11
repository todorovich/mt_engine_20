module WindowsMessageManagerInterface;

import Error;

using namespace mt::error;
using namespace mt::windows;

using namespace std::literals;

std::unique_ptr<WindowsMessageManagerInterface> WindowsMessageManagerInterface::_defaultWindowsMessageHandler =
	std::make_unique<NullWindowsMessageManager>();

WindowsMessageManagerInterface* WindowsMessageManagerInterface::_instance = _defaultWindowsMessageHandler.get();

WindowsMessageManagerInterface::WindowsMessageManagerInterface(Error& error) noexcept
{
	if (_instance == _defaultWindowsMessageHandler.get())
		_instance = this;
	else
		error = mt::error::Error{
			L"Only one WindowsMessageManagerInterface may exist at a time."sv,
			mt::error::ErrorCode::GRAPHICS_FAILURE,
			__func__, __FILE__, __LINE__
		};
}