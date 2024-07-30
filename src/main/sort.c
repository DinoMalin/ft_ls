#include "header.h"

int compare_name(File *a, File *b) {
	return strcoll(NAME(a), NAME(b));
}

int compare_time(File *a, File *b) {
	if (a->last_modif < b->last_modif)
		return 1;
	else if ((a->last_modif > b->last_modif))
		return 0;
	return compare_name(a, b);
}

static void swap(File **a, File **b) {
	File *temp = *a;
	*a = *b;
	*b = temp;
}

static int partition(Command *cmd, File **arr, int low, int high, int (*compare)(File *a, File *b)) {
	File *pivot = arr[high];
	int i = low - 1;
	
	for (int j = low; j < high; j++) {
		if ((cmd->flags & reverse && (compare(arr[j], pivot) > 0))
			|| (!(cmd->flags & reverse) && compare(arr[j], pivot) <= 0)) {
			i++;
			swap(&arr[i], &arr[j]);
		}
	}
	swap(&arr[i + 1], &arr[high]);
	return i + 1;
}

static void quicksort(Command *cmd, File **arr, int low, int high, int (*compare)(File *a, File *b)) {
	if (low < high) {
		int pi = partition(cmd, arr, low, high, compare);
		quicksort(cmd, arr, low, pi - 1, compare);
		quicksort(cmd, arr, pi + 1, high, compare);
	}
}

void sort(Command *cmd, File **arr, int size) {
	if (cmd->flags & no_sort)
		return;
	else if (cmd->flags & last_modif_sort)
		quicksort(cmd, arr, 0, size - 1, compare_time);
	else
		quicksort(cmd, arr, 0, size - 1, compare_name);
}
