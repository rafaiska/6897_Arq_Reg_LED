#include "arvore_b.h"

uint16_t Carregar_Pagina(FILE *arq_arv, no_arvore_b_t *pagina)
{
	uint16_t RRN;
	int i;
	
	RRN = POS_to_RRN((uint16_t)(ftell(arq_arv)));

	fread(pagina, sizeof(no_arvore_b_t), 1, arq_arv);

	return RRN;
}

uint16_t Criar_Pagina(FILE *arq_arv, no_arvore_b_t nova)
{
	uint16_t topo_pilha;
	uint16_t pos_insert;
	uint16_t RRN;
	int i;

	fseek(arq_arv, 0, SEEK_SET);
	fread(&topo_pilha, sizeof(uint16_t), 1, arq_arv);

	if(topo_pilha == 0) //PED estah vazia
	{
		fseek(arq_arv, 0, SEEK_END);
		RRN = POS_to_RRN((uint16_t)(ftell(arq_arv)));
	}
	else
	{
		RRN = topo_pilha;
		pos_insert = RRN_to_POS(topo_pilha);
		fseek(arq_arv, pos_insert +1, SEEK_SET); //soma 1 para contabilizar o caractere '*'
		fread(&topo_pilha, sizeof(uint16_t), 1, arq_arv);
		fseek(arq_arv, 0, SEEK_SET);
		fwrite(&topo_pilha, sizeof(uint16_t), 1, arq_arv);
		fseek(arq_arv, pos_insert, SEEK_SET);
	}

	RRN = Escrever_Pagina(arq_arv, nova);

	return RRN;
}

uint16_t Escrever_Pagina(FILE *arq_arv, no_arvore_b_t pagina)
{
	uint16_t RRN;
	
	RRN = POS_to_RRN((uint16_t)(ftell(arq_arv)));

	fwrite(&pagina, sizeof(no_arvore_b_t), 1, arq_arv);

	return RRN;
}

uint16_t Inserir_No_ArvoreB(char *arquivo, uint32_t id, uint16_t offset)
{
	uint16_t raiz;
	uint16_t raiz_pos;
	uint16_t nova_rrn;
	uint16_t promo_r_child;
	uint32_t promo_key_id;
	uint16_t promo_key_offset;
	uint8_t flag;
	FILE *arq_arv;
	no_arvore_b_t nova;
	no_arvore_b_t raiz_a;

	arq_arv = fopen(arquivo, "r+");
	fseek(arq_arv, 2, SEEK_SET);
	fread(&raiz, sizeof(uint16_t), 1, arq_arv);

	if(raiz == 0) //o arquivo de indices estah vazio
	{
		nova.n = 1;
		nova.id[0] = id;
		nova.offset[0] = offset;
		nova.filhas[0] = 0;
		nova.filhas[1] = 0;
		nova_rrn = (Criar_Pagina(arq_arv, nova));
		fseek(arq_arv, 2, SEEK_SET);
		fwrite(&nova_rrn, sizeof(uint16_t), 1, arq_arv);
	}
	else
	{
		flag = Recursive_Insertion(arq_arv, raiz, id, offset, &promo_r_child, &promo_key_id, &promo_key_offset);
		switch(flag)
		{
			case INSERTION_PROMOTION:
					raiz_a.filhas[0] = raiz;
					raiz_a.id[0] = promo_key_id;
					raiz_a.offset[0] = promo_key_offset;
					raiz_a.filhas[1] = promo_r_child;
					raiz = Criar_Pagina(arq_arv, raiz_a);
					fseek(arq_arv, 2, SEEK_SET);
					fwrite(&raiz, sizeof(uint16_t), 1, arq_arv);
				break;
			case INSERTION_NO_PROMOTION: break;
			case INSERTION_ERROR: break;
		}
	}

	fclose(arq_arv);
	return nova_rrn;
}

