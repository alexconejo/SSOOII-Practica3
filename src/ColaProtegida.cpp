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
#pragma once
#include "../include/ColaProtegida.h"
#include <string>
#include <queue>
#include <signal.h>

// Introduce datos a la cola.
void ColaProtegida::Push(Cliente pl){ 
    std::unique_lock<std::mutex> ul (semaforo);
    protected_queue.push(pl);
    ul.unlock();
}


// Saca datos de la cola.
void ColaProtegida::Pop() { 
    std::unique_lock<std::mutex> ul (semaforo);
    protected_queue.pop();
    ul.unlock();
}

Cliente ColaProtegida::Front() {
    std::unique_lock<std::mutex> ul (semaforo);
    Cliente pl(protected_queue.front().GetClientId(), protected_queue.front().GetCategory(), protected_queue.front().GetCreditos());
    ul.unlock();
    return pl;
}

void ColaProtegida::Recharge(int saldo) {
    std::unique_lock<std::mutex> ul (semaforo);
    //protected_queue.front().SetSaldo(saldo);
    ul.unlock();
}

bool ColaProtegida::Empty() {
    bool empty;
    std::unique_lock<std::mutex> ul (semaforo);
    empty = protected_queue.empty();
    ul.unlock();
    return empty;
}

int ColaProtegida::Size(){
    int size;
    std::unique_lock<std::mutex> ul (semaforo);
    size = protected_queue.size();
    ul.unlock();
    return size;
}