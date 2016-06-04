#ifndef ARQ_REG_H
#define ARQ_REG_H

/* ======== UNIVERSIDADE ESTADUAL DE MARINGA ========== *
 * ============== CENTRO DE TECNOLOGIA ================ *
 * ========== DEPARTAMENTO DE INFORMATICA ============= *
 * ===== 6897 - ORGANIZACAO E RECUPERACAO DE DADOS ==== *
 * ========== PROFESSORA VALERIA FELTRIM ============== *
 * ================== TRABALHO 1 ====================== *
 * Alunos:	Chen Po Hsiang		RA		*
 * 		Rafael Cortez Sanchez	RA 82357	*
 
 O cabecalho arq_reg.h contem funcoes para a manipulacao de um arquivo de registros com lista de espacos disponiveis (LED). Os registros possuem tamanho variavel, e o cabecalho do arquivo eh um inteiro de 16 bits sem sinal que sinaliza o offset do primeiro espaco disponivel no arquivo. Caso nao existam espacos vazios, esse inteiro assume valor 0 (zero).

 Os registros armazenam informacoes sobre trabalhos academicos, mapeados pelos atributos id, autor, titulo, curso e tipo de trabalho.
 */

#include <string.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>

typedef struct registro_t {
	uint32_t id;							//Numero identificador do registro (chave primaria)
	char autor[128];						//Nome do autor
	char titulo[128];						//Titulo do trabalho
	char curso[64];							//Nome do curso;
	enum trabalho_t {TRABALHO_TCC, TRABALHO_MESTRADO, TRABALHO_DOUTORADO};	//Tipo do trabalho
}registro_t;

int Importar_arquivo_catalogo(FILE *catalogo, *FILE arq_reg);	//Importa arquivo texto de catalogo com registros

#endif
