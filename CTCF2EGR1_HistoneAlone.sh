# modeling
time python votingModel_inference.py -model LogisticRegressionL1,LogisticRegressionL2,RandomForest -train ../CTCFOutputFeature_2cat_HistoneAlone.txt_5_1000 -test ../EGR1OutputFeature_Gm12878_genomewide_HistoneAlone.txt_5_1000 -output default -k 8
