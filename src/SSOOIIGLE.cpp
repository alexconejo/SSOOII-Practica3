
/******************
* Proyecto:                Práctica 2 de Sistemas Operativos II
* 
* Nombre de la Clase:      SSOOIIGLE.cPP
* 
* Autor:                   César Braojos Corroto
* 
*  
* Proposito:               Busqueda de palabras en un fichero de texto , y mostrar por pantalla con varios datos mas  
*                          
* 
 ******************/

#include <iostream>
#include <thread>
#include <string>
#include <algorithm> //std::for_each
#include <vector>
#include <list>
#include <queue>
#include <fstream> 
#include <mutex>
#include <locale>   //std::tolower
#include <functional>
#include <sstream>  //std::isstringstream 
#include <cctype>   //ispunct
#include <clocale>  //std::setlocale
#include <atomic>
#include <condition_variable>
#pragma once
#include "../src/Main.cpp"
#include "../src/Cliente.cpp" //Incluimos el cliente gratuito
#include "../include/color.h" 


#define LIMITE                  1000000
#define NUM_CARACTER_ERASE      1 //Number of characters erase when find symbols


class SSOOIIGLE {
    private:
        std::mutex g_semaforo;
        Cliente cliente;
        std::string g_palabra;
        std::condition_variable &cv_banco;
        ColaProtegida &peticiones_banco;
        std::mutex &semaforo_busqueda;
        std::mutex &semaforo_sistema_pago;

    public:
        SSOOIIGLE (Cliente cliente, std::string g_palabra, std::condition_variable &cv_banco, ColaProtegida &peticiones_banco, std::mutex &semaforo_busqueda, std::mutex &semaforo_sistema_pago);
        Cliente GetClient();
        std::string Simbols(std::string word);
        std::string changeToLowercaseAndEraseSimbols(std::string word);
        std::int16_t CountLines(char* p_fichero );
        void Print(std::string id,int size);
        void SearchWord(std::string p_palabra ,char* p_fichero);
        void messageWelcome();
        void messageEnd();
        void Busqueda();
};

SSOOIIGLE :: SSOOIIGLE (Cliente c, std::string p, std::condition_variable &b,ColaProtegida &p_pb, std::mutex &p_sb, std::mutex&p_ssp) : cliente(c), g_palabra(p), cv_banco(b), peticiones_banco(p_pb), semaforo_busqueda(p_sb), semaforo_sistema_pago(p_ssp){}

/******************
Metodo para limpiar las palabras de signos de puntuacion delanteros

(¡ ¿ << "")

*******************/
std::string SSOOIIGLE :: Simbols(std::string word){
    if(!isalpha(word[0])){
        
        word.erase(0,2);
        word=word;
        if(!isalpha(word[0])){
        word.erase(0,2);
        word=word;
        }
    }
    return word;
}

Cliente SSOOIIGLE :: GetClient(){
    return cliente;
}
/******************
Metodo para limpiar las palabras de signos de puntuacion traseros

(. , : ; ! ? )

*******************/
std::string SSOOIIGLE :: changeToLowercaseAndEraseSimbols(std::string word){
    std::for_each(word.begin(), word.end(), [](char & c){
        c = ::tolower(c);
    });

    for (unsigned i = 0; i < word.size(); i++) { 
        
        if (ispunct(word[i])){             
            word.erase(i, NUM_CARACTER_ERASE); 
        }
    }

    return word;
}
/******************
Metodo para contar las lineas de un fichero

*******************/
std::int16_t SSOOIIGLE :: CountLines(char* p_fichero ){
    std::ifstream       on ;
    std::string         cadena;

    on.open(p_fichero);
    int linea=0;
    while (!on.eof()) {
        std::string    anterior;
        while (getline(on,cadena))
        {
            linea++;
        }
    }
    return linea;
}


/******************
Metodo para imprimir y ordenar la salida

*******************/


