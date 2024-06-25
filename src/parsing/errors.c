#include "header.h"

bool fatal_error(Command *cmd) {
	bool fatal_error = false;
	for (int i = 0; i < cmd->size; i++) {
		if (cmd->args[i].error)
			fatal_error = true;
	}
	if (!fatal_error)
		return false;
	for (int i = 0; i < cmd->nb_file; i++) {
		free_file(cmd->file_system[i], cmd->flags & long_display);
	}
	free_command(cmd);
	return true;
}
