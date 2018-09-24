#include <stdio.h>

int main(int argc, char **argv)
{
	while (*argv)
		printf("arg: '%s'\n", *argv++);
	return (0);
}
