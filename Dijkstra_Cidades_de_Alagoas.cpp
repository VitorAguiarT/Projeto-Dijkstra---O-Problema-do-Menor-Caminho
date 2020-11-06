#include <iostream>
#include <sstream>
#include <fstream>

//Número de cidades do estado de Alagoas:
#define numberCities 102

using namespace std;

//Estrutura auxiliar para o print do caminho:
typedef struct auxGraph
{
    string burgh = " ";
    int indice = -1;
} auxGraph;

//Estrutura principal do grafo:
typedef struct Graph
{
    auxGraph auxPrint[numberCities];
    int adjMatrix[numberCities][numberCities] = {0};
} Graph;
//Utilização da abordagem de representação de grafo por martriz de adjacências
//Vetor contendo todas as 102 cidades do estado de Alagoas com seus nomes e seus respectivos índices na matriz de adjacências
//Lembrando que o grafo em questão é simples, não dirigido e ponderado

//Funções:
void dijkstra(Graph graph, int origin, int dest);
int priorityQueue(bool *marked, int *weight);
void printPath(Graph graph, int *predecessors, int indice);

int main()
{
    //Inicialização das principais variáveis:
    Graph graph;
    ifstream input;
    string text, textAux, textWeight, origin, dest;
    int numberVertex = 0, indiceAux = 1, indiceOrigin = 0, indiceDest = 0;
    bool vertexFound = false;

    //Leitura do documento da base de dados do problema que contém as conexões entre as cidades do estado de Alagoas:
    input.open ("Conexões_Entre_Cidades_de_Alagoas_e_Suas_Distâncias_Por_Estradas_em_Km.txt");

    //Loop para leitura linha por linha da minha base de dados:
    do
    {
        //Captura linha por linha
        getline(input, text);
        int aux = 0;
        int indice1 = 0;
        int indice2 = 0;
        //Após capturar a linha, percorre todos os caracteres da linha e verifica alguns pontos importantes
        for(int i = 0; i < text.length(); i++)
        {
            //Primeiro se o caracter anterior ao atual é uma vírgula, caso seja, significa que é um espaço em branco fora do nome da cidade
            if(text[i - 1] == ',')
            {
                //ESPAÇO EM BRANCO.
            }

            //Caso o caracter atual seja uma vírgula, o nome da cidade chegou ao fim, temos que passar a string para o vetor auxiliar e marcar o peso na matriz de adjacências
            else if(text[i] == ',')
            {
                //Verificação de repetição de vértices na entrada
                for(int j = 0; j < numberCities; j++)
                {
                    if(graph.auxPrint[j].burgh == textAux) 
                    {
                        vertexFound = true;
                        //Guarda os índices para armazenar os pesos das arestas entre os vértices na matriz de adjacências
                        if(aux == 0) indice1 = graph.auxPrint[j].indice;
                        else if(aux == 1) indice2 = graph.auxPrint[j].indice;
                    }
                }    

                //Se o vértice (cidade) for novo, adiciona ao auxPrint e incrementa o índice
                if(!vertexFound)
                {
                    graph.auxPrint[numberVertex].burgh = textAux;
                    graph.auxPrint[numberVertex].indice = numberVertex;

                    //Guarda os índices para armazenar os pesos das arestas entre os vértices na matriz de adjacências
                    if(aux == 0) indice1 = graph.auxPrint[numberVertex].indice;
                    else if(aux == 1) indice2 = graph.auxPrint[numberVertex].indice;
                    
                    numberVertex++;
                }
                vertexFound = false;
                aux++;
                textAux = "";

                //Ao atingir o valor auxiliar == 2, é a hora de pegar o valor do peso e adicionar à matriz de adjacências
                if(aux == 2)
                {
                    //Lê os ultimos caracteres após o ultimo espaço e armazena em uma string auxiliar de pesos ou distâncias
                    for(int j = i + 1; j < text.length(); j++)
                    {
                        textWeight += text[j];
                    }
                
                    //Adiciona o peso na matriz de adjacências
                    int temp;
                    istringstream(textWeight) >> temp;
                    if(graph.adjMatrix[indice1][indice2] == 0 || temp <= graph.adjMatrix[indice1][indice2])
                    {
                        graph.adjMatrix[indice1][indice2] = temp;
                        graph.adjMatrix[indice2][indice1] = temp;
                    }
                    
                    textWeight = "";
                    indiceAux = indiceAux + 2;
                }
            }
            //Enquanto o auxiliar for menor que 2 e o caractere não for vírgula ou o espaço entre a vírgula e a próxima cidade, 
            //Armazena os caracteres em uma string auxiliar
            else if(aux < 2) textAux += text[i];
        }

    } while(text != "");

    //Captura a entrada do teclado do usuária para aplicar o algoritmo de Dijkstra para encontrar o menor caminho entre dois pontos
    cout << "Digite o nome de uma cidade de Alagoas, o ponto de partida do caminho: ";
    getline(cin, origin);
    cout << "Digite o nome de uma cidade de Alagoas, o ponto de chegada do caminho: ";
    getline(cin, dest);

    //Verifica a qual índice pertence a cidade recebida pelo input do teclado e envia para a função Dijkstra
    for(int i = 0; i < numberVertex; i++)
    {
        if(graph.auxPrint[i].burgh == origin) indiceOrigin = graph.auxPrint[i].indice;
        else if(graph.auxPrint[i].burgh == dest) indiceDest = graph.auxPrint[i].indice;
    }

    cout << endl << "O menor caminho da cidade " << origin << " até a cidade " << dest 
    << " é: " << endl << endl;

    //A própria função do dijkstra tem uma outra função que printa o caminho 
    dijkstra(graph, indiceOrigin, indiceDest);

    return 0;
}

