# modeling
time python votingModel_inference.py -model LogisticRegressionL1,LogisticRegressionL2,RandomForest -train ../CMYCOutputFeature_2cat_DNaseHistone.txt_5_1000 -test ../CTCFOutputFeature_Gm12878_genomewide_DNaseHistone.txt_5_1000 -output default -k 10
