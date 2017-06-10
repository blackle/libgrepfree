#include <stdio.h>
#include <stdlib.h>

int main(int argc, char * argv[])
{
	char * mem = malloc(10);
	mem[1] = 'a';
	mem[2] = 'b';
	mem[3] = 'c';
	free(mem);
	return 0;
}
