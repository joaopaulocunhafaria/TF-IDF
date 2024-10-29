#include "processBooks.hpp"

ProcessBooks::ProcessBooks(int booksQuantity)
{

    this->booksQuantity = booksQuantity;
};

void ProcessBooks::run()
{

    this->stopWords = processStopWords();

    this->wordsInDocument = processWords();

    TfIdf tfIdf = TfIdf();

    tfIdf.run(wordsInDocument);
}
vector<string> ProcessBooks::processStopWords()
{

    ifstream file(STOPWORDSPATH);

    vector<string> stopWords;
    string line;

    while (getline(file, line))
    {
        stopWords.push_back(line);
    }

    file.close();
    return stopWords;
}

vector<string> ProcessBooks::processLine(string line)
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

            if (find(stopWords.begin(),stopWords.end(),word)  == stopWords.end()&& !word.empty() && word != "")
            {
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
    }

    return result;
}

vector<string> ProcessBooks::splitString(string str)
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

vector<vector<PalavraContagem>> ProcessBooks::processWords()
{

    vector<vector<PalavraContagem>> result;

    for (int i = 0; i < booksQuantity; i++)
    {
        string filePath = BOOKSPATH + to_string(i + 1) + ".txt";

        ifstream file(filePath);
        string line;
        vector<PalavraContagem> wordCounter;
        while (getline(file, line))
        {
            if (!line.empty())
            {
                vector<string> lineWords = processLine(line);

                for (auto word : lineWords)
                {
                    int indice = findIndex(wordCounter, word);
                    if (indice != -1)
                    {
                        // Palavra já existe, incrementa o contador
                        wordCounter[indice].contagem++;
                    }
                    else
                    {
                        // Palavra não existe, adiciona ao vetor com contagem inicial 1
                        wordCounter.push_back({word, 1});
                    }
                }
            }
        }
        result.push_back(wordCounter);
        file.close();
    } 

    return result;
}

int ProcessBooks::findIndex(const std::vector<PalavraContagem>& palavras, const std::string& palavra){
    for (size_t i = 0; i < palavras.size(); ++i) {
        if (palavras[i].palavra == palavra) {
            return i;
        }
    }
    return -1;
}