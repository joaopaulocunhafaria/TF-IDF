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
    vector<string> stopWords;

    vector<vector<PalavraContagem>> wordsInDocument;

    vector<float> documentRate;

    int booksQuantity;

    ProcessBooks(int booksQuantity);

    vector<string> processLine(string line);
    vector<vector<PalavraContagem>> processWords();
    unordered_map<string, int> processBook(string path);
    vector<string> processStopWords();
    vector<string> splitString(string str);
    int findIndex(const std::vector<PalavraContagem> &palavras, const std::string &palavra);

    void run();
};

#endif