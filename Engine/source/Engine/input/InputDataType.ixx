export module InputDataType;

export namespace mt::input
{
	enum struct InputDataType : unsigned char
	{
		NO_DATA_TYPE				= 0x00,
		BUTTON_IDLE					= 0x01,
		BUTTON_PRESSED				= 0x02,
		BUTTON_HELD					= 0x03,
		BUTTON_RELEASED				= 0x04,
		ONE_DIMENSIONAL				= 0x05,
		TWO_DIMENSIONAL				= 0x06, 
		THREE_DIMENSIONAL			= 0x07
	};
}