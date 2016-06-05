#include "arq_reg.h"

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
	strcpy(retorno.autor, campo);
	campo = strtok(NULL, ";");
	strcpy(retorno.titulo, campo);
	campo = strtok(NULL, ";");
	strcpy(retorno.curso, campo);
	campo = strtok(NULL, ";");
	strcpy(retorno.tipo, campo);

	return retorno;
}

FILE *Abrir_arquivo(char *caminho, char *modo)
{
	FILE *arquivo;

	arquivo = fopen(caminho,modo);
	if(arquivo == NULL)
        	perror("Falha ao carregar arquivo: ");

	return arquivo;
}

int Importar_arquivo_catalogo(char *caminho_catalogo, char *caminho_registro)
{
	char leitura[1024];
	char c;
	int i;
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
		printf("%x, %s, %s, %s, %s\n\n", lido.id, lido.autor, lido.titulo, lido.curso, lido.tipo);

		//TODO: ESCREVER REZISTRO NO ARQUIVO
	}		
}
