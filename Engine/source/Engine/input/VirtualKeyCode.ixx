export module VirtualKeyCode;

import <string>;

using namespace std::literals;

export namespace mt::input
{
	enum struct VirtualKeyCode : unsigned char
	{
		NO_KEY = 0x00,

		// 0x01 - 0x07 NOT ASCII
		CONTROL = 0x01, // NOT ASCII
		ALT = 0x02, // NOT ASCII
		SHIFT = 0x03, // NOT ASCII
		WINDOWS = 0x04, // NOT ASCII
		APPLICATIONS = 0x05, // NOT ASCII
		PRINT_SCREEN = 0x06, // NOT ASCII
		PAUSE = 0x07, // NOT ASCII

		BACKSPACE = 0x08,
		TAB = 0x09,

		// 0x0A - 0x0C NOT ASCII
		CAPS_LOCK = 0x0A, // NOT ASCII
		SCROLL_LOCK = 0x0B, // NOT ASCII
		NUM_LOCK = 0x0C, // NOT ASCII

		RETURN = 0x0D, // CARRIAGE RETURN 

		// 0x0E - 0x1A

		ESCAPE = 0x1B,

		// 0x1C - 0x1F

		SPACE = 0x20,

		// 0x21 - 0x26

		APOSTROPHE = 0x27,

		// 0x28 - 0x2B

		COMMA = 0x2C,
		MINUS = 0x2D,
		PERIOD = 0x2E,
		FORWARD_SLASH = 0x2F,

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

		// 0x3A

		SEMICOLON = 0x3B,
		LEFT_ANGLE_BRACKET = 0x3C,
		EQUALS = 0x3D,
		RIGHT_ANGLE_BRACKET = 0x3E,

		// 0x40

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

		LEFT_SQUARE_BRACKET = 0x5B,
		BACK_SLASH = 0x5C,
		RIGHT_SQUARE_BRACKET = 0x5D,

		// 0x5E, 0x5F

		BACKTICK = 0x60,

		// 0x61 - 0x7E NOT ASCII
		DIVIDE = 0x61, // NOT ASCII
		MULTIPLY = 0x62, // NOT ASCII
		ADD = 0x63, // NOT ASCII
		SUBTRACT = 0x64, // NOT ASCII

		LEFT_ARROW = 0x65, // NOT ASCII
		RIGHT_ARROW = 0x66, // NOT ASCII
		UP_ARROW = 0x67, // NOT ASCII
		DOWN_ARROW = 0x68, // NOT ASCII

		HOME = 0x69, // NOT ASCII
		END = 0x6A, // NOT ASCII
		PAGE_UP = 0x6B, // NOT ASCII
		PAGE_DOWN = 0x6C, // NOT ASCII

		INSERT = 0x7E, // NOT ASCII
		DEL = 0x7F,
	};

