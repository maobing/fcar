/*-----------------------------*/
/*         macro               */
/*-----------------------------*/
#define MAX_DIR_LEN 255
#define MAX_BAM_FILES 400
#define MAX_TRAINING_REGIONS 10000
#define NUM_SEQ 23 // chrY & chrM excluded


/*-----------------------------*/
/*         struct              */
/*-----------------------------*/

/* extract feature parameters */
struct extractFeatureParam {
	
	/* length of bin for calculating coverage*/
	int resolution;
	
	/* window size for extracting feature */
	int windowSize;

	/* bams are pairend */
	int pairend;
};

/* training region */
struct trainingRegion{
	int chr;
	int coordinate;
	int response;
};

/* model matrix */
struct modelMatrix {
	int n;
	int p;
	float **features;
	struct trainingRegion *trainingRegions;
};

/*------------------------------*/
/*        extractFeature        */
/*------------------------------*/
int extractFeature(char *bamsFile, char *trainingFile, char *outputFile, char *paramFile);

/*------------------------------*/
/*        parseParam            */
/*------------------------------*/
int parseParam(char *paramFile, struct extractFeatureParam *param);

/*------------------------------*/
/*   extractFeature_core        */
/*------------------------------*/
int extractFeature_core(char *bamFileName, char *binCountFileName, 
					int binSize, int stdSeqDepth, char *seqDepthFile);

/*------------------------------*/
/*            extract           */
/*------------------------------*/
struct modelMatrix *extract(char *bamsFile, char *trainingFile, struct extractFeatureParam *param);

/*------------------------------*/
/*       extract core           */
/*------------------------------*/
int extract_core(float **features, struct trainingRegion *trainingRegions, int totalTrainingRegions,
	char **bams, int totalBams, struct extractFeatureParam *param);

/*------------------------------*/
/*      saveModelMatrix         */
/*------------------------------*/
int saveModelMatrix(struct modelMatrix *modelMatrix, struct extractFeatureParam *param, char *outputFile);

/*------------------------------*/
/*           coverage           */
/*------------------------------*/
int coverage(char *bamsFile, struct extractFeatureParam *param);

/*------------------------------*/
/*      coverage_core           */
/*------------------------------*/
int coverage_core(char *bam, char *outputFile, struct extractFeatureParam *param);
