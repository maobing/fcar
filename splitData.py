#!/usr/bin/python

import sys, os.path, re

def splitData(file) :
  f = open(file, 'r')
  lines = f.readlines()
  f.close()

  f1 = open(file + '_test', 'w')
  f1.write(''.join( lines[::3] ))
  f1.close()

  f2 = open(file + '_train', 'w')
  lines2 = lines
  for l in lines[::3] :
    lines2.remove(l)
  f2.write(''.join( lines2 ))
  f2.close()

  return 0

def main(argv):
  if len(argv) != 2:
    print 'useage: python splitData.py dataFile'
    return 1
  else :
    splitData(sys.argv[1])
    return 0

if __name__ == '__main__':
  main(sys.argv)
