/*
 * Usage: gcc -DDEBUG inset_sort.c
 */

#include <stdio.h>
#include "common.h"
#include "insert_sort.h"

int arr[] = {53, 27, 36, 15, 69, 42};
int brr[] = {53, 27, 36, 15, 69, 42};

int main(int argc, char **argv)
{
	int len;

	len = ARRAY_LEN(arr);
	insert_sort(arr, len);

	InsertionSort(brr, ARRAY_LEN(brr));

	return 0;
}
