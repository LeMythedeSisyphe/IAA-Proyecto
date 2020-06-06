program:
	g++ -o program informe1.cpp

.PHONY: run
run: program
	./program
clean:
	rm ./program.exe