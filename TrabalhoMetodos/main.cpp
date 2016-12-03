#include<Windows.h>
#include<string>
#include<iostream>
#include<fstream>
#include<vector>
#include<limits>
#include<ctime>
#include"main.h"
#include"Gantt.h"

#define INFINITO_POSITIVO INT_MAX
#define INFINITO_NEGATIVO INT_MIN
#define ALPHA 0.5
int TAMANHO_LISTA_RESTRITIVA;

double PCFreq = 0.0;
__int64 CounterStart = 0;


int main(int argc, char** argv)
{
	int Qt_Total_Operacoes = 0;
	std::vector<maquina> maquinas;
	std::vector<job> jobs;
	std::vector<op> operacoes;
	std::vector<int> operacoesFlexiveis;       // guarda quais operações são flexiveis.
	std::stack<int>* Pilha_Ordem_Topologica;   // guarda a ordem topologica de um grafo.
	int* Vetor_Distancia;                      // guarda as distancias da origem para cada vertice.
	int melhorSolucaoBusca = INFINITO_POSITIVO;
	int melhorSolucaoFinal = INFINITO_POSITIVO;
	std::string NomeArquivo;
	bool melhorou = false;
	int* Aux;

	srand((unsigned int)time(NULL));

	std::cout << "Nome da instancia: ";
	std::cin >> NomeArquivo;

	if (NomeArquivo.length() > 4)
	{
		if (NomeArquivo[NomeArquivo.length() - 1] != 't' ||
			NomeArquivo[NomeArquivo.length() - 2] != 'x' ||
			NomeArquivo[NomeArquivo.length() - 3] != 't' ||
			NomeArquivo[NomeArquivo.length() - 4] != '.')

			NomeArquivo.append(".txt");
	}
	else
	{
		NomeArquivo.append(".txt");
	}

	if (!leitura(NomeArquivo, maquinas, &Qt_Total_Operacoes, jobs, operacoes)) { return 0; }

	TAMANHO_LISTA_RESTRITIVA = jobs.size() * ALPHA;

	StartCounter();

	// iteracao do GRASP.
	for (int i = 0; i < 1000; i++)
	{
		// construção de uma solução inicial.
		contrucaoGulosaAleatoriaLRC(maquinas, operacoes, jobs);

		// cria grafo disjuntivo para o problema.
		grafoDisjuntivo grafo(Qt_Total_Operacoes);

		colocaArestaConjuntiva(&grafo, jobs, operacoes);
		colocaArestaDisjuntiva(&grafo, maquinas, operacoes);

		// calcula o makespam para solucao inicial.
		Pilha_Ordem_Topologica = orderarTopologico(grafo);
		if (!Pilha_Ordem_Topologica->empty())
		{
			Vetor_Distancia = maiorDistancia(grafo, Pilha_Ordem_Topologica, grafo.Origem);
			melhorSolucaoBusca = Vetor_Distancia[grafo.Fim];
		}
		else
		{
			std::cout << "grafo com ciclo\n";
		}

		// busca local.
		do
		{
			Aux = melhorMelhora(maquinas, operacoes, &grafo, melhorSolucaoBusca);
			if (Aux != NULL)
			{
				delete(Vetor_Distancia);
				Vetor_Distancia = Aux;
				melhorou = true;
			}
			else
			{
				melhorou = false;
			}

			melhorSolucaoBusca = Vetor_Distancia[grafo.Fim];
		}
		while (melhorou);

		if (melhorSolucaoBusca < melhorSolucaoFinal)
			melhorSolucaoFinal = melhorSolucaoBusca;

		// limpa sequencia de operacoes das maquinas.
		for (std::vector<maquina>::iterator i = maquinas.begin(); i != maquinas.end(); i++)
		{
			i->Sequencia_Operacoes.clear();
		}
		delete(Pilha_Ordem_Topologica);
		system("cls");
		std::cout << i << ", " << melhorSolucaoFinal;
		//std::cout << "Makespam depois busca: " << melhorSolucaoBusca << std::endl;
		//std::cout << "melhor solucao: " << melhorSolucaoFinal << std::endl << std::endl;

	}
	std::cout << std::endl;
	std::cout << GetCounter() << std::endl;
	//	


	// mostra grafico de gantt.
	GanttParameters parametros;
	GanttElement elemento;
	parametros.category_Quantity = jobs.size();
	parametros.line_Quantity = maquinas.size();
	parametros.total_Units = Vetor_Distancia[Qt_Total_Operacoes + 1];
	for (unsigned int i = 0; i < operacoes.size(); i++)
	{
		elemento.beginning = Vetor_Distancia[i];
		elemento.category = operacoes[i].job;
		elemento.line = operacoes[i].maquina_Atual;
		elemento.size = operacoes[i].peso;
		parametros.Elements.push_back(elemento);
	}
	Ganttmain(parametros);

	return 0;
}

