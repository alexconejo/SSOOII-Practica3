/***********************************************************
 * Project         : Practica 3 de Sistemas Operativos II
 * 
 * Program Name    : ClientePL.cpp
 * 
 * Author          : Álex Conejo y César Braojos
 * 
 * Purpose         : Objeto para el Cliente Premium Limitado.
 *                   Se define mediante un id de cliente, una
 *                   categoría y un número máximo de saldo. 
 * *********************************************************/

#include "../include/ClientePL.h"
#include <string>

// Constructor
ClientePL::ClientePL(int client_id, std::string category, int saldo) : Cliente(client_id, category){
    this -> saldo = saldo; 
}

// Getter del id del cliente
int ClientePL::GetSaldo() { return saldo; }

// Setter del id del cliente
void ClientePL::SetSaldo(int s){
    saldo = s;
}

