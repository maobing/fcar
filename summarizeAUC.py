# summarize auc results to 8 tables

TFs = ['CMYC','CTCF','E2F','EGR1','GABP','NRSF','SRF','USF1']

for TF in TFs :
  trainTF = TF
  outFp = open(trainTF + 'OutputFeature_2cat.txt_5_1000' + '_auc' , 'w')
  testTF = [a for a in TFs if a != TF]
  for teTF in testTF :
    file = ''.join( ['../', trainTF, 'OutputFeature_2cat.txt_5_1000', teTF, \
        'OutputFeature_2cat.txt_5_1000', 'LogisticRegressionL1,RandomForest,',\
        'LogisticRegressionL2,Benchmark,voting_result_auc' ] )
    print 'opening file %s' % file
    fp = open(file, 'r')
    content = fp.readlines()
    outFp.write(teTF + ' ' + content[2])
    fp.close()

  outFp.close()
