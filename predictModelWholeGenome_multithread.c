/*
 * This script directly use liblinear class interface
 * to provide genome-wide scan prediction for ChIPseq
 * with multithread
 * note that you need to request at least 32 threads,
 * as I am assuming each thread handles one chr
 */
// compile
// g++ -Wall -o predictModelWholeGenome_multithread predictModelWholeGenome_multithread.c ./liblinear-1.96/tron.o ./liblinear-1.96/linear.o ./fcarLib.o ./liblinear-1.96/blas/blas.a -L/home/bst/student/bhe2/hji/samtools-0.1.18 -lbam -lz -lpthread

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <pthread.h>
#include "fcarLib.h"
#include "./liblinear-1.96/linear.h"
#include "./liblinear-1.96/tron.h"

// #define NUM_THREADS NUM_SEQ
#define Num_THREADS 3
// create thread argument struct for thr_func()
struct thread_data_ {
  int chr; 
  char *trainedModel;
  struct htsFile_ *htsFiles;
  int totalHtsFiles;
};

// thread function 
void *predictModelWholeGenome( void *arg );

// other function declaration
int menu_predictModelWholeGenome( int argc, char **argv );

/*------------------------*/
/*         main           */
/*------------------------*/
int main(int argc, char **argv) {

	menu_predictModelWholeGenome(argc, argv);

	exit(EXIT_SUCCESS);
	
}


int menu_predictModelWholeGenome(int argc, char **argv) {

	/* ------------------------------- */
	/*        predictModel             */
  /* -tm trainedModel                */
  /* -i htsFilesList                 */
	/* -o output results               */
	/* ------------------------------- */

	if (argc == 1) {
		printf("/*------------------------------------*/\n");
		printf("/*    menu_predictModelGenomeWide     */\n");
    printf("/* -tm trainedModel                   */\n");
    printf("/* -i htsFilesList                    */\n");
		printf("/* -o output results                  */\n");
		printf("/*------------------------------------*/\n");
		return(EXIT_SUCCESS);
	}


	char *trainedModel = (char *)calloc(MAX_DIR_LEN, sizeof(char));
  char *htsFilesList = (char *)calloc(MAX_DIR_LEN, sizeof(char));
	char *outputFile = (char *)calloc(MAX_DIR_LEN, sizeof(char));

	int ni;
	int tmOK = 0, iOK = 0, oOK = 0;

	ni = 1;
	while (ni < argc) {
		if (strcmp(argv[ni], "-tm") == 0) {
			ni++;
			strcpy(trainedModel, argv[ni]);
			tmOK = 1;
		}
		else if (strcmp(argv[ni], "-i") == 0){
			ni++;
			strcpy(htsFilesList, argv[ni]);
			iOK = 1;
		}
		else if (strcmp(argv[ni], "-o") == 0){
			ni++;
			strcpy(outputFile, argv[ni]);
			oOK = 1;
		}
		else {
			printf("Error: unkown parameters!\n");
			return(EXIT_FAILURE);
		}
		ni++;
	}

	/* check args */
	if ((tmOK + iOK + oOK) < 3) {
		printf("Error: input arguments not correct!\n");
		exit(EXIT_FAILURE);
	}

  // init thr
  pthread_t thr[NUM_THREADS];
  // init thr_data
  struct thread_data_ thr_data[NUM_THREADS];

  // parse htsFilesList
  int totalHtsFiles;
  struct htsFile_ *htsFiles = (struct htsFile_ *)calloc(max_hts_files, sizeof(struct htsFile_));
  totalHtsFiles = parseHtsFile(htsFilesList, &htsFiles);

  /* creat thread */
  int i, rc; // rc for holding error code
  for(i = 0; i < NUM_THREADS; i++) {
    thr_data[i].chr = i;
    thr_data[i].trainedModel = trainedModel;
    thr_data[i].htsFiles = htsFiles;
    thr_data[i].totalHtsFiles = totalHtsFiles;
    printf("creating %dth thread\n", i);
    fflush(stdout); 
    if( (rc = pthread_create(&thr[i], NULL, predictModelWholeGenome, &thr_data[i])) ) {
      printf("error: pthread_create, rc:%d\n", rc);
      return EXIT_SUCCESS;
    }
  }

  /* block until all threads complete */
  /* then write to output in sequential order */
  float **predResult = (float **)calloc(NUM_THREADS, sizeof(float *));
  for(i = 0; i < NUM_THREADS; i++) {
    pthread_join(thr[i], (void **) &predResult[i]);
  }

  /* write to output */
  FILE *outputFileFp = NULL;
  if( (outputFileFp = fopen(outputFile, "wb")) == NULL ) {
    printf("Error: cannot write to outputFile %s\n", outputFile);
    return EXIT_SUCCESS;
  }
  printf("writing to output %s\n", outputFile);

  // we assume resolution for all htsFiles are the same
  for(i = 0; i < NUM_THREADS; i++) {
    fwrite(predResult[i], sizeof(float), (int)(chrlen[i] / htsFiles[0].resolution)+1 , outputFileFp);
    free(predResult[i]);
  }
  fclose(outputFileFp);
  
	/* free pointers */
	free(trainedModel);
  free(htsFilesList);
	free(outputFile);
  free(predResult);

	return 0;
}



/*---------------------------------------------------------------*/
/*                      thread function                          */
/*---------------------------------------------------------------*/

