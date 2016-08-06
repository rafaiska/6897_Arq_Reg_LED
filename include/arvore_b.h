#ifndef ARVORE_B_H_
#define ARVORE_B_H_

/* arvore_b.h: Dados e instrucoes utilizados para manter um
 * arquivo de índice no formato de árvore B.
 *
 * ORGANIZACAO DO ARQUIVO DE INDICE (EXEMPLO COM ARVORE DE
 * ORDEM 5):
 *
 *	Cabeçalho:	Ponteiro da PED (2 bytes)		|Byte offset 0x00
 *			Raiz da arvore (2 bytes)		|Byte offset 0x02
 *	Corpo:		No de RRN 1 (9 bytes)
 *			{
 *				Ocupacao da pagina (1 byte)	|Byte offset 0x04
 *				5 Ponteiros de 2 bytes		|Byte offset 0x05
 *				4 Registros de 6 bytes		|Byte offset 0x0f
 *			}
 *			No de RRN 2
 *			.
 *			.
 *			.
 *
 * NOTAS:
 * Cada registro possui um campo identificador de 4 bytes para
 * um registro do arquivo principal, alem de um byte offset
 * de 2 bytes para a posicao desse mesmo registro no arquivo
 * principal de registros.
 *
 * O arquivo de indice mantem uma pilha de espacos disponiveis
 * para evitar fragmentacao externa. Cada no da arvore de
 * indices possui tamanho fixo (34 bytes, no caso da arvore de
 * ordem 5).
 * 
 */

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <time.h>

#define ORDEM_ARVORE 5

#define INSERTION_ERROR 0
#define INSERTION_NO_PROMOTION 1
#define INSERTION_PROMOTION 2

#define INICIALIZA_ARVORE_ERRO 0 
#define INICIALIZA_ARVORE_SUCESSO 1 
#define INICIALIZA_ARVORE_JAEXISTE 2 

typedef struct no_arvore_b_t
{
	uint8_t n; //numero de elementos na pagina
	uint32_t id[ORDEM_ARVORE -1]; //campos id dos elementos
	uint16_t offset[ORDEM_ARVORE -1]; //campos offset dos elementos
	uint16_t filhas[ORDEM_ARVORE]; //RRN das paginas filhas dessa pagina
}no_arvore_b_t;

uint16_t Carregar_Pagina(FILE *arq_arv, no_arvore_b_t *pagina); //Carrega uma pagina da arvore B contida em arq_arv, assumindo que o ponteiro de arquivo estah no inicio de um registro de pagina. Carrega a pagina na struct correspondente e retorna o RRN da pagina carregada
uint16_t Criar_Pagina(FILE *arq_arv, no_arvore_b_t nova); //Cria nova pagina no arquivo, inserindo-a no primeiro espaco vazio da PED ou no final do arquivo, caso PED esteja vazia
uint16_t Escrever_Pagina(FILE *arq_arv, no_arvore_b_t pagina); //Escreve uma pagina da arvore B no arquivo arq_arv, assumindo que o ponteiro de arquivo ja esteja na posicao correta
uint16_t Inserir_No_ArvoreB(char *arquivo, uint32_t id, uint16_t offset); //Recebe o id e o offset de um registro, retorna o RRN em que o no foi inserido na arvore
uint8_t Inicializar_ArvoreB(char *arquivo); //Cria uma arvore B com cabecalho: PED = RRN 0 (pilha de espacos disponiveis vazia), Raiz = RRN 0 (Primeiro elemento possui RRN 1. O valor 0 indica que a arvore estah vazia
void Insertion_Split(FILE *arq_arv, no_arvore_b_t *pagina, int pos_insert, uint16_t *promo_r_child, uint32_t *promo_key_id, uint16_t *promo_key_offset); //Funcao que trata a divisao de uma pagina quando a insercao de key_id provoca um overflow nela. O elemento promovido eh devolvido nas variaveis promo_key
uint16_t POS_to_RRN(uint16_t POS);
uint8_t Recursive_Insertion(FILE *arq_arv, uint16_t rrn, uint32_t key_id, uint16_t key_offset, uint16_t *promo_r_child, uint32_t *promo_key_id, uint16_t *promo_key_offset); //Funcao recursiva de insercao, conforme apresentada nos slides da materia
uint16_t RRN_to_POS(uint16_t RRN);
void Print_Arvore_B(char *arquivo); //Imprime todos os nos do arquivo (na sequencia do proprio arquivo)

#endif
