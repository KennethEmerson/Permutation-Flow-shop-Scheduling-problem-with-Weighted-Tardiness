###################################################################################################
# PERMUTATION FLOW-SHOP SCHEDULING PROBLEM                                                                              
# This is the makefile required to compile the flowshop program
###################################################################################################


# linking the main program (DEFAULT)
flowshop: main.o pfspinstance.o configuration.o configparser.o initsolution.o neighbourhood.o helpers.o solver.o fileio.o runners.o 
	@echo "Linking all object files for flowshop"
	@g++ ./bin/main.o \
	./bin/configuration.o \
	./bin/configparser.o \
	./bin/pfspinstance.o \
	./bin/initsolution.o \
	./bin/neighbourhood.o \
	./bin/helpers.o \
	./bin/solver.o \
	./bin/fileio.o \
	./bin/runners.o \
	-pthread \
	-o flowshop

# linking the flowshoptest program containing the tests
flowshoptest: tests.o pfspinstance.o configuration.o initsolution.o neighbourhood.o helpers.o 
	@echo "Linking all object files for flowshoptest"
	@g++ ./bin/tests.o \
	./bin/configuration.o \
	./bin/pfspinstance.o \
	./bin/initsolution.o \
	./bin/neighbourhood.o \
	./bin/helpers.o \
	-o flowshoptest

###################################################################################################

main.o: ./src/main.cpp 
	@echo "compiling main.cpp"
	@g++ -c ./src/main.cpp -o ./bin/main.o

tests.o: ./src/tests.cpp 
	@echo "compiling tests.cpp"
	@g++ -c ./src/tests.cpp -o ./bin/tests.o

configuration.o: ./src/configuration.cpp ./src/configuration.h
	@echo "compiling configuration.cpp"
	@g++ -c ./src/configuration.cpp -o ./bin/configuration.o

configparser.o: ./src/configparser.cpp ./src/configparser.h
	@echo "compiling configparser.cpp"
	@g++ -c ./src/configparser.cpp -o ./bin/configparser.o

pfspinstance.o: ./src/pfspinstance.cpp ./src/pfspinstance.h
	@echo "compiling pfspinstance.cpp"
	@g++ -c ./src/pfspinstance.cpp -o ./bin/pfspinstance.o

initsolution.o: ./src/initsolution.cpp ./src/initsolution.h
	@echo "compiling initsolution.cpp"
	@g++ -c ./src/initsolution.cpp -o ./bin/initsolution.o

neighbourhood.o: ./src/neighbourhood.cpp ./src/neighbourhood.h
	@echo "compiling neighbourhood.cpp"
	@g++ -c ./src/neighbourhood.cpp -o ./bin/neighbourhood.o

helpers.o: ./src/helpers.cpp ./src/helpers.h
	@echo "compiling helpers.cpp"
	@g++ -c ./src/helpers.cpp -o ./bin/helpers.o

solver.o: ./src/solver.cpp ./src/solver.h
	@echo "compiling solver.cpp"
	@g++ -c ./src/solver.cpp -o ./bin/solver.o

fileio.o: ./src/fileio.cpp ./src/fileio.h
	@echo "compiling fileio.cpp"
	@g++ -c ./src/fileio.cpp -o ./bin/fileio.o

runners.o: ./src/runners.cpp ./src/runners.h
	@echo "compiling runners.cpp"
	@g++ -c ./src/runners.cpp -pthread -o ./bin/runners.o

###################################################################################################

.PHONY: clean
clean:
	@echo "removing all compiled files"
	@rm bin/*.o flowshop