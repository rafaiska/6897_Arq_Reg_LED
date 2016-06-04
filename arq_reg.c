#include "arq_reg.h"

int Importar_arquivo_catalogo(FILE *catalogo, *FILE arq_reg)
{
	char leitura[512];
	char c;
	int i;
	uint16_t header = 0;

	fwrite(&header, sizeof(uint16_t), 1, arq_reg);

	while()
	{
		i = 0;
		while((c = fgetc(catalogo)) != '\n' || c != EOF)
		{
			leitura[i] = c;
			++i;
		}
		leitura[i] = '\0';

		if(strlen(leitura) < 4)
			break;	//acabou o arquivo



	}		
}
