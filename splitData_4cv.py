#!/usr/bin/python

import sys, os.path, re

def splitData(file, fold) :
  f = open(file, 'r')
  lines = f.readlines()
  f.close()

  for i in range(fold):
    f1 = open(file + '_cv' + repr(i+1) + '_test', 'w')
    f1.write(''.join( lines[i::fold] ))
    f1.close()

    f2 = open(file + '_cv' + repr(i+1) + '_train', 'w')
    lines2 = lines[:] # copy the list, instead of copying the ref
    for l in lines[i::fold] :
      lines2.remove(l)
    f2.write(''.join( lines2 ))
    f2.close()

  return 0

def main(argv):
  if len(argv) != 3:
    print 'usage: python splitData.py dataFile fold'
    return 1
  else :
    splitData(sys.argv[1], int(sys.argv[2]))
    return 0

if __name__ == '__main__':
  main(sys.argv)
