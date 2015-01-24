<<COMMENT
#---------------
# count coverage
#---------------
./countCoverage -i ../DNaseFlash_SRR -p ../param
##./countCoverage -i ../DNaseFlash_SRR801880 -p ../param_SRR801880
##./countCoverage -i ../DNaseFlash_SRR801881 -p ../param_SRR801881

#---------------
# extractFeature
# DNaseHistone
#---------------
./extractFeature -i ../coveragesFile_DNaseHistone  -t ../CMYCData_narrowPeak_Gm12878_genomewide.txt -o ../CMYCOutputFeature_Gm12878_genomewide_DNaseHistone.txt -p ../param_DNaseHistonee
./extractFeature -i ../coveragesFile_DNaseHistone  -t ../CTCFData_narrowPeak_Gm12878_genomewide.txt -o ../CTCFOutputFeature_Gm12878_genomewide.txt -p ../param_DNaseHistone
./extractFeature -i ../coveragesFile_DNaseHistone  -t ../E2FData_narrowPeak_Gm12878_genomewide.txt -o ../E2FOutputFeature_Gm12878_genomewide.txt -p ../param_DNaseHistone
./extractFeature -i ../coveragesFile_DNaseHistone  -t ../EGR1Data_narrowPeak_Gm12878_genomewide.txt -o ../EGR1OutputFeature_Gm12878_genomewide.txt -p ../param_DNaseHistone
./extractFeature -i ../coveragesFile_DNaseHistone  -t ../GABPData_narrowPeak_Gm12878_genomewide.txt -o ../GABPOutputFeature_Gm12878_genomewide.txt -p ../param_DNaseHistone
./extractFeature -i ../coveragesFile_DNaseHistone  -t ../NRSFData_narrowPeak_Gm12878_genomewide.txt -o ../NRSFOutputFeature_Gm12878_genomewide.txt -p ../param_DNaseHistone
./extractFeature -i ../coveragesFile_DNaseHistone  -t ../SRFData_narrowPeak_Gm12878_genomewide.txt -o ../SRFOutputFeature_Gm12878_genomewide.txt -p ../param_DNaseHistone
./extractFeature -i ../coveragesFile_DNaseHistone  -t ../USF1Data_narrowPeak_Gm12878_genomewide.txt -o ../USF1OutputFeature_Gm12878_genomewide.txt -p ../param_DNaseHistone

COMMENT

#----------------
# extractFeature
# DNaseFlash
#----------------
## only CTCF has ChIPseq in cellline Ag09319
./extractFeature -i ../coveragesFile_DNaseFlash  -t ../CTCFData_narrowPeak_Ag09319_genomewide.txt -o ../CTCFOutputFeature_Ag09319_genomewide.txt -p ../param_SRR801880

python votingModel_inference.py -model LogisticRegressionL1,LogisticRegressionL2,RandomForest -train ../CMYCOutputFeature_2cat_SRR_filtered.txt_5_1000 -test ../CTCFOutputFeature_Ag09319_genomewide.txt_5_1000 -output default -k 2

python votingModel_inference.py -model LogisticRegressionL1,LogisticRegressionL2,RandomForest -train ../GABPOutputFeature_2cat_SRR_filtered.txt_5_1000 -test ../CTCFOutputFeature_Ag09319_genomewide.txt_5_1000 -output default -k 2

