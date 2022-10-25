CC = g++

CFLAGS = -Wall --std=c++20
INLCUDES = -I ./src/include
RAYLIB = `pkg-config --cflags --libs raylib`
OPENCV = `pkg-config --cflags --libs opencv4`
WISARDPKG = `-I src/include/`

output:
	$(CC) src/main.cpp $(INLCUDES) $(RAYLIB) $(OPENCV) $(CFLAGS) -o main

clean:
	rm ./*.o main