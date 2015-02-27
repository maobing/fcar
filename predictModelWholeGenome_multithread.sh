# valgrind --leak-check=yes 
./predictModelWholeGenome_multithread -m LogisticRegressionL1 -tm ../EGR1OutputFeature_2cat_ChIPseq.txt_5_1000_train_trainedLogisticRegressionL1_0.1 -train ../EGR1OutputFeature_2cat_ChIPseq.txt_5_1000 -coverage ../coveragesFile_ChIPseq -o multiTEST_valgrind_qsub -p ../param_ChIPseq

