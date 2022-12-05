export module InputContext;

export import std.core;

using namespace std::literals;

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

	constexpr std::wstring to_wstring(InputContext key)
	{
		if (key == InputContext::NO_CONTEXT) 
		{
			return L"NO CONTEXT";
		}
		else 
		{
			std::set<std::wstring> descriptions;

			if ((key & InputContext::CHIRAL) == InputContext::CHIRAL) descriptions.insert(L"CHIRAL");
			if ((key & InputContext::RIGHT) == InputContext::RIGHT) descriptions.insert(L"RIGHT");
			if ((key & InputContext::FUNCTION_KEY) == InputContext::FUNCTION_KEY) descriptions.insert(L"FUNCTION KEY");
			if ((key & InputContext::EXTENDED_KEY) == InputContext::EXTENDED_KEY) descriptions.insert(L"EXTENDED KEY");
			if ((key & InputContext::RELATIVE) == InputContext::RELATIVE) descriptions.insert(L"RELATIVE");

			if (auto it = descriptions.begin(); it != descriptions.end())
			{
				std::wstring retval = *it;

				for (it++; it != descriptions.end(); it++)
				{
					retval += L" | " + *it;
				}

				return retval;
			}
			else
			{
				return L"INVALID CONTEXT";
			}
		}
	}
}