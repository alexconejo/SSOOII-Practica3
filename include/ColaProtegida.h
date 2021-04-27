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
#include "../src/ClientePL.cpp"


class ColaProtegida 
    {
    private: 
        std::mutex semaforo;
        std::queue <ClientePL> protected_queue;
        bool empty;
        
    public:
        void Push(ClientePL pl);
        void Pop();   
        ClientePL Front();
        bool Empty();
        void Recharge(int saldo);
    };