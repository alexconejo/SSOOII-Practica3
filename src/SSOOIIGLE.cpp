/***********************************************************
 * Project         : Practica 3 de Sistemas Operativos II
 * 
 * Program Name    : SSOOIIGLE.cpp
 * 
 * Author          : Álex Conejo y César Braojos
 * 
 * Purpose         : Search de palabras para un cliente en
 *                   los distintos libros.
 *  
 * *********************************************************/

#include <iostream>
#include <thread>
#include <string>
#include <algorithm> //std::for_each
#include <vector>
#include <list>
#include <queue>
#include <fstream>
#include <mutex>
#include <locale> //std::tolower
#include <functional>
#include <sstream> //std::isstringstream
#include <cctype>  //ispunct
#include <clocale> //std::setlocale
#include <atomic>
#include <condition_variable>
#pragma once
#include "../src/Main.cpp"
#include "../src/Cliente.cpp" //Incluimos el cliente gratuito
#include "../include/color.h"

#define LIMITE 1000000
#define NUM_CARACTER_ERASE 1 //Number of characters erase when find symbols

class SSOOIIGLE
{
private:
    std::mutex              queue_mutex;
    Cliente                 cliente;
    std::string             search_word;
    std::condition_variable &cv_banc;
    ColaProtegida           &banc_request;
    std::mutex              &search_mutex;
    std::mutex              &pay_system_mutex;

public:
    SSOOIIGLE(Cliente cliente, std::string search_word, std::condition_variable &cv_banc, ColaProtegida &banc_request, std::mutex &search_mutex, std::mutex &pay_system_mutex);
    Cliente GetClient();
    std::string Simbols(std::string word);
    std::string changeToLowercaseAndEraseSimbols(std::string word);
    std::int16_t CountLines(char *p_fichero);
    void SearchWord(std::string p_palabra, char *p_fichero);
    void Search();
};

/******************
 
Constructor de la clase SSOOIIGLE

*******************/
SSOOIIGLE ::SSOOIIGLE(Cliente c, std::string p, std::condition_variable &b, ColaProtegida &p_pb, std::mutex &p_sb, std::mutex &p_ssp) : cliente(c), search_word(p), cv_banc(b), banc_request(p_pb), search_mutex(p_sb), pay_system_mutex(p_ssp) {}

/******************
 
Metodo para limpiar las palabras de signos de puntuacion delanteros
(¡ ¿ << "")

*******************/
std::string SSOOIIGLE ::Simbols(std::string word)
{
    if (!isalpha(word[0]))
    {
        word.erase(0, 2);
        word = word;
        if (!isalpha(word[0]))
        {
            word.erase(0, 2);
            word = word;
        }
    }
    return word;
}
/******************
 
Metodo para obtener la informacion de un cliente

*******************/

Cliente SSOOIIGLE ::GetClient()
{
    return cliente;
}

/******************
 
Metodo para limpiar las palabras de signos de puntuacion traseros
(. , : ; ! ? )

*******************/

std::string SSOOIIGLE ::changeToLowercaseAndEraseSimbols(std::string word)
{
    std::for_each(word.begin(), word.end(), [](char &c) {
        c = ::tolower(c);
    });

    for (unsigned i = 0; i < word.size(); i++)
    {

        if (ispunct(word[i]))
        {
            word.erase(i, NUM_CARACTER_ERASE);
        }
    }

    return word;
}

/******************
 
Metodo para contar las lineas de un fichero

*******************/
std::int16_t SSOOIIGLE ::CountLines(char *p_fichero)
{
    std::ifstream on;
    std::string   cadena;

    on.open(p_fichero);
    int linea = 0;
    while (!on.eof())
    {
        std::string anterior;
        while (getline(on, cadena))
        {
            linea++;
        }
    }
    return linea;
}

