# modeling
time python votingModel_inference.py -model LogisticRegressionL1,LogisticRegressionL2,RandomForest -train ../CTCFOutputFeature_2cat_DNaseAlone.txt_5_1000 -test ../GABPOutputFeature_Gm12878_genomewide_DNaseAlone.txt_5_1000 -output default -k 2
