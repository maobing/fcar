import os.path, time

trainTF = ['CMYC']
testTF = ['CTCF','E2F','EGR1','GABP','NRSF','SRF','USF1']
model = 'LogisticRegressionL1,LogisticRegressionL2,RandomForest'

k = 10
for trTF in trainTF :
	for teTF in testTF :
		cmd = 'python votingModel.py -model %s -train ../%sOutputFeature_2cat.txt_5_1000 -test ../%sOutputFeature_2cat.txt_5_1000 -output default -k %d' % (model, trTF, teTF, k)
		# print ','.join(['$', repr(time.time()), repr(time.clock())])
		subprocess.call(shlex.split(cmd))
		# print ','.join(['$', repr(time.time()), repr(time.clock())])

k = 2
for trTF in trainTF :
	for teTF in testTF :
		cmd = 'python votingModel.py -model %s -train ../%sOutputFeature_2cat_SRR.txt_5_1000 -test ../%sOutputFeature_2cat_SRR.txt_5_1000 -output default -k %d' % (model, trTF, teTF, 2)
		# print ','.join(['$', repr(time.time()), repr(time.clock())])
		subprocess.call(shlex.split(cmd))
		# print ','.join(['$', repr(time.time()), repr(time.clock())])
