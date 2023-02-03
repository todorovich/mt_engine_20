// Copyright 2023 Micho Todorovich, all rights reserved.
export module InputContext;

export import <string_view>;

import <vector>;

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

	constexpr InputContext operator|(InputContext a, InputContext b) noexcept
	{
		return static_cast<InputContext>(static_cast<unsigned short>(a) | static_cast<unsigned short>(b));
	}

	constexpr InputContext operator&(InputContext a, InputContext b) noexcept
	{
		return static_cast<InputContext>(static_cast<unsigned short>(a) & static_cast<unsigned short>(b));
	}

	constexpr std::wstring_view to_wstring(InputContext key) noexcept
	{
		if (key == InputContext::NO_CONTEXT) 
		{
			return L"NO CONTEXT"sv;
		}
		else 
		{
			std::vector<std::wstring_view> descriptions;

			auto chiral = L"CHIRAL"sv;
			auto right = L"RIGHT"sv;
			auto function_key = L"FUNCTION KEY"sv;
			auto extended_key = L"EXTENDED KEY"sv;
			auto relative = L"RELATIVE"sv;
			auto separator = L" | "sv;

			if ((key & InputContext::CHIRAL) == InputContext::CHIRAL) descriptions.emplace_back(chiral);
			if ((key & InputContext::RIGHT) == InputContext::RIGHT) descriptions.emplace_back(right);
			if ((key & InputContext::FUNCTION_KEY) == InputContext::FUNCTION_KEY) descriptions.emplace_back(function_key);
			if ((key & InputContext::EXTENDED_KEY) == InputContext::EXTENDED_KEY) descriptions.emplace_back(extended_key);
			if ((key & InputContext::RELATIVE) == InputContext::RELATIVE) descriptions.emplace_back(relative);

			// Set the size to the size of all the separators
			std::size_t size = (descriptions.size() - 1) * separator.length() * sizeof(wchar_t);
			for (auto description : descriptions)
			{
				// add in the size of the strings.
				size += description.size() * sizeof(wchar_t);
			}

			if (auto it = descriptions.begin(); it != descriptions.end())
			{
				std::wstring retval{};
				
				retval.reserve(size);

				retval += *it;

				for (it++; it != descriptions.end(); it++)
				{
					retval += L" | ";
					retval += *it;
				}

				return retval;
			}
			else
			{
				return L"INVALID CONTEXT"sv;
			}
		}
	}
}