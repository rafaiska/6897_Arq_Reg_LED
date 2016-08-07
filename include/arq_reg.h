#ifndef ARQ_REG_H
#define ARQ_REG_H

/* ======== UNIVERSIDADE ESTADUAL DE MARINGA ========== *
 * ============== CENTRO DE TECNOLOGIA ================ *
 * ========== DEPARTAMENTO DE INFORMATICA ============= *
 * ===== 6897 - ORGANIZACAO E RECUPERACAO DE DADOS ==== *
 * ========== PROFESSORA VALERIA FELTRIM ============== *
 * ================== TRABALHO 1 ====================== *
 * Alunos:	Chen Po Hsiang		RA 83473	*
 * 		Rafael Cortez Sanchez	RA 82357	*
 
 O cabecalho arq_reg.h contem funcoes para a manipulacao de um arquivo de registros com lista de espacos disponiveis (LED). Os registros possuem tamanho variavel, e o cabecalho do arquivo eh um inteiro de 16 bits sem sinal que sinaliza o offset do primeiro espaco disponivel no arquivo. Caso nao existam espacos vazios, esse inteiro assume valor 0 (zero).

 Os registros armazenam informacoes sobre trabalhos academicos, mapeados pelos atributos id, autor, titulo, curso e tipo de trabalho.

 A LED eh ordenada de forma decrescente, otimizada para a abordagem WORST FIT para reutilizacao dos espacos em branco.
 */

#include <string.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include "arvore_b.h"

typedef struct registro_t {
	uint32_t id;		//Numero identificador do registro (chave primaria)
	char autor[128];	//Nome do autor
	char titulo[256];	//Titulo do trabalho
	char curso[64];		//Nome do curso;
	char tipo[32];		//Tipo do trabalho: TCC, Mestrado, Doutorado...
}registro_t;

uint16_t Buscar_Registro(char *caminho_registro, uint32_t id, registro_t *registro); //Busca um registro com id especifica a partir do arquivo apontado pelo endereco "caminho_registro". Retorna a posicao do registro no arquivo. Se passado parametro "registro_t*" diferente de NULL, escreve o registro encontrado nesse endereco

uint16_t Buscar_Registro_Com_Indice(char *caminho_registro, uint32_t id, registro_t *registro); //Como a funcao acima, mas utilizando o arquivo de indice para uma busca mais eficiente

uint16_t Calcular_Tamanho(registro_t registro); //Calcula tamanho de um registro para sua insercao. Nao eh chamada diretamente pelo usuario

uint16_t Inserir_ED(FILE *arq_reg, uint16_t posicao); //Insere um espaco de registro em branco na LED, seguindo a ordem de WORST FIT. Nao eh chamada diretamente pelo usuario, mas pelas funcoes Inserir_Registro(char*, registro_t) e Remover_Registro(char*, uint32_t)

int Inserir_Registro(char *caminho_registro, registro_t registro); //Insere um registro novo no arquivo, inserindo-o no primeiro espaco disponivel, caso seja grande o suficiente. Caso contrario, insere o novo registro no final.

int Inserir_Registro_Final(FILE *arq_reg, registro_t registro); //Insere um registro no final do arquivo. Nao eh chamada diretamente pelo usuario, mas pela funcao Inserir_Registro(char*, registro_t)

uint16_t Remover_Registro(char *caminho_registro, uint32_t id); //Remove do arquivo um registro de id especifico. Retorna a posicao do registro removido dentro do arquivo, ou o valor 0, caso o registro nao exista

registro_t String_to_reg(char *cadeia); //Converte uma string do arquivo de catalogo para um registro "struct registro_t"

FILE *Abrir_arquivo(char *caminho, char *modo); //Funcao para abrir arquivos (tanto de registro quanto de catalogo). Nao eh chamada diretamente pelo usuario

int Importar_arquivo_catalogo(char *caminho_catalogo, char *caminho_registro);	//Importa arquivo texto de catalogo com registros. Se o arquivo de registros (destino) ja existir, insere os registros importados nesse arquivo existente

void Imprimir_Registro(registro_t registro); //Imprime na saida padrao um registro_t devidamente formatado

void Get_Caminho_Indice(char *arq_reg, char *arq_idx); //Retorna o caminho de indice em arq_idx, dado um caminho de registro em arq_reg. Exemplo: "./res/exem.idx" de "./res/exem.reg"

#endif