bool leitura(std::string NomeArquivo, std::vector<maquina>& maquinas, int* Qt_Total_Operacoes, std::vector<job>& jobs, std::vector<op>& operacoes)
{
	int Qt_Jobs, Qt_Maquinas;
	int Qt_Operacoes = 0;
	int Qt_Maquinas_Disponiveis = 0;
	int Maquina = 0;                     // usada para ler as maquinas disponiveis para cada operação.
	int Peso = 0;                        // usada para ler o peso de cada operação.
	int Contador_Seq_Tec = 0;            // variavel que guarda a quantidade de operações de um mesmo job que já foram lidas. 
	op operacaoAux;                      // auxilia a contrução do vetor de operações.
	job jobAux;                          // auxilia a construção do vetor de jobs.
	std::ifstream arq;

	//leitura.
	// abre o arquivo com a instancia do problema.
	arq.open(NomeArquivo, std::ifstream::in);
	if (arq.is_open())
	{
		arq >> Qt_Jobs;
		arq >> Qt_Maquinas;
		{
			maquina Maquina;
			for (int i = 0; i < Qt_Maquinas; i++)
			{
				maquinas.push_back(Maquina);
			}
		}
		arq.ignore(999, '\n');
		for (int k = 0; k < Qt_Jobs; k++) //cada iteração lê os dados de um job.
		{
			Contador_Seq_Tec = 0;
			arq >> Qt_Operacoes;
			for (int i = 0; i < Qt_Operacoes; i++) // cada iteração lê os dados de uma operação.
			{
				if (i != Qt_Operacoes - 1)
				{
					jobAux.Sequencia_tecnologica.push_back(*Qt_Total_Operacoes);
				}
				else
				{
					jobAux.Sequencia_tecnologica.push_back(*Qt_Total_Operacoes);
					jobs.push_back(jobAux);
					jobAux.Sequencia_tecnologica.clear();
				}
				operacaoAux.id = *Qt_Total_Operacoes;
				operacaoAux.Posicao_Tecnogica = Contador_Seq_Tec;
				operacaoAux.maquinas.clear();
				arq >> Qt_Maquinas_Disponiveis;
				// se a operação tem mais de uma maquina disponivel, ela é marcada como flexivel.
				/*if (Qt_Maquinas_Disponiveis > 1)
				{
					operacoesFlexiveis.push_back(Qt_Total_Operacoes);
				}*/
				for (int j = 0; j < Qt_Maquinas_Disponiveis; j++) // cada iteração lê os dados de uma maquina disponivel para a operação.
				{
					arq >> Maquina;
					arq >> Peso;
					operacaoAux.job = k;
					operacaoAux.peso = Peso;
					operacaoAux.maquinas.push_back(Maquina - 1);
				}
				operacoes.push_back(operacaoAux);
				*Qt_Total_Operacoes = *Qt_Total_Operacoes + 1;
				Contador_Seq_Tec++;
			}
		}
	}
	else
	{
		std::cout << "falha na leitura do arquivo";
		return false;
	}
	arq.close();
	return true;
}

