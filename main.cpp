#include "prt.h"

int main(int argc, char *argv[])
{
	ProjectTemplate t;

	if(argc < 3) return 1;

	if(strcmp(argv[1], "euler") == 0)
		t.create_euler_project(argv[2]);

	if(strcmp(argv[1], "algo") == 0)
		t.create_algorithm_project();

	return 0;
}
