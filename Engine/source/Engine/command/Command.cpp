module Command;

import Engine;

void mt::command::Command::add_to_command_queue()
{
	mt::Engine::GetCommandManager().add_command(this);
}

mt::command::Command::Command(EXECUTION_TYPE method_of_execution)
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
	case EXECUTION_TYPE::UNMANAGED:
		break;
	}
}