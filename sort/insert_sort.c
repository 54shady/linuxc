#include "insert_sort.h"
#include "common.h"

#define DEBUG

void insert_sort(int arr[], int len)
{
	int i, j, key;

	/* 从第一个数开始遍历 */
	for (j = 1; j < len; j++)
	{
		/* 保存每次遍历的这个数,作为比较数 */
		key = arr[j];

#ifdef DEBUG
		if (j == 1)
			insert_sort_show_array(arr, len, j);
#endif

		/*
		 * 从最开始取数和子序列比较(i = j - 1)
		 * 如果满足条件,进行交换
		 */
		for (i = j - 1; i >= 0 && arr[i] > key; i--)
			arr[i + 1] = arr[i];
		arr[i + 1] = key;

#ifdef DEBUG
		insert_sort_show_array(arr, len, j + 1);
#endif
	}
}

/*
 * A{a, b, c, d, e}
 *         ^     ^
 *         |     |
 *  [...] (P)    N
 *         ^
 *         |
 *         Tmp
 */
void show_each_round(ElementType A[], int N, int P)
{
	/* suqare brace already sorted element */
	printf("[");
	show_arrayx(A, P);
	printf("]");

	/* circul brace current key */
	printf("(");
	show_arrayx(&A[P], 1);
	printf(")");

	/* the reset */
	show_arrayx(&A[P+1], N - P - 1);
	printf("\n");
}

void InsertionSort(ElementType A[], int N)
{
	int j, P;
	ElementType Tmp;

	/* 从第一个数开始遍历 */
	for (P = 1; P < N; P++)
	{
		/* 先保存未排序的数 */
		Tmp = A[P];

#ifdef DEBUG
		show_each_round(A, N, P);
#endif

		/* 排成左小右大的顺序 */
		for (j = P; j > 0 && A[j -1] > Tmp; j--)
			A[j] = A[j - 1];
		A[j] = Tmp;
	}

#ifdef DEBUG
	printf("[");
	show_arrayx(A, N);
	printf("]\n");
#endif
}
