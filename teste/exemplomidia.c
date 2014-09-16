#include "atmidia.h"

int main(int argc, char **argv)
{
	ImagemJPEG imagem;
	lerJPEG("teste.jpg", &imagem);
	int i;
	for(i = 0; i < 10; i++)
	{
		printf("%d\n", imagem.buffer[i]);
	}
	return 0;
}