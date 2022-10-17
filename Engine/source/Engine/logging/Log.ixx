// Copyright 2022 Micho Todorovich, all rights reserved.
export module Log;

export import std.core;
export import std.filesystem;

export namespace mt::logging
{
	class Log
	{
		std::fstream filestream;

	public:

		// Default Constructor
		Log()
		{
			namespace fs = std::filesystem;

			fs::path log_path = std::filesystem::current_path() /= "Logs";

			//OutputDebugStringW(wstring(
			//	L"The current path " + log_path.wstring() + L" decomposes into:\n"
			//	+ L"Root name: " + log_path.root_name().wstring() + L'\n'
			//	+ L"Root Directory: " + log_path.root_directory().wstring() + L'\n'
			//	+ L"Root Path: " + log_path.root_path().wstring() + L'\n'
			//	+ L"Relative Path: " + log_path.relative_path().wstring() + L'\n'
			//	+ L"Parent Path: " + log_path.parent_path().wstring() + L'\n'
			//	+ L"Filename: " + log_path.filename().wstring() + L'\n'
			//	+ L"Stem: " + log_path.stem().wstring() + L'\n'
			//	+ L"Extention: " + log_path.extension().wstring() + L'\n'
			//).c_str());

			bool directory_exists = fs::exists(log_path);

			if (!directory_exists)
			{
				fs::create_directory(log_path);
			}

			log_path /= "Log.html";

			filestream.open(log_path, std::ios::out | std::ios::trunc);

			if (!filestream.is_open())
			{
				throw;
			}

			const auto tab = "    ";

			filestream
				<< "<!DOCTYPE html>" << '\n'
				<< "<html>" << '\n' << '\n'
				<< "<head>" << '\n'
				<< tab << "<title>Log File</title>" << '\n'
				<< tab << "<style type = \"text/css\">" << '\n'
				<< tab << tab << "p.detail{ color:#4C4C4C }" << '\n'
				<< tab << tab << "span.name{ color:#FF0000 }" << '\n'
				<< tab << "</style>" << '\n'
				<< "</head>" << '\n' << '\n'
				<< "<body>" << '\n';
		};

		// Copy Constructor
		Log(const Log& other) = delete;

		// Move Constructor
		Log(Log&& other) = default;

		// Copy Assignment
		Log& operator=(const Log& other) = delete;

		// Move Assignment
		Log& operator=(Log&& other) = default;

		std::ostream& GetOStream() { return filestream; }

		// Destructor
		~Log()
		{
			filestream
				<< '\n'
				<< "</body>" << '\n' << '\n'
				<< "</html>" << std::endl;

			filestream.sync();

			filestream.close();
		};
	};
}