export module InputDevice;

export namespace mt::input
{
	enum struct InputDevice : unsigned char
	{
		NO_INPUT_DEVICE = 0x00,

		KEYBOARD = 0x01,
		MOUSE = 0x02,
		GAMEPAD = 0x04,
		FLIGHTSTICK = 0x08,
	};
}