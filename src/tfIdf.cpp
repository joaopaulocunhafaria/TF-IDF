#include "tfIdf.hpp"

TfIdf::TfIdf()
{
}

void TfIdf::run(vector<vector<PalavraContagem>> wordsInDocs)
{

    this->keyWords = processKeyWords();
    this->idfRank = idf(wordsInDocs);
    this->tfRank = tf(wordsInDocs);
    this->wordsScore = calculateTfIdf();
    this->lineScore = calculateScore();

    showScore();
}

vector<IdfWord> TfIdf::idf(vector<vector<PalavraContagem>> wordsInDocs)
{
    vector<IdfWord> result;

    int totalNumberOfDocuments = wordsInDocs.size();
    for (auto line : keyWords)
    {
        for (auto key : line)
        {
            int numberOfDocsKeyAppers;
            for (int i = 0; i < totalNumberOfDocuments; i++)
            {

                if (findIndex(wordsInDocs[i], key) != -1)
                {
                    numberOfDocsKeyAppers++;
                }
            }

            double idf = log(static_cast<double>(totalNumberOfDocuments) / 1 + static_cast<double>(numberOfDocsKeyAppers));
            result.push_back({key, idf});
        }
    }

    return result;
}

vector<TfWord> TfIdf::tf(vector<vector<PalavraContagem>> wordsInDocs)
{
    vector<TfWord> tfPerDoc;
    int docsQuantity = wordsInDocs.size();

    vector<string> processedKeys;

    for (auto line : keyWords)
    {
        for (auto key : line)
        {

            if (find(processedKeys.begin(), processedKeys.end(), key) == processedKeys.end())
            {

                vector<double> rank;
                for (int i = 0; i < docsQuantity; i++)
                {
                    int howManyApperanceInEachDocument = 0;
                    int totalTermsInDoc;

                    totalTermsInDoc = wordsInDocs[i].size();

                    int index = findIndex(wordsInDocs[i], key);
                    if (index != -1)
                    {
                        howManyApperanceInEachDocument = wordsInDocs[i][index].contagem;
                    }
                    double result = static_cast<double>(howManyApperanceInEachDocument) / static_cast<double>(totalTermsInDoc);

                    rank.push_back(result);
                }
                tfPerDoc.push_back({key, rank});
            }

            processedKeys.push_back(key);
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

vector<TfWord> TfIdf::calculateTfIdf()
{

    vector<TfWord> score;

    for (size_t i = 0; i < keyWords.size(); i++)
    {
        vector<string> line = keyWords.at(i);

        for (size_t j = 0; j < line.size(); j++)
        {
            string key = line[j];

            vector<double> resultPerKey;
            int tfIndex = findIndexTfRank(tfRank, key);
            int idfIndex = findIndexIdfRank(idfRank, key);

            for (size_t k = 0; k < tfRank[tfIndex].tfRate.size(); k++)
            {
                resultPerKey.push_back(tfRank[tfIndex].tfRate[k] * idfRank[idfIndex].idfRate);
            }

            score.push_back({key, resultPerKey});
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

        cout << "Rank de relevancia para a frase " << i + 1 << ": " << endl;
        cout << "Documentos:  ";

        for (size_t j = 0; j < rank.size(); j++)
        {
            cout << rank[j] + 1 << ", ";
        }
        cout << endl<< endl;
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
            int tfIdfIndex = findIndexTfRank(wordsScore, key);
            score = sumVector(score, wordsScore[tfIdfIndex].tfRate);
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

int TfIdf::findIndex(const std::vector<PalavraContagem> &palavras, const std::string &palavra)
{
    for (size_t i = 0; i < palavras.size(); ++i)
    {
        if (palavras[i].palavra == palavra)
        {
            return i;
        }
    }
    return -1;
}

int TfIdf::findIndexTfRank(const std::vector<TfWord> &palavras, const std::string &palavra)
{
    for (size_t i = 0; i < palavras.size(); ++i)
    {
        if (palavras[i].word == palavra)
        {
            return i;
        }
    }
    return -1;
}

int TfIdf::findIndexIdfRank(const std::vector<IdfWord> &palavras, const std::string &palavra)
{
    for (size_t i = 0; i < palavras.size(); ++i)
    {
        if (palavras[i].word == palavra)
        {
            return i;
        }
    }
    return -1;
}
