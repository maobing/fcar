#include "stdlib.h"
#include "stdio.h"
#include "string.h"
#include "fcarLib.h"

int menu_countCoverage(int argc, char **argv);

int main(int argc, char **argv) {

	/* menu */
	menu_countCoverage(argc, argv);

	/* exit */
	exit(EXIT_SUCCESS);
}

int menu_countCoverage(int argc, char **argv) {

	/* ------------------------------- */
	/*        countCoverage            */
	/* -i htsFilesList                 */
  /* @NOTE: this list contains both  */
  /* file names and parameters       */
	/* ------------------------------- */

	if (argc == 1) {
		printf("/*-----------------------------------*/\n");
		printf("/*            extractFeature         */\n");
		printf("/* -i hts file list                  */\n");
		printf("/*-----------------------------------*/\n");
		return(0);
	}

	char *htsFilesList = (char *)calloc(MAX_DIR_LEN, sizeof(char)); 
	int ni;
	int iOK = 0;

	ni = 1;
	while (ni < argc) {
		if (strcmp(argv[ni], "-i") == 0) {
			ni++;
			strcpy(htsFilesList, argv[ni]);
			iOK = 1;
		}
		else {
			printf("Error: unkown parameters!\n");
			return(-1);
		}
		ni++;
	}

	/* check args */
	if (iOK < 1){
		printf("Error: input arguments not correct!\n");
    return(-1);
	}
	else {
		coverage(htsFilesList);
	}

	/* free pointers */
	free(htsFilesList);

	return 0;
}
