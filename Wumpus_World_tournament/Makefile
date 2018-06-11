UNAME := $(shell uname)

.PHONY: all clean

all:
	bin/RunTournament.sh
	
clean:
	rm -f unzip_ais_errors.log
	rm -f Tournament_Results.txt
	rm -f Agents_cpp/*.txt
	
generate_Worlds:
	rm -f Worlds/*

ifeq ($(UNAME) , Linux)
	bin/World_Generator Worlds/TestWorld4x4 1000 4
	bin/World_Generator Worlds/TestWorld5x5 1000 5
	bin/World_Generator Worlds/TestWorld6x6 1000 6
	bin/World_Generator Worlds/TestWorld7x7 1000 7
else
	bin/World_Generator.exe Worlds/TestWorld4x4 1000 4
	bin/World_Generator.exe Worlds/TestWorld5x5 1000 5
	bin/World_Generator.exe Worlds/TestWorld6x6 1000 6
	bin/World_Generator.exe Worlds/TestWorld7x7 1000 7
endif
