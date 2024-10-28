#include <iostream> 

#include <cstring>
#include "processBooks.hpp"
using namespace std;

#define BOOKS_QUANTITY 6 

int main()
{   

    ProcessBooks p = ProcessBooks(BOOKS_QUANTITY);

    p.run();
    
    return 0;
}