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
#include "../src/SearchManagement.cpp"
#include "../src/Client.cpp" //Incluimos el cliente gratuito
#include "../include/colour.h"

#define LIMITE 1000000
#define NUM_CARACTER_ERASE 1 //Number of characters erase when find symbols

class SSOOIIGLE
{
private:
    std::mutex              queue_mutex;
    Client                  client;
    std::string             search_word;
    std::condition_variable &cv_bank;
    ProtectedQueue          &bank_request;
    std::mutex              &search_mutex;
    std::mutex              &pay_system_mutex;

public:
    SSOOIIGLE(Client client, std::string search_word, std::condition_variable &cv_bank, ProtectedQueue &bank_request, std::mutex &search_mutex, std::mutex &pay_system_mutex);
    Client GetClient();
    std::string Simbols(std::string word);
    std::string changeToLowercaseAndEraseSimbols(std::string word);
    std::int16_t CountLines(char *p_file);
    void SearchWord(std::string p_search_word, char *p_file);
    void Search();
};

/******************
 
Constructor de la clase SSOOIIGLE

*******************/
SSOOIIGLE ::SSOOIIGLE(Client c, std::string p, std::condition_variable &b, ProtectedQueue &p_pb, std::mutex &p_sb, std::mutex &p_ssp) : client(c), search_word(p), cv_bank(b), bank_request(p_pb), search_mutex(p_sb), pay_system_mutex(p_ssp) {}

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

Client SSOOIIGLE ::GetClient()
{
    return client;
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
std::int16_t SSOOIIGLE ::CountLines(char *p_file)
{
    std::ifstream on;
    std::string   word_chain;

    on.open(p_file);
    int line = 0;
    while (!on.eof())
    {
        std::string past_word;
        while (getline(on, word_chain))
        {
            line++;
        }
    }
    return line;
}

/*****************
 
Metodo que ejecuta cada hilo para buscar la palabra en distintas partes del fichero

*******************/
void SSOOIIGLE ::SearchWord(std::string p_search_word, char *p_file)
{
    std::vector<std::list<std::string> *> h_vector;
    std::string                           word_chain, aux_word;
    std::ifstream                         in;
    std::string                           past_word;
    int                                   line = 0;

    in.open(p_file);

    while (!in.eof())
    {
        while (getline(in, word_chain))
        {
            line++;
            std::string clean_word = changeToLowercaseAndEraseSimbols(word_chain);
            std::istringstream p(clean_word);

            while (!p.eof() && client.GetCredits() > 0)
            {
                std::string aux_word;
                p >> aux_word;
                std::string word = Simbols(aux_word);
                if (word == p_search_word)
                {
                    std::string next_word;
                    std::string line_number = std::__cxx11::to_string(line);
                    std::queue<std::string> thread_queue;
                    p >> next_word;
                    //Introducimos los datos de la palabra encontrada en una cola
                    thread_queue.push(p_file);
                    thread_queue.push(line_number);
                    thread_queue.push(past_word);
                    thread_queue.push(word);
                    thread_queue.push(next_word);

                    //Seccion Critica. Introducimos a la cola general la cola con los datos de la palabra
                    queue_mutex.lock();

                    client.Push(thread_queue);

                    queue_mutex.unlock();
                    if (client.GetCategory() != "PI")
                    {
                        client.SetCredits(client.GetCredits() - 1);
                    }
                    if (client.GetCredits() == 0 && client.GetCategory() == "PL")
                    {
                        std::cout << BOLDBLUE << "[CLIENTE: " << client.GetClientId() << "]" << RESET << " Necesita creditos" << std::endl;
                        bank_request.Push(client);
                        cv_bank.notify_one();
                        search_mutex.lock();
                        client.SetCredits(bank_request.Front().GetCredits());
                        pay_system_mutex.unlock();
                    }

                    if (next_word == word && client.GetCredits() > 0)
                    {
                        p >> next_word;
                        past_word = word;
                        line_number = std::__cxx11::to_string(line);
                        std::queue<std::string> thread_queue;
                        //Introducimos los datos de la palabra encontrada en una cola
                        thread_queue.push(p_file);
                        thread_queue.push(line_number);
                        thread_queue.push(past_word);
                        thread_queue.push(word);
                        thread_queue.push(next_word);
                        //Seccion Critica. Introducimos a la cola general la cola con los datos de la palabra
                        queue_mutex.lock();
                        client.Push(thread_queue);
                        queue_mutex.unlock();
                        if (client.GetCategory() != "PI")
                        {
                            client.SetCredits(client.GetCredits() - 1);
                        }
                        if (client.GetCredits() == 0 && client.GetCategory() == "PL")
                        {
                            std::cout << BOLDBLUE << "[CLIENTE: " << client.GetClientId() << "]" << RESET << " Necesita creditos" << std::endl;
                            bank_request.Push(client);
                            cv_bank.notify_one();
                            search_mutex.lock();
                            client.SetCredits(bank_request.Front().GetCredits());
                            pay_system_mutex.unlock();
                        }
                    }
                }
                past_word = aux_word;
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