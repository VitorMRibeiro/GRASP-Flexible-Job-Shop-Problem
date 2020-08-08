#include"include/graph_util.h"
#include<climits>

// utilitarios para grafos

// retorna falso se encontrar um ciclo.
bool ordenarTopologicoAux(grafoDisjuntivo grafo, int v, bool visitados[], bool visitadosLocal[], std::stack<int>* Pilha)
{
	visitados[v] = true;
	visitadosLocal[v] = true;
	for (int i = 0; i < grafo.Tamanho; i++)
	{
		if (grafo.MatrizAdjacencia[v][i] != -1)
		{
			if (!visitadosLocal[i])
			{
				if (!visitados[i])
				{
					if (ordenarTopologicoAux(grafo, i, visitados, visitadosLocal, Pilha) == false)
					{
						return false;
					}
				}
			}
			else
			{
				return false;
			}
		}
	}
	Pilha->push(v);
	visitadosLocal[v] = false;
	return true;
}

// salva ordem topologica do grafo em uma pilha; caso encontre um ciclo retorna uma pilha vazia.
std::stack<int>* orderarTopologico(grafoDisjuntivo grafo)
{
	std::stack<int>* Pilha = new std::stack<int>;
	bool* visitados = new bool[grafo.Tamanho];
	bool* visitadosLocal = new bool[grafo.Tamanho];

	for (int i = 0; i < grafo.Tamanho; i++)
	{
		visitados[i] = false;
		visitadosLocal[i] = false;
	}

	for (int i = 0; i < grafo.Tamanho; i++)
	{
		if (visitados[i] == false)
		{
			if (ordenarTopologicoAux(grafo, i, visitados, visitadosLocal, Pilha) == false)
			{
				// esvazia a pilha e retorna a pilha vazia.
				while (!Pilha->empty())
				{
					Pilha->pop();
				}
				return Pilha;
			}
		}
	}
	return Pilha;
}

// calcula a maior distancia da origem para cada vertice de um grafo ordenado.
int* maiorDistancia(grafoDisjuntivo grafo, std::stack<int>* Pilha_Ord_Topologica, int Origem)
{
	int* MaiorDistancia = new int[grafo.Tamanho];
	;
	// inicia todas as distancias como -infinito.
	for (int i = 0; i < grafo.Tamanho; i++)
	{
		MaiorDistancia[i] = INT_MIN;
	}
	MaiorDistancia[Origem] = 0;

	while (Pilha_Ord_Topologica->empty() == false)
	{
		// pega o proximo vertice em ordem topologica.
		int v = Pilha_Ord_Topologica->top();
		Pilha_Ord_Topologica->pop();

		// atualiza as distancias dos vertices adjacentes se necessario.
		if (MaiorDistancia[v] != INT_MIN)
		{
			for (int i = 0; i < grafo.Tamanho; i++)
			{
				if (grafo.MatrizAdjacencia[v][i] != -1)
				{
					// se a maior distacia atual ate i for menor que a maior distacia ate v somada a distacia de v para i, atualiza a maior distancia para i.
					if (MaiorDistancia[i] < MaiorDistancia[v] + grafo.MatrizAdjacencia[v][i])
					{
						MaiorDistancia[i] = MaiorDistancia[v] + grafo.MatrizAdjacencia[v][i];
					}
				}
			}
		}
	}
	return MaiorDistancia;
}