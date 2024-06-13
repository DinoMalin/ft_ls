#include "header.h"

char *clean_join(char *origin, const char *to_join) {
	char *tmp = origin;
	char *res = ft_strjoin(origin, to_join);
	free(tmp);
	return res;
}

int strcmp_caseless(char *str1, char *str2) {
	size_t	i;

	i = 0;
	while (str1[i] && str2[i]) {
		char c1 = ft_tolower(str1[i]);
		char c2 = ft_tolower(str2[i]);
	
		if (c1 != c2)
			return (unsigned char)c1 - (unsigned char)c2;

		i++;	
	}
	return ((unsigned char)str1[i] - (unsigned char)str2[i]);
}

int	compare_name(File *a, File *b) {
	return strcmp_caseless(a->name, b->name);
}

void sort(File *parent, int (*compare)(File *a, File *b)) {
	for (int i = 0; i < parent->nb_childs; i++) {
		for (int j = i + 1; j < parent->nb_childs; j++) {
			if (compare(parent->childs[i], parent->childs[j]) > 0) {
				File *temp = parent->childs[j];
				parent->childs[j] = parent->childs[i];
				parent->childs[i] = temp;
			}
		}
	}
}