/******************
Metodo que ejecuta cada hilo para buscar la palabra en distintas partes del fichero

*******************/
void SSOOIIGLE :: SearchWord(std::string p_palabra ,char* p_fichero)
{   
    std::vector<std::list<std::string>*>    h_vector;
    std::string                             cadena,palabra;
    std::ifstream                           in ;
    std::string                             anterior;
    char*                                   palabras;
    int                                     linea               =0;
    in.open(p_fichero);
    
        while (!in.eof()) {
            while (getline(in,cadena))
            {
                linea++;
                std::string         palabra_limpia     = changeToLowercaseAndEraseSimbols(cadena);
                std::istringstream  p(palabra_limpia);
                
                while(!p.eof() && cliente.GetCreditos() > 0){
                    std::string     palabra;
                    p >> palabra; 
                    std::string     word= Simbols(palabra);
                    if(word == p_palabra){
                        std::string                 posterior;
                        std::string                 numero_linea         =std::__cxx11::to_string(linea);
                        std::queue<std::string>     cola_hilo;
                        p >> posterior;
                        //Introducimos los datos de la palabra encontrada en una cola 
                            cola_hilo.push(p_fichero);
                            cola_hilo.push(numero_linea);
                            cola_hilo.push(anterior);
                            cola_hilo.push(word);
                            cola_hilo.push(posterior);

                        //Seccion Critica. Introducimos a la cola general la cola con los datos de la palabra
                            g_semaforo.lock();
                            
                            cliente.Push(cola_hilo);
                            
                            g_semaforo.unlock();
                            if(cliente.GetCategory()!="PI"){
                                cliente.SetCreditos(cliente.GetCreditos()-1);
                            }
                            if(cliente.GetCreditos()==0 && cliente.GetCategory()=="PL"){
                                
                                /*std::cout<<"cbanco" <<std::endl;
                                peticiones_banco.Push(cliente);
                                cv_banco.notify_one();
            
                                semaforo_busqueda.lock();
                                cliente.SetCreditos(peticiones_banco.Front().GetCreditos());
                                semaforo_sistema_pago.unlock();*/

                                peticiones_banco.Push(cliente);
                                    cv_banco.notify_one();
                                    semaforo_busqueda.lock();
                                    cliente.SetCreditos(1000);
                                    semaforo_sistema_pago.unlock();
                                
                            }
                            
                        if (posterior==word && cliente.GetCreditos()>0){
                            p >> posterior;
                            anterior        =word;
                            numero_linea     =std::__cxx11::to_string(linea);
                            std::queue<std::string>cola_hilo;
                            //Introducimos los datos de la palabra encontrada en una cola
                                cola_hilo.push(p_fichero);
                                cola_hilo.push(numero_linea);
                                cola_hilo.push(anterior);
                                cola_hilo.push(word);
                                cola_hilo.push(posterior);
                            //Seccion Critica. Introducimos a la cola general la cola con los datos de la palabra
                                g_semaforo.lock();
                                cliente.Push(cola_hilo);
                                g_semaforo.unlock(); 
                                if(cliente.GetCategory()!="PI"){
                                    cliente.SetCreditos(cliente.GetCreditos()-1);
                                }
                                if(cliente.GetCreditos()==0 && cliente.GetCategory()=="PL"){
                                    peticiones_banco.Push(cliente);
                                    cv_banco.notify_one();
                                    semaforo_busqueda.lock();
                                    cliente.SetCreditos(1000);
                                    semaforo_sistema_pago.unlock();
                                    
                                    
                                    
                                }
                        }
                    }
                    anterior=palabra;
                   
                }
    
            }
        }
}

/******************
Metodo principal
*******************/
void SSOOIIGLE :: Busqueda()
{  
   
    
    std::vector<std::thread>        v_hilos;
    std::ifstream                   in ;
    
    //Creacion de hilos , y llamada diviendo el fichero dependiendo de los hilos 
    SearchWord(g_palabra,"utils/books/21_leyes_del_liderazgo.txt");
    SearchWord(g_palabra,"utils/books/17_leyes_del_trabajo_en_equipo.txt");
    SearchWord(g_palabra,"utils/books/Actitud_de_vencedor.txt");
    SearchWord(g_palabra,"utils/books/Vive_tu_sueño.txt");
    //std::cout<<"cliente " <<cliente.GetClientId() << "tiene tamanio cola "<< cliente.GetQueue().size() <<std::endl;
}