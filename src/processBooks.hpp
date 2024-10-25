#ifndef PROCESSBOOKS_HPP
#define PROCESSBOOKS_HPP

#define BOOKSPATH "datasets/books/doc"
#define STOPWORDSPATH "datasets/stopWords.txt"

#include <bits/stdc++.h>
#include <cstring>
#include <locale>
#include <codecvt>
#include <algorithm>

#include "tfIdf.hpp"

using namespace std;

class ProcessBooks
{
public:
    unordered_set<string> stopWords;

    vector<unordered_map<string, int>> wordsInDocument;
    
    vector<float> documentRate;

    int booksQuantity;

    ProcessBooks(int booksQuantity);

    vector<string> processLine(string line);
    vector<unordered_map<string, int>> processWords();
    unordered_map<string, int> processBook(string path);
    unordered_set<string> processStopWords();
    vector<string> splitString(string str);
};

#endif