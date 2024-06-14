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

void analyze_file(File *file) {
	struct stat statbuf;

	if (stat(file->path, &statbuf) == -1) {
		ft_fprintf(2, "ERROR STAT");
		file->error = ft_strdup(strerror(errno));
		return ;
	}

	if (S_ISDIR(statbuf.st_mode))
		file->type = DIRECTORY;
	else if (S_ISLNK(statbuf.st_mode))
		file->type = SYMLINK;
	else if (S_ISREG(statbuf.st_mode))
		file->type = REGULAR_FILE;
}

Command *init_cmd(int ac, char **av) {
	Command *result = ft_calloc(1, sizeof(Command));

	result->args = parse_args(ac, av);
	result->size = ac - 1;
	result->perm_errors = ft_strdup("");
	get_flags(result);

	for (int i = 0; i < result->size; i++) {
		if (result->args[i].type & ARG)
			result->nb_file++;
	}

	if (result->nb_file > 1 || result->flags & recursive)
		result->flags |= basic_display;

	if (result->nb_file == 0) {
		result->file_system = malloc(sizeof(File *));
		result->file_system[0] = ft_calloc(1, sizeof(File));
		result->file_system[0]->path = ft_strdup(".");
		result->nb_file++;
		return result;
	}

	result->file_system = malloc(result->nb_file * sizeof(File *));

	for (int i = 0, j = 0; i < result->size; i++) {
		if (result->args[i].type & ARG) {
			result->file_system[j] = ft_calloc(1, sizeof(File));
			result->file_system[j]->name = ft_strdup(result->args[i].content);
			result->file_system[j]->path = ft_strdup(result->args[i].content);
			analyze_file(result->file_system[j++]);
		}
	}



	return result;
}
