#include <stdio.h>

struct Person {
	int age;
	char *name;
	int number;
};

/* typedef结构体数组 */
typedef struct Person Persons[];

int main(int argc, char *argv[])
{
	Persons person = {
		{ 11, "tom", 33 },
		{ 22, "jerry", 44 }
	};

	for (int i = 0; i < 2; i++)
		printf("%s %d [%d]\n", person[i].name, person[i].age, person[i].number);

	return 0;
}
