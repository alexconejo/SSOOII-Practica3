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

#include "../include/ClientePI.h"
#include <string>

// Constructor
ClientePI::ClientePI(int client_id, std::string category) : Cliente(client_id, category){}



