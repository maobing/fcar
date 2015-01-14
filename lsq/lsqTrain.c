/*
compile:  gcc -Wall -I/home/bst/student/bhe2/hji/fcar/fcar_src/lsq/ -c lsqTrain.c
link: gcc -L/home/bst/student/bhe2/hji/fcar/fcar_src/lsq/ lsqTrain.o -lgsl -lgslcblas -lm -o lsqTrain
run: ./lsqTrain ../../CMYCOutputFeature_2cat_ave.txt_5_1000 5975 1000 ./lsqTrainTest
*/

#include <stdio.h>
#include "./gsl-1.16/multifit/gsl_multifit.h"

int readData(FILE *fp, gsl_matrix *X, gsl_vector *y, int n, int p);

int main (int argc, char **argv) {
  int i, n, p;
  double chisq;
  gsl_matrix *X, *cov;
  gsl_vector *y, *c;

  if (argc != 5) {
	  printf("/*---------------------------------------*/\n");
      printf("/* usage: lsqTrain data n p trainedModel */\n");
	  printf("/* data: datafile, fcar formatted        */\n");
	  printf("/* n: num of observations                */\n");
	  printf("/* p: num of covariates                  */\n");
	  printf("/* trainedModel: output file             */\n");
      printf("/*---------------------------------------*/\n");
	  return EXIT_SUCCESS;
  }

  char *dataFile = argv[1];
  char *trainedModelFile = argv[4];
  n = atoi (argv[2]);
  p = atoi (argv[3]);
  X = gsl_matrix_alloc (n, p+1);
  y = gsl_vector_alloc (n);

  c = gsl_vector_alloc (p+1); // c: coefficients
  cov = gsl_matrix_alloc (p+1, p+1);

  // read in data
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
  printf("Training Least Square\n");
  
  // fit least square
  {
    gsl_multifit_linear_workspace *work 
      = gsl_multifit_linear_alloc (n, p+1);
    gsl_multifit_linear (X, y, c, cov,
                          &chisq, work);
    gsl_multifit_linear_free (work);
  }

  // write to file trained model
  FILE *trainedModelFp = NULL;
  if((trainedModelFp = fopen(trainedModelFile, "w")) == NULL) {
	printf("Cannot open output trained model file %s\n", trainedModelFile);
	return EXIT_SUCCESS;
  }
  
  for(i = 0; i < p+1; i++) {
    fprintf(trainedModelFp, "%f\n", gsl_vector_get(c,(i)));
  }
  fclose(trainedModelFp);
  
  printf ("# chisq = %g\n", chisq);

  if(p < 10) {
	printf ("coefficients: ");
	for(i = 0; i < p+1; i++) {
		printf ("%f ", gsl_vector_get(c,(i)));
	}
  }
  
  gsl_matrix_free (X);
  gsl_vector_free (y);
  gsl_vector_free (c);
  gsl_matrix_free (cov);

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