#include "arq_reg.h"

uint8_t ECHO = 0;
char CAMINHO_REGISTRO[256] = "./res/rezistro.rez\0";

void Print_Menu()
{
	printf("Escolha uma opcao:\n");
	printf("\t1. Importar arquivo de catalogo\n");
	printf("\t2. Adicionar registro ao arquivo\n");
	printf("\t3. Apagar registro do arquivo\n");
	printf("\t4. Buscar registro no arquivo\n");
	printf("\t5. Sair\n");
}

void Opcao1()
{
	char CAMINHO_CATALOGO[256];
	char c;
	int i = 0;

	getchar(); //LIMPA O BUFFER
	printf("Qual o caminho para o arquivo de catalogo? (Caso deixe em branco, sera usado o caminho padrao: './res/catalogo.txt')\n");
	while((c = getchar()) != '\n')
	{
		CAMINHO_CATALOGO[i] = c;
		++i;
	}
	CAMINHO_CATALOGO[i] = '\0';

	if(strlen(CAMINHO_CATALOGO) == 0)
		i = Importar_arquivo_catalogo("./res/catalogo.txt", CAMINHO_REGISTRO);
	else
		i = Importar_arquivo_catalogo(CAMINHO_CATALOGO, CAMINHO_REGISTRO);

	printf("\nArquivo importado com sucesso: %d registros gravados em '%s'.\n\n", i, CAMINHO_REGISTRO);
}

void Opcao3()
{
	uint32_t id;
	uint32_t pos;

	printf("Qual id do registro a ser apagado? ");
	scanf("%d", &id);

	pos = Remover_Registro(CAMINHO_REGISTRO, id);

	if(pos != 0)
	{
		printf("\nRegistro apagado na posicao %d a partir do inicio do arquivo\n\n", pos);
	}
	else
	{
		printf("Registro nao encontrado!\n\n");
	}
}

void Opcao4()
{
	uint32_t id;
	uint32_t pos;

	printf("Qual id do registro a ser buscado? ");
	scanf("%d", &id);

	pos = Buscar_Registro(CAMINHO_REGISTRO, id);

	if(pos != 0)
	{
		printf("\nRegistro encontrado na posicao %d a partir do inicio do arquivo\n\n", pos);
	}
	else
	{
		printf("Registro nao encontrado!\n\n");
	}
}

int main(int argc, char *argv[])
{
	int opcao=0;
	int i;

	for(i=0; i<argc; ++i)
	{
		if(!strcmp(argv[i],"--echo"))
			ECHO = 1;
		else if(!strcmp(argv[i],"-r") && argc > i+1)
			strcpy(CAMINHO_REGISTRO, argv[i+1]);
	}

	printf("Programa editor de arquivos binarios de registros com lista de espacos disponiveis (LED)\n");
	printf("Arquivo de registro selecionado para edicao: %s\n\n", CAMINHO_REGISTRO);

	while(opcao != 5)
	{
		Print_Menu();
		scanf("%d", &opcao);
		
		switch(opcao)
		{
			case 1: Opcao1(); break;
			case 2: break;
			case 3: Opcao3(); break;
			case 4: Opcao4(); break;
			case 5: break;
			default: printf("Opcao invalida!\n"); break;
		}
	}

	return 0;
}
