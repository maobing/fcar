import os.path, time
import subprocess, shlex

TFs = ['CMYC','CTCF','E2F','EGR1','GABP','NRSF','SRF','USF1']
model = 'LogisticRegressionL1,LogisticRegressionL2,RandomForest'

for TF in TFs :
  trainTF = [TF]
  testTF = [ a for a in TFs if a != TF ]
  
  # MYC does not have Gm12878 narrowPeak
  if 'CMYC' in trainTF :
    continue

  # DNase+Histone
  k = 10 
  for trTF in trainTF :
    for teTF in testTF :
      cmd = 'python votingModel.py -model %s -train ../%sOutputFeature_2cat.txt_5_1000 -test ../%sOutputFeature_2cat.txt_5_1000 -output default -k %d' % (model, trTF, teTF, k)
      subprocess.call(shlex.split(cmd))
      print cmd

  # DNaseFlash
  k = 2
  for trTF in trainTF :
    for teTF in testTF :
      cmd = 'python votingModel.py -model %s -train ../%sOutputFeature_2cat_SRR.txt_5_1000 -test ../%sOutputFeature_2cat_SRR.txt_5_1000 -output default -k %d' % (model, trTF, teTF, 2)
      subprocess.call(shlex.split(cmd))
      print cmd


