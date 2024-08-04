#include "header.h"

void check_return_status(Command *cmd) {
	for (int i = 0; i < cmd->nb_file; i++) {
		if (cmd->file_system[i]->error)
			cmd->return_status = 2;
	}
}

void display_file_system(Command *cmd) {
	for (int i = 0; i < cmd->nb_file; i++) {
		ft_ls(cmd, cmd->file_system[i]);
	}
	check_return_status(cmd);
}

void list_regular_file(Command *cmd) {
	int	regular_files = 0;
	int	errors = 0;
	Padding padding = {};

	if (cmd->flags & long_display) {
		for (int i = 0; i < cmd->nb_file; i++) {
			if (cmd->file_system[i]->type != DIRECTORY && cmd->file_system[i]->type != SYMLINK && !cmd->file_system[i]->error)
				calculate_padding(&padding, cmd->file_system[i]);
			cmd->last_file = i;
		}
	}

	for (int i = 0; i < cmd->nb_file; i++) {
		if (cmd->file_system[i]->type != DIRECTORY && cmd->file_system[i]->type != SYMLINK && !cmd->file_system[i]->error) {
			display_file(cmd, cmd->file_system[i], &padding, true);
			if (cmd->def && i != cmd->last_file && !(cmd->flags & long_display))
				ft_printf("\n");
			else if (i != cmd->last_file && !(cmd->flags & long_display))
				ft_printf("  ");
		}
		if (cmd->file_system[i]->type != DIRECTORY && cmd->file_system[i]->type != SYMLINK && !cmd->file_system[i]->error) 
			regular_files++;
		if (cmd->file_system[i]->error)
			errors++;
	}


	if (regular_files && cmd->nb_file != regular_files + errors && !(cmd->flags & long_display))
		ft_printf("\n");
	else if (regular_files && cmd->nb_file != regular_files + errors)
		ft_printf("\n");
	else if (regular_files && !(cmd->flags & long_display))
		ft_printf("\n");
}

int main(int ac, char **av, char **env) {
	setlocale(LC_ALL, "");
	Command *cmd = init_cmd(ac, av, env);

	if (cmd->flags & help) {
		ft_printf(HELP_MSG);
		return free_command(cmd);
	} else if (cmd->flags & version) {
		ft_printf(VERSION_MSG);
		return free_command(cmd);
	}
	if (fatal_error(cmd)) {
		ft_printf(TRY_HELP);
		return 2;
	}
	if (cmd->error_colors)
		ft_fprintf(2, ERPARSECOL);

	sort(cmd, cmd->file_system, cmd->nb_file);
	list_regular_file(cmd);
	display_file_system(cmd);
	Padding placeholder = {};

	for (int i = 0; i < cmd->nb_file; i++) {
		if (cmd->flags & dir_only)
			display_file(cmd, cmd->file_system[i], &placeholder, i == cmd->nb_file - 1);
	}

	return free_command(cmd);
}
