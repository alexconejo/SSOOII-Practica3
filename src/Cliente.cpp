/***********************************************************
 * Project         : Practica 3 de Sistemas Operativos II
 * 
 * Program Name    : Cliente.cpp
 * 
 * Author          : Álex Conejo y César Braojos
 * 
 * Purpose         : Objeto para el Cliente. Se define mediante
 *                   un id de cliente y una categoría. 
 * *********************************************************/

#include "../include/Cliente.h"
#include <string>

// Getter del id del cliente
int Cliente::GetClientId() { return client_id; }

// Getter de la categoría del cliente
std::string Cliente::GetCategory() { return category; }

// Setter del id del cliente
void Cliente::SetClientId(int id){
    client_id = id;
}

// Setter de la categoría del cliente
void Cliente::SetCategory(std::string c){
    category = c;
}


