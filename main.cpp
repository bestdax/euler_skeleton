#include "prt.h"

int main(int argc, char *argv[])
{
	ProjectTemplate t;

	if(argc < 2) return 1;

	t.create_project(argv[1]);
	return 0;
}
