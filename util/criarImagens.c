#define LARGURA_MAXIMA 1024
#define ALTURA_MAXIMA 1024
#define CANAIS_MAXIMO 3

#include <stdio.h>
#include <stdlib.h>
#include <setjmp.h>
#include <jpeglib.h>

// PARA O LIBJPEG
struct my_error_mgr {
struct jpeg_error_mgr pub;	/* "public" fields */
jmp_buf setjmp_buffer;	/* for return to caller */
};
typedef struct my_error_mgr * my_error_ptr;

METHODDEF(void)
my_error_exit (j_common_ptr cinfo)
{
/* cinfo->err really points to a my_error_mgr struct, so coerce pointer */
my_error_ptr myerr = (my_error_ptr) cinfo->err;
/* Always display the message. */
/* We could postpone this until after returning, if we chose. */
(*cinfo->err->output_message) (cinfo);
/* Return control to the setjmp point */
longjmp(myerr->setjmp_buffer, 1);
}

int escreverJPEG(char* nome, int largura, int altura, int canais)
{
	
    struct jpeg_compress_struct cinfo;  // Informações da imagem
    struct jpeg_error_mgr jerr;         // Tratamento de erro
    FILE* outfile;                      // Saída
    JSAMPROW row_pointer[1];            // Ponteiro para a as linhas
    int row_stride;                     // Tamanho da linha

    // Passo 1: Alocar e inicializar objeto de compressão JPEG
    cinfo.err = jpeg_std_error(&jerr);  // Definir o tratador de erro
    jpeg_create_compress(&cinfo);       // Criar o compressor

    // Passo 2: Abrir o destino
    if((outfile = fopen(nome, "wb")) == NULL)
    {
        fprintf(stderr, "Não pode abrir %s\n", nome);
        return(1);
    }
    jpeg_stdio_dest(&cinfo, outfile);

    // Passo 3: Comprimir
    cinfo.image_width = largura;                        // Largura
    cinfo.image_height = altura;                        // Altura
    cinfo.input_components = canais;                    // Número de componentes (canais)
    if(canais == 1) cinfo.in_color_space = JCS_GRAYSCALE;
    else            cinfo.in_color_space = JCS_YCbCr;
    jpeg_set_defaults(&cinfo);                                  // Definir os outros parâmetros como default
    jpeg_set_quality(&cinfo, 95, TRUE);                  // Definir a qualidade

    // Passo 4: Iniciar o compressor após as infos
    jpeg_start_compress(&cinfo, TRUE);

    // Passo 5: escrever cada linha
    row_stride = largura * canais;
		unsigned char* buffer = malloc(sizeof(unsigned char) * largura * altura * canais);
		
		int tamanho = largura * altura * canais;
    int i;
		for(i = 0; i < tamanho; i++)
		{
			buffer[i] = rand()%256;
		}

    while(cinfo.next_scanline < cinfo.image_height)
    {
        row_pointer[0] = &buffer[cinfo.next_scanline * row_stride];
        jpeg_write_scanlines(&cinfo, row_pointer, 1);
    }

    // Passo 6: Finalizar
    jpeg_finish_compress(&cinfo);   // Fechar o objeto de compressão
    // Passo 7: Limpar
    fclose(outfile);                // Fechar o arquivo
    jpeg_destroy_compress(&cinfo);  // Limpar o cinfo (muita memória)

    return 0;
}

int main(int argc, char **argv)
{
	int largura, altura, canais;
  char nome[50];
	for(largura = 64; largura <= LARGURA_MAXIMA; largura+=64)
	{
		for(altura = 64; altura <= ALTURA_MAXIMA; altura+=64)
		{
			for(canais = 1; canais <= CANAIS_MAXIMO; canais+=2)
			{
				sprintf(nome, "%05d%05d%02d.jpg", largura, altura, canais);
				escreverJPEG(nome, largura, altura, canais);
			}
		}
	}
	return 0;
}

