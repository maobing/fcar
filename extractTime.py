import sys, re

def extractTime(file) :
  f = open(file, 'r')

  for line in f:
    if re.search(r'elapsed', line):
      print line

if __name__ == '__main__' :
  extractTime(sys.argv[1])

