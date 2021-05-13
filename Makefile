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
	$(CC)  $(DIRSRC)SearchManagement.cpp -o $(DIREXE)SearchManagement -pthread -std=c++11

test:
	./$(DIREXE)SearchManagement 


clean:

	rm -rf $(DIREXE)
	rm -rf clientes




