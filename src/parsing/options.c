#include "header.h"

char *long_options[] = {"recursive", "reverse", "all", "help", "quote-name", "directory", "almost-all", "color=auto", "color=never", NULL};
char short_options[] = "lRartmQgdA";

int ambiguous_option(Arg *arg) {
	arg->error = true;
	ft_fprintf(2, ERAMBIGUOUS, arg->content);
	ft_fprintf(2, " possibilites: ");
	for (int i = 0; long_options[i]; i++) {
		if (!ft_strncmp(arg->content, long_options[i], ft_strlen(arg->content)))
			ft_fprintf(2, " '--%s'", long_options[i]);
	}
	ft_printf("\n");
	return 1;
}

int invalid_option(Arg *arg, int index) {
	arg->error = true;
	if (index == -1)
		ft_fprintf(2, ERNOOPT, arg->content);
	else
		ft_fprintf(2, ERNOOPTC, arg->content[index]);
	return 1;
}

void put_flag(Command *cmd, char flag) {
	if (flag == 'l' && !(cmd->flags & commas))
		cmd->flags |= long_display;
	else if (flag == 'R')
		cmd->flags |= recursive;
	else if (flag == 'a') {
		cmd->flags |= all;
		cmd->flags |= dotfiles;
	} else if (flag == 'r')
		cmd->flags |= reverse;
	else if (flag == 't')
		cmd->flags |= time_modif;
	else if (flag == 'h')
		cmd->flags |= help;
	else if (flag == 'm') {
		cmd->flags |= commas;
		cmd->flags &= ~long_display;
	} else if (flag == 'Q')
		cmd->flags |= quotes;
	else if (flag == 'g') {
		cmd->flags |= long_display;
		cmd->flags |= no_owner;
	} else if (flag == 'd')
		cmd->flags |= dir_only;
	else if (flag == 'A')
		cmd->flags |= dotfiles;
	else if (flag == 'c')
		cmd->flags |= colors;
	else if (flag == 'x')
		cmd->flags &= ~colors;
}

int check_long_option(Command *cmd, Arg *arg) {
	int index = -1;

	for (int i = 0; long_options[i]; i++) {
		if (!ft_strncmp(arg->content, long_options[i], ft_strlen(arg->content))) {
			if (index != -1)
				return ambiguous_option(arg);

			switch (i) {
				case 0:
					put_flag(cmd, 'R');
					break;
				case 1:
					put_flag(cmd, 'r');
					break;
				case 2:
					put_flag(cmd, 'a');
					break;
				case 3:
					put_flag(cmd, 'h');
					break;
				case 4:
					put_flag(cmd, 'Q');
					break;
				case 5:
					put_flag(cmd, 'd');
					break;
				case 6:
					put_flag(cmd, 'A');
					break;
				case 7:
					put_flag(cmd, 'c');
					break;
				case 8:
					put_flag(cmd, 'x');
					break;
			}
			index = i;
		}			
	}
	if (index == -1)
		return invalid_option(arg, index);
	return 0;
}

int check_short_option(Command *cmd, Arg *arg) {
	for (int i = 0; arg->content[i]; i++) {
		if (!ft_strchr(short_options, arg->content[i]))
			return invalid_option(arg, i);
		put_flag(cmd, arg->content[i]);	
	}
	return 0;
}

void get_flags(Command *cmd) {
	for (int i = 0; i < cmd->size; i++) {
		if (!(cmd->args[i].type & OPTION))
			continue;
		if (cmd->args[i].type & LONG_OPTION) {
			if (check_long_option(cmd, &cmd->args[i]))
				return;
		} else if (check_short_option(cmd, &cmd->args[i]))
			return ;
	}
}
