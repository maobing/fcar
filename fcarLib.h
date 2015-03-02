/*-----------------------------*/
/*         macro               */
/*-----------------------------*/
#define MAX_DIR_LEN 255
#define MAX_BAM_FILES 10
#define MAX_TRAINING_REGIONS 1000

/*-----------------------------*/
/*         struct              */
/*-----------------------------*/

/* struct to hold info about a 
 * high-throughput sequencing file
 */
struct htsFile_ {
	int resolution;
	int windowSize; // units: bp
	int pairend; 
  int min; // min fragment length filter for pairend
  int max; // max fragment length filter for pairend
};

/* training region */
struct trainingRegion_{
	int chr;
	int coordinate;
	int response;
};

/* model matrix */
struct modelMatrix_ {
	int n;
	int p;
	float **features;
	struct trainingRegion *trainingRegions;
};

/*------------------------------*/
/*        extractFeature        */
/*------------------------------*/
int extractFeature(char *trainingRegionsFile, char *outputFile, char *htsFilesList);

/*------------------------------*/
/*        parseHtsFile            */
/*------------------------------*/
int parseHtsFile(char *file, struct htsFile_ *htsFiles);

/*------------------------------*/
/*            extract           */
/*------------------------------*/
struct modelMatrix *extract(char *trainingRegionsFile, struct htsFile_ *htsFile);

/*------------------------------*/
/*    extractFeature core       */
/*    lower level function      */
/*------------------------------*/
int extract_core(float **features, struct trainingRegion *trainingRegions, int totalTrainingRegions,
	char **coverages, int totalCoverages, struct extractFeatureParam *param);

/*--------------------------------*/
/*      saveModelMatrix           */
/* get feature from coverage file */
/* as required input format       */
/*--------------------------------*/
int saveModelMatrix(struct modelMatrix *modelMatrix, struct extractFeatureParam *param, char *outputFile);

/*------------------------------*/
/*           coverage           */
/*------------------------------*/
int coverage(char *bamsFile, char *paramFile);

/*------------------------------*/
/*      coverage_core           */
/*------------------------------*/
int coverage_core(char *bam, char *outputFile, struct extractFeatureParam *param);
