#include "stdlib.h"
#include "stdio.h"
#include "string.h"
#include "fcarLib.h"

int menu_extractFeature(int argc, char **argv);

int main(int argc, char **argv) {

	/* menu */
	menu_extractFeature(argc, argv);

	/* exit */
	exit(EXIT_SUCCESS);
}

int menu_extractFeature(int argc, char **argv) {

	/* ------------------------------- */
	/*        extractFeature           */
	/* -i htsFilesList                */
	/* -t traini region coordinate     */
	/* -o outputFile                   */
	/* ------------------------------- */

	if (argc == 1) {
		printf("/*-----------------------------------*/\n");
		printf("/*            extractFeature         */\n");
		printf("/* -i ths file list                  */\n");
		printf("/* -t training region coordinate     */\n");
		printf("/* -o output file                    */\n");
		printf("/*-----------------------------------*/\n");
		exit(EXIT_SUCCESS);
	}

	char *trainingRegionsFile = (char *)calloc(MAX_DIR_LEN, sizeof(char)); 
	char *outputFile = (char *)calloc(MAX_DIR_LEN, sizeof(char));
	char *htsFilesList = (char *)calloc(MAX_DIR_LEN, sizeof(char));
	int ni;
	int iOK = 0, tOK = 0, oOK = 0;

	ni = 1;
	while (ni < argc) {
		if (strcmp(argv[ni], "-i") == 0) {
			ni++;
			strcpy(htsFilesList, argv[ni]);
			iOK = 1;
		}
		else if (strcmp(argv[ni], "-t") == 0){
			ni++;
			strcpy(trainingRegionsFile, argv[ni]);
			tOK = 1;
		}
		else if (strcmp(argv[ni], "-o") == 0){
			ni++;
			strcpy(outputFile, argv[ni]);
			oOK = 1;
		}
		else {
			printf("Error: unkown parameters!\n");
			exit(EXIT_FAILURE);
		}
		ni++;
	}

	/* check args */
	if ((iOK + tOK + oOK) < 3){
		printf("Error: input arguments not correct!\n");
		exit(EXIT_FAILURE);
	}
	else {
		extractFeature(trainingRegionsFile, outputFile, htsFilesList);
	}

	/* free pointers */
	free(htsFilesList);
	free(trainingRegionsFile);
	free(outputFile);

	return 0;
}