void dijkstra(Graph graph, int origin, int dest)
{
    //Inicializa tanto o vetor de marcados, o vetor peso e o vetor de antecessores com os valores iniciais
    bool marked[numberCities];
    int weight[numberCities];
    int predecessors[numberCities];
    for(int i = 0; i < numberCities; i++)
    {
        marked[i] = false;
        weight[i] = __INT_MAX__;
        predecessors[i] = -1;
    }
    //Marca o peso do vértice de origem como 0, é o primeiro caminho 
    weight[origin] = 0;

    //Enquanto todos os vértices não forem percorridos:
    for(int i = numberCities; i > 0; i--)
    {
        //Pegamos o menor caminho através da função priorityQueue e marca esse caminho como percorrido
        int nextVertex = priorityQueue(marked, weight);
        marked[nextVertex] = true;   

        //Após encontrar o vértice a seguir, procura todos os vértices adjacêntes a ele
        for(int j = 0; j < numberCities; j++)
        {
            int weightNextVertex = graph.adjMatrix[nextVertex][j] + weight[nextVertex];
            //Se o vértice adjacênte ao marcado anteriormente não for marcado, precisamos verificar se ele é diferente de 0 (tem peso) 
            //e se ele o seu peso é diferente de "infinito"
            if(!marked[j])
            {
                if(weight[nextVertex] != __INT_MAX__ && graph.adjMatrix[nextVertex][j] != 0)
                {
                    //Caso essas condições sejam atendidas, basta aplicar o relaxamento. 
                    if(weight[j] > weightNextVertex) 
                    {   
                        weight[j] = weightNextVertex;
                        predecessors[j] = nextVertex;
                    }
                }
            }
        }
    }

    //Após percorrer todos os vértices, aplicamos a função recursiva printPath para mostrar o caminho através dos índices de antecessores
    printPath(graph, predecessors, dest);
    cout << weight[dest] << "km" << endl;
}

int priorityQueue(bool *marked, int *weight)
{
	int nextVertex = 0, minWeight = __INT_MAX__; 
    
    //Escolhe o caminho com o menor peso entre os vértices acessíveis no momento
	for(int i = 0; i < numberCities; i++) 
	{
        if(!marked[i])
        {
            if(weight[i] < minWeight) 
			{
                nextVertex = i;
                minWeight = weight[i];
            }
        }
    } 

	return nextVertex;
}

void printPath(Graph graph, int *predecessors, int indice)
{ 
    //Verifica se ele chegou ao caminho inicial e printa recursivamente o caminho encontrado atravé do algoritmo dijkstra
	if (predecessors[indice] == -1)
    {
        cout << graph.auxPrint[indice].burgh << ", ";
        return; 
    }
    printPath(graph, predecessors, predecessors[indice]); 
    cout << graph.auxPrint[indice].burgh << ", ";
}