void contrucaoGulosaAleatoriaLRC(std::vector<maquina>& maquinas, std::vector<op>& operacoes, std::vector<job> jobs)
{
	bool* op_usadas;
	std::vector<int>::iterator* iJobs;  // vetor que guarda a proximo elemento a ser alocado da sequncia de um job. 
	int* vFuncaoGulosa;                 // guarda os valores da funcao gulosa para cada job.
	int* LRC;                           // lista restritiva de candidatos.
	int indiceMaquinasDisponiveis;
	int indiceMaquina;
	int indiceJob;
	int cont;
	int min;

	// inicializa os vetores.
	LRC = new int[TAMANHO_LISTA_RESTRITIVA];
	vFuncaoGulosa = new int[jobs.size()];
	iJobs = new std::vector<int>::iterator[jobs.size()];
	op_usadas = new bool[operacoes.size()];

	for (unsigned int i = 0; i < operacoes.size(); i++)
	{
		op_usadas[i] = false;
	}
	for (unsigned int i = 0; i < jobs.size(); i++)
	{
		iJobs[i] = jobs[i].Sequencia_tecnologica.begin();
	}

	// para cada iteracao aloca uma operacao a uma de suas maquinas disponiveis.
	for (unsigned int j = 0; j < operacoes.size(); j++)
	{
		min = INFINITO_POSITIVO;
		cont = 0;
		// itera por todos os jobs, e calcula o valor da funcao gulosa para cada um.
		for (unsigned int i = 0; i < jobs.size(); i++)
		{
			// calcular funcao gulosa para o job.
			if (iJobs[i] == jobs[i].Sequencia_tecnologica.end())
			{
				vFuncaoGulosa[i] = INFINITO_POSITIVO;
			}
			else
			{
				vFuncaoGulosa[i] = operacoes[*iJobs[i]].Posicao_Tecnogica;
			}

			// adiciona a lista de restrita de candidatos.
			if (vFuncaoGulosa[i] <= min)
			{
				LRC[cont % TAMANHO_LISTA_RESTRITIVA] = i;
				min = vFuncaoGulosa[i];
				cont++;
			}
		}
		// escolher um elemento da lista retrita de candidatos aleatoriamente.
		do
		{
			indiceJob = LRC[rand() % TAMANHO_LISTA_RESTRITIVA];

		} while ( vFuncaoGulosa[indiceJob] == INFINITO_POSITIVO );

		// adicionar a operacao corrente do job a uma das maquinas disponiveis.
		indiceMaquinasDisponiveis = rand() % operacoes[*iJobs[indiceJob]].maquinas.size();
		indiceMaquina = operacoes[*iJobs[indiceJob]].maquinas[indiceMaquinasDisponiveis];
		operacoes[*iJobs[indiceJob]].maquina_Atual = indiceMaquina;
		maquinas[indiceMaquina].Sequencia_Operacoes.push_back(*iJobs[indiceJob]);
		// incrementar iterador do job.
		iJobs[indiceJob]++;
	}
	return;
}



