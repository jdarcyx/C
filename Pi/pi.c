#include <stdio.h>
#include <limits.h>

double pi (unsigned long long int max)
{
	double pi = 0;
	unsigned long long int div = 1;
	unsigned long long int i = 0;
	while (i < max)
	{
		if (i++ % 2)
			pi -= 4. / div;
		else
			pi += 4. / div;
		div += 2;
	}
	return pi;
}

int main (int argc, char **argv)
{
	printf("%.50lf\n", pi(ULLONG_MAX));
	return 0;
}
