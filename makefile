program:
	g++ -o program entrega2.cpp

.PHONY: run
run: program
	./program $f $s