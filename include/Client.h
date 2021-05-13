/***********************************************************
 * Project         : Practica 3 de Sistemas Operativos II
 * 
 * Program Name    : Client.h
 * 
 * Author          : Álex Conejo y César Braojos
 * 
 * Purpose         : Contiene la estructura del objeto Client
 * *********************************************************/

#include <iostream>
#include <string>
#include <queue>

class Client 
    {
    private: 
        int client_id;
        std::string category;
        int credits;
        std::queue<std::queue<std::string>> client_queue;
        bool found = false;
   
    public:
        Client(int client_id, std::string category,int credits, std::queue<std::queue<std::string>> client_queue);
        int GetClientId();
        std::string GetCategory();
        void SetClientId(int id);
        void SetCategory(std::string c);  
        int GetCredits();
        void SetCredits(int c);   
        std::queue <std::queue <std::string>> GetQueue();
        void Push(std::queue<std::string> l_queue);
        std::queue<std::string> Pop();
        bool Empty();
    };