#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int main(int argc, char *argv[]) {
	srand(time(NULL));
	int par1 = (argc > 1) ? strtol(argv[1], NULL, 10) : rand() % (8 * sizeof(int));
	int par2 = (argc > 2) ? strtol(argv[2], NULL, 10) : rand() % (8 * sizeof(int));
	int par3 = (argc > 3) ? strtol(argv[3], NULL, 10) : rand() % (8 * sizeof(int));
	fprintf(stderr, "%d %d %d\n", par1, par2, par3);

	for (int t = 0;; ++t) {
		putchar(t * ((t >> par1) | (t >> par2) | (t >> par3)));
	}
	
	return EXIT_SUCCESS;
}

