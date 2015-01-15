import os.path, time
import subprocess, shlex

TFs = ['CMYC','CTCF','E2F','EGR1','GABP','NRSF','SRF','USF1']
model = 'LogisticRegressionL1,LogisticRegressionL2,RandomForest'

# scaled DNase+Histone
  k = 10
  for trTF in trainTF :
    for teTF in testTF :
      cmd = 'python votingModel.py -model %s -train ../%sOutputFeature_2cat.txt_5_1000_scale -test ../%sOutputFeature_2cat.txt_5_1000_scale -output default -k %d' % (model, trTF, teTF, k)
      subprocess.call(shlex.split(cmd))
      print cmd

# scaled DNaseFlash
  k = 2
  for trTF in trainTF :
    for teTF in testTF :
      cmd = 'python votingModel.py -model %s -train ../%sOutputFeature_2cat_SRR.txt_5_1000_scale -test ../%sOutputFeature_2cat_SRR.txt_5_1000_scale -output default -k %d' % (model, trTF, teTF, 2)
      subprocess.call(shlex.split(cmd))
      print cmd

