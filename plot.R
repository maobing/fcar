#!/bin/Rscript
args <- commandArgs(TRUE)

print(args)

# args = c('','')
# args[1] = "../trainCMYCpredictCTCFLogisticRegressionL1,RandomForest,voting,Benchmark_result"
# args[2] =  "RandomForest,Benchmark,LogisticRegressionL1_0.005,LogisticRegressionL1_0.01,LogisticRegressionL1_0.05,LogisticRegressionL1_0.1,LogisticRegressionL1_0.5,LogisticRegressionL1_1,voting"

outputFile = args[1]
models = strsplit(x = args[2],split=",")[[1]]

testFile = read.table(outputFile, stringsAsFactor = FALSE)
for(i in 2:ncol(testFile)) {
	testFile[,i] = unlist(strsplit(x = testFile[,i], split = ":"))[(1:(nrow(testFile)*2)) %% 2 == 0]
}

# plot
roc <- function(prediction, truth) {
  prediction = as.numeric(prediction)
  truth = as.numeric(truth)
	# cutoff = seq(0,1,by=0.05)
  cutoff = sort(unique(prediction))
	TPR = vector("numeric", length(cutoff))
	FPR = vector("numeric", length(cutoff))
	for(i in 1:length(cutoff)) {
		TPR[i] <- sum(truth[prediction >= cutoff[i]])/sum(truth)
		FPR[i] <- sum(truth[prediction >= cutoff[i]] == 0)/sum(truth == 0)
	}
	roc = cbind(TPR, FPR)
	names(roc) = c("TPR","FPR")
	return(roc)
}

# calculate area under curve
auc <- function(TPR, FPR) {
  area = 0
  for(i in 2:length(TPR)) {
    area = area + 1/2*(TPR[i-1]+TPR[i])*(FPR[i]-FPR[i-1])*(-1)
  }
  return(area)
}

# library(RColorBrewer)
# cols = brewer.pal(ncol(testFile)-1,"Set1")
aucs = numeric(length(models))
cols = rainbow(ncol(testFile) - 1)
png(file = paste0(outputFile, ".png"), type = "cairo", bg = "transparent",
	width = 480*2, height = 480*2)
plot(NA, xlim=c(0,1),ylim=c(0,1), xlab = "FPR", ylab = "TPR", 
	main = paste0(substr(args[1],1,23),"\n",substr(args[1],24,nchar(args[1]))))

for(i in 2:ncol(testFile)) {
  i = 3; mylty = 1
	if(models[i-1] == "voting" || models[i-1] == "Benchmark") {
		mylty = 1
	}
	else {
		mylty = 2
	}
	tmp = roc(prediction = testFile[,i], truth = testFile[,1])
  aucs[i-1] = auc(TPR = tmp[,1], FPR = tmp[,2])
  cat(paste0("Model ", models[i-1], "'s auc is ", aucs[i-1],"\n"))
	lines(tmp[,1]~tmp[,2], col = cols[i-1], lty = mylty)
}

legend("bottomright", legend=c(models), col = cols, lty=1, lwd=2)
dev.off()

sink(file = paste0(args[1],"_auc"), append = TRUE)
cat(paste0(args[1],"\n"))
cat(paste0(models,collapse = " "))
cat(paste0(paste0(aucs,collapse = " "),"\n"))
sink(NULL)
