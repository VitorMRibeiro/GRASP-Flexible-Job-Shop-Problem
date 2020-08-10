#include"include/graph_util.h"
#include<unordered_set>
#include<vector>
#include<iostream>
#include<climits>
#include<cstring>
#include<algorithm>

// utilitarios para grafos

void printGrafo(grafoDisjuntivo grafo){
	for(int i = 0; i < grafo.Tamanho; i++){
		for(int j = 0; j < grafo.Tamanho; j++){
			if(grafo.MatrizAdjacencia[i][j] == -1){
				std::cout << "-" << ",";
			}
			else{
				std::cout << grafo.MatrizAdjacencia[i][j] << ",";
			}
		}
		std::cout << std::endl;
	}
}

bool DFS(int* tarjan_vetex_index, int* tarjan_lowlink, int& tarjan_min_index, grafoDisjuntivo &grafo, std::stack<int> &Pilha, int indice){
	tarjan_min_index++;
	
	//std::cout << indice << std::endl;

	tarjan_vetex_index[indice] = tarjan_min_index;
	tarjan_lowlink[indice] = tarjan_min_index;

	for(int i = 0; i < grafo.Tamanho; i++){
		if(grafo.MatrizAdjacencia[indice][i] != -1 && tarjan_vetex_index[i] == 0){
			if(!DFS(tarjan_vetex_index, tarjan_lowlink, tarjan_min_index, grafo, Pilha, i))
				return false;

			tarjan_lowlink[indice] = std::min(tarjan_lowlink[indice], tarjan_lowlink[i]);
		}
		else{
			tarjan_lowlink[indice] = std::min(tarjan_lowlink[indice], tarjan_vetex_index[i]);
		}
	}

	Pilha.push(indice);

	if(tarjan_lowlink[indice] == tarjan_vetex_index[indice]){
		std::cout << "ordenarTopologico: grafo com ciclo\n";
		return false;
	}
	return true;
}

// Ordena o grafo topologicamente e procura por ciclos.
std::stack<int> orderarTopologico(grafoDisjuntivo grafo){
	std::stack<int> Pilha;
	std::stack<int> tarjan_stack;
	int* tarjan_vetex_index;
	int* tarjan_lowlink;
	int tarjan_min_index = 0;

	//printGrafo(grafo);

	tarjan_vetex_index = new int[grafo.Tamanho];
	tarjan_lowlink = new int[grafo.Tamanho];

	memset(tarjan_vetex_index, 0, sizeof(int) * grafo.Tamanho);

	if(DFS(tarjan_vetex_index, tarjan_lowlink, tarjan_min_index, grafo, Pilha, grafo.Origem)){
		delete(tarjan_vetex_index);
		delete(tarjan_lowlink);
		
		return Pilha;
	}
	return std::stack<int>();
}


// calcula a maior distancia da origem para cada vertice de um grafo ordenado.
int* maiorDistancia(grafoDisjuntivo grafo, std::stack<int> Pilha_Ord_Topologica, int Origem){
	int* MaiorDistancia = new int[grafo.Tamanho];

	// inicia todas as distancias como -infinito.
	memset(MaiorDistancia, INT_MIN, sizeof(int) * grafo.Tamanho);
	MaiorDistancia[Origem] = 0;

	while (Pilha_Ord_Topologica.empty() == false){
		// pega o proximo vertice em ordem topologica.
		int v = Pilha_Ord_Topologica.top();
		Pilha_Ord_Topologica.pop();

		// atualiza as distancias dos vertices adjacentes se necessario.
		if (MaiorDistancia[v] != INT_MIN){
			for (int i = 0; i < grafo.Tamanho; i++){
				if (grafo.MatrizAdjacencia[v][i] != -1){
					// se a maior distacia atual ate i for menor que a maior distacia ate v somada a distacia de v para i, atualiza a maior distancia para i.
					if (MaiorDistancia[i] < MaiorDistancia[v] + grafo.MatrizAdjacencia[v][i]){
						MaiorDistancia[i] = MaiorDistancia[v] + grafo.MatrizAdjacencia[v][i];
					}
				}
			}
		}
	}
	return MaiorDistancia;
}