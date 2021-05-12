DIROBJ := obj/
DIREXE := exec/
DIRSRC := src/
LDLIBS := -lpthread -lrt
CC := g++
FILS = Log.txt
COPY = CopiaDeSeguridad

all:   dir SSOOIIGLE

dir:
	mkdir  exec
	mkdir  clientes

SSOOIIGLE:
	$(CC)  $(DIRSRC)Main.cpp -o $(DIREXE)Main.cpp -pthread -std=c++11

test:
	./$(DIREXE)Main.cpp 


clean:

	rm -rf $(DIREXE)
	rm -rf clientes
	

