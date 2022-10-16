// Copyright 2018 Micho Todorovich, all rights reserved.
export module CommandManager;

export import Command;

export import Status;

export import std.core;

export namespace mt::command
{
	class CommandManager
	{
	public:
		CommandManager() = default;

		~CommandManager() = default;

		CommandManager(const CommandManager& other) = default;

		CommandManager(CommandManager&& other) = default;

		CommandManager& operator=(const CommandManager& other) = default;

		CommandManager& operator=(CommandManager&& other) = default;

		Status add_command(Command* const command_to_add)
		{
			command_queue.push(command_to_add);

			return Status::success;
		}

		Status execute_commands()
		{
			for (auto i = 0u; i < command_queue.size(); i++)
			{
				auto command = command_queue.front();

				command->execute();
				command->Destroy();

				command_queue.pop();
			}

			return mt::Status::success;
		}

	private:
		std::queue<Command*> command_queue;
	};
}