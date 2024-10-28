#include "tfIdf.hpp"

TfIdf::TfIdf()
{
}

void TfIdf::run(vector<unordered_map<string, int>> wordsInDocs)
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

    unordered_set<string> processedKeys;

    for (auto line : keyWords)
    {
        for (auto key : line)
        {

            if (processedKeys.find(key) == processedKeys.end())
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

            processedKeys.insert(key);
        }
    }

    processedKeys.clear();

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

    std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;
    wchar_t comAcentos[] = L"ÄÅÁÂÀÃäáâàãÉÊËÈéêëèÍÎÏÌíîïìÖÓÔÒÕöóôòõÜÚÛüúûùÇç";
    wchar_t semAcentos[] = L"AAAAAAaaaaaEEEEeeeeIIIIiiiiOOOOOoooooUUUuuuuCc";

    if (!line.empty())
    {
        std::wstring text = converter.from_bytes(line);
        for (size_t i = 0; i < wcslen(comAcentos); i++)
        {
            std::replace(text.begin(), text.end(), comAcentos[i], semAcentos[i]);
        }

        line = converter.to_bytes(text);

        vector<string> lineWords = splitString(line);

        for (size_t i = 0; i < lineWords.size(); i++)
        {
            string word = lineWords.at(i);

            if (word.begin() != word.end())
            {

                // Configurar a conversão para UTF-8
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
        vector<int> rank = sortedIndices(score);

        cout << "A frase " << i + 1 << " tem mais relevancia para os seguintes documentos: " << endl;
        for (size_t j = 0; j < rank.size(); j++)
        {
            cout << "Documento " << rank[j] + 1 << endl;
        }
    }
}

// Merges two halves of indices based on the values in `arr`
void TfIdf::merge(const std::vector<double> &arr, std::vector<int> &indices, int left, int mid, int right)
{
    int leftSize = mid - left + 1;
    int rightSize = right - mid;

    std::vector<int> leftIndices(leftSize);
    std::vector<int> rightIndices(rightSize);

    for (int i = 0; i < leftSize; i++)
        leftIndices[i] = indices[left + i];
    for (int j = 0; j < rightSize; j++)
        rightIndices[j] = indices[mid + 1 + j];

    int i = 0, j = 0, k = left;
    while (i < leftSize && j < rightSize)
    {
        if (arr[leftIndices[i]] >= arr[rightIndices[j]])
        {
            indices[k] = leftIndices[i];
            i++;
        }
        else
        {
            indices[k] = rightIndices[j];
            j++;
        }
        k++;
    }

    while (i < leftSize)
    {
        indices[k] = leftIndices[i];
        i++;
        k++;
    }

    while (j < rightSize)
    {
        indices[k] = rightIndices[j];
        j++;
        k++;
    }
}

// Recursive merge sort on indices
void TfIdf::mergeSort(const std::vector<double> &arr, std::vector<int> &indices, int left, int right)
{
    if (left < right)
    {
        int mid = left + (right - left) / 2;
        mergeSort(arr, indices, left, mid);
        mergeSort(arr, indices, mid + 1, right);
        merge(arr, indices, left, mid, right);
    }
}

// Main function to return sorted indices based on vector of doubles
vector<int> TfIdf::sortedIndices(const vector<double> &arr)
{
    int n = arr.size();
    std::vector<int> indices(n);
    for (int i = 0; i < n; i++)
        indices[i] = i;

    mergeSort(arr, indices, 0, n - 1);
    return indices;
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