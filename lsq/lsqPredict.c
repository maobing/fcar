/*
compile:  gcc -Wall -I/home/bst/student/bhe2/hji/fcar/fcar_src/lsq/ -c lsqPredict.c
link: gcc -L/home/bst/student/bhe2/hji/fcar/fcar_src/lsq/ lsqPredict.o -lgsl -lgslcblas -lm -o lsqPredict
run: ./lsqPredict ./lsqTrainTest ../../CTCFOutputFeature_2cat_ave.txt_5_1000 6859 1000 output
*/

#include <stdio.h>
#include "./gsl-1.16/multifit/gsl_multifit.h"

int readData(FILE *fp, gsl_matrix *X, gsl_vector *y, int n, int p);

int main (int argc, char **argv) {
  int i, j, n, p;
  gsl_matrix *X;
  gsl_vector *y;

  if (argc != 6) {
	  printf("/*-----------------------------------------*/\n");
      printf("/* usage: lsqPredict trainedModel testData */\n"); 
	  printf("/*                 n p predictResult       */\n");
	  printf("/* trainedModel: output from lsqTrain      */\n");
	  printf("/* testData                                */\n");
	  printf("/* n: num of observations                  */\n");
	  printf("/* p: num of covariates                    */\n");
	  printf("/* predictResult: predictResult            */\n");
      printf("/*-----------------------------------------*/\n");
	  return EXIT_SUCCESS;
  }

  char *trainedModelFile = argv[1];
  char *dataFile = argv[2];
  char *predictResultFile = argv[5];
  n = atoi (argv[3]);
  p = atoi (argv[4]);
  X = gsl_matrix_alloc (n, p+1);
  y = gsl_vector_alloc (n);
  
  double c[p+1];
  
  // read coefficents
  FILE *trainedModelFp = NULL;
  if((trainedModelFp = fopen(trainedModelFile, "r")) == NULL) {
	printf("Cannot open trainedModelFile %s\n", trainedModelFile);
	return EXIT_SUCCESS;
  }
  for(i = 0; i < p+1; i++) {
	fscanf(trainedModelFp, "%lf\n", &c[i]);
  }
  fclose(trainedModelFp);
  
  // read in test data
  FILE *dataFp = NULL;
  if((dataFp = fopen(dataFile, "r")) == NULL) {
	printf("Cannot open data file %s\n", dataFile);
	return EXIT_SUCCESS;
  }

  if(readData(dataFp, X, y, n, p) < 0) {
	printf("Error reading data %s\n", dataFile);
	return EXIT_SUCCESS;
  }
  fclose(dataFp);
  printf("Predicting Least Square\n");
  
  // predict least square
  double *predictResult;
  predictResult = (double *)calloc(n, sizeof(double));
  
  for(i = 0; i < n; i++) {
	for(j = 0; j < p+1; j++) {
		predictResult[i] += gsl_matrix_get(X, i, j) * c[j];
	}
  }

  // write to predictResult
  FILE *predictResultFp = NULL;
  if((predictResultFp = fopen(predictResultFile, "w")) == NULL) {
	printf("Cannot open output predictResult file %s\n", predictResultFile);
	return EXIT_SUCCESS;
  }
  
  for(i = 0; i < n; i++) {
    fprintf(predictResultFp, "%f\n", predictResult[i]);
  }
  fclose(predictResultFp);
  
  gsl_matrix_free (X);
  gsl_vector_free (y);
  free(predictResult);
  
  return 0;
}

/*----------*/
/* fun def  */
/*----------*/
int readData(FILE *fp, gsl_matrix *X, gsl_vector *y, int n, int p) {
	int i, j;
	double xij, yi;
	
	for (i = 0; i < n; i++) {
		gsl_matrix_set (X, i, 0, 1.0);
	}
	
	for (i = 0; i < n; i++) {
		// printf ("i:%d\n",i);
		for(j = 1; j < p+1; j++) {
			// printf ("j:%d\n",j);
			if(j == 1) {
				if((fscanf (fp, "%lf 1:%lf ", &yi, &xij)) !=2 ) {
					printf("Error reading %d,%d from data\n", i,j);
					return -1;
				}
			} else if(j == p) {
				if((fscanf (fp, "%*d:%lf\n", &xij)) !=1) {
					printf("Error reading %d,%d from data\n", i,j);
					return -1;
				}
			} else {
				if((fscanf(fp, "%*d:%lf ", &xij)) != 1) {
					printf("Error reading %d,%d from data\n", i,j);
					return -1;					
				}
			}
			// printf("[%d,%d]: yi,xij is %f,%f", i,j, (float)yi, (float)xij);
			gsl_matrix_set (X, i, j, xij);			  
		}
		gsl_vector_set (y, i, yi);
	}
	return 0;
}