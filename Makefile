CC = g++

all:  
	mkdir ./bin
	$(CC) -o ./bin/LiChEn src/LiChEn.cpp

clean:
	rm -rf bin
