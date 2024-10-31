#include <iostream> 
#include <cstring>
#include <chrono>
#include "processBooks.hpp"
using namespace std;

#define BOOKS_QUANTITY 6 

int main()
{   


    auto start = std::chrono::high_resolution_clock::now();

    ProcessBooks p = ProcessBooks(BOOKS_QUANTITY);

    p.run();


    auto end = std::chrono::high_resolution_clock::now();

    std::chrono::duration<double> duration = end - start;

    cout << "Tempo de execucao: " <<  duration.count() << endl; 
    
    return 0;
}