// troca ordem de duas operacoes no grafo e na maquina.
bool mover(grafoDisjuntivo* grafo, std::vector<op> operacoes, std::vector<maquina>& maquinas, int indiceM, int indice1, int indice2)
{
	if (operacoes[maquinas[indiceM].Sequencia_Operacoes[indice1]].job == operacoes[maquinas[indiceM].Sequencia_Operacoes[indice2]].job)
		return false;
	
	if (indice1 > indice2)
	{
		int aux = indice1;
		indice1 = indice2;
		indice2 = aux;
	}

	// trocar direcao da aresta.
	grafo->MatrizAdjacencia[maquinas[indiceM].Sequencia_Operacoes[indice1]][maquinas[indiceM].Sequencia_Operacoes[indice2]] = -1;
	grafo->MatrizAdjacencia[maquinas[indiceM].Sequencia_Operacoes[indice2]][maquinas[indiceM].Sequencia_Operacoes[indice1]] = 
		operacoes[maquinas[indiceM].Sequencia_Operacoes[indice2]].peso;

	if (indice1 == 0)
	{
		grafo->MatrizAdjacencia[maquinas[indiceM].Sequencia_Operacoes[indice2]][maquinas[indiceM].Sequencia_Operacoes[indice2 + 1]] = -1;
		grafo->MatrizAdjacencia[maquinas[indiceM].Sequencia_Operacoes[indice1]][maquinas[indiceM].Sequencia_Operacoes[indice2 + 1]] =
			operacoes[maquinas[indiceM].Sequencia_Operacoes[indice1]].peso;
	}
	else
	{
		if (indice2 == maquinas[indiceM].Sequencia_Operacoes.size() - 1)
		{
			grafo->MatrizAdjacencia[maquinas[indiceM].Sequencia_Operacoes[indice1-1]][maquinas[indiceM].Sequencia_Operacoes[indice1]] = -1;
			grafo->MatrizAdjacencia[maquinas[indiceM].Sequencia_Operacoes[indice1 - 1]][maquinas[indiceM].Sequencia_Operacoes[indice2]] =
				operacoes[maquinas[indiceM].Sequencia_Operacoes[indice1 - 1]].peso;
		}
		else
		{
			grafo->MatrizAdjacencia[maquinas[indiceM].Sequencia_Operacoes[indice2]][maquinas[indiceM].Sequencia_Operacoes[indice2 + 1]] = -1;
			grafo->MatrizAdjacencia[maquinas[indiceM].Sequencia_Operacoes[indice1]][maquinas[indiceM].Sequencia_Operacoes[indice2 + 1]] =
				operacoes[maquinas[indiceM].Sequencia_Operacoes[indice1]].peso;
			grafo->MatrizAdjacencia[maquinas[indiceM].Sequencia_Operacoes[indice1 - 1]][maquinas[indiceM].Sequencia_Operacoes[indice1]] = -1;
			grafo->MatrizAdjacencia[maquinas[indiceM].Sequencia_Operacoes[indice1 - 1]][maquinas[indiceM].Sequencia_Operacoes[indice2]] =
				operacoes[maquinas[indiceM].Sequencia_Operacoes[indice1 - 1]].peso;
		}
	}

	// troca a ordem na sequencia da maquina.
	{
		int aux;
		aux = maquinas[indiceM].Sequencia_Operacoes[indice1];
		maquinas[indiceM].Sequencia_Operacoes[indice1] = maquinas[indiceM].Sequencia_Operacoes[indice2];
		maquinas[indiceM].Sequencia_Operacoes[indice2] = aux;
	}

	return true;
}

int* melhorMelhora(std::vector<maquina>& maquinas, std::vector<op> operacoes, grafoDisjuntivo* grafo, int melhorSolucao)
{
	int* Vetor_Distancia = NULL;
	int* Vetor_Distancia_Aux = NULL;
	std::stack<int>* Pilha_Ordem_Topologica;
	int indiceMelhorMaquina;
	int indiceMelhorOpSM;
	int makespam;
	int max = INFINITO_NEGATIVO;

	// percorre operacoes de todas as maquinas.
	for (std::vector<maquina>::iterator i = maquinas.begin(); i != maquinas.end(); i++)
	{
		for (unsigned int j = 0; j < i->Sequencia_Operacoes.size() - 1; j++)
		{
			makespam = -1;
			if (mover(grafo, operacoes, maquinas, i - maquinas.begin(), j, j + 1) ) // troca a ordem de j com o proximo da maquina no grafo.
			{
				// calcular o makespam.
				Pilha_Ordem_Topologica = orderarTopologico(*grafo);
				if (!Pilha_Ordem_Topologica->empty())
				{
					Vetor_Distancia_Aux = maiorDistancia(*grafo, Pilha_Ordem_Topologica, grafo->Origem);
					makespam = Vetor_Distancia_Aux[grafo->Fim];
				}
				delete(Pilha_Ordem_Topologica);
				if (makespam < melhorSolucao && makespam != -1)
				{
					melhorSolucao = makespam;
					delete(Vetor_Distancia);
					Vetor_Distancia = Vetor_Distancia_Aux;
					indiceMelhorMaquina = i - maquinas.begin();
					indiceMelhorOpSM = j;
					// descomente essa linha para primeira melhora.
					//return Vetor_Distancia;
				}
				else
				{
					//delete(Vetor_Distancia_Aux);
				}
				// mover de volta.
				mover(grafo, operacoes, maquinas, i - maquinas.begin(), j, j + 1);
			}
		}
	}
	if (Vetor_Distancia != NULL)
	{
		// atualizar o grafo.
		mover(grafo, operacoes, maquinas, indiceMelhorMaquina, indiceMelhorOpSM, indiceMelhorOpSM + 1);
	}
	
	return Vetor_Distancia;
}


