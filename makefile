program:
	g++ -o program entrega3.cpp

.PHONY: run
run: program
	./program $f $s