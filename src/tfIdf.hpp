#ifndef TFIDF_HPP
#define TFIDF_HPP

#define KEYWORDSPATH "datasets/keyWords.txt"

#include <bits/stdc++.h>

using namespace std;

class TfIdf
{

public:

    unordered_map<string, vector<double>> tfRank;


    unordered_map<string, double> idfRank;

    
    vector<vector<string>> keyWords;
    unordered_map<string, vector<double>> wordsScore;
    vector<vector<double>> lineScore;

    TfIdf(vector<unordered_map<string, int>> wordsInDocs);

    unordered_map<string, vector<double>> tf(vector<unordered_map<string, int>> wordsInDocs);
    unordered_map<string, double> idf(vector<unordered_map<string, int>> wordsInDocs);
    unordered_map<string, vector<double>> calculateTfIdf();
    vector<vector<double>> calculateScore();

    vector<vector<string>> processKeyWords();
    vector<string> processLine(string line);
    vector<string> splitString(string str);
    void rankScore(vector<double> score);
    vector<double> sumVector(vector<double> vec, vector<double> sum);

    int partition(std::vector<double> &arr, std::vector<int> &indices, int low, int high);
    void quickSort(std::vector<double> &arr, std::vector<int> &indices, int low, int high);
    vector<int> sortByIndex(std::vector<double> &arr);
    int findMaxIndex(const std::vector<double> &arr);

    void showScore();
};

#endif