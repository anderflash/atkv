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

int lerJPEG(const char* nome, ImagemJPEG* imagem)
{
	struct jpeg_decompress_struct cinfo; // parâmetros
	struct my_error_mgr jerr;			 // erro
	FILE *infile;						 // arquivo
	JSAMPARRAY buffer;					 // pixels
	int row_stride;						 // largura da linha

	// Verificar se pode abrir o arquivo
	if((infile = fopen(nome, "rb")) == NULL)            // se não abrir o arquivo
	{
		fprintf(stderr, "Não pode abrir %s\n", nome); // exiba mensagem de erro
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

	// Passo 5: descomprimir
	jpeg_start_decompress(&cinfo);         // passa o struct

	// Obter o tamanho da linha (largura x Nº de canais)
	row_stride = cinfo.output_width * cinfo.output_components;

	imagem->buffer = (*cinfo.mem->alloc_sarray)((j_common_ptr)&cinfo, JPOOL_IMAGE, row_stride * cinfo.output_height, 1);

	// Passo 6: enquanto houver linhas, leia-os
	while(cinfo.output_scanline < cinfo.output_height)
	{
		jpeg_read_scanlines(&cinfo, imagem->buffer, cinfo.output_height);
		//put_scanline_someplace(buffer[0], row_stride);
	}

	// Passo 7: finalizar o processo
	jpeg_finish_decompress(&cinfo);

	// Passo 8: limpar o objeto
	jpeg_destroy_decompress(&cinfo);

	// Fechando o arquivo
	fclose(infile);


	return 1;
}