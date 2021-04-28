/***********************************************************
 * Project         : Practica 3 de Sistemas Operativos II
 * 
 * Program Name    : Cliente.h
 * 
 * Author          : Álex Conejo y César Braojos
 * 
 * Purpose         : Contiene la estructura del objeto Cliente
 *                   gratuito
 * *********************************************************/

#include <iostream>
#include <string>
#include "../src/Cliente.cpp"

class ClienteNP : public Cliente
    {
    private:
        int creditos;

    public:
        ClienteNP(int client_id, std::string category, int creditos);
        int GetCreditos();
        void SetCreditos (int c);
    
    };