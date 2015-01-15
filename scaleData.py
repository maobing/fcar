#!/usr/bin/python

#-------------------------------
# col-wise center and scale data
# input format: default
#-------------------------------
import re, sys, os.path, math

def mean(alist) :
  return sum(alist)/len(alist)

def stdev(alist):
  m = mean(alist)
  return math.sqrt( sum([ (a-m)**2 for a in alist])/(len(alist)-1) )

def scaleData(inputFile) :
  fp = open(inputFile, 'r')
  y = []
  x = []
  for line in fp :
    line.strip('\n')
    terms = line.split(' ')
    y.append(terms[0])
    x.append( [ float(a.split(':')[1]) for a in terms[1:] if a != '\n' ] )

  fp.close()

  for j in range(len(x[0])) :
    m = mean( [ x[i][j] for i in range(len(x)) ] )
    s = stdev( [ x[i][j] for i in range(len(x)) ] )
    
    for i in range(len(x)) :
      x[i][j] = (x[i][j] - m)/s

  outFp = open(inputFile + '_scale', 'w')
  
  for i in range(len(x)) :
    outFp.write(y[i]+' ')
    for j in range(len(x[0])) :
      if j == len(x[0]) - 1:
        outFp.write( ''.join( [repr(j+1),':', '%.5f' % x[i][j],'\n'] ) )
      else :
        outFp.write( ''.join( [repr(j+1),':', '%.5f' % x[i][j],' '] ) )

  outFp.close()
  return 0

def main() :
  TFs = ['CMYC','CTCF','USF1','SRF','NRSF','GABP','EGR1','E2F']
  fileDNase = [ '../' + TF + 'OutputFeature_2cat.txt_5_1000' for TF in TFs ] 
  fileDNaseFlash = [ '../' + TF + 'OutputFeature_2cat_SRR.txt_5_1000' for TF in TFs ]

  for file in fileDNase + fileDNaseFlash :
    print 'scaling %s' % file
    scaleData(file)

if __name__ == '__main__' :
  main()
 
