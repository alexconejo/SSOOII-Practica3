/***********************************************************
 * Project         : Practica 3 de Sistemas Operativos II
 * 
 * Program Name    : ColaProtegida.cpp
 * 
 * Author          : Álex Conejo y César Braojos
 * 
 * Purpose         : Objeto para la cola protegida. Tendrá
 *                   un método para introducir o sacar datos
 *                   de la cola. 
 * *********************************************************/

#include "../include/ColaProtegida.h"
#include <string>
#include <queue>
#include <signal.h>

// Introduce datos a la cola.
std::queue <Cliente> ColaProtegida::Introduce(std::queue <Cliente> client_queue){ 
    
    return client_queue; 
}


// Saca datos de la cola.
std::queue <Cliente> ColaProtegida::TakeOut(std::queue <Cliente> client_queue) { 

    return client_queue; 
}