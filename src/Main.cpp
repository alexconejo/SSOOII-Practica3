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
#include "../src/ColaProtegida.cpp" //Incluimos la cola protegida
#include "../src/SSOOIIGLE.cpp" //Incluimos el cliente gratuito

#define NUMCLIENTESPL 10
#define N 4

std::condition_variable cv_banco;
std::condition_variable cv_clientes;
std::condition_variable cv_server;
std::mutex semaforo_clientes;
std::mutex semaforo_banco;
std::mutex semaforo_server;
ColaProtegida peticiones_banco;
ColaProtegida clientes_premium;
ColaProtegida clientes_gratuitos;
ColaProtegida peticiones;
int g_n=0;

void banco()
{   
    while(1){
        std::unique_lock<std::mutex> ul(semaforo_banco);
        cv_banco.wait(ul,[]{return !peticiones_banco.Empty();});
        std::this_thread::sleep_for(std::chrono::seconds(1));
        peticiones_banco.Recharge(1000);
        Cliente pl(peticiones_banco.Front().GetClientId(), peticiones_banco.Front().GetCategory(), peticiones_banco.Front().GetCreditos());
        std::cout<<"añadimos dinero al cliente " << pl.GetClientId() << " ahora con saldo: " << pl.GetCategory() <<std::endl;
        peticiones_banco.Pop();      
    }
}
void Clientes()
{   

    int random;
    std::unique_lock<std::mutex> ul_clientes(semaforo_clientes);
    cv_clientes.wait(ul_clientes,[]{return peticiones.Size()<N;});
    std::cout<<"size: " << peticiones.Size() <<std::endl;
    random = rand() % 10;
    Cliente aux(0, "", 0);
    if(!clientes_premium.Empty()&& !clientes_gratuitos.Empty()){
        if(random<=7)
        {
            std::cout<<"busqueda de  la cola premium "<<std::endl;
            aux.SetClientId(clientes_premium.Front().GetClientId());
            aux.SetCategory(clientes_premium.Front().GetCategory());
            aux.SetCreditos(clientes_premium.Front().GetCreditos());
            clientes_premium.Pop();
            peticiones.Push(aux);
        }
        else
        {
            std::cout<<"busqueda de  la cola gratis"<<std::endl;
            aux.SetClientId(clientes_gratuitos.Front().GetClientId());
            aux.SetCategory(clientes_gratuitos.Front().GetCategory());
            aux.SetCreditos(clientes_gratuitos.Front().GetCreditos());
            clientes_gratuitos.Pop();
            peticiones.Push(aux);
    }
    }
    else if(clientes_premium.Empty()){
        std::cout<<"busqueda de  la cola gratis con premium vacia"<<std::endl;
        aux.SetClientId(clientes_gratuitos.Front().GetClientId());
        aux.SetCategory(clientes_gratuitos.Front().GetCategory());
        aux.SetCreditos(clientes_gratuitos.Front().GetCreditos());
        clientes_gratuitos.Pop();
        peticiones.Push(aux);
    }
    else 
    {
        std::cout<<"busqueda de  la cola premium con gratis vacia"<<std::endl;
        aux.SetClientId(clientes_premium.Front().GetClientId());
        aux.SetCategory(clientes_premium.Front().GetCategory());
        aux.SetCreditos(clientes_premium.Front().GetCreditos());
        clientes_premium.Pop();
        peticiones.Push(aux);
        
    }
    cv_server.notify_all();
    ul_clientes.unlock();
    while(aux.GetFound()==false){};
    
}
void Busqueda()
{   
    std::unique_lock<std::mutex> ul_server(semaforo_server);
    while(1){ 
        cv_server.wait(ul_server,[]{return !peticiones.Empty();});
        SSOOIIGLE SSOOIIGLE(peticiones.Front() , "hola" );
        std::thread busqueda(&SSOOIIGLE::Busqueda,&SSOOIIGLE);  
        busqueda.join();
        peticiones.Front().SetFound(true);
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
        Cliente c (i, "",0);
        switch (random){
            case 0:
                c.SetCategory("NP");
                c.SetCreditos(10);
                clientes_gratuitos.Push(c);
                
                break;

            case 1:                
                c.SetCategory("PL");
                c.SetCreditos(50);
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
    std::for_each(vhilos.begin(), vhilos.end(), std::mem_fn(&std::thread::join));
    b.join();

return 0;
}