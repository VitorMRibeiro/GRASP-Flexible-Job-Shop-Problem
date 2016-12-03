#pragma once
#include<stack>

struct op
{
	int id;
	int peso;                 
	int job;                  
	int maquina_Atual;
	int Posicao_Tecnogica;    // posição da operação na sequencia tecnologica do job.
	std::vector<int>maquinas; // lista de maquinas disponiveis para a operação.
};
typedef struct op op;

struct maquina
{
	std::vector<int>Sequencia_Operacoes;
};
typedef struct maquina maquina;

struct job
{
	std::vector<int>Sequencia_tecnologica;
};

struct aresta
{
	int origem;
	int destino;
};
typedef struct aresta aresta;

// estrutura que representa o grafo disjuntivo do FJSP, o elemento [i][j] da matriz representa o Peso da aresta que liga a operação i à j.
// caso o valor do elemento seja -1 significa que não há conexão entre as operações.
struct grafoDisjuntivo
{
	int Tamanho;
	int Origem;
	int Fim;
	std::vector<std::vector<int> > MatrizAdjacencia;
	grafoDisjuntivo(int n)
	{
		this->Tamanho = n + 2;
		this->Origem = n;
		this->Fim = n + 1;
		// inicia todos os arestas em -1.
		for (int i = 0; i < n + 2; i++)
		{
			std::vector<int> linha;
			for (int j = 0; j < n + 2; j++)
			{
				linha.push_back(-1);
			}
			MatrizAdjacencia.push_back(linha);
		}
	}
};
typedef struct grafoDisjuntivo grafoDisjuntivo;


bool leitura(std::string NomeArquivo, std::vector<maquina>& maquinas, int * Qt_Total_Operacoes, std::vector<job>& jobs, std::vector<op>& operacoes);

void contrucaoGulosaAleatoriaLRC(std::vector<maquina>& maquinas, std::vector<op>& operacoes, std::vector<job> jobs);

bool mover(grafoDisjuntivo * grafo, std::vector<op> operacoes, std::vector<maquina>& maquinas, int indiceM, int indice1, int indice2);

int * melhorMelhora(std::vector<maquina>& maquinas, std::vector<op> operacoes, grafoDisjuntivo* grafo, int melhorSolucao);

void colocaArestaConjuntiva(grafoDisjuntivo* grafo, std::vector<job> jobs, std::vector<op> operacoes);

void colocaArestaDisjuntiva(grafoDisjuntivo* grafo, std::vector<maquina> maquinas, std::vector<op> operacoes);

void imprimirGrafo(grafoDisjuntivo grafo);

void imprimeSequenciaMaquina(std::vector<maquina> maquinas);

bool ordenarTopologicoAux(grafoDisjuntivo grafo, int v, bool visitados[], bool visitadosLocal[], std::stack<int>* Pilha);

std::stack<int>* orderarTopologico(grafoDisjuntivo grafo);

int * maiorDistancia(grafoDisjuntivo grafo, std::stack<int>* Pilha_Ord_Topologico, int Origem);

void StartCounter();

double GetCounter();
