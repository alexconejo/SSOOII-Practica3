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

// Getter del id del cliente
int ClientePL::GetSaldo() { return saldo; }

// Setter del id del cliente
void ClientePL::SetSaldo(int s){
    saldo = s;
}


