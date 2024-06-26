#include "header.h"

void check_return_status(Command *cmd) {
	for (int i = 0; i < cmd->nb_file; i++) {
		if (cmd->file_system[i]->error)
			cmd->return_status = 2;
	}
}

void display_file_system(Command *cmd) {
	sort(cmd, cmd->file_system, cmd->nb_file);
	for (int i = 0; i < cmd->nb_file; i++) {
		ft_ls(cmd, cmd->file_system[i]);
	}
	check_return_status(cmd);
}

void list_regular_file(Command *cmd) {
	int regular_files = 0;
	Size size = {};

	for (int i = 0; i < cmd->nb_file; i++) {
		if (cmd->file_system[i]->type == REGULAR_FILE && cmd->flags & long_display)
			calculate_size(&size, cmd->file_system[i]);
	}

	if (cmd->flags & reverse) {
		for (int i = cmd->nb_file - 1; i >= 0; i--) {
			if ((cmd->file_system[i]->type == REGULAR_FILE || cmd->file_system[i]->type == EXECUTABLE) && !cmd->file_system[i]->error) {
				display_file(cmd, cmd->file_system[i], &size, true);
				if (cmd->def && i > 0)
					ft_printf("\n");
				else if (i > 0)
					ft_printf("  ");
				regular_files++;
			}
		}
	} else {
		for (int i = 0; i < cmd->nb_file; i++) {
			if ((cmd->file_system[i]->type == REGULAR_FILE || cmd->file_system[i]->type == EXECUTABLE) && !cmd->file_system[i]->error) {
				display_file(cmd, cmd->file_system[i], &size, true);
				if (cmd->def && i < cmd->nb_file - 1)
					ft_printf("\n");
				else if (i < cmd->nb_file - 1)
					ft_printf("  ");
				regular_files++;
			}
		}
	}

	if (regular_files && cmd->nb_file != regular_files)
		ft_printf("\n\n");
	else if (regular_files)
		ft_printf("\n");
}

int main(int ac, char **av, char **env) {
	setlocale(LC_ALL, "");
	Command *cmd = init_cmd(ac, av, env);

	if (cmd->size == 1 && cmd->flags & help) {
		ft_printf(HELP_MSG);
		return 0;
	}
	if (fatal_error(cmd)) {
		ft_printf(TRY_HELP);
		return 2;
	}
	if (cmd->error_colors)
		ft_fprintf(2, ERPARSECOL);

	list_regular_file(cmd);
	display_file_system(cmd);
	Size placeholder = {};

	for (int i = 0; i < cmd->nb_file; i++) {
		if (cmd->flags & dir_only)
			display_file(cmd, cmd->file_system[i], &placeholder, i == cmd->nb_file - 1);
	}

	return free_command(cmd);
}
