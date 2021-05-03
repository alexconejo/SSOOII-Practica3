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

#pragma once //Para los errores de duplicacion de include
#include "../include/Cliente.h"
#include <string>

// Constructor
Cliente::Cliente(int client_id, std::string category,int creditos, std::queue < std::queue <std::string>> client_queue){
    this -> client_id = client_id; 
    this -> category = category;
    this -> creditos = creditos;
    this -> client_queue = client_queue;
}

// Getter del id del cliente
int Cliente::GetClientId() { return client_id; }

// Getter de la categoría del cliente
std::string Cliente::GetCategory() { return category; }

int Cliente::GetCreditos() { return creditos; }
// Setter del id del cliente
void Cliente::SetClientId(int id){
    client_id = id;
}

// Setter de la categoría del cliente
void Cliente::SetCategory(std::string c){
    category = c;
}

// Setter de la categoría del cliente
void Cliente::SetCreditos(int c){
    creditos = c;
}

bool Cliente::GetFound(){return found;}

void Cliente::SetFound(bool b){
    found = b;
}

std::queue <std::queue <std::string>> Cliente::GetQueue(){return client_queue;}

void Cliente::SetQueue(std::queue <std::queue <std::string>> q){
    client_queue=q;
}

void Cliente::Push(std::queue<std::string> l_queue){
    client_queue.push(l_queue);  
}

std::queue<std::string> Cliente::Pop(){
    std::queue<std::string> frontelement = client_queue.front();
    client_queue.pop();
    return frontelement;  
}

bool Cliente::Empty(){
    return client_queue.empty();
}



