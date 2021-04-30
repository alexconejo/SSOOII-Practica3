
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
#include "../src/prueba.cpp"
//#include "../include/color.h" 

int main( int argc, char ** argv )
{
    std::string h = "hola";
    Prueba object1 (h);

    std::thread t1(&Prueba::imprimir,&object1);
    t1.join();

    return 0;
}
