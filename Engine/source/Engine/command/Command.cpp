module Command;

import Engine;
import CommandManager;

void mt::command::Command::add_to_command_queue()
{
	_command_manager.add_command(this);
}

mt::command::Command::Command(CommandManager& command_manager, EXECUTION_TYPE method_of_execution)
	: _command_manager(command_manager)
{
	switch (method_of_execution)
	{
	case EXECUTION_TYPE::IMMEDIATE:
		execute();
		Destroy();
		break;
	case EXECUTION_TYPE::ASYNCHRONOURS:
		add_to_command_queue();
		break;
	case EXECUTION_TYPE::UNMANAGED: // ResearchMe: Use Case?
		break;
	}
}