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

#include "../include/ClienteNP.h"
#include <string>

// Constructor
ClienteNP::ClienteNP(int client_id, std::string category, int creditos) : Cliente(client_id, category){
    this -> creditos = creditos;
}

// Getter del id del cliente
int ClienteNP::GetCreditos() { return creditos; }

// Setter del id del cliente
void ClienteNP::SetCreditos(int c){
    creditos = c;
}


