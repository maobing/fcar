linuxgcc:
	g++ -g -O0 -Wall -I /home/bst/student/bhe2/hji/samtools-0.1.18 -L/home/bst/student/bhe2/hji/samtools-0.1.18 -c fcarLib.c -lbam -lz -lm -o fcarLib.o
	g++ -g -O0 -Wall countCoverage.c fcarLib.o -L/home/bst/student/bhe2/hji/samtools-0.1.18 -lbam -lz -lm -o countCoverage
	g++ -g -O0 -Wall extractFeature.c fcarLib.o -L/home/bst/student/bhe2/hji/samtools-0.1.18 -lbam -lz -lm -o extractFeature
	g++ -g -O0 -Wall predictModel.c fcarLib.o -L/home/bst/student/bhe2/hji/samtools-0.1.18 -lbam -lz -lm -o predictModel
	g++ -g -O0 -Wall trainModel.c fcarLib.o -L/home/bst/student/bhe2/hji/samtools-0.1.18 -lbam -lz -lm -o trainModel
	g++ -g -O0 -Wall -o predictModelWholeGenome_multithread predictModelWholeGenome_multithread.c ./liblinear-1.96/tron.o ./liblinear-1.96/linear.o ./fcarLib.o ./liblinear-1.96/blas/blas.a -L/home/bst/student/bhe2/hji/samtools-0.1.18 -lbam -lz -lpthread
