
all: clean run

run: Practice.cpp
	g++ -o Practice Practice.cpp
	g++ -o PracticeClient PracticeClient.cpp
	./Practice
	./PracticeClient
clean: Practice PracticeClient
	rm Practice 
	rm PracticeClient