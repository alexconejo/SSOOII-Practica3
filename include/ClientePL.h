/***********************************************************
 * Project         : Practica 3 de Sistemas Operativos II
 * 
 * Program Name    : ClientePL.h
 * 
 * Author          : Álex Conejo
 * 
 * Purpose         : Contiene la estructura del objeto Cliente
 *                   Premium con saldo limitado.
 * *********************************************************/

#include <iostream>
#include <vector>
#include "../src/Cliente.cpp"
#include <string>

class ClientePL : public Cliente
    {
    private:
        int saldo;

    public:

        ClientePL(int client_id, std::string category, int saldo);
        int GetSaldo();
        void SetSaldo(int s);
        
    };