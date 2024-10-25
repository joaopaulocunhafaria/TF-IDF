## OBJETIVOS

O trabalho consiste na implementação de um sistema de ranqueamento de documentos utilizando o algoritmo TF/IDF (Term Frequency-Inverse Document Frequency). O objetivo principal é classificar um conjunto de documentos de acordo com a relevância de cada um em relação a frases de pesquisa específicas, utilizando a métrica TF/IDF para calcular essa relevância.

### Algoritmo TF/IDF

O TF/IDF é uma técnica amplamente utilizada em recuperação de informações e mineração de texto. Ele combina duas medidas:

- **TF (Term Frequency)**: Mede a frequência de um termo em um documento, indicando a importância do termo dentro daquele documento específico.
- **IDF (Inverse Document Frequency)**: Avalia a importância de um termo em relação a todo o conjunto de documentos, penalizando termos que aparecem em muitos documentos, pois eles tendem a ser menos informativos.

### Objetivo do Trabalho

1. Ler e processar documentos de texto.
2. Calcular a relevância de cada documento em relação a uma lista de frases de pesquisa, utilizando a métrica TF/IDF.
3. Classificar os documentos de acordo com essa relevância, utilizando métodos de ordenação eficientes.

### Tipo de Entrada

O sistema receberá como entrada:

1. Um conjunto de arquivos de texto contendo documentos aleatórios.
2. Uma lista de frases de pesquisa, onde cada frase será comparada aos documentos para determinar a relevância de cada um.

## Estruturas de Dados

Para modelar o trabalho em questão, de forma a obter a melhor performance possível, foram utilizadas diversas estruturas de dados abordadas na disciplina de Algoritmos e Estruturas de Dados I. Foram empregadas, em sua maioria, estruturas de dados complexas ou de segunda ordem, ou seja, diferentes das estruturas primárias. Em cada etapa da implementação, foram usadas estruturas que melhor modelassem o problema a ser resolvido, e, em diferentes etapas, estruturas semelhantes foram usadas para resolver problemas distintos. Sendo assim, para cada etapa da solução, serão apresentadas as estruturas empregadas, exemplificando da melhor maneira possível a solução implementada.

## Leitura de Palavras

Como a entrada deste trabalho, tanto as **keywords** quanto os documentos a serem ranqueados, estão em formato de arquivos `.txt`, é necessário ser capaz de acessar cada palavra de maneira eficiente. Para isso, todas as palavras a serem pesquisadas (**keywords**), após serem lidas do arquivo, foram armazenadas em um **unordered_set**, permitindo acesso rápido e eficiente. Isso porque o **unordered_set** oferece busca em tempo constante, O(1), em comparação ao **vector**, cuja busca tem custo linear, O(n). Esse ganho de performance é essencial quando lidamos com grandes volumes de dados.

## Contagem de Palavras

Para abordar corretamente o problema apresentado, primeiramente é necessário realizar a normalização dos textos presentes em cada documento. Para isso, é preciso eliminar todas as palavras que geralmente não alteram o sentido do texto. Essas palavras são as chamadas "stop words", como: "a", "as", "que", "da", "de", entre outras. Durante esse processo, é necessário contar as palavras relevantes em cada documento. Para cada documento, foi utilizada uma estrutura do tipo **Hash Table**, ou **unordered_map** em C++, para auxiliar na contagem. Como as **Hash Tables** possuem custo de acesso em tempo constante, O(1), elas são uma excelente escolha para atribuir a um índice específico o número de ocorrências de cada palavra.

Durante a iteração sobre as palavras no documento, cada uma é alocada na tabela. Sempre que uma palavra se repete, seu índice é acessado e o valor correspondente é incrementado. Dessa forma, após percorrer todo o documento, temos todas as palavras contadas e armazenadas na tabela, junto com a quantidade de vezes que se repetem.

Essa abordagem resolve o problema para um único documento, mas é necessário expandir a solução para processar vários documentos. Assim, a estrutura final para realizar essa tarefa foi um array, **vector** em C++, de tabelas hash, onde cada posição do array representa as palavras processadas em um documento. O resultado final é uma estrutura que permite acessar todas essas informações de maneira eficiente.

