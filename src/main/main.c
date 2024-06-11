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
		if (cmd.args[i].type & OPTION)
			ft_printf("OPTION (%s)\n%s\n\n", cmd.args[i].type & LONG_OPTION ? "LONG" : "SIMPLE", cmd.args[i].content);
		else if (cmd.args[i].type & ARG)
			ft_printf("ARG\n%s\n\n", cmd.args[i].content);
		else
			ft_printf("I have no idea what this thing is\n%s\n\n", cmd.args[i].content);	
	}

	ft_printf("--FLAGS\n");
	if (cmd.flags & recursive)
		ft_printf("\t-Recursive\n");
	if (cmd.flags & list)
		ft_printf("\t-List\n");
	if (cmd.flags & all)
		ft_printf("\t-All\n");	
	if (cmd.flags & reverse)
		ft_printf("\t-Reverse\n");	
	if (cmd.flags & time)
		ft_printf("\t-Time\n");	
	if (cmd.flags & help)
		ft_printf("\t-Help\n");

	free_command(&cmd);	
}
