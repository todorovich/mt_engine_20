// Copyright 2022 Micho Todorovich, all rights reserved.
export module KeyboardKeys;

export import std.core;

export namespace mt::input
{
	enum struct KeyboardKeys
	{
		NO_KEY = 0x00,
		BREAK = 0x03, // ctrl + pause?
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
		LEFT = 0x25, // arrow
		UP = 0x26, // arrow
		RIGHT = 0x27, // arrow
		DOWN = 0x28, // arrow
		SELECT = 0x29, // Select key?
		PRINT = 0x2A,
		EXECUTE = 0x2B,
		SNAPSHOT = 0x2C, // printscreen
		INSERT = 0x2D,
		DELETE_KEY = 0x2E,
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

		LWIN = 0x5B, // windows key left side
		RWIN = 0x5C, // windows key right side
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
	};

	enum struct KeyState
	{
		NO_STATE,
		PRESSED,
		HELD,
		RELEASED,
	};
}