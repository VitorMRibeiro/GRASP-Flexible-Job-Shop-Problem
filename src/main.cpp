#include<iostream>
#include<fstream>
#include<climits>
#include"include/main.h"
#include"include/Gantt.h"

#define ALPHA 0.5
int TAMANHO_LISTA_RESTRITIVA;


int main(int argc, char** argv)
{
	int Qt_Total_Operacoes = 0;
	std::vector<maquina> maquinas;
	std::vector<job> jobs;
	std::vector<op> operacoes;
	std::vector<int> operacoesFlexiveis;       // guarda quais operacoes sao flexiveis.
	std::stack<int>* Pilha_Ordem_Topologica;   // guarda a ordem topologica de um grafo.
	int* Vetor_Distancia;                      // guarda as distancias da origem para cada vertice.
	int melhorSolucaoBusca = INT_MAX;
	int melhorSolucaoFinal = INT_MAX;
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

	// iteracao do GRASP.
	for (int i = 0; i < 100; i++)
	{
		// construcao de uma solucao inicial.
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
		{
			melhorSolucaoFinal = melhorSolucaoBusca;
		}

		// limpa sequencia de operacoes das maquinas.
		for (std::vector<maquina>::iterator i = maquinas.begin(); i != maquinas.end(); i++)
		{
			i->Sequencia_Operacoes.clear();
		}
		delete(Pilha_Ordem_Topologica);
		std::cout << "makespam: " << melhorSolucaoFinal << std::endl;
	}

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
	int Maquina = 0;                     // usada para ler as maquinas disponiveis para cada operacao.
	int Peso = 0;                        // usada para ler o peso de cada operacao.
	int Contador_Seq_Tec = 0;            // variavel que guarda a quantidade de operacoes de um mesmo job que ja foram lidas. 
	op operacaoAux;                      // auxilia a contrucao do vetor de operacoes.
	job jobAux;                          // auxilia a construcao do vetor de jobs.
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
		for (int k = 0; k < Qt_Jobs; k++) //cada iteracao le os dados de um job.
		{
			Contador_Seq_Tec = 0;
			arq >> Qt_Operacoes;
			for (int i = 0; i < Qt_Operacoes; i++) // cada iteracao le os dados de uma operacao.
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
				// se a operacao tem mais de uma maquina disponivel, ela é marcada como flexivel.
				/*if (Qt_Maquinas_Disponiveis > 1)
				{
					operacoesFlexiveis.push_back(Qt_Total_Operacoes);
				}*/
				for (int j = 0; j < Qt_Maquinas_Disponiveis; j++) // cada iteracao le os dados de uma maquina disponivel para a operacao.
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
		min = INT_MAX;
		cont = 0;
		// itera por todos os jobs, e calcula o valor da funcao gulosa para cada um.
		for (unsigned int i = 0; i < jobs.size(); i++)
		{
			// calcular funcao gulosa para o job.
			if (iJobs[i] == jobs[i].Sequencia_tecnologica.end())
			{
				vFuncaoGulosa[i] = INT_MAX;
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

		} while ( vFuncaoGulosa[indiceJob] == INT_MAX );

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
bool mover(grafoDisjuntivo* grafo, std::vector<op> operacoes, std::vector<maquina>& maquinas, unsigned int indiceM, unsigned int indice1, unsigned int indice2)
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
		// coloca aresta entre a ultima operacao e o fim no grafo.
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
		// percorre a sequencia de operacoes de uma maquina.
		for (std::vector<int>::iterator j = i->Sequencia_Operacoes.begin(); j != i->Sequencia_Operacoes.end(); j++)
		{
			aux = j;
			aux++;
			// se nao e a ultima operacao.
			if (aux != i->Sequencia_Operacoes.end())
			{
				// coloca aresta entre operacao e sucessor na sequencia da maquina.
				grafo->MatrizAdjacencia[*j][*aux] = operacoes[*j].peso;
			}
		}
	}
	return;
}
