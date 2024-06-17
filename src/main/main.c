#include "header.h"

void check_return_status(Command *cmd) {
	for (int i = 0; i < cmd->nb_file; i++) {
		if (cmd->file_system[i]->error)
			cmd->return_status = 2;
	}
}

int main(int ac, char **av) {
	Command *cmd = init_cmd(ac, av);

	if (cmd->size == 1 && cmd->flags & help) {
		ft_printf(HELP_MSG);
		return 0;
	}
	if (fatal_error(cmd)) {
		ft_printf(TRY_HELP);
		return 2;
	}

	for (int i = 0; i < cmd->nb_file; i++) {
		ft_ls(cmd, cmd->file_system[i]);
	}
	check_return_status(cmd);

	sort(cmd->file_system, cmd->nb_file,
		cmd->flags & time_modif ? compare_time : compare_name);	

	bool files_in_args = false;
	for (int i = 0; i < cmd->nb_file; i++) {
		if (files_in_args)
			ft_printf(" ");
		if (cmd->file_system[i]->type == REGULAR_FILE) {
			ft_printf("%s%s%s%s",
				cmd->flags & quotes ? "\"" : "",
				cmd->file_system[i]->path,
				cmd->flags & quotes ? "\"" : "",
				cmd->flags & commas && i < cmd->nb_file - 1 ? "," : ""
			);
			files_in_args = true;
		}
	}
	if (files_in_args)
		ft_printf("\n\n");
	for (int i = 0; i < cmd->nb_file; i++) {
		display(cmd, cmd->file_system[i]);
	}

	return free_command(cmd);
}
