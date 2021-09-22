#include <stdio.h>

/* 一个3x3的图形中星号的位置 */
typedef struct {
	char *name;
	int left, right, up, down;
} direction_s;

/* 判断需要在那里打印星号,使用嵌套三目运算 */
void this_row(direction_s d)
{
	printf(d.left ? "*..\n"
			: d.right ? "..*\n"
			: ".*.\n");
}

void draw_box(direction_s d)
{
	printf("%s\n", (d.name ? d.name : "a box"));
	d.up ? this_row(d) : printf("...\n");
	(!d.up && !d.down)? this_row(d) : printf("...\n");
	d.down ? this_row(d) : printf("...\n");
	printf("\n");
}

int main(int argc, char *argv[])
{
	direction_s d = {.name = "left", .left = 1};
	draw_box(d);

	d = (direction_s){"upper right", .up = 1, .right = 1};
	draw_box(d);

	return 0;
}
