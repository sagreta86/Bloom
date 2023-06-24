default:
	g++ -pg -m64 -march=native -mtune=native -mssse3 -Wall -Wextra -Wno-deprecated-copy -Ofast -ftree-vectorize -flto -c smhasher/MurmurHash3.cpp -o MurmurHash3.o
	g++ -m64 -march=native -mtune=native -mssse3 -Wall -Wextra -Wno-deprecated-copy -Ofast -ftree-vectorize -o bloom bloom.cpp MurmurHash3.o -lm -lpthread
	