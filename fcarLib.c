#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <math.h>
#include "fcarLib.h"
#include "sam.h"
#include "bam.h"

/* glocal variable */
int max_hts_files = 10;
int max_training_regions = 1000;

/*----------------------------*/
/*     extractFeature()       */
/* extract feature from input */
/*----------------------------*/
int extractFeature(char *trainingRegionsFile, char *outputFile, char *htsFilesList) {

  /* initialize param */
  struct htsFile_ *htsFiles = (struct htsFile_ *)calloc(max_hts_files, sizeof(struct htsFile_));
  struct modelMatrix_ *modelMatrix = (struct modelMatrix_ *)calloc(1, sizeof(struct modelMatrix_));
  int totalHtsFiles;

  /* parse pars */
  totalHtsFiles = parseHtsFile(htsFilesList, &htsFiles);
  if(totalHtsFiles < 0) {
    printf("error: parsing htsFilesList %s\n", htsFilesList);
    return(-1);
  }
  printf("extractFeature(): Detect %d hts files\n", totalHtsFiles);

  /* extract feature */
  extract(trainingRegionsFile, htsFiles, totalHtsFiles, modelMatrix);

  /* save feature */
  saveModelMatrix(modelMatrix, outputFile);

  freeHtsFiles(htsFiles, totalHtsFiles);
  free(modelMatrix);
  return 0;
}

/*-----------------------------------------*/
/*          saveModelMatrix                */
/* save modelMatirx                        */
/* i.e., extracted features                */
/* outputFormat, see libSVM                */
/*-----------------------------------------*/
int saveModelMatrix(struct modelMatrix_ *modelMatrix, char *outputFile) {
  
  int i, j;
  FILE *outputFileFp = NULL;

  if ((outputFileFp = fopen(outputFile, "w")) == NULL) {
    printf("Error: cannot open file %s\n", outputFile);
    exit(EXIT_FAILURE);
  }

  // further changed it to sparse representation with idx:0 omitted
  for (i = 0; i < modelMatrix->n; i++) {
    fprintf(outputFileFp, "%d ", modelMatrix->trainingRegions[i].response);
    for (j = 0; j < modelMatrix->p-1; j++) {
      if(modelMatrix->features[i][j] != 0.0) {
        fprintf(outputFileFp, "%d:%.4f", j+1, modelMatrix->features[i][j]);
      }
      if(modelMatrix->features[i][j+1] != 0.0) fprintf(outputFileFp, " ");
    }
    if(modelMatrix->features[i][modelMatrix->p - 1] != 0.0) {
      fprintf(outputFileFp, "%d:%.4f\n", modelMatrix->p, modelMatrix->features[i][modelMatrix->p - 1]);
    }
    else {
      fprintf(outputFileFp, "\n");
    }
  }
  
  fclose(outputFileFp);
  printf("\nsaveModelMatrix(): Writing features to output %s\n\n", outputFile);
  
  return 0;
}

/*-----------------------------------------*/
/*                extract                  */
/* extract features around training region */
/*-----------------------------------------*/
int extract(char *trainingRegionsFile, struct htsFile_ *htsFiles, int totalHtsFiles, struct modelMatrix_ *modelMatrix){
  
  int i;

  /* read in training regions */
  int totalTrainingRegions;
  FILE *trainingRegionsFileFp = NULL;
  struct trainingRegion_ *trainingRegions = (struct trainingRegion_ *)calloc(max_training_regions, sizeof(struct trainingRegion_));
  
  if ((trainingRegionsFileFp = fopen(trainingRegionsFile, "r")) == NULL) {
    printf("Error: cannot open file %s\n", trainingRegionsFile);
    exit(EXIT_FAILURE);
  }
  i = 0;
  while(!feof(trainingRegionsFileFp)) {  
    
    if (i >= max_training_regions) {
      max_training_regions *= 2;
      trainingRegions = (struct trainingRegion_ *)realloc(trainingRegions, max_training_regions*sizeof(struct trainingRegion_));
    }

    if( (fscanf(trainingRegionsFileFp, "%d %d %d\n", &trainingRegions[i].chr, &trainingRegions[i].coordinate,&trainingRegions[i].response)) != 3) {
      printf("error reading from %dth row from %s\n", i, trainingRegionsFile);
      free(trainingRegions);
      exit(-1);
    }

    i++;
  }
  totalTrainingRegions = i;
  fclose(trainingRegionsFileFp);
  
  /* extract features */
  modelMatrix->n = totalTrainingRegions;
  for(i = 0; i < totalHtsFiles; i++) {
    modelMatrix->p += (htsFiles[i].windowSize / htsFiles[i].resolution); 
  }

  float **features = (float **)calloc(modelMatrix->n, sizeof(float *));
  for (i = 0; i < modelMatrix->n; i++) {
    features[i] = (float *)calloc(modelMatrix->p, sizeof(float));
  }
  
  extract_core(features, trainingRegions, totalTrainingRegions, htsFiles, totalHtsFiles);

  /* return model matrix */
  modelMatrix->trainingRegions = trainingRegions;
  modelMatrix->features = features;

  /* return model matrix pointer */
  return 0;
}


