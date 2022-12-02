export module InputContext;

export namespace mt::input
{

	// Feel like i need a custom comparator for this when used in the input map.
	// THe problem is i want it to consider one with only pressed to be equal to any one with pressed
	// ,even if its pressed|released for example, for the purpose of finding matching actions.
	// For the purpose of add/replacing/removing from the map, only exact matches will do.
	// Will have to allow mutlple things to register for the same input when combos/sequences are intro'd
	// for example a and a+b would both need to be able to register for A PRESSED
	enum struct InputContext : unsigned char
	{
		NO_CONTEXT		= 0x00,
		CHIRAL			= 0x01,
		RIGHT			= 0x02,
		FUNCTION_KEY	= 0x04, // SENT in Conjuction with ZERO-NINE, A-Z (should stop at B because f12)
		EXTENDED_KEY	= 0x08, // This is numpad input, use with ZERO-NINE, and ENTER
		RELATIVE		= 0x10,
	};

	constexpr InputContext operator|(InputContext a, InputContext b)
	{
		return static_cast<InputContext>(static_cast<unsigned short>(a) | static_cast<unsigned short>(b));
	}

	constexpr InputContext operator&(InputContext a, InputContext b)
	{
		return static_cast<InputContext>(static_cast<unsigned short>(a) & static_cast<unsigned short>(b));
	}

}