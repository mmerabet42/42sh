#include <stdio.h>

int main(int argc, char **argv)
{
	(void)argc;
	while (*argv)
		printf("l: '%s'\n", *argv++);
	return (0);
}
