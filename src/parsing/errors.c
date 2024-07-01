#include "header.h"

bool fatal_error(Command *cmd) {
	bool fatal_error = false;
	for (int i = 0; i < cmd->size; i++) {
		if (cmd->args[i].error)
			fatal_error = true;
	}
	if (!fatal_error)
		return false;
	free_command(cmd);
	return true;
}
