#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "Compilers.h"
#include "Step1Coder.h"
#include "Step5Writer.h"

grevity_intg main5Writer(grevity_intg argc, grevity_strg* argv) {
	if (argc >= 3) {
		grevity_strg source = argv[2];
		printf("Reading file %s ....Please wait\n", source);
		grevity_intg size = getSizeOfFile(source);
		grevity_strg content = vigenereMem(source, STR_LANGNAME, DECYPHER);
		if (size <= 0 || !content) {
			printf("The input file %s %s\n", source, "is not completely loaded.");
		}
		else {
			process_content(content);
			free(content);
		}
	}
	return EXIT_SUCCESS;
}
