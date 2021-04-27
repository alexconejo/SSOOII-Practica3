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
#include "../src/ColaProtegida.cpp"

#define NUMCLIENTESPL 8

std::condition_variable cv;
std::mutex turno2;
std::mutex semaforo;
std::queue<ClientePL> Peticiones_Banco;
int i=0;

void banco()
{
    while(1){
        std::unique_lock<std::mutex> ul(semaforo);
        cv.wait(ul,[]{return !Peticiones_Banco.empty();});
        std::this_thread::sleep_for(std::chrono::seconds(1));
        Peticiones_Banco.front().SetSaldo(1000);
        std::cout<<"añadimos dinero al cliente " << Peticiones_Banco.front().GetClientId() << " ahora con saldo: " << Peticiones_Banco.front().GetSaldo() <<std::endl;
        Peticiones_Banco.pop();
        cv.notify_one();
    }
}
 

int main()
{   
    for (int i= 0; i<NUMCLIENTESPL;i++){
        ClientePL pl;
        pl.SetClientId(i);
        pl.SetCategory("PL");
        pl.SetSaldo(0);
        Peticiones_Banco.push(pl);
    }

    std::thread thread (banco); 

    for (int i= NUMCLIENTESPL; i<NUMCLIENTESPL + 4;i++){
        ClientePL pl;
        pl.SetClientId(i);
        pl.SetCategory("PL");
        pl.SetSaldo(0);
        Peticiones_Banco.push(pl);
    }

    thread.join();
    return 0;
}