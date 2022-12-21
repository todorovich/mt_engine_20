export module InputDataType;

export import <string_view>;

using namespace std::literals;

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

	constexpr std::wstring_view to_wstring(InputDataType key)
	{
		switch (key)
		{
			case InputDataType::NO_DATA_TYPE:		return L"NO DATA TYPE"sv;
			case InputDataType::BUTTON_IDLE:		return L"BUTTON IDLE"sv;
			case InputDataType::BUTTON_PRESSED:		return L"BUTTON PRESSED"sv;
			case InputDataType::BUTTON_HELD:		return L"BUTTON HELD"sv;
			case InputDataType::BUTTON_RELEASED:	return L"BUTTON RELEASED"sv;
			case InputDataType::ONE_DIMENSIONAL:	return L"ONE DIMENSIONAL"sv;
			case InputDataType::TWO_DIMENSIONAL:	return L"TWO DIMENSIONAL"sv;
			case InputDataType::THREE_DIMENSIONAL:	return L"THREE DIMENSIONAL"sv;
			default: return L"INVALID DATA TYPE"sv;
		}
	}
}