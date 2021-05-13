/***********************************************************
 * Project         : Practica 3 de Sistemas Operativos II
 * 
 * Program Name    : Client.cpp
 * 
 * Author          : Álex Conejo y César Braojos
 * 
 * Purpose         : Objeto para el Client. Se define mediante
 *                   un id de cliente y una categoría. 
 * *********************************************************/

#pragma once //Para los errores de duplicacion de include
#include "../include/Client.h"
#include <string>

// Constructor. //
Client::Client(int client_id, std::string category,int credits, std::queue < std::queue <std::string>> client_queue){
    this -> client_id = client_id; 
    this -> category = category;
    this -> credits = credits;
    this -> client_queue = client_queue;
}

// Getter del id del cliente. //
int Client::GetClientId() { return client_id; }

// Getter de la categoría del cliente. //
std::string Client::GetCategory() { return category; }

// Getter de los creditos del cliente. //
int Client::GetCredits() { return credits; }

// Setter del id del cliente. //
void Client::SetClientId(int id){
    client_id = id;
}

// Setter de la categoría del cliente. //
void Client::SetCategory(std::string c){
    category = c;
}

// Setter de los creditos del cliente. //
void Client::SetCredits(int c){
    credits = c;
}

// Getter de la cola del cliente. //
std::queue <std::queue <std::string>> Client::GetQueue(){return client_queue;}

// Añade colas a la cola del cliente. //
void Client::Push(std::queue<std::string> l_queue){
    client_queue.push(l_queue);  
}

// Saca la cola que encabeza la cola del cliente. //
std::queue<std::string> Client::Pop(){
    std::queue<std::string> frontelement = client_queue.front();
    client_queue.pop();
    return frontelement;  
}

// Comprueba si la cola del cliente está vacia. //
bool Client::Empty(){
    return client_queue.empty();
}
