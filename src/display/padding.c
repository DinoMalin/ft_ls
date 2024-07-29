#include "header.h"

void put_spaces_str(char *str, int max_size, int curr_size) {
	ft_printf("%s ", str);
	while (curr_size++ < max_size) {
		ft_putchar_fd(' ', 1);
	}
}

void put_spaces_nbr(int nbr, int max_size, bool commas) {
	int curr_size = get_size_nbr(nbr);
	while (curr_size++ < max_size) {
		ft_putchar_fd(' ', 1);
	}
	ft_printf("%d%s", nbr, commas ? ", " : " ");
}

void calculate_padding(Padding *padding, File *node) {
	int len_links = get_size_nbr(node->nb_links);
	int len_size = get_size_nbr(node->size);
	int len_major = get_size_nbr(node->major);
	int len_minor = get_size_nbr(node->minor);

	if (node->has_ext)
		padding->one_got_ext = true;
	if (len_links > (int)padding->link)
		padding->link = len_links;
	if (len_size > (int)padding->size_minor)
		padding->size_minor = len_size;
	if (ft_strlen(node->owner) > padding->owner)
		padding->owner = ft_strlen(node->owner);
	if (ft_strlen(node->group) > padding->group)
		padding->group = ft_strlen(node->group);
	if (node->type == CHARACTER && len_major > (int)padding->major)
		padding->major = len_major;
	if (node->type == CHARACTER && len_minor > (int)padding->size_minor)
		padding->size_minor = len_minor;
}

void get_padding(Command *cmd, File *parent, Padding *padding) {
	if (cmd->flags & long_display) {
		for (int i = 0; i < parent->nb_childs; i++) {
			calculate_padding(padding, parent->childs[i]);
		}
		return ;
	}

	int total_len = ((parent->longest_el + 2) * parent->nb_childs) - 2;

	padding->n_lines = round_split(total_len, cmd->cols);
	if (!padding->n_lines)
		padding->n_lines = 1;

	padding->n_cols = round_split(parent->nb_childs, padding->n_lines);
	padding->cols = ft_calloc(padding->n_cols, sizeof(Col));

	return ;
}