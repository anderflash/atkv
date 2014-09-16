#include "atmidia.h"

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

int lerJPEG(ATImagemJPEG* imagem)
{
	struct jpeg_decompress_struct cinfo; // parâmetros
	struct my_error_mgr jerr;			 // erro
	FILE *infile;						 // arquivo
	int row_stride;						 // largura da linha

	// Verificar se pode abrir o arquivo
	if((infile = fopen(imagem->nome, "rb")) == NULL)            // se não abrir o arquivo
	{
		fprintf(stderr, "Não pode abrir %s\n", imagem->nome); // exiba mensagem de erro
		return 0;                                     // finalize a função
	}

	// Passo 1: alocar o objeto
	cinfo.err = jpeg_std_error(&jerr.pub); // usar o erro do jerr
	jerr.pub.error_exit = my_error_exit;   // usar o error exit
	if(setjmp(jerr.setjmp_buffer))         // houve erro no buffer?
	{
		jpeg_destroy_decompress(&cinfo);   // limpar o objeto
		fclose(infile);                    // fechar o arquivo
		return 0;                          // terminar a função
	}
	jpeg_create_decompress(&cinfo);        // Criar o objeto de descompressão

	// Passo 2: especificar a fonte dos dados
	jpeg_stdio_src(&cinfo, infile);        // altera cinfo para referenciar o arquivo

	// Passo 3: ler parâmetros do arquivo com jpeg_read_header()
	jpeg_read_header(&cinfo, TRUE);        // TRUE para rejeitar arquivos JPEG que contenham apenas uma tabela como um erro

	// Passo 4: colocar parâmetros de descompressão
	// Nesse caso não faremos nada
	switch(imagem->formato)
	{
	    case AT_YCBCR: cinfo.out_color_space = JCS_YCbCr; break;
        case AT_RGB: cinfo.out_color_space = JCS_RGB; break;
        case AT_GRAYSCALE: cinfo.out_color_space = JCS_GRAYSCALE; break;
        case AT_CMYK: cinfo.out_color_space = JCS_CMYK; break;
        case AT_YCCK: cinfo.out_color_space = JCS_YCCK; break;
        default:break;
	}

	// Passo 5: descomprimir
	jpeg_start_decompress(&cinfo);         // passa o struct

	// Obter o tamanho da linha (largura x Nº de canais)
	row_stride = cinfo.output_width * cinfo.output_components;

	imagem->buffer = (unsigned char*) malloc(sizeof(unsigned char) * cinfo.output_height * cinfo.output_width * cinfo.output_components);// (*cinfo.mem->alloc_sarray)((j_common_ptr)&cinfo, JPOOL_IMAGE, row_stride * cinfo.output_height, 1);

	// Passo 6: enquanto houver linhas, leia-os

	while(cinfo.output_scanline < cinfo.output_height)
	{
	    unsigned char *rowp[1];
	    rowp[0] = (unsigned char*) imagem->buffer + row_stride * cinfo.output_scanline;
	    jpeg_read_scanlines(&cinfo, rowp, 1);
	    //put_scanline_someplace(buffer[0], row_stride);
	}

	// Passo 7: finalizar o processo
	jpeg_finish_decompress(&cinfo);

	imagem->largura = cinfo.output_width;
	imagem->altura = cinfo.output_height;
	imagem->canais = cinfo.num_components;

	// Passo 8: limpar o objeto
	jpeg_destroy_decompress(&cinfo);

	// Fechando o arquivo
	fclose(infile);

	return 1;
}

int escreverJPEG(const char* saida, ATImagemJPEG* imagem, int qualidade)
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
    if((outfile = fopen(saida, "wb")) == NULL)
    {
        fprintf(stderr, "Não pode abrir %s\n", saida);
        return(1);
    }
    jpeg_stdio_dest(&cinfo, outfile);

    // Passo 3: Comprimir
    cinfo.image_width = imagem->largura;                        // Largura
    cinfo.image_height = imagem->altura;                        // Altura
    cinfo.input_components = imagem->canais;                    // Número de componentes (canais)
    switch(imagem->formato)                                     // Formato do espaço de cores
    {
        case AT_RGB:  cinfo.in_color_space = JCS_RGB;  break;
        case AT_CMYK: cinfo.in_color_space = JCS_CMYK; break;
        case AT_YCBCR:cinfo.in_color_space = JCS_YCbCr;break;
        case AT_YCCK: cinfo.in_color_space = JCS_YCCK; break;
        default:      cinfo.in_color_space = JCS_YCbCr;break;
    }
    jpeg_set_defaults(&cinfo);                                  // Definir os outros parâmetros como default
    jpeg_set_quality(&cinfo, qualidade, TRUE);                  // Definir a qualidade

    // Passo 4: Iniciar o compressor após as infos
    jpeg_start_compress(&cinfo, TRUE);

    // Passo 5: escrever cada linha
    row_stride = imagem->largura * imagem->canais;
    while(cinfo.next_scanline < cinfo.image_height)
    {
        row_pointer[0] = &imagem->buffer[cinfo.next_scanline * row_stride];
        jpeg_write_scanlines(&cinfo, row_pointer, 1);
    }

    // Passo 6: Finalizar
    jpeg_finish_compress(&cinfo);   // Fechar o objeto de compressão
    // Passo 7: Limpar
    fclose(outfile);                // Fechar o arquivo
    jpeg_destroy_compress(&cinfo);  // Limpar o cinfo (muita memória)

    return 0;
}

void destruirImagem(ATImagemJPEG* imagem)
{
    free(imagem->buffer);
}
