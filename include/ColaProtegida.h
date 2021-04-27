/***********************************************************
 * Project         : Practica 3 de Sistemas Operativos II
 * 
 * Program Name    : ColaProtegida.h
 * 
 * Author          : Álex Conejo y César Braojos
 * 
 * Purpose         : Contiene la estructura del objeto ColaProtegida
 * *********************************************************/

#include <iostream>
#include <string>
#include <mutex>
#include <queue>
#include "../src/Cliente.cpp"

class ColaProtegida 
    {
    private: 
        std::mutex semaforo;

    public:
        std::queue <Cliente> Introduce(std::queue <Cliente> client_queue);
        std::queue <Cliente> TakeOut(std::queue <Cliente> client_queue);   
    };