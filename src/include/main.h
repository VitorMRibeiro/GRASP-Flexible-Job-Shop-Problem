#ifndef MAIN_H
#define MAIN_H


#include<vector>
#include<string>
#include<stack>
#include"graph_util.h"


struct op
{
	int id;
	int peso;                 
	int job;                  
	int maquina_Atual;
	int Posicao_Tecnogica;    // posicao da operacao na sequencia tecnologica do job.
	std::vector<int>maquinas; // lista de maquinas disponiveis para a operacao.
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

bool leitura(std::string NomeArquivo, std::vector<maquina>& maquinas, int * Qt_Total_Operacoes, std::vector<job>& jobs, std::vector<op>& operacoes);

void contrucaoGulosaAleatoriaLRC(std::vector<maquina>& maquinas, std::vector<op>& operacoes, std::vector<job> jobs);

bool mover(grafoDisjuntivo * grafo, std::vector<op> operacoes, std::vector<maquina>& maquinas, unsigned int indiceM, unsigned int indice1, unsigned int indice2);

int * melhorMelhora(std::vector<maquina>& maquinas, std::vector<op> operacoes, grafoDisjuntivo* grafo, int melhorSolucao);

void colocaArestaConjuntiva(grafoDisjuntivo* grafo, std::vector<job> jobs, std::vector<op> operacoes);

void colocaArestaDisjuntiva(grafoDisjuntivo* grafo, std::vector<maquina> maquinas, std::vector<op> operacoes);

void imprimeSequenciaMaquina(std::vector<maquina> maquinas);


#endif