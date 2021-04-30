/******************************************************
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
 ******************************************************/

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

//#include "../include/color.h" 



class Prueba {
    private:
        std::string g_palabra;
        int i;

    public:
        Prueba (std::string g_palabra);
        void imprimir();
};

Prueba::Prueba (std::string g_palabra){
    this-> g_palabra = g_palabra;
}

void Prueba:: imprimir(){
    std::cout << "Imprimime con este id " << i << " la palabra: " << g_palabra << std::endl;
}

