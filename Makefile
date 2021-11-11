#makefile

all: crcExperiments.cpp
	g++ -g -Wall -o crcExperiments crcExperiments.cpp

clean: 
	$(RM) crcExperiments *~