void colocaArestaConjuntiva(grafoDisjuntivo* grafo, std::vector<job> jobs, std::vector<op> operacoes)
{
	std::vector<int>::iterator aux;
	// percorre os jobs.
	for (std::vector<job>::iterator i = jobs.begin(); i != jobs.end(); i++)
	{
		// coloca aresta entre primeira operacao do job e a origem.
		grafo->MatrizAdjacencia[grafo->Origem][*(i->Sequencia_tecnologica.begin())] = 0;

		// percorre a sequencia tecnologica do job.
		for (std::vector<int>::iterator j = i->Sequencia_tecnologica.begin(); j != i->Sequencia_tecnologica.end() - 1; j++)
		{
			// colocar aresta entre a operacao do job e a proxima na sequencia tecnologica.
			aux = j;
			aux++;
			grafo->MatrizAdjacencia[*j][*aux] = operacoes[*j].peso;
		}
		// coloca aresta entre a ultima operação e o fim no grafo.
		grafo->MatrizAdjacencia[ *(i->Sequencia_tecnologica.end() - 1) ][grafo->Fim] = operacoes[*(i->Sequencia_tecnologica.end() - 1)].peso;
	}
	return;
}


void colocaArestaDisjuntiva(grafoDisjuntivo* grafo, std::vector<maquina> maquinas, std::vector<op> operacoes)
{
	std::vector<int>::iterator aux;
	// percorre todas as maquinas
	for (std::vector<maquina>::iterator i = maquinas.begin(); i != maquinas.end(); i++)
	{
		// percorre a sequencia de operações de uma maquina.
		for (std::vector<int>::iterator j = i->Sequencia_Operacoes.begin(); j != i->Sequencia_Operacoes.end(); j++)
		{
			aux = j;
			aux++;
			// se nao é a ultima operação.
			if (aux != i->Sequencia_Operacoes.end())
			{
				// coloca aresta entre operação e sucessor na sequencia da maquina.
				grafo->MatrizAdjacencia[*j][*aux] = operacoes[*j].peso;
			}
		}
	}
	return;
}


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

// salva ordem topologica do grafo em uma pilha, caso encontre um ciclo retorna uma pilha vazia.
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
int* maiorDistancia(grafoDisjuntivo grafo, std::stack<int>* Pilha_Ord_Topologico, int Origem)
{
	int* MaiorDistancia = new int[grafo.Tamanho];
	;
	// inicia todas as distancias como -infinito.
	for (int i = 0; i < grafo.Tamanho; i++)
	{
		MaiorDistancia[i] = INFINITO_NEGATIVO;
	}
	MaiorDistancia[Origem] = 0;

	while (Pilha_Ord_Topologico->empty() == false)
	{
		// pega o proximo vertice em ordem topologica.
		int v = Pilha_Ord_Topologico->top();
		Pilha_Ord_Topologico->pop();

		// atualiza as distancias dos vertices adjacentes se necessário.
		if (MaiorDistancia[v] != INFINITO_NEGATIVO)
		{
			for (int i = 0; i < grafo.Tamanho; i++)
			{
				if (grafo.MatrizAdjacencia[v][i] != -1)
				{
					// se a maior distacia atual até i for menor que maior distacia até v mais a distacia de v para i, atualiza a maior distancia para i.
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

void StartCounter()
{
	LARGE_INTEGER li;
	if (!QueryPerformanceFrequency(&li))
		std::cout << "QueryPerformanceFrequency Failed.\n";
	PCFreq = (double)(li.QuadPart) / 1000.0;
	QueryPerformanceCounter(&li);
	CounterStart = li.QuadPart;
}

double GetCounter()
{
	LARGE_INTEGER li;
	QueryPerformanceCounter(&li);
	return (double)(li.QuadPart - CounterStart) / PCFreq;
}
