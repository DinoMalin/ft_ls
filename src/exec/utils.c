#include "header.h"

void calculate_size(Size *size, File *node) {
	if (ft_strlen(node->nb_links) > size->link)
		size->link = ft_strlen(node->nb_links);
	if (ft_strlen(node->size) > size->size)
		size->size = ft_strlen(node->size);
	if (ft_strlen(node->owner) > size->owner)
		size->owner = ft_strlen(node->owner);
	if (ft_strlen(node->group) > size->group)
		size->group = ft_strlen(node->group);
}

void	add_to_file_system(File *parent, struct dirent *entry, bool long_display) {
	File **new_file_system = malloc((parent->nb_childs + 1) * sizeof(File*));

	for (int i = 0; i < parent->nb_childs; i++) {
		new_file_system[i] = parent->childs[i];
	}
	free(parent->childs);
	parent->childs = new_file_system;

	File *new_entry = ft_calloc(1, sizeof(File));
	new_file_system[parent->nb_childs] = new_entry;
	parent->nb_childs++;

	new_entry->name = ft_strdup(entry->d_name);
	new_entry->path = ft_strdup(parent->path);
	new_entry->path = clean_join(new_entry->path, "/");
	new_entry->path = clean_join(new_entry->path, new_entry->name);

	analyze_file(new_entry, long_display);
	if (!long_display)
		return ;
	calculate_size(&parent->size_childs, new_entry);
	parent->total += new_entry->blocks / 2;
}

void	add_file_to_link(File *link) {
	struct stat statbuf;

	if (lstat(link->link_to, &statbuf) == -1) {
		link->type = DEAD_LINK;
		link->link_type = DEAD_LINK;
		return ;
	}

	if (S_ISDIR(statbuf.st_mode))
		link->link_type = DIRECTORY;
	else if (S_ISREG(statbuf.st_mode))
		link->link_type = REGULAR_FILE;
}