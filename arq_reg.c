#include "arq_reg.h"

uint16_t Buscar_Registro_Com_Indice(char *caminho_registro, uint32_t id, registro_t *registro)
{
	FILE *arq_reg;
	char caminho_indice[128];
	uint16_t pos;
	int i;
	char c;

	Get_Caminho_Indice(caminho_registro, caminho_indice);
	pos = Buscar_Registro_ArvoreB(caminho_indice, id);

	if(pos != 0 && registro != NULL)
	{
		arq_reg = fopen(caminho_registro, "r");
		fseek(arq_reg, pos +2, SEEK_SET); //2 bytes do tamanho

		fread(&(registro->id), sizeof(uint32_t), 1, arq_reg);
		fgetc(arq_reg);

		i=0;
		while((c = fgetc(arq_reg)) != '|')
		{
			registro->autor[i] = c;
			++i;	
		}
		registro->autor[i] = '\0';

		i=0;
		while((c = fgetc(arq_reg)) != '|')
		{
			registro->titulo[i] = c;
			++i;	
		}
		registro->titulo[i] = '\0';

		i=0;
		while((c = fgetc(arq_reg)) != '|')
		{
			registro->curso[i] = c;
			++i;	
		}
		registro->curso[i] = '\0';
		i=0;

		while((c = fgetc(arq_reg)) != '|')
		{
			registro->tipo[i] = c;
			++i;	
		}
		registro->tipo[i] = '\0';
		fclose(arq_reg);
	}

	return pos;
}

