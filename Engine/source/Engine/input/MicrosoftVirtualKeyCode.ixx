module;

#include "windows.h"

export module MicrosoftVirtualKeyCode;

export import std.core;

export namespace mt::input
{
	enum struct MicrosoftVirtualKeyCode : unsigned short
	{
		LEFT_MOUSE_BUTTON = 0x01,
		RIGHT_MOUSE_BUTTON = 0x02,
		MIDDLE_MOUSE_BUTTON = 0x04,
		MOUSE_BUTTON_FOUR = 0x05,
		MOUSE_BUTTON_FIVE = 0x06,
		BREAK = 0x03, // ctrl + pause, registers ctrl press and release as well as break
		BACKSPACE = 0x08,
		TAB = 0x09,
		CLEAR = 0x0C, // Apple Numpad Only?
		RETURN = 0x0D,

		SHIFT = 0x10, // Either left or right
		CONTROL = 0x11, // Either left or right
		ALT = 0x12, // Either left or right
		PAUSE = 0x13,
		CAPS_LOCK = 0x14, // caps lock
		ESCAPE = 0x1B,

		SPACE = 0x20,
		PRIOR = 0x21, // pg up
		NEXT = 0x22, // gp dn
		END = 0x23,
		HOME = 0x24,
		LEFT_ARROW = 0x25, // arrow
		UP_ARROW = 0x26, // arrow
		RIGHT_ARROW = 0x27, // arrow
		DOWN_ARROW = 0x28, // arrow
		SELECT = 0x29, // Select key?
		PRINT = 0x2A,
		EXECUTE = 0x2B,
		PRINT_SCREEN = 0x2C, // printscreen
		INSERT = 0x2D,
		DEL = 0x2E,
		HELP = 0x2F, // help key?

		ZERO = 0x30, // 0-9  (0x30 - 0x39)
		ONE = 0x31,
		TWO = 0x32,
		THREE = 0x33,
		FOUR = 0x34,
		FIVE = 0x35,
		SIX = 0x36,
		SEVEN = 0x37,
		EIGHT = 0x38,
		NINE = 0x39,

		A = 0x41, //and A - Z(0x41 - 0x5A) are same as ascii
		B = 0x42,
		C = 0x43,
		D = 0x44,
		E = 0x45,
		F = 0x46,
		G = 0x47,
		H = 0x48,
		I = 0x49,
		J = 0x4A,
		K = 0x4B,
		L = 0x4C,
		M = 0x4D,
		N = 0x4E,
		O = 0x4F,

		P = 0x50,
		Q = 0x51,
		R = 0x52,
		S = 0x53,
		T = 0x54,
		U = 0x55,
		V = 0x56,
		W = 0x57,
		X = 0x58,
		Y = 0x59,
		Z = 0x5A,
		LEFT_WINDOWS_KEY = 0x5B, // windows key left side
		RIGHT_WINDOWS_KEY = 0x5C, // windows key right side
		MENU = 0x5D,
		SLEEP = 0x5F,

		NUMPAD_0 = 0x60,
		NUMPAD_1 = 0x61,
		NUMPAD_2 = 0x62,
		NUMPAD_3 = 0x63,
		NUMPAD_4 = 0x64,
		NUMPAD_5 = 0x65,
		NUMPAD_6 = 0x66,
		NUMPAD_7 = 0x67,
		NUMPAD_8 = 0x68,
		NUMPAD_9 = 0x69,
		NUMPAD_MULTIPLY = 0x6A,
		NUMPAD_ADD = 0x6B,
		SEPARATOR = 0x6C,
		NUMPAD_SUBTRACT = 0x6D,
		NUMPAD_DECIMAL = 0x6E,
		NUMPAD_DIVIDE = 0x6F,

		F1 = 0x70,
		F2 = 0x71,
		F3 = 0x72,
		F4 = 0x73,
		F5 = 0x74,
		F6 = 0x75,
		F7 = 0x76,
		F8 = 0x77,
		F9 = 0x78,
		F10 = 0x79,
		F11 = 0x7A,
		F12 = 0x7B,
		F13 = 0x7C,
		F14 = 0x7D,
		F15 = 0x7E,
		F16 = 0x7F,

