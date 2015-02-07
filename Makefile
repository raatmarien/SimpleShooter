CC=g++

simpleShooter: main.cpp
	$(CC) -c main.cpp -std=c++11
	$(CC) main.o -o simpleShooter -lsfml-graphics -lsfml-window -lsfml-system
	rm main.o
	./simpleShooter
