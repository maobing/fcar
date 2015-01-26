# train CMYC, CTCF
time ./extractFeature -i ../coveragesFile_DNaseHistone  -t ../CMYCData_narrowPeak_2cat.txt -o ../CMYCOutputFeature_2cat_DNaseHistone.txt -p ../param_DNaseHistone
time ./extractFeature -i ../coveragesFile_DNaseHistone  -t ../CTCFData_narrowPeak_2cat.txt -o ../CTCFOutputFeature_2cat_DNaseHistone.txt -p ../param_DNaseHistone

# test CTCF, GABP, EGR1
time ./extractFeature -i ../coveragesFile_DNaseHistone  -t ../CTCFData_narrowPeak_Gm12878_genomewide.txt -o ../CTCFOutputFeature_Gm12878_genomewide_DNaseHistone.txt -p ../param_DNaseHistone
time ./extractFeature -i ../coveragesFile_DNaseHistone  -t ../GABPData_narrowPeak_Gm12878_genomewide.txt -o ../GABPOutputFeature_Gm12878_genomewide_DNaseHistone.txt -p ../param_DNaseHistone
time ./extractFeature -i ../coveragesFile_DNaseHistone  -t ../EGR1Data_narrowPeak_Gm12878_genomewide.txt -o ../EGR1OutputFeature_Gm12878_genomewide_DNaseHistone.txt -p ../param_DNaseHistone