/*----------------------------*/
/*     extract_core           */
/*----------------------------*/
int extract_core(float **features, struct trainingRegion_ *trainingRegions, 
          int totalTrainingRegions, struct htsFile_ *htsFiles, int totalHtsFiles) {

  int i, j, k;
  printf("\n");
  printf("extract_core(): Extracting features for training regions,\n");

  // note that we are using a name convention *.bam.coverage[Resolution]
  FILE *coveragesFp[totalHtsFiles];
  for(i = 0; i < totalHtsFiles; i++) {
    char *tmpFilename = (char *)calloc(MAX_DIR_LEN, sizeof(char));
    strcpy(tmpFilename, htsFiles[i].file);
    strcat(tmpFilename, ".coverage");
    char tmpResolution[5];
    sprintf(tmpResolution, "%d", htsFiles[i].resolution);
    strcat(tmpFilename, tmpResolution);
    
    if ((coveragesFp[i] = fopen(tmpFilename, "rb")) == NULL) {
        printf("Error: cannot open %s\n", tmpFilename);
        printf("Info: run ./coverage -i bamfiles -p param\n");
        return(-1);
    }

    free(tmpFilename); 
  }

  /* loop through each training region */
  for (i = 0; i < totalTrainingRegions; i++){
    if((i+1) % 1000 == 0) {
      printf("\tprocessing %dth training region\n", i+1);
    }
    int featuresPos = 0; // record start pos for different coverage files
    /* loop through each coverage file */
    for (k = 0; k < totalHtsFiles; k++) {
      // check boundary training case
      if (trainingRegions[i].coordinate - htsFiles[k].windowSize / 2 < 0 &&
        trainingRegions[i].coordinate + htsFiles[k].windowSize / 2 > chrlen[trainingRegions[i].chr - 1]) {
        printf("Warning: cannot extract feature for boundary training case %d %d %d, ignored\n",
          trainingRegions[i].chr, trainingRegions[i].coordinate, trainingRegions[i].response);
        return(-1);
      }
      /* find position for training region */
      // Note: position's unit is bin! not bp!
      int position = 0;
      if (trainingRegions[i].chr >= 2) { // not chr1
        for (j = 0; j < trainingRegions[i].chr - 1; j++) {
          position += (int)(chrlen[j] / htsFiles[k].resolution) + 1;
        }
      }
      // now position is in the training region
      position += (int)((float)trainingRegions[i].coordinate / (float)htsFiles[k].resolution + 0.5);
      // now change position to begining of the windowSize around training region
      position -= (int)((float)(htsFiles[k].windowSize / 2) / (float)htsFiles[k].resolution + 0.5);

      fseek(coveragesFp[k], position*sizeof(float), SEEK_SET);
      
      /* note windowSize's unit is bp not bins! */
      fread(&features[i][featuresPos], sizeof(float), htsFiles[k].windowSize / htsFiles[k].resolution, coveragesFp[k]);
      featuresPos += htsFiles[k].windowSize / htsFiles[k].resolution;

    } // end of loop of coverage files
  } // end of loop of training regions

  for(i = 0; i < totalHtsFiles; i++) {
    fclose(coveragesFp[i]);
  }
  return 0;
}