/*****************
 
Metodo que ejecuta cada hilo para buscar la palabra en distintas partes del fichero

*******************/
void SSOOIIGLE ::SearchWord(std::string p_palabra, char *p_fichero)
{
    std::vector<std::list<std::string> *> h_vector;
    std::string                           cadena, palabra;
    std::ifstream                         in;
    std::string                           anterior;
    char                                  *palabras;
    int                                   linea = 0;

    in.open(p_fichero);

    while (!in.eof())
    {
        while (getline(in, cadena))
        {
            linea++;
            std::string palabra_limpia = changeToLowercaseAndEraseSimbols(cadena);
            std::istringstream p(palabra_limpia);

            while (!p.eof() && cliente.GetCreditos() > 0)
            {
                std::string palabra;
                p >> palabra;
                std::string word = Simbols(palabra);
                if (word == p_palabra)
                {
                    std::string posterior;
                    std::string numero_linea = std::__cxx11::to_string(linea);
                    std::queue<std::string> cola_hilo;
                    p >> posterior;
                    //Introducimos los datos de la palabra encontrada en una cola
                    cola_hilo.push(p_fichero);
                    cola_hilo.push(numero_linea);
                    cola_hilo.push(anterior);
                    cola_hilo.push(word);
                    cola_hilo.push(posterior);

                    //Seccion Critica. Introducimos a la cola general la cola con los datos de la palabra
                    queue_mutex.lock();

                    cliente.Push(cola_hilo);

                    queue_mutex.unlock();
                    if (cliente.GetCategory() != "PI")
                    {
                        cliente.SetCreditos(cliente.GetCreditos() - 1);
                    }
                    if (cliente.GetCreditos() == 0 && cliente.GetCategory() == "PL")
                    {
                        std::cout << BOLDBLUE << "[CLIENTE: " << cliente.GetClientId() << "]" << RESET << " Necesita creditos" << std::endl;
                        banc_request.Push(cliente);
                        cv_banc.notify_one();
                        search_mutex.lock();
                        cliente.SetCreditos(banc_request.Front().GetCreditos());
                        pay_system_mutex.unlock();
                    }

                    if (posterior == word && cliente.GetCreditos() > 0)
                    {
                        p >> posterior;
                        anterior = word;
                        numero_linea = std::__cxx11::to_string(linea);
                        std::queue<std::string> cola_hilo;
                        //Introducimos los datos de la palabra encontrada en una cola
                        cola_hilo.push(p_fichero);
                        cola_hilo.push(numero_linea);
                        cola_hilo.push(anterior);
                        cola_hilo.push(word);
                        cola_hilo.push(posterior);
                        //Seccion Critica. Introducimos a la cola general la cola con los datos de la palabra
                        queue_mutex.lock();
                        cliente.Push(cola_hilo);
                        queue_mutex.unlock();
                        if (cliente.GetCategory() != "PI")
                        {
                            cliente.SetCreditos(cliente.GetCreditos() - 1);
                        }
                        if (cliente.GetCreditos() == 0 && cliente.GetCategory() == "PL")
                        {
                            std::cout << BOLDBLUE << "[CLIENTE: " << cliente.GetClientId() << "]" << RESET << " Necesita creditos" << std::endl;
                            banc_request.Push(cliente);
                            cv_banc.notify_one();
                            search_mutex.lock();
                            cliente.SetCreditos(banc_request.Front().GetCreditos());
                            pay_system_mutex.unlock();
                        }
                    }
                }
                anterior = palabra;
            }
        }
    }
}

/*****************
 
Metodo principal

*******************/
void SSOOIIGLE ::Search()
{

    std::vector<std::thread> v_threads;
    std::ifstream            in;

    //Creacion de hilos y llamada diviendo el fichero dependiendo de los hilos
    SearchWord(search_word, "utils/books/21_leyes_del_liderazgo.txt");
    SearchWord(search_word, "utils/books/17_leyes_del_trabajo_en_equipo.txt");
    SearchWord(search_word, "utils/books/Actitud_de_vencedor.txt");
    SearchWord(search_word, "utils/books/Vive_tu_sueño.txt");
}