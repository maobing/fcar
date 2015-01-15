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
	/* -i bamsFile                     */
	/* -t traini region coordinate     */
	/* -o outputFile                   */
	/* -p paramFile                    */
	/* ------------------------------- */

	if (argc == 1) {
		printf("/*-----------------------------------*/\n");
		printf("/*            extractFeature         */\n");
		printf("/* -i bams file list                 */\n");
		printf("/* -t training region coordinate     */\n");
		printf("/* -o output file                    */\n");
		printf("/* -p parameter setting file         */\n");
		printf("/*-----------------------------------*/\n");
		exit(EXIT_SUCCESS);
	}

	char *bamsFile = (char *)calloc(MAX_DIR_LEN, sizeof(char)); 
	char *trainingFile = (char *)calloc(MAX_DIR_LEN, sizeof(char)); 
	char *outputFile = (char *)calloc(MAX_DIR_LEN, sizeof(char));
	char *paramFile = (char *)calloc(MAX_DIR_LEN, sizeof(char));
	int ni;
	int iOK = 0, tOK = 0, oOK = 0, pOK = 0;

	ni = 1;
	while (ni < argc) {
		if (strcmp(argv[ni], "-i") == 0) {
			ni++;
			strcpy(bamsFile, argv[ni]);
			iOK = 1;
		}
		else if (strcmp(argv[ni], "-t") == 0){
			ni++;
			strcpy(trainingFile, argv[ni]);
			tOK = 1;
		}
		else if (strcmp(argv[ni], "-o") == 0){
			ni++;
			strcpy(outputFile, argv[ni]);
			oOK = 1;
		}
		else if (strcmp(argv[ni], "-p") == 0){
			ni++;
			strcpy(paramFile, argv[ni]);
			pOK = 1;
		}
		else {
			printf("Error: unkown parameters!\n");
			exit(EXIT_FAILURE);
		}
		ni++;
	}

	/* check args */
	if ((iOK + tOK + oOK + pOK) < 4){
		printf("Error: input arguments not correct!\n");
		exit(EXIT_FAILURE);
	}
	else {
		extractFeature(bamsFile, trainingFile, outputFile, paramFile);
	}

	/* free pointers */
	free(bamsFile);
	free(trainingFile);
	free(outputFile);
	free(paramFile);

	return 0;
}
