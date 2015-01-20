linuxgcc:
	gcc -Wall -I /home/bst/student/bhe2/hji/samtools-0.1.18 -L/home/bst/student/bhe2/hji/samtools-0.1.18 -c fcarLib.c -lbam -lz -lm -o fcarLib.o
	gcc countCoverage.c fcarLib.o -L/home/bst/student/bhe2/hji/samtools-0.1.18 -lbam -lz -lm -o countCoverage
	gcc extractFeature.c fcarLib.o -L/home/bst/student/bhe2/hji/samtools-0.1.18 -lbam -lz -lm -o extractFeature
	gcc predictModel.c fcarLib.o -L/home/bst/student/bhe2/hji/samtools-0.1.18 -lbam -lz -lm -o predictModel
	gcc trainModel.c fcarLib.o -L/home/bst/student/bhe2/hji/samtools-0.1.18 -lbam -lz -lm -o trainModel
