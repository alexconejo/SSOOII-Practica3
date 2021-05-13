/***********************************************************
 * Project         : Practica 3 de Sistemas Operativos II
 * 
 * Program Name    : ProctectedQueue.h
 * 
 * Author          : Álex Conejo y César Braojos
 * 
 * Purpose         : Contiene la estructura del objeto para 
 *                   cola protegida ProtectedQueue.
 * *********************************************************/

#include <iostream>
#include <string>
#include <mutex>
#include <queue>
#include "../src/Client.cpp"


class ProtectedQueue 
    {
    private: 
        std::mutex mutex_queue;
        std::queue <Client> protected_queue;
        
    public:
        void Push(Client pl);
        void Pop();   
        Client Front();
        bool Empty();
        void Recharge(int credits);
        int Size();
    };