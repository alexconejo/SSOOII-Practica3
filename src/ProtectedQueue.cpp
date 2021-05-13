/***********************************************************
 * Project         : Practica 3 de Sistemas Operativos II
 * 
 * Program Name    : ProtectedQueue.cpp
 * 
 * Author          : Álex Conejo y César Braojos
 * 
 * Purpose         : Objeto para la cola protegida. Tendrá
 *                   un método para introducir o sacar datos
 *                   de la cola. 
 * *********************************************************/
#pragma once
#include "../include/ProtectedQueue.h"
#include <string>
#include <queue>
#include <signal.h>

// Introduce datos a la cola. //
void ProtectedQueue::Push(Client pl){ 
    std::unique_lock<std::mutex> ul (mutex_queue);
    protected_queue.push(pl);
    ul.unlock();
}


// Saca datos de la cola. //
void ProtectedQueue::Pop() { 
    std::unique_lock<std::mutex> ul (mutex_queue);
    protected_queue.pop();
    ul.unlock();
}

// Devuelve el cliente que está encabezando la cola. //
Client ProtectedQueue::Front() {
    std::unique_lock<std::mutex> ul (mutex_queue);
    Client pl(protected_queue.front().GetClientId(), protected_queue.front().GetCategory(), protected_queue.front().GetCredits(), protected_queue.front().GetQueue());
    ul.unlock();
    return pl;
}

// Recarga una cantidad de creditos al cliente que encabeza la cola. //
void ProtectedQueue::Recharge(int credits) {
    std::unique_lock<std::mutex> ul (mutex_queue);
    protected_queue.front().SetCredits(credits);
    ul.unlock();
}

// Comprueba si la cola está vacia. //
bool ProtectedQueue::Empty() {
    bool empty;
    std::unique_lock<std::mutex> ul (mutex_queue);
    empty = protected_queue.empty();
    ul.unlock();
    return empty;
}

// Devuelve el tamaño de la cola. //
int ProtectedQueue::Size(){
    int size;
    std::unique_lock<std::mutex> ul (mutex_queue);
    size = protected_queue.size();
    ul.unlock();
    return size;
}