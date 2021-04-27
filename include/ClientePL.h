/***********************************************************
 * Project         : Practica 3 de Sistemas Operativos II
 * 
 * Program Name    : ClientePL.h
 * 
 * Author          : Álex Conejo
 * 
 * Purpose         : Contiene la estructura del objeto Cliente
 *                   Premium Limitado.
 * *********************************************************/

#include <iostream>
#include <vector>
#include "../src/Cliente.cpp"

class ClientePL : public Cliente
    {
    private:
        int saldo;

    public:

        int GetSaldo();
        void SetSaldo(int s);
        
    };