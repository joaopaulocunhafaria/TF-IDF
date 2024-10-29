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

    void run(vector<unordered_map<string, int>> wordsInDocs);

    TfIdf();

    unordered_map<string, vector<double>> tf(vector<unordered_map<string, int>> wordsInDocs);
    unordered_map<string, double> idf(vector<unordered_map<string, int>> wordsInDocs);
    unordered_map<string, vector<double>> calculateTfIdf();
    vector<vector<double>> calculateScore();

    vector<vector<string>> processKeyWords();
    vector<string> processLine(string line);
    vector<string> splitString(string str);
    void rankScore(vector<double> score);
    vector<double> sumVector(vector<double> vec, vector<double> sum);

    void merge(const vector<double> &arr, vector<int> &indices, int left, int mid, int right);
    void mergeSort(const vector<double> &arr, vector<int> &indices, int left, int right);
    vector<int> sortedIndices(const std::vector<double>& arr);


    void showScore();


};

#endif