#include <stdio.h>
#include "common.h"

void swap(int *a, int *b)
{
	int tmp;

	tmp = *a;
	*a = *b;
	*b = tmp;
}

void show_array(int array[], int len)
{
	int i;

	for (i = 0; i < len; i++)
		printf("%3d ", array[i]);
	printf("\n");
}

void show_arrayx(int array[], int len)
{
	int i;

	for (i = 0; i < len; i++)
		printf("%3d ", array[i]);
}

void insert_sort_show_array(int array[], int len, int key_index)
{
	int i;

	if (0 != key_index)
		printf("[");

	for (i = 0; i < len; i++)
	{
		if (i == key_index && key_index > 0)
		{
			printf("(%d)\t", array[i]);
			continue;
		}
		if (i == key_index - 1)
		{
			printf("%d]\t", array[i]);
			continue;
		}
		else
			printf("%d\t", array[i]);
	}
	printf("\n");
}
