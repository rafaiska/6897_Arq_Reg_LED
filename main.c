#include "arq_reg.h"

uint8_t ECHO = 0;
char CAMINHO_REGISTRO[256] = "./res/rezistro.rez\0";

void Print_Menu()
{
	printf("\nEscolha uma opcao:\n");
	printf("\t1. Importar arquivo de catalogo\n");
	printf("\t2. Adicionar registro ao arquivo\n");
	printf("\t3. Apagar registro do arquivo\n");
	printf("\t4. Buscar registro no arquivo\n");
	printf("\t5. Imprimir paginas do arquivo de indice\n");
	printf("\t6. Sair\n");
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

	printf("\nArquivo importado com sucesso: %d registros gravados em '%s'.\n", i, CAMINHO_REGISTRO);
}

void Opcao2()
{
	registro_t novo;

	printf("\nQual o numero identificador do novo registro? ");
	scanf("%d", &(novo.id));
	getchar(); //limpa o buffer de entrada
	printf("Qual o nome do autor? ");
	gets(novo.autor);
	printf("Qual o titulo do trabalho? ");
	gets(novo.titulo);
	printf("Qual o curso do autor? ");
	gets(novo.curso);
	printf("Qual o tipo do trabalho? ");
	gets(novo.tipo);

	Inserir_Registro(CAMINHO_REGISTRO, novo); 
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
		printf("\nRegistro apagado na posicao %d a partir do inicio do arquivo\n", pos);
	}
	else
	{
		printf("\nRegistro nao encontrado!\n");
	}
}

void Opcao4()
{
	uint32_t id;
	uint32_t pos;
	registro_t buscado;

	printf("Qual id do registro a ser buscado? ");
	scanf("%d", &id);

	//pos = Buscar_Registro(CAMINHO_REGISTRO, id, &buscado);
	pos = Buscar_Registro_Com_Indice(CAMINHO_REGISTRO, id, &buscado);

	if(pos != 0)
	{
		printf("\nRegistro encontrado na posicao %d (0x%x) a partir do inicio do arquivo\n", pos, pos);
		Imprimir_Registro(buscado);
		printf("\n");
	}
	else
	{
		printf("\nRegistro nao encontrado!\n");
	}
}

void Opcao5()
{
	char caminho_indice[128];

	Get_Caminho_Indice(CAMINHO_REGISTRO, caminho_indice);
	printf("\n==================\nARQUIVO DE INDICES\n==================\n");
	printf("Armazenado em: %s\n\n", caminho_indice);
	Print_Arvore_B(caminho_indice);
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

	while(opcao != 6)
	{
		Print_Menu();
		scanf("%d", &opcao);
		
		switch(opcao)
		{
			case 1: Opcao1(); break;
			case 2: Opcao2(); break;
			case 3: Opcao3(); break;
			case 4: Opcao4(); break;
			case 5: Opcao5(); break;
			case 6: break;
			default: printf("Opcao invalida!\n"); break;
		}
	}

	return 0;
}