uint16_t Buscar_Registro(char *caminho_registro, uint32_t id, registro_t *registro)
{
	FILE *arq_reg;
	uint32_t id_c;
	uint16_t tamanho_c;
	int tamanho_arq;
	uint16_t retorno = 2;
	char is_vazio; //variavel para verificar se registro eh vazio (asterisco *)
	int i;
	char c;

	arq_reg = Abrir_arquivo(caminho_registro, "r");

	fseek(arq_reg, 0, SEEK_END);
	tamanho_arq = ftell(arq_reg);

	fseek(arq_reg, 2, SEEK_SET);
	fread(&tamanho_c, sizeof(uint16_t), 1, arq_reg);
	fread(&id_c, sizeof(uint32_t), 1, arq_reg);
	fseek(arq_reg, tamanho_c -4, SEEK_CUR);

	while(id_c != id && ftell(arq_reg) < tamanho_arq)
	{
		retorno = ftell(arq_reg);
		fread(&tamanho_c, sizeof(uint16_t), 1, arq_reg);
		is_vazio = fgetc(arq_reg);

		if(is_vazio == '*')
		{
			id_c = id+1; // para que id_c seja diferente de id
			fseek(arq_reg, tamanho_c -1, SEEK_CUR);
		}
		else
		{
			fseek(arq_reg, -1, SEEK_CUR);
			fread(&id_c, sizeof(uint32_t), 1, arq_reg);
			fseek(arq_reg, tamanho_c -4, SEEK_CUR);
		}
	}

	if(id_c != id)
		retorno = 0;
	else if(registro != NULL)
	{
		fseek(arq_reg, retorno +7, SEEK_SET);
		registro->id = id_c;

		i=0;
		while((c = fgetc(arq_reg)) != '|')
		{
			registro->autor[i] = c;
			++i;	
		}
		registro->autor[i] = '\0';

		i=0;
		while((c = fgetc(arq_reg)) != '|')
		{
			registro->titulo[i] = c;
			++i;	
		}
		registro->titulo[i] = '\0';

		i=0;
		while((c = fgetc(arq_reg)) != '|')
		{
			registro->curso[i] = c;
			++i;	
		}
		registro->curso[i] = '\0';
		i=0;

		while((c = fgetc(arq_reg)) != '|')
		{
			registro->tipo[i] = c;
			++i;	
		}
		registro->tipo[i] = '\0';
	}


	fclose(arq_reg);
	return retorno;
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

uint16_t Inserir_ED(FILE *arq_reg, uint16_t posicao)
{
	uint16_t tamanho, tamanho_ant, tamanho_prox, offset, offset_ant, offset_prox;
	char c;
	int nivel = 0;

	fseek(arq_reg, posicao, SEEK_SET);
	fread(&tamanho, sizeof(uint16_t), 1, arq_reg);
	c = fgetc(arq_reg);

	if(c != '*')
	{
		printf("Erro: Inserindo na LED bloco de registro que nao eh vazio");
		exit(2);
	}

	fseek(arq_reg, 0, SEEK_SET);
	fread(&offset_ant, sizeof(uint16_t), 1, arq_reg);

	if(offset_ant == 0) //LED VAZIA
	{
		fseek(arq_reg, 0, SEEK_SET);
		fwrite(&posicao, sizeof(uint16_t), 1, arq_reg);
		fseek(arq_reg, posicao +3, SEEK_SET);
		offset_prox = 0;
		fwrite(&offset_prox, sizeof(uint16_t), 1, arq_reg); //ponteiro para o FIM DA LISTA
	}
	else
	{
		fseek(arq_reg, offset_ant, SEEK_SET);
		fread(&tamanho_ant, sizeof(uint16_t), 1, arq_reg);

		if(tamanho_ant < tamanho) //INSERIR NA CABECA
		{
			fseek(arq_reg, 0, SEEK_SET);
			fwrite(&posicao, sizeof(uint16_t), 1, arq_reg);
			fseek(arq_reg, posicao +3, SEEK_SET);
			fwrite(&offset_ant, sizeof(uint16_t), 1, arq_reg);
			return 0;
		}

		fseek(arq_reg, 1, SEEK_CUR); //PULA O CARACTERE '*'
		fread(&offset_prox, sizeof(uint16_t), 1, arq_reg);
		if(offset_prox != 0)
		{
			fseek(arq_reg, offset_prox, SEEK_SET);
			fread(&tamanho_prox, sizeof(uint16_t), 1, arq_reg);
		}
		++nivel;

		while(tamanho_ant > tamanho && offset_prox != 0)
		{
			offset_ant = offset_prox;
			tamanho_ant = tamanho_prox;
			fseek(arq_reg, offset_ant, SEEK_SET);
			fseek(arq_reg, 3, SEEK_CUR); //PULA O TAMANHO E O CARACTERE '*'
			fread(&offset_prox, sizeof(uint16_t), 1, arq_reg);
			if(offset_prox != 0)
			{
				fseek(arq_reg, offset_prox, SEEK_SET);
				fread(&tamanho_prox, sizeof(uint16_t), 1, arq_reg);
			}
		}

		fseek(arq_reg, offset_ant +3, SEEK_SET);
		fwrite(&posicao, sizeof(uint16_t), 1, arq_reg);
		fseek(arq_reg, posicao +3, SEEK_SET);		
		fwrite(&offset_prox, sizeof(uint16_t), 1, arq_reg);
		++nivel;
	}
	return nivel;
}

int Inserir_Registro(char *caminho_registro, registro_t registro)
{
	FILE *arq_reg;
	uint16_t first_led;
	uint16_t nova_first_led;
	uint16_t tamanho, tamanho_first_led, tamanho_novo_branco;
	int retorno;
	char caminho_indice[128];

	tamanho = Calcular_Tamanho(registro);
	tamanho_first_led = 0;

	arq_reg = Abrir_arquivo(caminho_registro, "r+");
	fseek(arq_reg, 0, SEEK_SET);
	fread(&first_led, sizeof(uint16_t), 1, arq_reg);

	if(first_led != 0)
	{
		fseek(arq_reg, first_led, SEEK_SET);
		fread(&tamanho_first_led, sizeof(uint16_t), 1, arq_reg);
	}

	if(first_led == 0 || tamanho_first_led < tamanho)
		retorno = Inserir_Registro_Final(arq_reg, registro);
	else
	{
		fgetc(arq_reg); //PULA O *
		fread(&nova_first_led, sizeof(uint16_t), 1, arq_reg);
		fseek(arq_reg, 0, SEEK_SET);
		fwrite(&nova_first_led, sizeof(uint16_t), 1, arq_reg); //grava no cabecalho o novo primeiro elemento da LED

		fseek(arq_reg, first_led, SEEK_SET);
		tamanho_novo_branco = tamanho_first_led - tamanho -2; //2 bytes para tamanho do novo registro

		if(tamanho_novo_branco > 8) //cria novo espaco em branco e insere na LED
		{
			fwrite(&tamanho_novo_branco, sizeof(uint16_t), 1, arq_reg);
			Inserir_ED(arq_reg, first_led);
			fseek(arq_reg, first_led +2 +tamanho_novo_branco, SEEK_SET);
		}
		else // cria fragmentacao interna no novo registro
			tamanho = tamanho_first_led;

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
	}

	fclose(arq_reg);
	Get_Caminho_Indice(caminho_registro, caminho_indice);
	Inserir_No_ArvoreB(caminho_indice, registro.id, (uint16_t)(retorno));
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

uint16_t Remover_Registro(char *caminho_registro, uint32_t id)
{
	FILE *arq_reg;
	uint16_t posicao;
	char asteristico = '*';
	char caminho_indice[128];

	//posicao = Buscar_Registro(caminho_registro, id, NULL);
	posicao = Buscar_Registro_Com_Indice(caminho_registro, id, NULL);

	if(posicao == 0)
		return 0;

	arq_reg = Abrir_arquivo(caminho_registro, "r+");
	fseek(arq_reg, posicao +2, SEEK_SET); //Os dois bytes que sinalizam o tamanho do bloco sao mantidos
	fwrite(&asteristico, sizeof(char), 1, arq_reg); //Esse caractere indica bloco de registro em branco

	Inserir_ED(arq_reg, posicao);

	fclose(arq_reg);
	Get_Caminho_Indice(caminho_registro, caminho_indice);
	Remocao_Arvore_B(caminho_indice, id);	
	return posicao;
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
	char caminho_indice[128];

	if(strcmp("r", modo) == 0)
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
		if(arquivo == NULL && strcmp("r+", modo) == 0)	
		{
			arquivo = fopen(caminho, "w");
			fwrite(&first_led, sizeof(uint16_t), 1, arquivo);
			fclose(arquivo);
			arquivo = fopen(caminho, modo);
			Get_Caminho_Indice(caminho, caminho_indice);
			Inicializar_ArvoreB(caminho_indice);
		}
		else if(arquivo == NULL)	
		{
			arquivo = fopen(caminho, modo);
			fwrite(&first_led, sizeof(uint16_t), 1, arquivo);
		}
		//Abre o arquivo existente em modo de escrita, r+ ou append
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

	fclose(catalogo);
	return n_reg;
}

void Imprimir_Registro(registro_t registro)
{
	printf("Numero de registro: %d\nAutor: %s\nTitulo: %s\nCurso: %s\nTipo: %s\n", registro.id, registro.autor, registro.titulo, registro.curso, registro.tipo);
}

void Get_Caminho_Indice(char *arq_reg, char *arq_idx)
{
	int i=0;

	if(arq_reg[0] == '.')
	{
		arq_idx[0] = '.';
		++i;
	}

	while(arq_reg[i] != '.' && arq_reg[i] != '\0')
	{
		arq_idx[i] = arq_reg[i];
		++i;
	}
	arq_idx[i] = '\0';

	strcat(arq_idx, ".idx\0");
}
