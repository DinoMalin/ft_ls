#include "header.h"

void get_arg(Arg *curr, char *str, int *no_more_opt) {
	if (str[0] == '-' && !*no_more_opt) {
		curr->type = OPTION;
		if (str[1] == '-') {
			if (!str[2]) {
				curr->type |= END_OPTION;
				curr->type &= ~LONG_OPTION;
				*no_more_opt = true;
			} else {
				curr->type |= LONG_OPTION;
			}
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
	int no_more_opt = false;

	result = ft_calloc((ac - 1), sizeof(Arg));
	for (int i = 1; i < ac; i++) {
		get_arg(&result[i - 1], av[i], &no_more_opt);
	}
	return result;
}

int get_cols() {
	struct winsize w;

	if (ioctl(STDOUT_FILENO, TIOCGWINSZ, &w) == -1)
		return -1;
	return w.ws_col;
}

void free_matrix(char **arr) {
	if (!arr)
		return ;
	for (int i = 0; arr[i]; i++) {
		free(arr[i]);
	}
	free(arr);
}

void parse_colors(Command *cmd, char **env) {
	char **colors = NULL;
	for (int i = 0; env[i]; i++) {
		if (ft_strncmp(env[i], "LS_COLORS", 9))
			continue;
		colors = ft_split(env[i] + 9, ':');
		break;
	}
	if (!colors) {
		cmd->error_colors = true;
		return ;
	}
	
	int size = 0;
	while (colors[size])
		size++;

	cmd->colors = ft_calloc(size, sizeof(Color));

	for (int i = 0; colors[i]; i++) {
		char **split = ft_split(colors[i], '=');
		if (!split || !split[0] || !split[1]) {
			free_colors(cmd);
			free_matrix(colors);
			free_matrix(split);
			cmd->n_colors = 0;
			cmd->error_colors = true;
			return ;
		}
		cmd->colors[i].identifier = ft_strdup(split[0]);
		cmd->colors[i].color = ft_strdup(split[1]);
		cmd->n_colors++;
		free_matrix(split);
	}
	free_matrix(colors);
}

Command *init_cmd(int ac, char **av, char **env) {
	Command *result = ft_calloc(1, sizeof(Command));

	result->args = parse_args(ac, av);
	result->size = ac - 1;
	get_flags(result);
	if (result->flags & colors)
		parse_colors(result, env);

	for (int i = 0; i < result->size; i++) {
		if (result->args[i].type & ARG)
			result->nb_file++;
	}

	if (result->nb_file > 1 || result->flags & recursive)
		result->flags |= basic_display;

	result->cols = get_cols();
	if (result->cols <= 0)
		result->def = true;

	if (result->nb_file == 0) {
		result->file_system = malloc(sizeof(File *));
		result->file_system[0] = ft_calloc(1, sizeof(File));
		result->file_system[0]->path = ft_strdup(".");
		if (!analyze_file(result->file_system[0], result->flags & long_display))
			result->return_status = 2;
		result->nb_file = 1;
		return result;
	}

	result->file_system = malloc(result->nb_file * sizeof(File *));

	for (int i = 0, j = 0; i < result->size; i++) {
		if (result->args[i].type & ARG) {
			result->file_system[j] = ft_calloc(1, sizeof(File));
			result->file_system[j]->path = ft_strdup(result->args[i].content);
			if (!analyze_file(result->file_system[j++], result->flags & long_display))
				result->return_status = 2;
		}
	}
	return result;
}
