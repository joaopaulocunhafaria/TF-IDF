#ifndef TFIDF_HPP
#define TFIDF_HPP

#define KEYWORDSPATH "datasets/keyWords.txt"

#include <bits/stdc++.h>

using namespace std;

struct IdfWord
{
    std::string word;
    double idfRate;
};

struct TfWord
{
    std::string word;
    vector<double> tfRate;
};

struct PalavraContagem
{
    std::string palavra;
    int contagem;
};

class TfIdf
{

public:
    vector<TfWord> tfRank;

    vector<IdfWord> idfRank;

    vector<vector<string>> keyWords;
    vector<TfWord> wordsScore;
    vector<vector<double>> lineScore;

    void run(vector<vector<PalavraContagem>> wordsInDocs);

    TfIdf();

    vector<TfWord> tf(vector<vector<PalavraContagem>> wordsInDocs);
    vector<IdfWord> idf(vector<vector<PalavraContagem>> wordsInDocs);
    vector<TfWord> calculateTfIdf();
    vector<vector<double>> calculateScore();

    vector<vector<string>> processKeyWords();
    vector<string> processLine(string line);
    vector<string> splitString(string str);
    void rankScore(vector<double> score);
    vector<double> sumVector(vector<double> vec, vector<double> sum);

    void merge(const vector<double> &arr, vector<int> &indices, int left, int mid, int right);
    void mergeSort(const vector<double> &arr, vector<int> &indices, int left, int right);
    vector<int> sortedIndices(const std::vector<double> &arr);

    void showScore();
    int findIndex(const std::vector<PalavraContagem> &palavras, const std::string &palavra);
    int findIndexTfRank(const std::vector<TfWord> &palavras, const std::string &palavra);
    int findIndexIdfRank(const std::vector<IdfWord> &palavras, const std::string &palavra);
};

#endif