void Insertion_Split(FILE *arq_arv, no_arvore_b_t *pagina, int pos_insert, uint16_t *promo_r_child, uint32_t *promo_key_id, uint16_t *promo_key_offset)
{
	no_arvore_b_t nova_pagina;
	int metade, i;

	nova_pagina.n = 0;
	metade = ORDEM_ARVORE /2;

	if(pos_insert == metade)
	{
		for(i= metade; i< ORDEM_ARVORE -1; ++i)
		{
			nova_pagina.id[i -metade] = pagina->id[i];
			nova_pagina.offset[i -metade] = pagina->offset[i];
			nova_pagina.filhas[i -metade +1] = pagina->filhas[i+1];
			--(pagina->n);
			++(nova_pagina.n);
		}
		nova_pagina.filhas[0] = *promo_r_child;
	}
	else if(pos_insert < metade)
	{
		for(i=metade; i< ORDEM_ARVORE -1; ++i)
		{
			nova_pagina.id[i -metade] = pagina->id[i];
			nova_pagina.offset[i -metade] = pagina->offset[i];
			nova_pagina.filhas[i -metade +1] = pagina->filhas[i+1];
			--(pagina->n);
			++(nova_pagina.n);
		}

		for(i=ORDEM_ARVORE -2; i> pos_insert; --i)
		{
			pagina->id[i] = pagina->id[i-1];
			pagina->offset[i] = pagina->offset[i-1];
			pagina->filhas[i+1] = pagina->filhas[i];
		}
		pagina->id[pos_insert] = *promo_key_id;
		pagina->offset[pos_insert] = *promo_key_offset;
		pagina->filhas[pos_insert +1] = *promo_r_child;
		*promo_key_id = pagina->id[metade];
		*promo_key_offset = pagina->offset[metade];
		nova_pagina.filhas[0] = pagina->filhas[metade +1];
	}
	else
	{
		for(i=metade +1; i< ORDEM_ARVORE -1; ++i)
		{
			nova_pagina.id[i -metade -1] = pagina->id[i];
			nova_pagina.offset[i -metade -1] = pagina->offset[i];
			nova_pagina.filhas[i -metade] = pagina->filhas[i+1];
			--(pagina->n);
			++(nova_pagina.n);
		}

		pos_insert -= (metade +1);
		for(i=ORDEM_ARVORE -2; i> pos_insert; --i)
		{
			nova_pagina.id[i] = nova_pagina.id[i-1];
			nova_pagina.offset[i] = nova_pagina.offset[i-1];
			nova_pagina.filhas[i+1] = nova_pagina.filhas[i];
		}
		nova_pagina.id[pos_insert] = *promo_key_id;
		nova_pagina.offset[pos_insert] = *promo_key_offset;
		nova_pagina.filhas[pos_insert +1] = *promo_r_child;
		*promo_key_id = pagina->id[metade];
		*promo_key_offset = pagina->offset[metade];
		nova_pagina.filhas[0] = pagina->filhas[metade +1];
		--(pagina->n);
		++(nova_pagina.n);
	}

	*promo_r_child = Criar_Pagina(arq_arv, nova_pagina);
}

uint8_t Inicializar_ArvoreB(char *arquivo)
{
	FILE *nova;
	uint16_t ped = 0;
	uint16_t raiz = 0;

	nova = fopen(arquivo, "r");
	if(nova == NULL)
	{
		nova = fopen(arquivo, "w");
		if(nova == NULL)
			return INICIALIZA_ARVORE_ERRO;

		fwrite(&ped, sizeof(uint16_t), 1, nova);
		fwrite(&raiz, sizeof(uint16_t), 1, nova);
		fclose(nova);
		return INICIALIZA_ARVORE_JAEXISTE;
	}

	fclose(nova);
	return INICIALIZA_ARVORE_SUCESSO;
}

uint16_t POS_to_RRN(uint16_t POS)
{	
	POS -= 4; //4 bytes do cabecalho
	POS /= sizeof(no_arvore_b_t); //Divide pelo tamanho dos registros
	POS += 1; //Os RRN das paginas comecam em 1
	return POS;
}

uint16_t RRN_to_POS(uint16_t RRN)
{	
	--RRN; //Os RRN das paginas comecam em 1
	RRN *= sizeof(no_arvore_b_t); //Multiplica pelo tamanho dos registros
	RRN += 4; //4 bytes do cabecalho
	return RRN;
}

