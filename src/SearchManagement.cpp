/***********************************************************
 * Project         : Practica 3 de Sistemas Operativos II
 * 
 * Program Name    : SearchManagement.cpp
 * 
 * Author          : Álex Conejo y César Braojos
 * 
 * Purpose         : Método principal donde se realizan las 
 *                   g_request de los clientes y se administra
 *                   el servidor y el sistema de pago con un
 *                   banco. 
 * 
 * *********************************************************/

#include <iostream>
#include <thread>
#include <vector>
#include <chrono>
#include <algorithm>
#include <functional>
#include <ctime>
#include <atomic>
#include <mutex>
#include <queue>
#include <condition_variable>
#pragma once
#include "../src/ProtectedQueue.cpp" //Incluimos la cola protegida
#include "../src/SSOOIIGLE.cpp"     //Incluimos la clase busqueda
#include "../src/SemCounter2.cpp"   //Incluimos la clase para los semaoforos contadores
#include "../include/colour.h"       //Incluimos la libreria de colores

#define NUMCLIENTSPL 50
#define N 4

//CREAMOS LAS VARIABLES DE CONDICION//
std::condition_variable g_cv_bank;
std::condition_variable &p_cv_bank = g_cv_bank;
std::condition_variable g_cv_server;

//Creamos los semaforos//
std::mutex g_bank_mutex;
std::mutex g_client_mutex;
std::mutex g_print_mutex;
std::mutex g_pay_system_mutex;
std::mutex g_search_mutex;
SemCounter Sem(3);
std::mutex &p_pay_system_mutex = g_pay_system_mutex;
std::mutex &p_search_mutex = g_search_mutex;

//Creamos las colas protegidas (colas en las cuales se añade con semaforos) //
ProtectedQueue g_bank_requests;
ProtectedQueue &p_bank_request = g_bank_requests;
ProtectedQueue g_premium_client;
ProtectedQueue g_free_client;
ProtectedQueue g_request;
ProtectedQueue g_print_request;

//Inicializamos los metodos utilizados//
void Print(double time, Client cl);
void PaySystem();
void ClientRequest();
void SearchServer();

/******************

Metodo principal. Creamos los clientes y lanzamos los servidores. 

******************/
int main()
{
    std::vector<std::thread> vthreads;
    std::vector<std::thread> v_search_threads;
    int                      random;

    for (int i = 0; i < NUMCLIENTSPL; i++)
    {
        random = rand() % 3;
        std::queue<std::queue<std::string>> client_queue;
        Client c(i, "", 0, client_queue);
        switch (random)
        {
            case 0:
                c.SetCategory("NP");
                c.SetCredits(10);
                g_free_client.Push(c);
                break;

            case 1:
                c.SetCategory("PL");
                c.SetCredits(10);
                g_premium_client.Push(c);
                break;

            case 2:
                c.SetCategory("PI");
                c.SetCredits(1);
                g_premium_client.Push(c);
                break;
        }

        vthreads.push_back(std::thread(ClientRequest));
    }

    for (int i = 0; i < 4; i++)
    {
        v_search_threads.push_back(std::thread(SearchServer));
    }

    std::thread bank_thread(PaySystem);
    bank_thread.detach();
    std::for_each(vthreads.begin(), vthreads.end(), std::mem_fn(&std::thread::join));
    std::for_each(v_search_threads.begin(), v_search_threads.end(), std::mem_fn(&std::thread::detach));

    return EXIT_SUCCESS;
}

/******************

Servidor Bancario. Este servidor esta siempre arrancando a la espera de g_request 

******************/
void PaySystem()
{

    p_search_mutex.lock();
    p_pay_system_mutex.lock();

    while (1)
    {
        std::unique_lock<std::mutex> ul(g_bank_mutex);
        p_cv_bank.wait(ul, [] { return !p_bank_request.Empty(); });
        
        try
        {
            p_bank_request.Recharge(1000);
            std::cout << BOLDRED << "[BANCO]" << RESET << " Ha atendido la peticion del " << BOLDBLUE << "cliente " << p_bank_request.Front().GetClientId() << RESET << std::endl;
        }
        catch (std::exception& ex)
        {
            std::cout << BOLDYELLOW << "[EXCEPTION] " << RESET << "Ha saltado la excepcion: " << BOLDYELLOW << ex.what() << RESET << " al recargar el cliente." << std::endl;
        }

        p_search_mutex.unlock();
        p_pay_system_mutex.lock();
        p_bank_request.Pop();
    }
}

