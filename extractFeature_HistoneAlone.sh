# train CMYC, CTCF
time ./extractFeature -i ../coveragesFile_HistoneAlone  -t ../CMYCData_narrowPeak_2cat.txt -o ../CMYCOutputFeature_2cat_HistoneAlone.txt -p ../param_HistoneAlone
time ./extractFeature -i ../coveragesFile_HistoneAlone  -t ../CTCFData_narrowPeak_2cat.txt -o ../CTCFOutputFeature_2cat_HistoneAlone.txt -p ../param_HistoneAlone

# test CTCF, GABP, EGR1
time ./extractFeature -i ../coveragesFile_HistoneAlone  -t ../CTCFData_narrowPeak_Gm12878_genomewide.txt -o ../CTCFOutputFeature_Gm12878_genomewide_HistoneAlone.txt -p ../param_HistoneAlone
time ./extractFeature -i ../coveragesFile_HistoneAlone  -t ../GABPData_narrowPeak_Gm12878_genomewide.txt -o ../GABPOutputFeature_Gm12878_genomewide_HistoneAlone.txt -p ../param_HistoneAlone
time ./extractFeature -i ../coveragesFile_HistoneAlone  -t ../EGR1Data_narrowPeak_Gm12878_genomewide.txt -o ../EGR1OutputFeature_Gm12878_genomewide_HistoneAlone.txt -p ../param_HistoneAlone