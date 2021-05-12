/***********************************************************
 * Project         : Practica 3 de Sistemas Operativos II
 * 
 * Program Name    : Main.cpp
 * 
 * Author          : Álex Conejo y César Braojos
 * 
 * Purpose         : Método principal donde se realizan las 
 *                   peticiones de los clientes y se administra
 *                   el servidor y el sistema de pago con un
 *                   banco. 
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
#include "../src/ColaProtegida.cpp" //Incluimos la cola protegida
#include "../src/SSOOIIGLE.cpp"     //Incluimos el cliente gratuito
#include "../src/SemCounter2.cpp"   //Incluimos el cliente gratuito
#include "../include/color.h"

#define NUMCLIENTESPL 50
#define N 4

//CREAMOS LAS VARIABLES DE CONDICION//
std::condition_variable cv_banco;
std::condition_variable &p_cv_banco = cv_banco;
std::condition_variable cv_server;


//CREAMOS LOS SEMAFOROS NECESARIOS//
std::mutex semaforo_banco;
std::mutex semaforo_clientes;
std::mutex semaforo_print;
std::mutex semaforo_sistema_pago;
std::mutex semaforo_busqueda;
SemCounter Sem(3);
std::mutex &p_semaforo_sistema_pago = semaforo_sistema_pago;
std::mutex &p_semaforo_busqueda     = semaforo_busqueda;

//CREAMOS LAS COLAS PROTEGIDAS (colas en las cuales se añade con semaforos)//
ColaProtegida peticiones_banco;
ColaProtegida &p_peticiones_banco = peticiones_banco;
ColaProtegida clientes_premium;
ColaProtegida clientes_gratuitos;
ColaProtegida peticiones;
ColaProtegida impresiones;


/******************
 
Metodo para imprimir la cola de cada objeto cliente

Este metodo imprime en un fichero pero descomentando el "while " lo podriamos ver por pantalla

******************/
void Print(Cliente cl)
{
    std::string                         filename = "clientes/Cliente_";
    int                                 i        = 0;
    std::queue<std::queue<std::string>> aux      = cl.GetQueue();

    filename += std::to_string(cl.GetClientId());
    std::ofstream file_client (filename);

    std::cout << BOLDBLUE << "[CLIENTE: "  <<cl.GetClientId() <<"] "<<RESET <<" Comienza a imprimir" << std::endl;

    file_client <<"Cliente: "<< cl.GetClientId() << " Tipo: " << cl.GetCategory()<<std::endl;

    while(!aux.empty()){
                
                std::queue<std::string>lista2 = aux.front();   
                std::chrono::milliseconds(10);
                file_client <<  "libro "<<lista2.front();
                lista2.pop();
                file_client << " linea " <<lista2.front();
                lista2.pop();
                file_client << ":: ..." <<lista2.front();
                lista2.pop();
                file_client << " "  <<lista2.front();
                lista2.pop();
                file_client<<" " << lista2.front()<< " ... "<<  std::endl;
                lista2.pop();     
                aux.pop();
            
            }
    file_client.close();

    
    /*

    while(!aux.empty()){
                
                std::queue<std::string>lista2 = aux.front();   
                std::chrono::milliseconds(10);
                std::cout << BOLDGREEN << "libro "<<lista2.front() <<RESET ;
                lista2.pop();
                std::cout << BOLDBLUE << " linea "<<RESET <<lista2.front();
                lista2.pop();
                std::cout << BOLDBLUE << ":: ..." <<RESET<<lista2.front();
                lista2.pop();
                std::cout << " "  <<RED <<lista2.front();
                lista2.pop();
                std::cout <<" " << RESET<<lista2.front()<< BOLDBLUE << " ... "<< RESET<< std::endl;
                lista2.pop();     
                aux.pop();
            
            }*/
}

