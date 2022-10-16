// Copyright 2022 Micho Todorovich, all rights reserved.
export module LogManager;

export import Log;

export namespace logging 
{
	class LogManager
	{
		Log _main_log;

	public:

		// Default Constructor
		LogManager() = default;

		// Copy Constructor
		LogManager(const LogManager& other) = default;

		// Move Constructor
		LogManager(LogManager&& other) = default;

		// Copy Assignment
		LogManager& operator=(const LogManager& other) = delete;

		// Move Assignment
		LogManager& operator=(LogManager&& other) = default;

		// Destructor
		~LogManager() = default;

		Log& GetMainLog() { return _main_log; }
	};
}