/* thr_func: predictModel */
// NOTE that pthread_create(pthread_t *thread, pthread_att_t *attr, 
//    void * (*start_routine) (void * ), viod *arg)
//    requires a pointer to a function which takes in and returns void pointers

void *predictModelWholeGenome(void *arg) {
  struct thread_data_ *data = (struct thread_data_ *) arg;

  // printf("data->trainedModel is %s\n", data->trainedModel);
  // printf("data->chr is %d\n", data->chr);

  char *trainedModel = data->trainedModel;
  struct htsFile_ *htsFiles = data->htsFiles;
  int chr = data->chr;
  int totalHtsFiles = data->totalHtsFiles;

  // utility var
  int i,j,k;
 
  // trainedModel
  struct model *mymodel;
  if( (mymodel = load_model(trainedModel)) == 0) {
    printf("cannot load model from file %s\n", trainedModel);
    return EXIT_SUCCESS;
  }

  // open coverage Files for htsFiles
  // Note the name convension
  FILE *coverageFps[totalHtsFiles];
  for(i = 0; i < totalHtsFiles; i++) {
    char *tmpFileName = (char *)calloc(MAX_DIR_LEN, sizeof(char));
    strcpy(tmpFileName, htsFiles[i].file);
    strcat(tmpFileName, ".coverage");
    char tmpResolution[5];
    sprintf(tmpResolution, "%d", htsFiles[i].resolution);
    strcat(tmpFileName, tmpResolution);

    if( (coverageFps[i] = fopen(tmpFileName, "rb")) == NULL ) {
      printf("Error opening coverage file %s\n", tmpFileName);
      return EXIT_SUCCESS;
    }
    free(tmpFileName);
  }

  // predict model: by default: predict probability
  int nr_class = get_nr_class(mymodel);
  double *prob_estimates = (double *)calloc(nr_class, sizeof(double));

  // predResult for storing results
  int totalBins = 0;
  int cumBins[NUM_SEQ];
  // we assum all htsFiles have the same resolution
  for (i = 0; i < NUM_SEQ; i++) {
    totalBins += (int)(chrlen[i] / htsFiles[0].resolution) + 1;
    cumBins[i] = totalBins;
  }

  // allocate memory for result based on thread data chr
  // as we are using one thread for each chr
  float *predResult = (float *)calloc( (int)(chrlen[chr] / htsFiles[0].resolution) + 1, sizeof(float));

  // read in feature for each bin and do prediction
  for(j = 0; j < (int)(chrlen[chr] / htsFiles[0].resolution) + 1; j++) {
    if(j % 1000000 == 0) {
      printf("Predicting chr%d:%dth bin\n", chr,j);
      fflush(stdout);
    }
    int max_nr_feature = 100;
    struct feature_node *myX = (struct feature_node *)calloc(max_nr_feature, sizeof(struct feature_node));
    int idx = 0;
    for(k = 0; k < totalHtsFiles; k++) {
      float *buffer = (float *)calloc( htsFiles[k].windowSize/htsFiles[0].resolution,sizeof(float));
      int offset = j;
      offset += -(int)((float)(htsFiles[k].windowSize / 2) / (float)htsFiles[0].resolution + 0.5);
      if(offset < 0 || offset + (int)((float)(htsFiles[k].windowSize) / (float)htsFiles[0].resolution + 0.5) > (int)(chrlen[i] / htsFiles[0].resolution) + 1) {
        // printf("offset is %d\n", offset);
        free(buffer);
        continue;
      }
      if(chr != 0) offset += cumBins[chr-1];
      // printf("offset is %d\n", offset);
      fseek(coverageFps[k], offset*sizeof(float), SEEK_SET);
      fread(buffer, sizeof(float), htsFiles[k].windowSize/htsFiles[0].resolution, coverageFps[k]);
      int l;
      // printf("buffer[%d] is:",l);
      for(l = 0; l < htsFiles[k].windowSize/htsFiles[0].resolution; l++) {
        // if(j == 289540) printf("%f,",buffer[l]);
        if(buffer[l] != 0) {
          myX[idx].index = k*(htsFiles[k].windowSize/htsFiles[0].resolution) + l + 1;
          myX[idx].value = buffer[l];
          idx++;
        }
        if(idx >= max_nr_feature -2) { // feature_node is not long enough
          max_nr_feature *= 2;
          myX = (struct feature_node *)realloc(myX, max_nr_feature*sizeof(struct feature_node));
        }
      }
      free(buffer);
    } // end of loop through coverageFiles
    // printf("\n");
    myX[idx].index = -1; // a flag for end of features
    if(idx == 0) {
      // printf("idx is %d\n",idx);
      predResult[j] = 0.0;
      free(myX);
      continue;
    }
    // printf("nr_feature is %d\n", idx);
    predict_probability(mymodel, myX, prob_estimates);
    // printf("num of feature is %d\n", get_nr_feature(mymodel));
    // printf("num of class is %d\n", get_nr_class(mymodel));
    predResult[j] = prob_estimates[0];
    free(myX);
  }


  for(i = 0; i < totalHtsFiles; i++) {
    fclose(coverageFps[i]);
  }
  freeHtsFiles(htsFiles, totalHtsFiles);
  free(prob_estimates);
  // give address of pointer to this function, so that the function can free the pointer.
  free_and_destroy_model(&mymodel); 
  pthread_exit((void *) predResult);
}

