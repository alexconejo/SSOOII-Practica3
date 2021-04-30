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


#define NUMCLIENTES 100

std::condition_variable cv;
std::mutex turno2;
std::mutex semaforo;
std::queue<Cliente> Peticiones_Banco;
ColaProtegida protected_queue;
int i=0;

void banco()
{
    while(1){
        std::unique_lock<std::mutex> ul(semaforo);
        cv.wait(ul,[]{return !protected_queue.Empty();});
        std::this_thread::sleep_for(std::chrono::seconds(1));
        protected_queue.Recharge(1000);
        Cliente pl (protected_queue.Front().GetClientId(), protected_queue.Front().GetCategory(), protected_queue.Front().GetSaldo());
        std::cout<<"añadimos dinero al cliente " << pl.GetClientId() << " ahora con saldo: " << pl.GetSaldo() <<std::endl;
        protected_queue.Pop();

    }
}
 

int main()
{

    for (int i= 0; i<NUMCLIENTES;i++){

        Cliente pl (i, "PL", 0);
        protected_queue.Push(pl);
    }
 
    std::thread thread (banco); 


    std::this_thread::sleep_for(std::chrono::seconds(10));

    for (int i= NUMCLIENTES; i<NUMCLIENTES + 4;i++){
        Cliente pl (i, "PL", 0);
        protected_queue.Push(pl);
        cv.notify_one();
    }

    thread.join();
    return 0;
}