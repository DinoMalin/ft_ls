#include "header.h"

int compare_name(File *a, File *b) {
	return strcoll(a->name, b->name);
}

int compare_time(File *a, File *b) {
	if (a->last_modif < b->last_modif)
		return 1;
	return 0;
}

void sort(File **arr, int size, int (*compare)(File *a, File *b)) {
	for (int i = 0; i < size; i++) {
		for (int j = i + 1; j < size; j++) {
			if (compare(arr[i], arr[j]) > 0) {
				File *temp = arr[j];
				arr[j] = arr[i];
				arr[i] = temp;
			}
		}
	}
}
