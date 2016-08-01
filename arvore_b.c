#include "arvore_b.h"

uint16_t Criar_Pagina(FILE *arq_arv, no_arvore_b_t nova)
{
	uint16_t topo_pilha;
	uint16_t RRN;
	int i;

	fseek(arq_arv, 0, SEEK_SET);
	fread(&topo_pilha, sizeof(uint16_t), 1, arq_arv);

	if(topo_pilha == 0) //PED estah vazia
	{
		fseek(arq_arv, 0, SEEK_END);
		RRN = ftell(arq_arv);
		RRN -= 4;
		RRN /= (ORDEM_ARVORE *2 + (ORDEM_ARVORE -1) *6);
		RRN += 1;

		fwrite(&(nova.n), sizeof(uint8_t), 1, arq_arv);
		for(i=0; i< ORDEM_ARVORE; ++i)
			fwrite(nova.filhas +i, sizeof(uint16_t), 1, arq_arv);
		for(i=0; i< ORDEM_ARVORE -1; ++i)
		{
			fwrite(nova.id +i, sizeof(uint32_t), 1, arq_arv);
			fwrite(nova.offset +i, sizeof(uint16_t), 1, arq_arv);	
		}	
	}
	else
	{
		//TODO: Percorrer a PED
	}

	return RRN;
}

uint16_t Inserir_No_ArvoreB(char *arquivo, uint32_t id, uint16_t offset)
{
	uint16_t raiz;
	uint16_t nova_rrn;
	FILE *arq_arv;
	no_arvore_b_t nova;

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
		//TODO: Inserir na arvore B
	}

	fclose(arq_arv);
	return nova_rrn;
}

FILE *Inicializar_ArvoreB(char *arquivo)
{
	FILE *nova;
	uint16_t ped = 0;
	uint16_t raiz = 0;

	nova = fopen(arquivo, "w");
	fwrite(&ped, sizeof(uint16_t), 1, nova);
	fwrite(&raiz, sizeof(uint16_t), 1, nova);
	fclose(nova);
	nova = fopen(arquivo, "r+");

	return nova;
}

int main()
{
	char fileh[] = "./res/arvreb.btr\0";
	FILE *arvre;

	arvre = fopen(fileh, "r+");
	if(arvre == NULL)
		arvre = Inicializar_ArvoreB(fileh);

	Inserir_No_ArvoreB(fileh, 24, 666);

	fclose(arvre);
	return 0;
}
