CXX = g++
CXXFLAGS = -Wall -pedantic -Wno-long-long -O0 -ggdb -std=c++14
LD = g++
LDFLAGS = -Wall -pedantic -Wno-long-long -O0 -ggdb -std=c++14
LIBRARIES = -lncurses -lm
PROGRAM = steuejan
FILES = ./src/block.o ./src/bomb.o ./src/bonus.o ./src/computer.o ./src/constants.o ./src/game.o ./src/human.o ./src/main.o ./src/player.o

$(PROGRAM): $(FILES)
	$(LD) $(LDFLAGS) $^ -o $(PROGRAM) $(LIBRARIES)	

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -f ./src/*.o $(PROGRAM)
	rm -rf ./doc

compile: $(PROGRAM)

run: compile
	./$(PROGRAM) $(map)

count:
	# wc -l ./src/*.cpp ./src/*.hpp

doc:
	doxygen Doxyfile

block.o: block.cpp block.hpp constants.hpp
bomb.o: bomb.cpp bomb.hpp block.hpp constants.hpp bonus.hpp player.hpp
bonus.o: bonus.cpp bonus.hpp player.hpp bomb.hpp block.hpp constants.hpp
computer.o: computer.cpp computer.hpp player.hpp bomb.hpp block.hpp constants.hpp bonus.hpp
constants.o: constants.cpp constants.hpp
game.o: game.cpp game.hpp player.hpp bomb.hpp block.hpp constants.hpp bonus.hpp
human.o: human.cpp human.hpp player.hpp bomb.hpp block.hpp constants.hpp bonus.hpp
main.o: main.cpp game.hpp player.hpp bomb.hpp block.hpp constants.hpp bonus.hpp human.hpp computer.hpp
player.o: player.cpp player.hpp bomb.hpp block.hpp constants.hpp bonus.hpp



