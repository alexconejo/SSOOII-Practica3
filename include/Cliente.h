/***********************************************************
 * Project         : Practica 3 de Sistemas Operativos II
 * 
 * Program Name    : Cliente.h
 * 
 * Author          : Álex Conejo y César Braojos
 * 
 * Purpose         : Contiene la estructura del objeto Cliente
 * *********************************************************/

#include <iostream>
#include <string>
#include <queue>

class Cliente 
    {
    private: 
        int client_id;
        std::string category;
        int creditos;
        std::queue<std::queue<std::string>> client_queue;
        bool found = false;
   
    public:
        Cliente(int client_id, std::string category,int creditos, std::queue<std::queue<std::string>> client_queue);
        int GetClientId();
        std::string GetCategory();
        void SetClientId(int id);
        void SetCategory(std::string c);  
        int GetCreditos();
        void SetCreditos(int c);   
        bool GetFound();
        std::queue <std::queue <std::string>> GetQueue();
        void SetQueue(std::queue <std::queue <std::string>> q);
        void SetFound(bool b);
        void Push(std::queue<std::string> l_queue);
        std::queue<std::string> Pop();
        bool Empty();
    };