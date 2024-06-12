#include "header.h"

void free_command(Command *cmd) {
	for (int i = 0; i < cmd->size; i++) {
		free(cmd->args[i].content);
	}
	free(cmd->args);
}

int main(int ac, char **av) {
	Command cmd = get_cmd(ac, av);

	if (fatal_error(&cmd)) {
		free_command(&cmd);
		return 2;
	}


	for (int i = 0; i < cmd.size; i++) {
		if (cmd.args[i].type & ARG) {
			ft_ls(cmd.args[i].content);
			ft_printf("\n");
		}
	}

	free_command(&cmd);
}
