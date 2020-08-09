#ifndef GRAPH_H
#define GRAPH_H


#include<stack>
#include<vector>


struct aresta
{
	int origem;
	int destino;
};
typedef struct aresta aresta;

// estrutura que representa o grafo disjuntivo do FJSP, o elemento [i][j] da matriz representa o Peso da aresta que liga a opera��o i � j.
// caso o valor do elemento seja -1 significa que n�o h� conex�o entre as opera��es.
struct grafoDisjuntivo
{
	int Tamanho;
	int Origem;
	int Fim;
	std::vector<std::vector<int>> MatrizAdjacencia;
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



bool ordenarTopologicoAux(grafoDisjuntivo grafo, int v, bool visitados[], bool visitadosLocal[], std::stack<int>* Pilha);

std::stack<int> orderarTopologico(grafoDisjuntivo grafo);

int * maiorDistancia(grafoDisjuntivo grafo, std::stack<int> Pilha_Ord_Topologico, int Origem);

#endif