#--------------------------
# 2014-12-19 BH
#-------------------------
# experiment 1
# trainTF: CMYC
# models: LogisticRegressionL1,
#	LogisticRegressionL2,SVM,RandomForest
# stack: TRUE
# test: CTCF,E2F,EGR1,GABP,NRSF,SRF,USF1
# inference: FALSE
#--------------------------
import shlex, subprocess, sys
import os.path, time

# extract feature for all TFs
print '------------------------------------'
print '- extracting features'
trainTF = ['CMYC']
testTF = ['CTCF','E2F','EGR1','GABP','NRSF','SRF','USF1']
model = 'LogisticRegressionL1,LogisticRegressionL2,RandomForest'
meta = 'RandomForest'

for TF in trainTF + testTF :
	print "extracting features for %s" % TF
	if os.path.isfile('../%sOutputFeature_2cat.txt_5_1000' % TF) :
		continue
	cmd = './extractFeature -i ../featureBamFile -t ../%sData_narrowPeak_2cat.txt -o ../%sOutputFeature_2cat.txt -p ../param ' % (TF,TF)

	
	# print ','.join(['$', repr(time.time()), repr(time.clock())])
	subprocess.call(shlex.split(cmd))
	# print ','.join(['$', repr(time.time()), repr(time.clock())])
	
for trTF in trainTF :
	for teTF in testTF :
		cmd = 'python stackModel.py -model %s -meta %s -train ../%sOutputFeature_2cat.txt_5_1000 -test ../%sOutputFeature_2cat.txt_5_1000 -output ../%sStackedOutputTrainedBy%s' % (model, meta, trTF, teTF, teTF, trTF)
		# print ','.join(['$', repr(time.time()), repr(time.clock())])
		subprocess.call(shlex.split(cmd))
		# print ','.join(['$', repr(time.time()), repr(time.clock())])

# added R script for plotting
