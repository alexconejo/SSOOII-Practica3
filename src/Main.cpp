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
#include "../src/ClientePI.cpp" //Incluimos el cliente premium con saldo ilimitado
#include "../src/ClienteNP.cpp" //Incluimos el cliente gratuito

#define NUMCLIENTESPL 10
#define N 4

std::condition_variable cv;
std::condition_variable cv2;
std::mutex clientes;
std::mutex semaforo;
std::mutex replicas;
std::queue<ClientePL> Peticiones_Banco;
ColaProtegida protected_queue;
ColaProtegida clientes_premium;
ColaProtegida clientes_gratuitos;
int g_n=0;

void banco()
{   
    while(1){
        std::unique_lock<std::mutex> ul(semaforo);
        cv.wait(ul,[]{return !protected_queue.Empty();});
        std::this_thread::sleep_for(std::chrono::seconds(1));
        protected_queue.Recharge(1000);
        Cliente pl (protected_queue.Front().GetClientId(), protected_queue.Front().GetCategory());
        std::cout<<"añadimos dinero al cliente " << pl.GetClientId() << " ahora con saldo: " << pl.GetCategory() <<std::endl;
        protected_queue.Pop();        
    }
}
void Clientes()
{   
    int random;
    std::unique_lock<std::mutex> ul(clientes);
    cv2.wait(ul,[]{return g_n<N;});
    random = rand() % 10;
    if(!clientes_premium.Empty()&& !clientes_gratuitos.Empty()){
        if(random<=7){
            std::cout<<"busqueda de  la cola premium "<<std::endl;
            clientes_premium.Pop();
        }
        else{
            std::cout<<"busqueda de  la cola gratis"<<std::endl;
            clientes_gratuitos.Pop();
    }
    }else if(clientes_premium.Empty()){
        std::cout<<"busqueda de  la cola gratis con premium vacia"<<std::endl;
        clientes_gratuitos.Pop();

    }else {
    std::cout<<"busqueda de  la cola premium con gratis vacia"<<std::endl;
        clientes_premium.Pop();
    }
    
}
 

int main()
{   
    std::vector<std::thread> vhilos;
    int random;

    for (int i= 0; i<NUMCLIENTESPL;i++){
        random = rand() % 3;
        Cliente c (i, "");
        switch (random){
            case 0:
                c.SetCategory("NP");
                clientes_gratuitos.Push(c);
                break;

            case 1:                
                c.SetCategory("PL");
                clientes_premium.Push(c);
                break;

            case 2:
                c.SetCategory("PI");
                clientes_premium.Push(c);
                break;
        }
        
        vhilos.push_back(std::thread(Clientes));
    }

    std::for_each(vhilos.begin(), vhilos.end(), std::mem_fn(&std::thread::join));


    std::this_thread::sleep_for(std::chrono::seconds(10));


    
    return 0;
}