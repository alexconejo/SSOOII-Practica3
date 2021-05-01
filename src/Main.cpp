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
//#include "../src/SSOOIIGLE.cpp" //Incluimos el cliente gratuito

#define NUMCLIENTESPL 10
#define N 4

std::condition_variable cv;
std::condition_variable cv2;
std::mutex clientes;
std::mutex semaforo;
std::mutex replicas;
ColaProtegida peticiones_banco;
ColaProtegida clientes_premium;
ColaProtegida clientes_gratuitos;
ColaProtegida peticiones;
int g_n=0;

void banco()
{   
    while(1){
        std::unique_lock<std::mutex> ul(semaforo);
        cv.wait(ul,[]{return !peticiones_banco.Empty();});
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
    std::unique_lock<std::mutex> ul(clientes);
    cv2.wait(ul,[]{return peticiones.Size()<N;});
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
    while(aux.GetFound()==false){
        std::this_thread::sleep_for(std::chrono::seconds(0));
    }
    
}
/*void Busqueda()
{   
while(1){ 

peticiones.pop();
SSOOIIGLE prueba(peticiones.pop() ,word )
std::thread hilo(SSOOIIGLE, )
    
}
}*/


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

    //std::thead(busqueda, peticiones);
    std::for_each(vhilos.begin(), vhilos.end(), std::mem_fn(&std::thread::join));
    std::this_thread::sleep_for(std::chrono::seconds(10));

return 0;
}