uint8_t Recursive_Insertion(FILE *arq_arv, uint16_t rrn, uint32_t key_id, uint16_t key_offset, uint16_t *promo_r_child, uint32_t *promo_key_id, uint16_t *promo_key_offset)
{
	no_arvore_b_t pagina, nova_pagina;
	int i, pos_insert, chamada;

	if(rrn == 0)
	{
		*promo_key_id = key_id;
		*promo_key_offset = key_offset;
		*promo_r_child = 0;
		return INSERTION_PROMOTION; 
	}

	fseek(arq_arv, RRN_to_POS(rrn), SEEK_SET);
	Carregar_Pagina(arq_arv, &pagina);

	i=0;
	while(i< pagina.n && key_id > pagina.id[i])
		++i;

	if(i< pagina.n)
		if(key_id == pagina.id[i])
			return INSERTION_ERROR;

	pos_insert = i;
	chamada = Recursive_Insertion(arq_arv, pagina.filhas[pos_insert], key_id, key_offset, promo_r_child, promo_key_id, promo_key_offset);

	switch(chamada)
	{
		case INSERTION_PROMOTION:
			if(pagina.n < ORDEM_ARVORE -1)
			{
				//Abrir espaco para novo elemento na pagina
				for(i=ORDEM_ARVORE -2; i> pos_insert; --i)
				{
					pagina.id[i] = pagina.id[i-1];
					pagina.offset[i] = pagina.offset[i-1];
					pagina.filhas[i+1] = pagina.filhas[i];
				}

				pagina.id[pos_insert] = *promo_key_id;
				pagina.offset[pos_insert] = *promo_key_offset;
				pagina.filhas[pos_insert +1] = *promo_r_child;
				++(pagina.n);
				fseek(arq_arv, RRN_to_POS(rrn), SEEK_SET);
				Escrever_Pagina(arq_arv, pagina);
				return INSERTION_NO_PROMOTION;
			}
			else
			{
				Insertion_Split(arq_arv, &pagina, pos_insert, promo_r_child, promo_key_id, promo_key_offset);
				fseek(arq_arv, RRN_to_POS(rrn), SEEK_SET);
				Escrever_Pagina(arq_arv, pagina);
				return INSERTION_PROMOTION;			
			}
		break;
		case INSERTION_NO_PROMOTION:
			return INSERTION_NO_PROMOTION;
		break;
		case INSERTION_ERROR:
			return INSERTION_ERROR;
		break;
	}
}

void Print_Arvore_B(char *arquivo)
{
	int fim, rrn, i;
	uint16_t raiz;
	FILE *arq_arv;
	no_arvore_b_t pagina;

	arq_arv = fopen(arquivo, "r");
	fseek(arq_arv, 0, SEEK_END);
	fim = ftell(arq_arv);
	fseek(arq_arv, 2, SEEK_SET);
	fread(&raiz, sizeof(uint16_t), 1, arq_arv);

	while(ftell(arq_arv) != fim)
	{
		rrn = Carregar_Pagina(arq_arv, &pagina);
		if(rrn == raiz)
			printf("====\nRAIZ\n====\n");
		printf("Pagina de RRN %d:\n", rrn);

		for(i=0; i< pagina.n; ++i)
			printf("\tChave de ID %d, offset 0x%04x\n", pagina.id[i], pagina.offset[i]);
		printf("\tPaginas Filhas: ");
		for(i=0; i< pagina.n +1; ++i)
			printf("%d ", pagina.filhas[i]);
		printf("\n\n");
	}

	fclose(arq_arv);
}

int main()
{
	char fileh[] = "./res/arvreb.btr\0";
	FILE *arvre;
	uint8_t flag;
	int i;

	srand(time(NULL));
	flag = Inicializar_ArvoreB(fileh);
	if(flag == INICIALIZA_ARVORE_ERRO)
		return 1;

	for(i=0; i< 100; ++i)
		Inserir_No_ArvoreB(fileh, rand() %100, rand() %0xffff);

	Print_Arvore_B(fileh);
	return 0;
}

