/***********************************************************
 * Project         : Practica 3 de Sistemas Operativos II
 * 
 * Program Name    : ClientePI.h
 * 
 * Author          : Álex Conejo y César Braojos
 * 
 * Purpose         : Contiene la estructura del objeto Cliente
 *                   Premium con saldo ilimitado.
 * *********************************************************/

#include <iostream>
#include <string>
#include "../src/Cliente.cpp"

class ClientePI : public Cliente
    {
   
    public:
        ClientePI(int client_id, std::string category);       
    };