#include "header.h"

void calculate_size(Size *size, File *node) {
	if (ft_strlen(node->nb_links) > size->link)
		size->link = ft_strlen(node->nb_links);
	if (ft_strlen(node->size) > size->size_minor)
		size->size_minor = ft_strlen(node->size);
	if (ft_strlen(node->owner) > size->owner)
		size->owner = ft_strlen(node->owner);
	if (ft_strlen(node->group) > size->group)
		size->group = ft_strlen(node->group);
	if (node->type == CHARACTER && ft_strlen(node->major) > size->major)
		size->major = ft_strlen(node->major);
	if (node->type == CHARACTER && ft_strlen(node->minor) > size->size_minor)
		size->size_minor = ft_strlen(node->minor);
}

int	add_to_file_system(File *parent, struct dirent *entry, bool long_display) {
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

	if (entry->d_type == DT_DIR)
		new_entry->type = DIRECTORY;
	else if (entry->d_type == DT_LNK)
		new_entry->type = SYMLINK;
	else if (entry->d_type == DT_CHR)
		new_entry->type = CHARACTER;
	else if (entry->d_type == DT_BLK)
		new_entry->type = BLOCK;
	else if (entry->d_type == DT_FIFO)
		new_entry->type = PIPE;
	else if (entry->d_type == DT_SOCK)
		new_entry->type = SOCKET;
	else
		new_entry->type = REGULAR_FILE;

	int len = ft_strlen(new_entry->name);
	if (parent->len.max_el < len)
		parent->len.max_el = len;

	if (!analyze_file(new_entry, long_display))
		return 0;
	if (!long_display || new_entry->error)
		return 1;
	calculate_size(&parent->len, new_entry);
	parent->total += new_entry->blocks / 2;
	return 1;
}
