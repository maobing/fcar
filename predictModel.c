#include "stdlib.h"
#include "stdio.h"
#include "string.h"
#include "fcarLib.h"

int menu_predictModel(int argc, char **argv);
int predictModel(char *method, char *trainingFile, char *testingFile, char *trainedModel, char *outputFile);

/*------------------------*/
/*  predictModel          */
/*------------------------*/
int main(int argc, char **argv) {

	/* menu */
	menu_predictModel(argc, argv);

	/* exit */
	exit(EXIT_SUCCESS);
	
}



int menu_predictModel(int argc, char **argv) {

	/* ------------------------------- */
	/*        predictModel             */
	/* -m method name                  */
  /* -tm trainedModel                */
  /* -train trainingFile             */
  /* -test testingFile               */
	/* -o output results               */
	/* ------------------------------- */

	if (argc == 1) {
		printf("/*------------------------------------*/\n");
		printf("/*           menu_predictModel        */\n");
		printf("/* -m method name                     */\n");
		printf("/*    LogisticRegressionL1            */\n");
		printf("/*    LogisticRegressionL2            */\n");
		printf("/*    SVM                             */\n");
		printf("/*    RandomFores                     */\n");
    printf("/* -tm trainedModel                   */\n");
    printf("/* -train trainFile                   */\n");
    printf("/* -test testFile                     */\n");
		printf("/* -o output results                  */\n");
		printf("/*------------------------------------*/\n");
		exit(EXIT_SUCCESS);
	}

  char *method = (char *)calloc(MAX_DIR_LEN, sizeof(char));
	char *trainedModel = (char *)calloc(MAX_DIR_LEN, sizeof(char));
	char *trainFile = (char *)calloc(MAX_DIR_LEN, sizeof(char)); 
  char *testFile = (char *)calloc(MAX_DIR_LEN, sizeof(char));
	char *outputFile = (char *)calloc(MAX_DIR_LEN, sizeof(char));

	int ni;
	int mOK = 0, tmOK = 0, trainOK = 0, testOK = 0, oOK = 0;

	ni = 1;
	while (ni < argc) {
		if (strcmp(argv[ni], "-m") == 0) {
			ni++;
			strcpy(method, argv[ni]);
			mOK = 1;
		}
		else if (strcmp(argv[ni], "-tm") == 0){
			ni++;
			strcpy(trainedModel, argv[ni]);
			tmOK = 1;
		}
		else if (strcmp(argv[ni], "-train") == 0){
			ni++;
			strcpy(trainFile, argv[ni]);
			trainOK = 1;
		}
		else if (strcmp(argv[ni], "-test") == 0){
			ni++;
			strcpy(testFile, argv[ni]);
			testOK = 1;
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
	if ((mOK + tmOK + trainOK + testOK + oOK) < 5){
		printf("Error: input arguments not correct!\n");
		exit(EXIT_FAILURE);
	}
	else {
    if(predictModel(method, trainedModel, trainFile, testFile, outputFile) != 0) {
      printf("Error: predictModel failed\n");
      return 1;
    }
	}

	/* free pointers */
  free(method);
	free(trainedModel);
	free(trainFile);
  free(testFile);
	free(outputFile);

	return 0;
}

/* predictModel */
int predictModel(char *method, char *trainedModel, char *trainFile, char *testFile, char *outputFile) {

  char *cmd = (char *)calloc(MAX_DIR_LEN, sizeof(char));
  
  if(strcmp(method, "SVM") == 0) {
    strcpy(cmd, "./liblinear-1.96/predict testFile trainedModel ");
  }
  else if (strcmp(method, "RandomForest") == 0) {
    strcpy(cmd, "");
  }
}

