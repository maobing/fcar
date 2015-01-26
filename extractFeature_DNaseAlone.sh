# train CMYC, CTCF
time ./extractFeature -i ../coveragesFile_DNaseAlone  -t ../CMYCData_narrowPeak_2cat.txt -o ../CMYCOutputFeature_2cat_DNaseAlone.txt -p ../param_DNaseAlone
time ./extractFeature -i ../coveragesFile_DNaseAlone  -t ../CTCFData_narrowPeak_2cat.txt -o ../CTCFOutputFeature_2cat_DNaseAlone.txt -p ../param_DNaseAlone

# test CTCF, GABP, EGR1
time ./extractFeature -i ../coveragesFile_DNaseAlone  -t ../CTCFData_narrowPeak_Gm12878_genomewide.txt -o ../CTCFOutputFeature_Gm12878_genomewide_DNaseAlone.txt -p ../param_DNaseAlone
time ./extractFeature -i ../coveragesFile_DNaseAlone  -t ../GABPData_narrowPeak_Gm12878_genomewide.txt -o ../GABPOutputFeature_Gm12878_genomewide_DNaseAlone.txt -p ../param_DNaseAlone
time ./extractFeature -i ../coveragesFile_DNaseAlone  -t ../EGR1Data_narrowPeak_Gm12878_genomewide.txt -o ../EGR1OutputFeature_Gm12878_genomewide_DNaseAlone.txt -p ../param_DNaseAlone