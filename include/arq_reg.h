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

 A LED eh ordenada de forma decrescente, otimizada para a abordagem WORST FIT para reutilizacao dos espacos em branco.
 */

#include <string.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>

typedef struct registro_t {
	uint32_t id;		//Numero identificador do registro (chave primaria)
	char autor[128];	//Nome do autor
	char titulo[256];	//Titulo do trabalho
	char curso[64];		//Nome do curso;
	char tipo[32];		//Tipo do trabalho: TCC, Mestrado, Doutorado...
}registro_t;

registro_t String_to_reg(char *cadeia);
FILE *Abrir_arquivo(char *caminho, char *modo);
int Importar_arquivo_catalogo(char *caminho_catalogo, char *caminho_registro);	//Importa arquivo texto de catalogo com registros

#endif
