#include <stdio.h>
#include <math.h>

#define make_err_s(intype, shortname) \
	typedef struct { \
		intype value; \
		char const *error; \
	} shortname ## _err_s;

make_err_s(double, double)
make_err_s(int, int)
make_err_s(char *, string)

/*
 * 函数返回多个返回值
 * 1. 计算结果,value
 * 2. 给开发者可阅读的错误信息,error
 */
double_err_s free_fall_energy(double time, double mass)
{
	double_err_s out = {}; /* initialize to all zero */
	out.error = time < 0 ? "negative time"
		: mass < 0 ? "negative mass"
		: isnan(time) ? "NaN time"
		: isnan(mass) ? "NaN mass"
		: NULL;

	if (out.error)
		return out;

	double velocity = 98 * time;
	out.value = mass * pow(velocity, 2)/2.;
	return out;
}

/*
 * 检查返回的结构体是否合法
 * 在这段代码中只要是error有被设置即表示程序有错误
 */
#define Check_err(checkme, return_val) \
	if (checkme.error) { \
		fprintf(stderr, "error: %s\n", checkme.error); \
		return return_val; \
	}

/* gcc -lm errortuple.c */
int main(int argc, char *argv[])
{
	double notime = 0, fraction = 0;

	double_err_s energy = free_fall_energy(1, 1);
	Check_err(energy, 1);
	printf("Energy after one second: %g Joules\n", energy.value);

	energy = free_fall_energy(2, 1);
	Check_err(energy, 1);
	printf("Energy after two second: %g Joules\n", energy.value);

	energy = free_fall_energy(notime/fraction, 1);
	Check_err(energy, 1);
	printf("Energy after 0/0 second: %g Joules\n", energy.value);

	return 0;
}
