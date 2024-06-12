#include "header.h"

void free_command(Command *cmd) {
	for (int i = 0; i < cmd->size; i++) {
		free(cmd->args[i].content);
	}
	free(cmd->args);
	free(cmd->perm_errors);
}

int main(int ac, char **av) {
	Command cmd = get_cmd(ac, av);

	if (fatal_error(&cmd)) {
		free_command(&cmd);
		return 2;
	}

	for (int i = 0; i < cmd.size; i++) {
		if (cmd.args[i].type & ARG) {
			pre_treatment(&cmd.args[i], &cmd);
		}
	}

	for (int i = 0; i < cmd.size; i++) {
		if (cmd.args[i].type & ARG && !cmd.args[i].error.importance) {
			ft_ls(cmd.args[i].content, &cmd);
			ft_printf("\n");
		}
	}

	ft_printf("%s", cmd.perm_errors);
	free_command(&cmd);

	return 0;
}