/******************

Servidor Bancario. Este servidor esta siempre arrancando a la espera de peticiones 

******************/
void banco()
{   
    p_semaforo_busqueda.lock();
    p_semaforo_sistema_pago.lock();
    while(1){
        
        std::unique_lock<std::mutex> ul(semaforo_banco);
        p_cv_banco.wait(ul,[]{return !p_peticiones_banco.Empty();}); 
        p_peticiones_banco.Recharge(1000);
        std::cout << BOLDRED << "[BANCO]" << RESET << " Ha atendido la peticion del " << BOLDBLUE << "cliente " << p_peticiones_banco.Front().GetClientId() <<RESET<< std::endl;
        Cliente pl(p_peticiones_banco.Front().GetClientId(), p_peticiones_banco.Front().GetCategory(), p_peticiones_banco.Front().GetCreditos(), p_peticiones_banco.Front().GetQueue());
        p_semaforo_busqueda.unlock();
        p_semaforo_sistema_pago.lock();
        
        p_peticiones_banco.Pop();  
            
    }
}
/******************

Metodo para organizar e imprimir la informacion de los clientes. Organizamos segun su categoria e imprimimos una vez acaba la busqueda 

******************/
void Clientes()
{   
    
    int random;
    random = rand() % 10;


    std::queue <std::queue <std::string>> client_queue;
    Cliente aux(0, "", 0, client_queue);
    if(!clientes_premium.Empty()&& !clientes_gratuitos.Empty()){
    
        if(random<=7)
        {
            aux.SetClientId(clientes_premium.Front().GetClientId());
            aux.SetCategory(clientes_premium.Front().GetCategory());
            aux.SetCreditos(clientes_premium.Front().GetCreditos());
            clientes_premium.Pop();
            peticiones.Push(aux);
        }
        else
        {
            aux.SetClientId(clientes_gratuitos.Front().GetClientId());
            aux.SetCategory(clientes_gratuitos.Front().GetCategory());
            aux.SetCreditos(clientes_gratuitos.Front().GetCreditos());
            clientes_gratuitos.Pop();
            peticiones.Push(aux);
        }
    }
    else if(clientes_premium.Empty()){
        aux.SetClientId(clientes_gratuitos.Front().GetClientId());
        aux.SetCategory(clientes_gratuitos.Front().GetCategory());
        aux.SetCreditos(clientes_gratuitos.Front().GetCreditos());
        clientes_gratuitos.Pop();
        peticiones.Push(aux);
    }
    else 
    {
        aux.SetClientId(clientes_premium.Front().GetClientId());
        aux.SetCategory(clientes_premium.Front().GetCategory());
        aux.SetCreditos(clientes_premium.Front().GetCreditos());
        
        peticiones.Push(aux);

        clientes_premium.Pop();
        
    }
    while(impresiones.Empty()){}
    while(impresiones.Front().GetClientId()!=aux.GetClientId()){}
    
    semaforo_print.lock();
    Print(impresiones.Front());
    semaforo_print.unlock();
    impresiones.Pop();
}
/******************

Servidor de busqueda. Lanzamos 4 hilos con este metodo que estan siempre activos a la espera de peticiones de busqueda. 

******************/
void Busqueda()
{   
    
    std::mutex semaforo_server;
    std::unique_lock<std::mutex> ul_server(semaforo_server);
    std::vector <std::string> v_palabra_aleatoria;
    int random;

    v_palabra_aleatoria.push_back("esta");
    v_palabra_aleatoria.push_back("maestro");
    v_palabra_aleatoria.push_back("moral");
    v_palabra_aleatoria.push_back("líder");
    
    
    while(1){ 
        
        cv_server.wait(ul_server,[]{return !peticiones.Empty();}); 
        Sem.wait();
        std::cout << BOLDGREEN << "[SERVER] " << RESET<< "Ha atendido la peticion de " << BOLDBLUE << "cliente " << peticiones.Front().GetClientId()<< RESET<< std::endl;
        random = rand() % 4;
        SSOOIIGLE SSOOIIGLE(peticiones.Front() , v_palabra_aleatoria[random] , p_cv_banco, p_peticiones_banco, p_semaforo_busqueda, p_semaforo_sistema_pago);
        peticiones.Pop();   
        Sem.signal();
        std::thread busqueda(&SSOOIIGLE::Busqueda,&SSOOIIGLE);  
        busqueda.join();
        Cliente cl(SSOOIIGLE.GetClient().GetClientId(), SSOOIIGLE.GetClient().GetCategory(), SSOOIIGLE.GetClient().GetCreditos(), SSOOIIGLE.GetClient().GetQueue());
        impresiones.Push(cl);

    }
}

/******************

Metodo principal. Creamos los clientes y lanzamos los servidores. 

******************/
int main()
{   
    std::vector<std::thread> vhilos;
    std::vector<std::thread> vhilos_busqueda;
    int random;
   

    for (int i= 0; i<NUMCLIENTESPL;i++){
        random = rand() % 3;
        std::queue <std::queue <std::string>> client_queue;
        Cliente c (i, "",0, client_queue);
        switch (random){
            case 0:
                c.SetCategory("NP");
                c.SetCreditos(10);
                clientes_gratuitos.Push(c);
                
                break;

            case 1:                
                c.SetCategory("PL");
                c.SetCreditos(10);
                clientes_premium.Push(c);
                break;

            case 2:
                c.SetCategory("PI");
                c.SetCreditos(1);
                clientes_premium.Push(c);
                break;
        }
        
        vhilos.push_back(std::thread(Clientes));
    }

    for(int i =0; i<4; i++){
        vhilos_busqueda.push_back(std::thread(Busqueda));
    }
    std::thread m_banco(banco);
    m_banco.detach();
    std::for_each(vhilos.begin(), vhilos.end(), std::mem_fn(&std::thread::join));
    std::for_each(vhilos_busqueda.begin(), vhilos_busqueda.end(), std::mem_fn(&std::thread::detach));
    return EXIT_SUCCESS;
}