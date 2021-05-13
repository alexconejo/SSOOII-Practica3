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
        std::mutex mutex_queue;
        std::queue <Cliente> protected_queue;
        
    public:
        void Push(Cliente pl);
        void Pop();   
        Cliente Front();
        bool Empty();
        void Recharge(int credits);
        int Size();
    };