		F17 = 0x80,
		F18 = 0x81,
		F19 = 0x82,
		F20 = 0x83,
		F21 = 0x84,
		F22 = 0x85,
		F23 = 0x86,
		F24 = 0x87,

		NUM_LOCK = 0x90,
		SCROLL_LOCK = 0x91,

		LEFT_SHIFT = 0xA0,
		RIGHT_SHIFT = 0xA1,
		LEFT_CONTROL = 0xA2,
		RIGHT_CONTROL = 0xA3,
		LEFT_ALT = 0xA4,
		RIGHT_ALT = 0xA5,

		SEMICOLON = 0xBA,
		EQUALS = 0xBB,
		COMMA = 0xBC,
		MINUS = 0xBD,
		PERIOD = 0xBE,
		FORWARD_SLASH = 0xBF,

		BACKTICK = 0xC0,

		RIGHT_BRACKET = 0xDB,
		BACK_SLASH = 0xDC,
		LEFT_BRACKET = 0xDD,
		APOSTROPHE = 0xDE,
	};

	constexpr std::wstring to_wstring(MicrosoftVirtualKeyCode key)
	{
		switch (key)
		{
		case MicrosoftVirtualKeyCode::LEFT_MOUSE_BUTTON: return L"LEFT MOUSE BUTTON";
		case MicrosoftVirtualKeyCode::RIGHT_MOUSE_BUTTON: return L"RIGHT MOUSE BUTTON";
		case MicrosoftVirtualKeyCode::MIDDLE_MOUSE_BUTTON: return L"MIDDLE MOUSE BUTTON";
		case MicrosoftVirtualKeyCode::MOUSE_BUTTON_FOUR: return L"MOUSE BUTTON FOUR";
		case MicrosoftVirtualKeyCode::MOUSE_BUTTON_FIVE: return L"MOUSE BUTTON FIVE";
		case MicrosoftVirtualKeyCode::BREAK: return L"BREAK";
		case MicrosoftVirtualKeyCode::BACKSPACE: return L"BACKSPACE";
		case MicrosoftVirtualKeyCode::TAB: return L"TAB";
		case MicrosoftVirtualKeyCode::CLEAR: return L"CLEAR";
		case MicrosoftVirtualKeyCode::RETURN: return L"RETURN";
		case MicrosoftVirtualKeyCode::SHIFT: return L"SHIFT";
		case MicrosoftVirtualKeyCode::CONTROL: return L"CONTROL";
		case MicrosoftVirtualKeyCode::ALT: return L"ALT";
		case MicrosoftVirtualKeyCode::PAUSE: return L"PAUSE";
		case MicrosoftVirtualKeyCode::CAPS_LOCK: return L"CAPS LOCK";
		case MicrosoftVirtualKeyCode::ESCAPE: return L"ESCAPE";
		case MicrosoftVirtualKeyCode::SPACE: return L"SPACE";
		case MicrosoftVirtualKeyCode::PRIOR: return L"PG UP";
		case MicrosoftVirtualKeyCode::NEXT: return L"PG DN";
		case MicrosoftVirtualKeyCode::END: return L"END";
		case MicrosoftVirtualKeyCode::HOME: return L"HOME";
		case MicrosoftVirtualKeyCode::LEFT_ARROW: return L"LEFT ARROW";
		case MicrosoftVirtualKeyCode::UP_ARROW: return L"UP ARROW";
		case MicrosoftVirtualKeyCode::RIGHT_ARROW: return L"RIGHT ARROW";
		case MicrosoftVirtualKeyCode::DOWN_ARROW: return L"DOWN ARROW";
		case MicrosoftVirtualKeyCode::SELECT: return L"SELECT";
		case MicrosoftVirtualKeyCode::PRINT: return L"PRINT";
		case MicrosoftVirtualKeyCode::EXECUTE: return L"EXECUTE";
		case MicrosoftVirtualKeyCode::PRINT_SCREEN: return L"PRINT_SCREEN";
		case MicrosoftVirtualKeyCode::INSERT: return L"INSERT";
		case MicrosoftVirtualKeyCode::DEL: return L"DEL";
		case MicrosoftVirtualKeyCode::HELP: return L"HELP";
		case MicrosoftVirtualKeyCode::ZERO: return L"0";
		case MicrosoftVirtualKeyCode::ONE: return L"1";
		case MicrosoftVirtualKeyCode::TWO: return L"2";
		case MicrosoftVirtualKeyCode::THREE: return L"3";
		case MicrosoftVirtualKeyCode::FOUR: return L"4";
		case MicrosoftVirtualKeyCode::FIVE: return L"5";
		case MicrosoftVirtualKeyCode::SIX: return L"6";
		case MicrosoftVirtualKeyCode::SEVEN: return L"7";
		case MicrosoftVirtualKeyCode::EIGHT: return L"8";
		case MicrosoftVirtualKeyCode::NINE: return L"9";
		case MicrosoftVirtualKeyCode::A: return L"A";
		case MicrosoftVirtualKeyCode::B: return L"B";
		case MicrosoftVirtualKeyCode::C: return L"C";
		case MicrosoftVirtualKeyCode::D: return L"D";
		case MicrosoftVirtualKeyCode::E: return L"E";
		case MicrosoftVirtualKeyCode::F: return L"F";
		case MicrosoftVirtualKeyCode::G: return L"G";
		case MicrosoftVirtualKeyCode::H: return L"H";
		case MicrosoftVirtualKeyCode::I: return L"I";
		case MicrosoftVirtualKeyCode::J: return L"J";
		case MicrosoftVirtualKeyCode::K: return L"K";
		case MicrosoftVirtualKeyCode::L: return L"L";
		case MicrosoftVirtualKeyCode::M: return L"M";
		case MicrosoftVirtualKeyCode::N: return L"N";
		case MicrosoftVirtualKeyCode::O: return L"O";
		case MicrosoftVirtualKeyCode::P: return L"P";
		case MicrosoftVirtualKeyCode::Q: return L"Q";
		case MicrosoftVirtualKeyCode::R: return L"R";
		case MicrosoftVirtualKeyCode::S: return L"S";
		case MicrosoftVirtualKeyCode::T: return L"T";
		case MicrosoftVirtualKeyCode::U: return L"U";
		case MicrosoftVirtualKeyCode::V: return L"V";
		case MicrosoftVirtualKeyCode::W: return L"W";
		case MicrosoftVirtualKeyCode::X: return L"X";
		case MicrosoftVirtualKeyCode::Y: return L"Y";
		case MicrosoftVirtualKeyCode::Z: return L"Z";
		case MicrosoftVirtualKeyCode::LEFT_WINDOWS_KEY: return L"LEFT WINDOWS KEY";
		case MicrosoftVirtualKeyCode::RIGHT_WINDOWS_KEY: return L"RIGHT WINDOWS KEY";
		case MicrosoftVirtualKeyCode::SLEEP: return L"SLEEP";
		case MicrosoftVirtualKeyCode::NUMPAD_0: return L"NUMPAD 0";
		case MicrosoftVirtualKeyCode::NUMPAD_1: return L"NUMPAD 1";
		case MicrosoftVirtualKeyCode::NUMPAD_2: return L"NUMPAD 2";
		case MicrosoftVirtualKeyCode::NUMPAD_3: return L"NUMPAD 3";
		case MicrosoftVirtualKeyCode::NUMPAD_4: return L"NUMPAD 4";
		case MicrosoftVirtualKeyCode::NUMPAD_5: return L"NUMPAD 5";
		case MicrosoftVirtualKeyCode::NUMPAD_6: return L"NUMPAD 6";
		case MicrosoftVirtualKeyCode::NUMPAD_7: return L"NUMPAD 7";
		case MicrosoftVirtualKeyCode::NUMPAD_8: return L"NUMPAD 8";
		case MicrosoftVirtualKeyCode::NUMPAD_9: return L"NUMPAD 9";
		case MicrosoftVirtualKeyCode::NUMPAD_MULTIPLY: return L"NUMPAD *";
		case MicrosoftVirtualKeyCode::NUMPAD_ADD: return L"NUMPAD +";
		case MicrosoftVirtualKeyCode::SEPARATOR: return L"SEPARATOR";
		case MicrosoftVirtualKeyCode::NUMPAD_SUBTRACT: return L"NUMPAD -";
		case MicrosoftVirtualKeyCode::NUMPAD_DECIMAL: return L"NUMPAD .";
		case MicrosoftVirtualKeyCode::NUMPAD_DIVIDE: return L"NUMPAD /";
		case MicrosoftVirtualKeyCode::F1: return L"F1";
		case MicrosoftVirtualKeyCode::F2: return L"F2";
		case MicrosoftVirtualKeyCode::F3: return L"F3";
		case MicrosoftVirtualKeyCode::F4: return L"F4";
		case MicrosoftVirtualKeyCode::F5: return L"F5";
		case MicrosoftVirtualKeyCode::F6: return L"F6";
		case MicrosoftVirtualKeyCode::F7: return L"F7";
		case MicrosoftVirtualKeyCode::F8: return L"F8";
		case MicrosoftVirtualKeyCode::F9: return L"F9";
		case MicrosoftVirtualKeyCode::F10: return L"F10";
		case MicrosoftVirtualKeyCode::F11: return L"F11";
		case MicrosoftVirtualKeyCode::F12: return L"F12";
		case MicrosoftVirtualKeyCode::F13: return L"F13";
		case MicrosoftVirtualKeyCode::F14: return L"F14";
		case MicrosoftVirtualKeyCode::F15: return L"F15";
		case MicrosoftVirtualKeyCode::F16: return L"F16";
		case MicrosoftVirtualKeyCode::F17: return L"F17";
		case MicrosoftVirtualKeyCode::F18: return L"F18";
		case MicrosoftVirtualKeyCode::F19: return L"F19";
		case MicrosoftVirtualKeyCode::F20: return L"F20";
		case MicrosoftVirtualKeyCode::F21: return L"F21";
		case MicrosoftVirtualKeyCode::F22: return L"F22";
		case MicrosoftVirtualKeyCode::F23: return L"F23";
		case MicrosoftVirtualKeyCode::F24: return L"F24";
		case MicrosoftVirtualKeyCode::NUM_LOCK: return L"NUM_LOCK";
		case MicrosoftVirtualKeyCode::SCROLL_LOCK: return L"SCROLL_LOCK";
		case MicrosoftVirtualKeyCode::LEFT_SHIFT: return L"LEFT_SHIFT";
		case MicrosoftVirtualKeyCode::RIGHT_SHIFT: return L"RIGHT_SHIFT";
		case MicrosoftVirtualKeyCode::LEFT_CONTROL: return L"LEFT_CONTROL";
		case MicrosoftVirtualKeyCode::RIGHT_CONTROL: return L"RIGHT_CONTROL";
		case MicrosoftVirtualKeyCode::LEFT_ALT: return L"LEFT_ALT";
		case MicrosoftVirtualKeyCode::RIGHT_ALT: return L"RIGHT_ALT";
		case MicrosoftVirtualKeyCode::MENU: return L"MENU";
		case MicrosoftVirtualKeyCode::BACKTICK: return L"`";
		case MicrosoftVirtualKeyCode::SEMICOLON: return L";";
		case MicrosoftVirtualKeyCode::EQUALS: return L"=";
		case MicrosoftVirtualKeyCode::COMMA: return L",";
		case MicrosoftVirtualKeyCode::MINUS: return L"-";
		case MicrosoftVirtualKeyCode::PERIOD: return L".";
		case MicrosoftVirtualKeyCode::FORWARD_SLASH: return L"/";
		case MicrosoftVirtualKeyCode::RIGHT_BRACKET: return L"[";
		case MicrosoftVirtualKeyCode::BACK_SLASH: return L"]";
		case MicrosoftVirtualKeyCode::LEFT_BRACKET: return L"[";
		case MicrosoftVirtualKeyCode::APOSTROPHE: return L"'";

		default:
			OutputDebugStringW(std::format(L"{:#x}", static_cast<unsigned int>(key)).c_str());
			return L"UNKNOWN INPUT BUTTON";
		}
	}
}