/*----------------------------*/
/*     parseHtsFile           */
/* parse file param settings  */
/*----------------------------*/
int parseHtsFile(char *htsFileList, struct htsFile_ **htsFiles) {

  FILE *htsFileListFp = NULL;
  int idx = -1, totalHtsFiles = 0;

  if ((htsFileListFp = fopen(htsFileList, "r")) == NULL) {
    printf("Error: cannot open file %s\n", htsFileList);
    return(-1);
  }

  printf("\n\n/*-------------------------------------\n");
  while (!feof(htsFileListFp)){
    char *name = (char *)calloc(MAX_DIR_LEN, sizeof(char));
    char *value = (char *)calloc(MAX_DIR_LEN, sizeof(char));

    if( (fscanf(htsFileListFp, "%[^=]=%s\n", name, value) != 2) ) {
      printf("parseHtsFile: error reading in from %s\n", htsFileList);
      free(name); free(value);
      return(-1);
    }

    if (strcmp(name, "file") == 0){
      /* if idx exceeds max_hts_files 
      realloc the memory, double the size */
      if(idx == max_hts_files - 1) {
        max_hts_files *= 2;
        *htsFiles = (struct htsFile_ *)realloc(*htsFiles, max_hts_files*sizeof(struct htsFile_));
      }
      idx++; // whenever come across a "file=", add idx to htsFiles
      (*htsFiles)[idx].file = (char *)calloc(MAX_DIR_LEN, sizeof(char));
      strcpy((*htsFiles)[idx].file, value);
      printf("/* %s is %s\n", name, value);
    }
    else if (strcmp(name, "resolution") == 0){
      (*htsFiles)[idx].resolution = atoi(value);
      printf("/* %s is %s\n", name, value);
    }
    else if (strcmp(name, "windowSize") == 0){
      (*htsFiles)[idx].windowSize = atoi(value);
      printf("/* %s is %s\n", name, value);
    }
    else if (strcmp(name, "pairend") == 0) {
      (*htsFiles)[idx].pairend = atoi(value);
      printf("/* %s is %s\n", name, value);
    }
    // min max length of fragment for pairend data
    else if (strcmp(name, "min") == 0) {
      (*htsFiles)[idx].min = atoi(value);
      printf("/* %s is %s\n", name, value);
    }
    else if (strcmp(name, "max") == 0) {
      (*htsFiles)[idx].max = atoi(value);
      printf("/* %s is %s\n", name, value);
    }
    else {
      printf("/* Warning: unkown parameters %s=%s, ignored\n", name, value);
    }
    free(name);
    free(value);
  }
  printf("/*-------------------------------------\n\n");
  
  fclose(htsFileListFp);
  totalHtsFiles = idx+1;
  return totalHtsFiles;
}

/*-------------------------------*/
/*          coverage             */
/* count coverage for bams       */
/* Note: by convention, coverage */
/* output filename is the bam file name */
/* plus '.coverage%d', resolution */
/*-------------------------------*/
int coverage(char *htsFilesList) {

  int i;
  int totalHtsFiles;
  struct htsFile_ *htsFiles = (struct htsFile_ *)calloc(max_hts_files, sizeof(struct htsFile_));

  /* parse param file */
  totalHtsFiles = parseHtsFile(htsFilesList, &htsFiles);
  if(totalHtsFiles < 0 ) {
    printf("error: parsing htsFilesList %s\n", htsFilesList);
    return(-1);
  }
  printf("\nExtractFeature(): Detect %d hts files\n", totalHtsFiles);
  
  /* count coverage for each bam */
  for (i = 0; i < totalHtsFiles; i++) {
    // note the outputfile name convention here
    char *outputFileName = (char*)calloc(MAX_DIR_LEN, sizeof(char));
    strcpy(outputFileName, htsFiles[i].file);
    strcat(outputFileName, ".coverage");
    char tmp[10];
    sprintf(tmp, "%d", htsFiles[i].resolution);
    strcat(outputFileName, tmp);

    printf("coverage(): Calculating coverage for %s\n", htsFiles[i].file);

    /* count coverage */
    coverage_core(htsFiles[i], outputFileName);

    printf("coverage(): Saving coverage to %s\n", outputFileName);
    free(outputFileName);
  }

  freeHtsFiles(htsFiles, totalHtsFiles);
  return EXIT_SUCCESS;
}

