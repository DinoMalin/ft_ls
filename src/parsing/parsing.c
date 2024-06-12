#include "header.h"

void get_arg(Arg *curr, char *str) {
	if (str[0] == '-') {
		curr->type = OPTION;
		if (str[1] == '-') {
			curr->type |= LONG_OPTION;
			curr->content = ft_strdup(str + 2);
		} else
			curr->content = ft_strdup(str + 1);
	} else {
		curr->type = ARG;
		curr->content = ft_strdup(str);
	}
}

Arg *parse_args(int ac, char **av) {
	Arg	*result;

	result = ft_calloc((ac - 1), sizeof(Arg));
	for (int i = 1; i < ac; i++) {
		get_arg(&result[i - 1], av[i]);
	}
	return result;
}

void find_last_file(Command *cmd) {
	int index = 0;
	for (int i = 0; i < cmd->size; i++) {
		if (cmd->args[i].type & ARG && !cmd->args[i].error.importance)
			index = i;
	}
	cmd->last_file = index;
}

Command get_cmd(int ac, char **av) {
	Command result = {};

	result.args = parse_args(ac, av);
	result.size = ac - 1;
	result.perm_errors = ft_strdup("");
	get_flags(&result);

	for (int i = 0; i < result.size; i++) {
		if (result.args[i].type & ARG)
			result.nb_file++;
	}

	if (result.nb_file > 1)
		result.flags |= basic_display;

	return result;
}
