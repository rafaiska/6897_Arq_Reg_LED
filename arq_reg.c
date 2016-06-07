#include "arq_reg.h"

uint16_t Buscar_Registro(char *caminho_registro, uint32_t id)
{
	FILE *arq_reg;
	uint32_t id_c;
	uint16_t tamanho_c;
	int tamanho_arq;

	arq_reg = Abrir_arquivo(caminho_registro, "r");

	fseek(arq_reg, 0, SEEK_END);
	tamanho_arq = ftell(arq_reg);

	fseek(arq_reg, 2, SEEK_SET);
	fread(&tamanho_c, sizeof(uint16_t), 1, arq_reg);
	fread(&id_c, sizeof(uint32_t), 1, arq_reg);

	while(id_c != id && ftell(arq_reg) < tamanho_arq)
	{
		fseek(arq_reg, tamanho_c -4, SEEK_CUR);
		fread(&tamanho_c, sizeof(uint16_t), 1, arq_reg);
		fread(&id_c, sizeof(uint32_t), 1, arq_reg);
	}

	if(id_c == id)
		return(ftell(arq_reg) -6);
	else
		return 0;
}

uint16_t Calcular_Tamanho(registro_t registro)
{
	uint16_t retorno = 0;

	retorno += 5; //5 bytes para os separadores '|'
	retorno += sizeof(uint32_t); //4 bytes para registro_t.id (uint32_t)
	retorno += strlen(registro.autor);
	retorno += strlen(registro.titulo);
	retorno += strlen(registro.curso);
	retorno += strlen(registro.tipo);

	return retorno;
}

int Inserir_Registro(char *caminho_registro, registro_t registro)
{
	FILE *arq_reg;
	uint16_t first_led;
	int retorno;

	arq_reg = Abrir_arquivo(caminho_registro, "a+");
	fseek(arq_reg, 0, SEEK_SET);
	fread(&first_led, sizeof(uint16_t), 1, arq_reg);

	if(first_led == 0)
		retorno = Inserir_Registro_Final(arq_reg, registro);
	else
	{
		//TODO: Percorrer a LED e inserir no primeiro espaco da lista. Atualizar a lista depois
	}

	fclose(arq_reg);
	return retorno;
}

int Inserir_Registro_Final(FILE *arq_reg, registro_t registro)
{
	uint16_t tamanho;
	int retorno;

	tamanho = Calcular_Tamanho(registro);
	fseek(arq_reg, 0, SEEK_END);
	retorno = ftell(arq_reg);
	fwrite(&tamanho, sizeof(uint16_t), 1, arq_reg);
	fwrite(&(registro.id), sizeof(uint32_t), 1, arq_reg);
	fputc('|', arq_reg);
	fwrite(registro.autor, sizeof(char), strlen(registro.autor), arq_reg);
	fputc('|', arq_reg);
	fwrite(registro.titulo, sizeof(char), strlen(registro.titulo), arq_reg);
	fputc('|', arq_reg);
	fwrite(registro.curso, sizeof(char), strlen(registro.curso), arq_reg);
	fputc('|', arq_reg);
	fwrite(registro.tipo, sizeof(char), strlen(registro.tipo), arq_reg);
	fputc('|', arq_reg);

	return retorno;
}

registro_t String_to_reg(char *cadeia)
{
	registro_t retorno;
	char *campo;

	if(cadeia == NULL)
	{
		printf("Erro ao converter arquivo de catalogo: linha vazia!\n");
		retorno.id = 0;
		return retorno;	
	}

	campo = strtok(cadeia, ";");
	retorno.id = (uint32_t) (atoi(campo));
	campo = strtok(NULL, ";");
	strcpy(retorno.autor, campo+1);
	campo = strtok(NULL, ";");
	strcpy(retorno.titulo, campo+1);
	campo = strtok(NULL, ";");
	strcpy(retorno.curso, campo+1);
	campo = strtok(NULL, "\r");
	strcpy(retorno.tipo, campo+1);

	return retorno;
}

FILE *Abrir_arquivo(char *caminho, char *modo)
{
	FILE *arquivo;
	uint16_t first_led = 0;

	if(modo[0] == 'r')
	{
		arquivo = fopen(caminho,modo);
		if(arquivo == NULL)
		{
			printf("Falha ao carregar arquivo em '%s'\n", caminho);
        		perror("Mensagem do sistema");
			exit(1);
		}
	}
	else
	{
		arquivo = fopen(caminho,"r");

		//Arquivo de registros com LED nao existe. Cria um arquivo vazio
		if(arquivo == NULL)	
		{
			arquivo = fopen(caminho, modo);
			fwrite(&first_led, sizeof(uint16_t), 1, arquivo);
		}
		//Abre o arquivo existente em modo de escrita ou append
		else
		{
			fclose(arquivo);
			arquivo = fopen(caminho, modo);
		}
	}

	return arquivo;
}

int Importar_arquivo_catalogo(char *caminho_catalogo, char *caminho_registro)
{
	char leitura[1024];
	char c;
	int i;
	int n_reg = 0;
	registro_t lido;
	FILE *catalogo;

	catalogo = Abrir_arquivo(caminho_catalogo, "r");

	while(1)
	{
		i = 0;
		while((c = fgetc(catalogo)) != '\n' && c != EOF)
		{
			leitura[i] = c;
			++i;
		}
		leitura[i] = '\0';

		if(strlen(leitura) < 4)
			break;	//acabou o arquivo

		lido = String_to_reg(leitura);

		//TODO: ESCREVER REZISTRO NO ARQUIVO
		i = Inserir_Registro(caminho_registro, lido);
		printf("Inserido registro na posicao %d do arquivo.\n", i);
		++n_reg;
	}

	return n_reg;
}