	constexpr std::wstring to_wstring(VirtualKeyCode virtual_key_code)
	{
		switch (virtual_key_code)
		{
			case VirtualKeyCode::NO_KEY: return L"NO KEY";
			case VirtualKeyCode::CONTROL: return L"CONTROL";
			case VirtualKeyCode::ALT: return L"ALT";
			case VirtualKeyCode::SHIFT: return L"SHIFT";
			case VirtualKeyCode::WINDOWS: return L"WINDOWS";
			case VirtualKeyCode::APPLICATIONS: return L"APPLICATIONS";
			case VirtualKeyCode::PRINT_SCREEN: return L"PRINT SCREEN";
			case VirtualKeyCode::PAUSE: return L"PAUSE";
			case VirtualKeyCode::BACKSPACE: return L"BACKSPACE";
			case VirtualKeyCode::TAB: return L"TAB";
			case VirtualKeyCode::CAPS_LOCK: return L"CAPS LOCK";
			case VirtualKeyCode::SCROLL_LOCK: return L"SCROLL LOCK";
			case VirtualKeyCode::NUM_LOCK: return L"NUM LOCK";
			case VirtualKeyCode::RETURN: return L"RETURN";
			case VirtualKeyCode::ESCAPE: return L"ESCAPE";
			case VirtualKeyCode::SPACE: return L"SPACE";
			case VirtualKeyCode::APOSTROPHE: return L"APOSTROPHE";
			case VirtualKeyCode::COMMA: return L"COMMA";
			case VirtualKeyCode::MINUS: return L"MINUS";
			case VirtualKeyCode::PERIOD: return L"PERIOD";
			case VirtualKeyCode::FORWARD_SLASH: return L"FORWARD_SLASH";
			case VirtualKeyCode::ZERO: return L"ZERO";
			case VirtualKeyCode::ONE: return L"ONE";
			case VirtualKeyCode::TWO: return L"TWO";
			case VirtualKeyCode::THREE: return L"THREE";
			case VirtualKeyCode::FOUR: return L"FOUR";
			case VirtualKeyCode::FIVE: return L"FIVE";
			case VirtualKeyCode::SIX: return L"SIX";
			case VirtualKeyCode::SEVEN: return L"SEVEN";
			case VirtualKeyCode::EIGHT: return L"EIGHT";
			case VirtualKeyCode::NINE: return L"NINE";
			case VirtualKeyCode::SEMICOLON: return L"SEMICOLON";
			case VirtualKeyCode::LEFT_ANGLE_BRACKET: return L"LEFT ANGLE BRACKET";
			case VirtualKeyCode::EQUALS: return L"EQUALS";
			case VirtualKeyCode::RIGHT_ANGLE_BRACKET: return L"RIGHT ANGLE BRACKET";
			case VirtualKeyCode::A: return L"A";
			case VirtualKeyCode::B: return L"B";
			case VirtualKeyCode::C: return L"C";
			case VirtualKeyCode::D: return L"D";
			case VirtualKeyCode::E: return L"E";
			case VirtualKeyCode::F: return L"F";
			case VirtualKeyCode::G: return L"G";
			case VirtualKeyCode::H: return L"H";
			case VirtualKeyCode::I: return L"I";
			case VirtualKeyCode::J: return L"J";
			case VirtualKeyCode::K: return L"K";
			case VirtualKeyCode::L: return L"L";
			case VirtualKeyCode::M: return L"M";
			case VirtualKeyCode::N: return L"N";
			case VirtualKeyCode::O: return L"O";
			case VirtualKeyCode::P: return L"P";
			case VirtualKeyCode::Q: return L"Q";
			case VirtualKeyCode::R: return L"R";
			case VirtualKeyCode::S: return L"S";
			case VirtualKeyCode::T: return L"T";
			case VirtualKeyCode::U: return L"U";
			case VirtualKeyCode::V: return L"V";
			case VirtualKeyCode::W: return L"W";
			case VirtualKeyCode::X: return L"X";
			case VirtualKeyCode::Y: return L"Y";
			case VirtualKeyCode::Z: return L"Z";
			case VirtualKeyCode::LEFT_SQUARE_BRACKET: return L"LEFT SQUARE BRACKET";
			case VirtualKeyCode::BACK_SLASH: return L"BACK SLASH";
			case VirtualKeyCode::RIGHT_SQUARE_BRACKET: return L"RIGHT SQUARE BRACKET";
			case VirtualKeyCode::BACKTICK: return L"BACKTICK";
			case VirtualKeyCode::DIVIDE: return L"DIVIDE";
			case VirtualKeyCode::MULTIPLY: return L"MULTIPLY";
			case VirtualKeyCode::ADD: return L"ADD";
			case VirtualKeyCode::SUBTRACT: return L"SUBTRACT";
			case VirtualKeyCode::LEFT_ARROW: return L"LEFT ARROW";
			case VirtualKeyCode::RIGHT_ARROW: return L"RIGHT ARROW";
			case VirtualKeyCode::UP_ARROW: return L"UP ARROW";
			case VirtualKeyCode::DOWN_ARROW: return L"DOWN ARROW";
			case VirtualKeyCode::HOME: return L"HOME";
			case VirtualKeyCode::END: return L"END";
			case VirtualKeyCode::PAGE_UP: return L"PAGE UP";
			case VirtualKeyCode::PAGE_DOWN: return L"PAGE DOWN";
			case VirtualKeyCode::INSERT: return L"INSERT";
			case VirtualKeyCode::DEL: return L"DEL";
			default: return std::to_wstring(static_cast<unsigned char>(virtual_key_code));
		}
	}
}