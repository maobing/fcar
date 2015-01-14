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
  fp = open(inputFile)
  y = []
  x = []
  for line in fp :
    line.strip('\n')
    terms = line.split(' ')
    y.append(terms[0])
    x.append( [ float(a.split(':')[1]) for a in terms if a != '\n' ] )

  fp.close()

  for j in range(len(x[0])) :
    m = mean( [ x[i][j] for i in 1:range(len(x)) ] )
    s = stdev( [ x[i][j] for i in 1:range(len(x)) ] )
    
    for i in range(len(x)) :
      x[i][j] = (x[i][j] - m)/s

  outFp = open(inputFile + '_scale', 'w')
  
  for i in range(len(x)) :
    outFp.write(y[i]+' ')
    for j in range(len(x[0])) :
      if j == len(x[0]) - 1:
        outFp.write( ''.join( [repr(j+1),':',x[i][j],'\n'] ) )
      else :
        outFp.write( ''.join( [repr(j+1),':',x[i][j],' '] ) )

  outFp.close()
  return 0

def main(argv) :
  if len(argv) != 2:
    print 'usage scaleData inputData'
    return -1
  else:
    scaleData(argv)

if __name__ == '__main__' :
  main(sys.argv)
 