/*--------------------------------*/
/*         coverage_core          */
/* count coverage core            */
/* 09-09-2014 @UPDATE: previously,  each read contribute to only one bin's signal,*/
/*   We now update it to contribute to all the bins covered by it. */
/*   This is necessary because when the resolution is high (10bp), */
/*   the old way will cause the signal to be not smooth and too sparse */
/* 01-16-2015 @UPDATE: added pair-end coverage counting */
/* @NOTE: float is 4 byte on jhpce01 */
/*--------------------------------*/
int coverage_core(struct htsFile_ htsFile, char *outputFile) {

  int i, j;

  /* open bam */
  samfile_t *bamFp = NULL;

  float **binCnt = (float **)calloc(NUM_SEQ, sizeof(float *));
  for (i = 0; i < NUM_SEQ; i++) {
    binCnt[i] = (float *)calloc(((int)(chrlen[i] / htsFile.resolution) + 1), sizeof(float));
  }

  if ((bamFp = samopen(htsFile.file, "rb", 0)) == NULL) {
    printf("Error: Cannot open the file %s\n", htsFile.file);
    return EXIT_SUCCESS;
  }

  /* adjust bam header ref chr order */
  int tidmap[NUM_SEQ + 1];
  for (i = 0; i < NUM_SEQ + 1; i++) {
    tidmap[i] = i;
  }
  if (strcmp(bamFp->header->target_name[1], "chr11") == 0) {
    tidmap[0] = 9; tidmap[1] = 10; tidmap[2] = 11; tidmap[3] = 12;
    tidmap[4] = 13; tidmap[5] = 14; tidmap[6] = 15; tidmap[7] = 16;
    tidmap[8] = 17; tidmap[9] = 18;
    tidmap[10] = 0;
    tidmap[11] = 19; tidmap[12] = 20; tidmap[13] = 21;
    tidmap[14] = 1; tidmap[15] = 2; tidmap[16] = 3; tidmap[17] = 4;
    tidmap[18] = 5; tidmap[19] = 6; tidmap[20] = 7; tidmap[21] = 8;
    tidmap[22] = 23; tidmap[23] = 22;
  }

  /* start count coverage */
  int nread = 0;
  bam1_t *read = NULL;

  read = bam_init1();

  if(htsFile.pairend == 0) {
    while (samread(bamFp, read) > 0) {
      if (read->core.tid < 0 || read->core.tid > 23) {
        continue;
      }
      /* @UPDATE on 09-09-2014 */
      /* increment signals at all covered bins */
      if (tidmap[read->core.tid] < NUM_SEQ) {
        int startBin, endBin;
        startBin = (int)((float)read->core.pos / (float)htsFile.resolution + 0.5);
        endBin = (int)((float)(read->core.pos + read->core.l_qseq) / (float)htsFile.resolution + 0.5);

        int b;
        for(b = startBin; b <= endBin; b++) {
          binCnt[tidmap[read->core.tid]][b]++;
        }
      }
      nread++;
      if (nread % 10000000 == 0) {
        printf("\t\tProcessing %dth read\n", nread);
      }
    }
  }

  if(htsFile.pairend == 1) {
    // added for pair end coverage counting
    int is2ndMate = 0;
    while (samread(bamFp, read) > 0) {
      if (read->core.tid < 0 || read->core.tid > 23) {
        continue;
      }
      /* @UPDATE on 09-09-2014 */
      /* increment signals at all covered bins */
      if (tidmap[read->core.tid] < NUM_SEQ) {
        if(is2ndMate == 1) {
          is2ndMate = 0; // assuming bam is sorted by id number
          continue;
        } 
        else {
          int left, right;
          if(read->core.pos > read->core.mpos) {
            left = read->core.mpos;
            right = read->core.pos;
          }
          else {
            left = read->core.pos;
            right = read->core.mpos;
          }
          // filter out fragments with specified [min,max]
          if(right - left + read->core.l_qseq < htsFile.min || right - left + read->core.l_qseq > htsFile.max) {
            is2ndMate = 1;
            continue;
          }
          int startBin, endBin;
          startBin = (int)((float) left / (float)htsFile.resolution + 0.5);
          endBin = (int)((float)(right + read->core.l_qseq) / (float)htsFile.resolution + 0.5);
          is2ndMate = 1;
          int b;
          // printf("startBin is %d and endBin is %d\n", startBin, endBin);
          for(b = startBin; b <= endBin; b++) {
            binCnt[tidmap[read->core.tid]][b]++;
          }
        }
      }
      nread++;
      if (nread % 10000000 == 0) {
        printf("\t\tProcessing %dth read\n", nread);
      }
    }
  }

  bam_destroy1(read);
  samclose(bamFp);

  /* take log2 of coverage */
  for(i = 0; i < NUM_SEQ; i++) {
    for(j = 0; j < (int)(chrlen[i] / htsFile.resolution) + 1; j++) {
      binCnt[i][j] = (float)(log2((double)(binCnt[i][j]+1)));
    }
  }
  
  /* write coverage */
  FILE *binCntFp = NULL;

  if ((binCntFp = fopen(outputFile, "wb")) == NULL) {
    printf("Error: Cannot open the file %s\n", outputFile);
    exit(EXIT_FAILURE);
  }

  for (i = 0; i < NUM_SEQ; i++) {
    fwrite(binCnt[i], sizeof(float), ((int)(chrlen[i] / htsFile.resolution) + 1), binCntFp);
  }
  fclose(binCntFp);

  // free binCnt
  for (i = 0; i < NUM_SEQ; i++) {
    free(binCnt[i]);
  }
  free(binCnt);

  return 0;
}



/*------------------------------------------*/
/*      freeHtsFiles(htsFiles)              */
/* cannot use free() directly,              */
/* because parseHtsFile() dynamically       */
/* allocates memory to the file component   */
/* of the htsFile                           */
/*------------------------------------------*/
void freeHtsFiles(struct htsFile_ *htsFiles, int totalHtsFiles) {
  int i;
  for(i = 0; i < totalHtsFiles; i++) {
    free(htsFiles[i].file);
  }
  free(htsFiles);
}
