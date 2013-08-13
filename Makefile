

all:
	g++ MOE/MrzLoader.cpp main.cpp PLQBVHMeshAccelerator.cpp -O2 -fopenmp -o edupt