#ifndef FCAR_LIB_H
#define FCAR_LIB_H

#define MAX_DIR_LEN 255
#define NUM_SEQ 23

/*-----------------------------*/
/*        global               */
/*-----------------------------*/
const int chrlen[] = { 249250621, 243199373, 198022430, 191154276, 180915260, \
171115067, 159138663, 146364022, 141213431, 135534747, 135006516, \
133851895, 115169878, 107349540, 102531392, 90354753, 81195210, \
78077248, 59128983, 63025520, 48129895, 51304566, 155270560 };

extern int max_hts_files;
extern int  max_training_regions;

/*-----------------------------*/
/*         struct              */
/*-----------------------------*/

/* struct to hold info about a 
 * high-throughput sequencing file
 */
struct htsFile_ {
  char *file;
	int resolution;
	int windowSize; // units: bp
	int pairend; 
  int min; // min fragment length filter for pairend
  int max; // max fragment length filter for pairend
};

/* training region */
struct trainingRegion_ {
	int chr;
	int coordinate;
	int response;
};

/* model matrix */
struct modelMatrix_ {
	int n;
	int p;
	float **features;
	struct trainingRegion_ *trainingRegions;
};

/*------------------------------*/
/*        extractFeature        */
/*------------------------------*/
int extractFeature(char *trainingRegionsFile, char *outputFile, char *htsFilesList);

/*------------------------------*/
/*       parseHtsFile           */
/*------------------------------*/
// use double reference, because we
//  may need to realloc htsFiles if files are 
//  more than MAX_BAM_FILE
int parseHtsFile(char *htsFilesList, struct htsFile_ **htsFiles);

/*------------------------------*/
/*            extract           */
/*------------------------------*/
int extract(char *trainingRegionsFile, struct htsFile_ *htsFile, int totalHtsFiles, struct modelMatrix_ *modelMatrix);

/*------------------------------*/
/*    extractFeature core       */
/*    lower level function      */
/*------------------------------*/
int extract_core(float **features, struct trainingRegion_ *trainingRegions, int totalTrainingRegions,
	struct htsFile_ *htsFiles,int totalHtsFiles);

/*--------------------------------*/
/*      saveModelMatrix           */
/* get feature from coverage file */
/* as required input format       */
/*--------------------------------*/
int saveModelMatrix(struct modelMatrix_ *modelMatrix, char *outputFile);

/*------------------------------*/
/*           coverage           */
/*------------------------------*/
int coverage(char *htsFilesList);

/*------------------------------*/
/*      coverage_core           */
/*------------------------------*/
int coverage_core(struct htsFile_ htsFile, char *outputFile);

/*-------------------------------*/
/*      freeHtsFiles(htsFiles)   */
/*-------------------------------*/
void freeHtsFiles(struct htsFile_ *htsFiles, int totalHtsFiles);

#endif /* FCARLIB_H */