<a href="https://github.com/joaopaulocunhafaria/Faculdade/blob/dd1c6e58b73d0b8b527b2f6da8990a175572fc0b/AEDS%20II/TF-IDF/src/processBooks.hpp#L22"> Acesse a declaração no código </a>

 

## Cálculo TF (Term Frequency)

Cada palavra a ser ranqueada usando o algoritmo TF-IDF possui um valor de **Term Frequency** (TF) para cada documento. Portanto, é necessário que exista uma estrutura que armazene, para cada palavra, seus valores de **Term Frequency** em cada documento. Para modelar essa etapa, novamente foram utilizadas tabelas hash. Porém, nesta etapa, ao invés de armazenar a contagem de ocorrências de uma palavra, armazenamos um array que contém os valores de **Term Frequency** para cada documento.

O resultado final é uma tabela hash onde a chave é a palavra, e o valor é um array. Cada posição do array contém o valor de **Term Frequency** para essa palavra em um documento específico. Os documentos seguem a ordem dos índices do array, ou seja, a primeira posição do array corresponde ao documento 1.

<a href="https://github.com/joaopaulocunhafaria/Faculdade/blob/dd1c6e58b73d0b8b527b2f6da8990a175572fc0b/AEDS%20II/TF-IDF/src/tfIdf.hpp#L15"> Acesse a declaração no código </a>
 

## Cálculo IDF (Inverse Document Frequency)

O cálculo do **Inverse Document Frequency** (IDF) é mais simples, pois cada palavra possui apenas um valor de IDF. Isso permite uma modelagem mais direta. Inicialmente, considerou-se a possibilidade de usar estruturas do tipo **Tuple** para essa etapa. No entanto, como será necessário acessar os valores de IDF posteriormente, **Tuples** não são a melhor opção, pois a busca dentro de um array de **Tuples** tem custo O(n). Por outro lado, o uso de tabelas hash oferece tempo de acesso constante, O(1).

Assim, os valores resultantes dos cálculos de **Inverse Document Frequency** para cada palavra foram armazenados em uma tabela hash, onde as chaves são as palavras e os valores são os resultados dos cálculos de IDF. Essa abordagem permite acesso rápido e eficiente a essas informações.

<a href="https://github.com/joaopaulocunhafaria/Faculdade/blob/dd1c6e58b73d0b8b527b2f6da8990a175572fc0b/AEDS%20II/TF-IDF/src/tfIdf.hpp#L18"> Acesse a declaração no código </a>
  
## Cálculo de Relevância TF/IDF

Finalmente, para termos acesso ao ranking que cada palavra possui em cada documento, é necessário realizar o cálculo proposto pelo algoritmo TF/IDF. Esse cálculo se baseia em multiplicar o TF **(Term Frequency)** de uma palavra em relação a determinado documento pelo seu IDF  **(Inverse Document Frequency)**  em relação à totalidade dos documentos.

Para armazenar essas informações, utilizou-se uma estrutura semelhante à empregada para armazenar apenas o TF  de cada palavra, uma vez que cada palavra terá um array contendo seu ranking para cada documento, semelhante ao valor de TF. Portanto, foi utilizada novamente uma tabela hash, contendo cada keyword como chave e um array com os resultados dos cálculos TF/IDF como valor.

Foi utilizado um vector para armazenar os valores, pois, apesar de ter um tempo de acesso pior quando comparado a outras opções possíveis, seu uso permite que os índices representem diretamente o ranking de cada documento. Dessa forma, de maneira semelhante à estrutura usada para armazenar os valores de TF, cada índice do array representa o ranking do respectivo documento. Ou seja, o primeiro valor do array corresponde ao ranking do documento 1, e assim sucessivamente.


<a href="https://github.com/joaopaulocunhafaria/Faculdade/blob/dd1c6e58b73d0b8b527b2f6da8990a175572fc0b/AEDS%20II/TF-IDF/src/tfIdf.hpp#L22"> Acesse a declaração no código </a>



## COMPILAÇÃO E EXECUÇÃO

| Comando     | Função                                                                 |
| ----------- | ---------------------------------------------------------------------- |
| make clean  | Apaga a última compilação realizada contida na pasta build              |
| make        | Executa a compilação do programa utilizando o gcc, e o resultado vai para a pasta build |
| make run    | Executa o programa da pasta build após a realização da compilação       |
