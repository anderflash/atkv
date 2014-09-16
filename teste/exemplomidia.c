#include "atmidia.h"

int main(int argc, char **argv)
{
    // Teste com imagem JPEG
	ATImagemJPEG imagem;
	imagem.nome = "teste.jpg";
	imagem.formato = AT_YCBCR;
	lerJPEG(&imagem);
	printf("largura= %d| altura=%d\n", imagem.largura, imagem.altura);
	int i;
	int tamanho = imagem.largura * imagem.altura;
	for(i = 0; i < tamanho; i++)
	{
		printf("%d %d %d\n", imagem.buffer[3*i],
                             imagem.buffer[3*i+1],
                             imagem.buffer[3*i+2]);
	}
    escreverJPEG("teste2.jpg",&imagem,95);
	destruirImagem(&imagem);

	// Teste com TIFF

	// Teste com Câmera
    ATDispositivoV4L dispositivo;
    dispositivo.caminho = "/dev/video0";
    dispositivo.formato = AT_YUV;
    abrirDispositivo(dispositivo);
    iniciarDispositivo(dispositivo);
    iniciarCaptura(dispositivo);
    // para cada frame
    //   obter o frame
    //   processar o frame

    // finalizar o vídeo


    // Teste com Vídeo



	return 0;
}
