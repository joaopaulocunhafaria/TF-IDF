#include "tfIdf.hpp"

TfIdf::TfIdf(vector<unordered_map<string, int>> wordsInDocs)
{
    this->keyWords = processKeyWords();
    this->idfRank = idf(wordsInDocs);
    this->tfRank = tf(wordsInDocs);
    this->wordsScore = calculateTfIdf();
    this->lineScore = calculateScore();

    showScore();
}

unordered_map<string, double> TfIdf::idf(vector<unordered_map<string, int>> wordsInDocs)
{
    unordered_map<string, double> result;

    int totalNumberOfDocuments = wordsInDocs.size();
    for (auto line : keyWords)
    {
        for (auto key : line)
        {
            int numberOfDocsKeyAppers;
            for (int i = 0; i < totalNumberOfDocuments; i++)
            {
                if (wordsInDocs[i].find(key) != wordsInDocs[i].end())
                {
                    numberOfDocsKeyAppers++;
                }
            }

            result[key] = log(static_cast<double>(totalNumberOfDocuments) / 1 + static_cast<double>(numberOfDocsKeyAppers));
        }
    }

    return result;
}

unordered_map<string, vector<double>> TfIdf::tf(vector<unordered_map<string, int>> wordsInDocs)
{
    unordered_map<string, vector<double>> tfPerDoc;
    int docsQuantity = wordsInDocs.size();

    for (auto line : keyWords)
    {
        for (auto key : line)
        {
            int howManyApperanceInEachDocument;
            int totalTermsInDoc;
            for (int i = 0; i < docsQuantity; i++)
            {
                totalTermsInDoc = wordsInDocs[i].size();
                howManyApperanceInEachDocument = wordsInDocs[i][key];
                double result = static_cast<double>(howManyApperanceInEachDocument) / static_cast<double>(totalTermsInDoc);
                tfPerDoc[key].push_back(result);
            }
        }
    }

    return tfPerDoc;
}

vector<vector<string>> TfIdf::processKeyWords()
{

    vector<vector<string>> result;

    ifstream file(KEYWORDSPATH);

    string line;

    while (getline(file, line))
    {
        vector<string> lineKeyWord = processLine(line);
        result.push_back(lineKeyWord);
    }
    file.close();

    return result;
}

vector<string> TfIdf::processLine(string line)
{

    vector<string> result;

    if (!line.empty())
    {

        vector<string> lineWords = splitString(line);
        for (size_t i = 0; i < lineWords.size(); i++)
        {
            string word = lineWords.at(i);

            transform(word.begin(), word.end(), word.begin(), [](unsigned char c)
                      {
                          return std::tolower(c); // Converte cada caractere para minúscula
                      });

            // tirar os acentos

            word.erase(remove_if(word.begin(), word.end(),
                                 [](unsigned char c)
                                 { return c == '.' || c == ',' || c == '!' || c == '?' || c == ':' || c == ';' || c == '"' || c == '\'' || c == '(' || c == ')' || c == '[' || c == ']' || c == '{' || c == '}' || c == '-' || c == '_'; }),
                       word.end());

            result.push_back(word);
        }
    }

    return result;
}

unordered_map<string, vector<double>> TfIdf::calculateTfIdf()
{

    unordered_map<string, vector<double>> score;

    for (size_t i = 0; i < keyWords.size(); i++)
    {
        vector<string> line = keyWords.at(i);
        for (size_t j = 0; j < line.size(); j++)
        {
            string key = line[j];

            vector<double> resultPerKey;

            for (size_t k = 0; k < tfRank[key].size(); k++)
            {
                resultPerKey.push_back(tfRank[key][k] * idfRank[key]);
            }

            score[key] = resultPerKey;
        }
    }

    return score;
}

void TfIdf::showScore()
{
    for (size_t i = 0; i < lineScore.size(); i++)
    {
        vector<double> score = lineScore.at(i);

        int sortedIndex = findMaxIndex(score);
        cout << "A frase " << i + 1  << " tem mais relevancia para o documento " << sortedIndex + 1 << endl;
    }
}

int TfIdf::partition(vector<double> &arr, std::vector<int> &indices, int low, int high)
{
    double pivot = arr[indices[high]];
    int i = (low - 1);

    for (int j = low; j < high; j++)
    {
        if (arr[indices[j]] <= pivot)
        {
            i++;
            std::swap(indices[i], indices[j]);
        }
    }
    std::swap(indices[i + 1], indices[high]);
    return (i + 1);
}

void TfIdf::quickSort(vector<double> &arr, vector<int> &indices, int low, int high)
{
    if (low < high)
    {
        int pi = partition(arr, indices, low, high);

        quickSort(arr, indices, low, pi - 1);
        quickSort(arr, indices, pi + 1, high);
    }
}

vector<int> TfIdf::sortByIndex(vector<double> &arr)
{
    vector<int> indices(arr.size());
    for (size_t i = 0; i < arr.size(); i++)
    {
        indices[i] = i;
    }

    cout << "Sorted array" << endl;
    quickSort(arr, indices, 0, arr.size() - 1);
    for (size_t i = 0; i < arr.size(); i++)
    {
        cout << arr[i] << ", ";
    }

    cout << endl;
    return indices;
}

int TfIdf::findMaxIndex(const std::vector<double> &arr)
{
    if (arr.empty())
    {
        return -1;
    }

    int indice_maior = 0;

    for (size_t i = 1; i < arr.size(); i++)
    {
        if (arr[i] > arr[indice_maior])
        {
            indice_maior = i;
        }
    }

    return indice_maior;
}

vector<string> TfIdf::splitString(string str)
{
    vector<string> result;
    stringstream ss(str);
    string token;
    char delimiter = ' ';

    while (std::getline(ss, token, delimiter))
    {
        result.push_back(token);
    }

    return result;
}

vector<vector<double>> TfIdf::calculateScore()
{
    vector<vector<double>> result;

    for (auto line : keyWords)
    {
        vector<double> score(line.size(), 0.0);
        for (auto key : line)
        {
            score = sumVector(score, wordsScore[key]);
        }

        result.push_back(score);
    }

    return result;
}

vector<double> TfIdf::sumVector(vector<double> vec, vector<double> sum)
{
    vector<double> result(sum.size(), 0.0);
    for (size_t i = 0; i < sum.size(); ++i)
    {
        result[i] = vec[i] + sum[i];
    }
    return result;
}