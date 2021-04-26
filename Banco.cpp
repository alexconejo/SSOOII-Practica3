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



std::condition_variable v;
std::mutex turno2;
std::mutex semaforo;
std::queue<int> Peticiones_Banco;
int i=0;

void banco(int id,  int i)

{   std::unique_lock<std::mutex> ul(turno2);
    while(1){
    v.wait(ul,[]{return !Peticiones_Banco.empty();});
        std::cout<<"añadimos dinero"<<std::endl;
        Peticiones_Banco.pop();

    }
}
 

int main()
{   
    int j=0;
    std::thread h(banco,i,j);
    Peticiones_Banco.push(j);
    v.notify_one();
    h.join();
    return 0;
}