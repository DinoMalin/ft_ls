#include "header.h"

void	add_to_file_system(File *parent, struct dirent *entry) {
	File **new_file_system = malloc((parent->nb_childs + 1) * sizeof(File));

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

	analyze_file(new_entry);
}
