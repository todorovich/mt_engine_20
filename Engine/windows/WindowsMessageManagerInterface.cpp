module WindowsMessageManagerInterface;

using namespace mt::windows;

std::unique_ptr<WindowsMessageManagerInterface> WindowsMessageManagerInterface::_defaultWindowsMessageHandler =
	std::make_unique<NullWindowsMessageManager>();

WindowsMessageManagerInterface* WindowsMessageManagerInterface::_instance = _defaultWindowsMessageHandler.get();

WindowsMessageManagerInterface::WindowsMessageManagerInterface()
{
	if (_instance == _defaultWindowsMessageHandler.get())
		_instance = this;
	else
		throw std::runtime_error("Only one WindowsMessageManagerInterface may exist at a time.");
}