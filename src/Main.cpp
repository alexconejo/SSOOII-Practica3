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
#include "../src/SSOOIIGLE.cpp" //Incluimos el cliente gratuito
#include "../include/color.h"

#define NUMCLIENTESPL 50
#define N 4

std::condition_variable cv_banco;
std::condition_variable &p_cv_banco = cv_banco;
std::condition_variable cv_clientes;
std::condition_variable cv_server;
std::mutex semaforo_clientes;
std::mutex semaforo_banco;
std::mutex semaforo_server;
std::mutex semaforo_print;
ColaProtegida peticiones_banco;
ColaProtegida clientes_premium;
ColaProtegida clientes_gratuitos;
ColaProtegida peticiones;
ColaProtegida impresiones;
int g_n=0;


void Print(Cliente cl)
{
    int                                 i       = 0;
    std::queue<std::queue<std::string>> aux     = cl.GetQueue();
   
    std::cout << BOLDGREEN << "Cliente: " <<BOLDBLUE <<cl.GetClientId()<< BOLDGREEN <<" Tipo: "<<BOLDBLUE <<cl.GetCategory() <<RESET<< std::endl;

    while(!aux.empty()){
                
                std::queue<std::string>lista2 = aux.front();   
                std::chrono::milliseconds(10);
                
                std::cout << BOLDBLUE << "linea "<<RESET <<lista2.front();
                lista2.pop();
                std::cout << BOLDBLUE << ":: ..." <<RESET<<lista2.front();
                lista2.pop();
                std::cout << " "  <<RED <<lista2.front();
                lista2.pop();
                std::cout <<" " << RESET<<lista2.front()<< BOLDBLUE << " ... "<< RESET<< std::endl;
                lista2.pop();     
                aux.pop();
            
            }
}

void banco()
{   
    while(1){
        std::cout<<"añadimos dinero al cliente " << impresiones.Front().GetClientId() << " ahora con saldo: " << impresiones.Front().GetCategory() <<std::endl;
        std::unique_lock<std::mutex> ul(semaforo_banco);
        p_cv_banco.wait(ul,[]{return !peticiones_banco.Empty();});
        peticiones_banco.Recharge(1000);
        Cliente pl(peticiones_banco.Front().GetClientId(), peticiones_banco.Front().GetCategory(), peticiones_banco.Front().GetCreditos(), peticiones_banco.Front().GetQueue());
        std::cout<<"añadimos dinero al cliente " << pl.GetClientId() << " ahora con saldo: " << pl.GetCategory() <<std::endl;
        peticiones_banco.Pop();      
    }
}
void Clientes()
{   

    int random;
    std::unique_lock<std::mutex> ul_clientes(semaforo_clientes);
    cv_clientes.wait(ul_clientes,[]{return peticiones.Size()<N;});
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
    
    cv_server.notify_all();

    while(impresiones.Empty()){}
    while(impresiones.Front().GetClientId()!=aux.GetClientId()){}
    std::this_thread::sleep_for(std::chrono::seconds(1));
    //semaforo_print.lock();
    Print(impresiones.Front());
    //semaforo_print.unlock();
    impresiones.Pop();
}
void Busqueda()
{   
    std::unique_lock<std::mutex> ul_server(semaforo_server);
    while(1){ 
        cv_server.wait(ul_server,[]{return !peticiones.Empty();});
        SSOOIIGLE SSOOIIGLE(peticiones.Front() , "esta" , p_cv_banco);        std::thread busqueda(&SSOOIIGLE::Busqueda,&SSOOIIGLE);  
        busqueda.join();
        Cliente cl(SSOOIIGLE.GetClient().GetClientId(), SSOOIIGLE.GetClient().GetCategory(), SSOOIIGLE.GetClient().GetCreditos(), SSOOIIGLE.GetClient().GetQueue());
        impresiones.Push(cl);
        peticiones.Pop();
        cv_clientes.notify_one();
    }
}


int main()
{   
    std::vector<std::thread> vhilos;
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

    std::thread b(Busqueda);
    //std::thread m_banco(banco);
    b.detach();
    //m_banco.detach();
    std::for_each(vhilos.begin(), vhilos.end(), std::mem_fn(&std::thread::join));
    std::cout<<"variable de condicion" <<std::endl;
    return EXIT_SUCCESS;
}