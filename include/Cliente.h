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

class Cliente 
    {
    private: 
        int client_id;
        std::string category;
   
    public:
        int GetClientId();
        std::string GetCategory();
        void SetClientId(int id);
        void SetCategory(std::string c);       
    };