build:
	g++ -o serial serial.cpp
	g++ -o paralel paralel.cpp -fopenmp
	
clean:
	rm -Rf *.o *~ serial paralel
