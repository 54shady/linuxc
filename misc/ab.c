#include <stdio.h>
#include <string.h>

#define elemType int
#define MAX_NR 255
#define LENGTH(a) ((sizeof(a)) / (sizeof(a[0])))

/* 集合数据结构 */
typedef struct set {
	int len;
	elemType data[MAX_NR];
	char name[20];
} set_t;

/* A与B的交集(A∩B):既属于A又属于B的元素构成的集合 */
int set_intersection(set_t A, set_t B, set_t *dest)
{
	int i, j;
	int k = 0;

	for (i = 0; i < A.len; i++)
	{
		for (j = 0; j < B.len; j++)
		{
			if (A.data[i] == B.data[j])
				dest->data[k++] = A.data[i];
		}
	}
	dest->len = k;

	return (dest->len) ? 1 : 0;
}

/* A与B的并集(AUB):A与B所有元素构成的集合 */
int set_union(set_t A, set_t B, set_t *dest)
{
	int i, j;
	int k = 0;

	for (i = 0; i < A.len; i++)
	{
		for (j = 0; j < B.len; j++)
			/* 既属于A又属于B的元素,跳过 */
			if (A.data[i] == B.data[j])
				break;

		/* 属于A但不属于B的元素,存入dest */
		if (j == B.len)
			dest->data[k++] = A.data[i];
	}

	/* B的所有元素,存入dest */
	for (j = 0; j < B.len; j++)
		dest->data[k++] = B.data[j];
	dest->len = k;

	return (dest->len) ? 1 : 0;
}

/* B在A中的相对补集:属于A但不属于B的元素构成的集合 */
int set_complement(set_t A, set_t B, set_t *dest)
{
	int i, j;
	int k = 0;

	for (i = 0; i < A.len; i++)
	{
		for (j = 0; j < B.len; j++)
			if (A.data[i] == B.data[j])
				break;

		if (j == B.len)
			dest->data[k++] = A.data[i];
	}
	dest->len = k;

	return (dest->len) ? 1 : 0;
}

int print_set(set_t s)
{
	int i;

	if (s.len == 0)
	{
		printf("The set_t is empty! ");
		return 0;
	}
	printf("%-3s:\t", s.name);
	for (i = 0; i < s.len; i++)
		printf ("%d ", s.data[i]);
	printf("\n");

	return 1;
}

set_t make_set(int array[], int len, char *name)
{
	int i;
	set_t s;

	for (i = 0; i < len; i++)
		s.data[i] = array[i];

	s.len = len;
	strcpy(s.name, name);

	return s;
}

int main(int argc, char *argv[])
{
	set_t A;
	set_t B;
	set_t AIB = {
		.name = "AIB",
	};
	set_t AUB = {
		.name = "AUB",
	};
	set_t ACB = {
		.name = "ACB",
	};
	set_t BCA = {
		.name = "BCA",
	};

	int arraya[] = {11, 1, 2, 3, 99, 41, 5};
	int arrayb[] = {33, 3, 4, 5, 41, 40, 8, 19};

	A = make_set(arraya, LENGTH(arraya), "A");
	B = make_set(arrayb, LENGTH(arrayb), "B");

	set_intersection(A, B, &AIB);
	set_union(A, B, &AUB);
	set_complement(A, B, &ACB);
	set_complement(B, A, &BCA);

	print_set(A);
	print_set(B);
	print_set(AIB);
	print_set(AUB);
	print_set(ACB);
	print_set(BCA);

	return 0;
}