/******************

Metodo para organizar e imprimir la informacion de los clientes. Organizamos segun su categoria e imprimimos una vez acaba la busqueda 

******************/
void ClientRequest()
{
    int                                 random;
    std::queue<std::queue<std::string>> client_queue;
    random = rand() % 10;
    Client aux(0, "", 0, client_queue);
    if (!g_premium_client.Empty() && !g_free_client.Empty())
    {

        if (random <= 7)
        {
            aux.SetClientId(g_premium_client.Front().GetClientId());
            aux.SetCategory(g_premium_client.Front().GetCategory());
            aux.SetCredits(g_premium_client.Front().GetCredits());
            g_premium_client.Pop();
            g_request.Push(aux);
        }
        else
        {
            aux.SetClientId(g_free_client.Front().GetClientId());
            aux.SetCategory(g_free_client.Front().GetCategory());
            aux.SetCredits(g_free_client.Front().GetCredits());
            g_free_client.Pop();
            g_request.Push(aux);
        }
    }
    else if (g_premium_client.Empty() && !g_free_client.Empty())
    {
        aux.SetClientId(g_free_client.Front().GetClientId());
        aux.SetCategory(g_free_client.Front().GetCategory());
        aux.SetCredits(g_free_client.Front().GetCredits());
        g_free_client.Pop();
        g_request.Push(aux);
    }
    else
    {
        aux.SetClientId(g_premium_client.Front().GetClientId());
        aux.SetCategory(g_premium_client.Front().GetCategory());
        aux.SetCredits(g_premium_client.Front().GetCredits());

        g_request.Push(aux);

        g_premium_client.Pop();
    }
    unsigned t0= clock();
    while (g_print_request.Empty()){
       
    }
    std::this_thread::sleep_for(std::chrono::milliseconds(3000));
    try
    {
        while ( g_print_request.Front().GetClientId() != aux.GetClientId()){}
    }
    catch (std::exception& ex)
    {
        std::cout << BOLDYELLOW << "[EXCEPTION] " << RESET << "Ha saltado la excepcion: " << BOLDYELLOW << ex.what() << RESET << " al comparar la cabecera de la cola g_print_request con el cliente" << std::endl;
    }
    

    g_print_mutex.lock();
    try
    {
        unsigned t1= clock();
        double time = (double(t1-t0)/CLOCKS_PER_SEC);
        Print(time,g_print_request.Front());
    }
    catch (std::exception& ex)
    {
        std::cout << BOLDYELLOW << "[EXCEPTION] " << RESET << "Ha saltado la excepcion: " << BOLDYELLOW << ex.what() << RESET << " al imprimir." << std::endl;
    }
    
    g_print_mutex.unlock();
    g_print_request.Pop();
}

/******************

Servidor de busqueda. Lanzamos 4 hilos con este metodo que estan siempre activos a la espera de peticiones de busqueda. 

******************/
void SearchServer()
{
    
    std::mutex                   server_mutex;
    std::unique_lock<std::mutex> ul_server(server_mutex);
    std::vector<std::string>     v_random_words;
    int                          random;

    //Añadimos las palabras que se pueden encontrar aleatoriamente//
    v_random_words.push_back("esta");
    v_random_words.push_back("maestro");
    v_random_words.push_back("moral");
    v_random_words.push_back("líder");

    while (1)
    {
        g_cv_server.wait(ul_server, [] { return !g_request.Empty(); });
        random = rand() % 4;
        Sem.wait();
        std::cout << BOLDGREEN << "[SERVER] " << RESET << "Ha atendido la peticion de " << BOLDBLUE << "cliente " << g_request.Front().GetClientId() << RESET << std::endl;
        SSOOIIGLE SSOOIIGLE(g_request.Front(), v_random_words[random], p_cv_bank, p_bank_request, p_search_mutex, p_pay_system_mutex);
        g_request.Pop();
        Sem.signal();
        std::thread busqueda(&SSOOIIGLE::Search, &SSOOIIGLE);
        busqueda.join();
        Client cl(SSOOIIGLE.GetClient().GetClientId(), SSOOIIGLE.GetClient().GetCategory(), SSOOIIGLE.GetClient().GetCredits(), SSOOIIGLE.GetClient().GetQueue());
        g_print_request.Push(cl);
    }
}

/******************
 
Metodo para imprimir la cola de cada objeto cliente
Este metodo imprime la informacion de cada cliente en su fichero /clientes/Cliente_*ID* dejamos la opcion de verlo por pantalla. 

******************/
void Print(double time,Client cl)
{
    std::string filename = "clientes/Cliente_";
    int i = 0;
    std::queue<std::queue<std::string>> aux = cl.GetQueue();

    filename += std::to_string(cl.GetClientId());
    std::ofstream file_client(filename);

    std::cout << BOLDBLUE << "[CLIENTE: " << cl.GetClientId() << "] " << RESET << " Comienza a imprimir" << std::endl;

    file_client << "Cliente: " << cl.GetClientId() << " Tipo: " << cl.GetCategory() << " Tiempo: " << time << " segundos" << std::endl;
    file_client << ""  << std::endl;
    while (!aux.empty())
    {
        std::queue<std::string> aux_list = aux.front();
        std::chrono::milliseconds(10);
        file_client << "libro " << aux_list.front();
        aux_list.pop();
        file_client << " linea " << aux_list.front();
        aux_list.pop();
        file_client << ":: ..." << aux_list.front();
        aux_list.pop();
        file_client << " " << aux_list.front();
        aux_list.pop();
        file_client << " " << aux_list.front() << " ... " << std::endl;
        aux_list.pop();
        aux.pop();
    }
    
    file_client.close();
    std::cout << BOLDBLUE << "[CLIENTE: " << cl.GetClientId() << "] " << RESET << " Ha finalizado" << std::endl;
    /*while(!aux.empty()){        
        std::queue<std::string>aux_list = aux.front();   
        std::chrono::milliseconds(10);
        std::cout << BOLDGREEN << "libro "<<aux_list.front() <<RESET ;
        aux_list.pop();
        std::cout << BOLDBLUE << " linea "<<RESET <<aux_list.front();
        aux_list.pop();
        std::cout << BOLDBLUE << ":: ..." <<RESET<<aux_list.front();
        aux_list.pop();
        std::cout << " "  <<RED <<aux_list.front();
        aux_list.pop();
        std::cout <<" " << RESET<<aux_list.front()<< BOLDBLUE << " ... "<< RESET<< std::endl;
        aux_list.pop();     
        aux.pop();
    }*/
}