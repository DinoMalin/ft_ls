#include "header.h"

bool fatal_error(Command *cmd) {
	for (int i = 0; i < cmd->size; i++) {
		if (cmd->args[i].error.importance == 3)
			return true;
	}
	return false;
}
