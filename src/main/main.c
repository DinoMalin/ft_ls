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

	result = malloc((ac - 1) * sizeof(Arg));
	for (int i = 1; i < ac; i++) {
		get_arg(&result[i - 1], av[i]);
	}
	return result;
}

int main(int ac, char **av) {
	Arg *args = parse_args(ac, av);

	for (int i = 0; i < ac - 1; i++) {	
		if (args[i].type & OPTION)
			ft_printf("OPTION (%s)\n%s\n\n", args[i].type & LONG_OPTION ? "LONG" : "SIMPLE", args[i].content);
		else if (args[i].type & ARG)
			ft_printf("ARG\n%s\n\n", args[i].content);
		else
			ft_printf("I have no idea what this thing is");	
	}
}
