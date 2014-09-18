#include "atimagem.h"
E4C_DEFINE_EXCEPTION(ATImagemException, "Erro com a imagem.", RuntimeException);
E4C_DEFINE_EXCEPTION(ATImagemNaoExisteException, "Não existe esta imagem.", ATImagemException);
E4C_DEFINE_EXCEPTION(ATImagemInvalidaException, "Imagem inválida.", ATImagemException);
E4C_DEFINE_EXCEPTION(ATImagemEscreverException, "Erro na escrita.", ATImagemException);
E4C_DEFINE_EXCEPTION(ATImagemEscreverVazioException, "Não pode salvar uma imagem vazia.", ATImagemEscreverException);
E4C_DEFINE_EXCEPTION(ATImagemConverterException, "Erro na conversão.", ATImagemException);
E4C_DEFINE_EXCEPTION(ATImagemNulaException, "Erro na conversão.", ATImagemException);


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

ATImagem* criarImagem(const char* nome, ATImagemTipo tipo)
{
    ATImagem* imagem;
    switch(tipo)
    {
        case AT_JPEG: imagem = (ATImagem*) malloc(sizeof(ATImagemJPEG)); break;
    }
    imagem->nome = (char*)nome;
    imagem->altura = 0;
    imagem->largura = 0;
    imagem->componentes = 0;
    imagem->dados = NULL;
    imagem->formato = AT_YCBCR;
    return imagem;
}

void lerImagem(ATImagem* imagem)
{
    if(imagem == NULL) throw(ATImagemNulaException, "Objeto vazio");
    switch(imagem->tipo)
    {
        case AT_JPEG: lerImagemJPEG((ATImagemJPEG*)imagem);
    }
}

void escreverImagem(ATImagem* imagem)
{
  if(imagem == NULL) throw(ATImagemNulaException, "Objeto vazio");
  switch(imagem->tipo)
  {
    case AT_JPEG: escreverImagemJPEG((ATImagemJPEG*)imagem);
  }
}

void converterImagem(ATImagem* imagem, ATImagemFormato formato)
{
  if(imagem == NULL) throw(ATImagemNulaException, "Objeto vazio");
  switch(imagem->tipo)
  {
    case AT_JPEG: converterImagemJPEG((ATImagemJPEG*)imagem, formato);
  }
}

void destruirImagem(ATImagem* imagem)
{
    free(imagem->dados);
    free(imagem);
}

void lerImagemJPEG(ATImagemJPEG* imagem)
{
    struct jpeg_decompress_struct cinfo; // parâmetros
	struct my_error_mgr jerr;			 // erro
	FILE *infile;						 // arquivo
	int row_stride;						 // largura da linha

	// Verificar se pode abrir o arquivo
	if((infile = fopen(imagem->super.nome, "rb")) == NULL)            // se não abrir o arquivo
	{
	    throw (ATImagemNaoExisteException, "Não pode ler uma imagem que não existe. Verifique se o nome do arquivo foi colocado corretamente ou se ou arquivo de fato existe");
	}

	// Passo 1: alocar o objeto
	cinfo.err = jpeg_std_error(&jerr.pub); // usar o erro do jerr
	jerr.pub.error_exit = my_error_exit;   // usar o error exit
	if(setjmp(jerr.setjmp_buffer))         // houve erro no buffer?
	{
		jpeg_destroy_decompress(&cinfo);   // limpar o objeto
		fclose(infile);                    // fechar o arquivo
		throw (ATImagemInvalidaException, "Não é uma imagem válida. Verifique se abriu o arquivo correto ou se os dados da memória estão corrompidos");
	}
	jpeg_create_decompress(&cinfo);        // Criar o objeto de descompressão

	// Passo 2: especificar a fonte dos dados
	jpeg_stdio_src(&cinfo, infile);        // altera cinfo para referenciar o arquivo

	// Passo 3: ler parâmetros do arquivo com jpeg_read_header()
	jpeg_read_header(&cinfo, TRUE);        // TRUE para rejeitar arquivos JPEG que contenham apenas uma tabela como um erro

	// Passo 4: colocar parâmetros de descompressão
	// Nesse caso não faremos nada
	switch(imagem->super.formato)
	{
	    case AT_YCBCR: cinfo.out_color_space = JCS_YCbCr; break;
        case AT_RGB: cinfo.out_color_space = JCS_RGB; break;
        case AT_GRAYSCALE: cinfo.out_color_space = JCS_GRAYSCALE; break;
        case AT_CMYK: cinfo.out_color_space = JCS_CMYK; break;
        default:break;
	}

	// Passo 5: descomprimir
	jpeg_start_decompress(&cinfo);         // passa o struct

	// Obter o tamanho da linha (largura x Nº de canais)
	row_stride = cinfo.output_width * cinfo.output_components;

	imagem->super.dados = (unsigned char*) malloc(sizeof(unsigned char) * cinfo.output_height * cinfo.output_width * cinfo.output_components);// (*cinfo.mem->alloc_sarray)((j_common_ptr)&cinfo, JPOOL_IMAGE, row_stride * cinfo.output_height, 1);

	// Passo 6: enquanto houver linhas, leia-os

	while(cinfo.output_scanline < cinfo.output_height)
	{
	    unsigned char *rowp[1];
	    rowp[0] = (unsigned char*) imagem->super.dados + row_stride * cinfo.output_scanline;
	    jpeg_read_scanlines(&cinfo, rowp, 1);
	    //put_scanline_someplace(buffer[0], row_stride);
	}

	// Passo 7: finalizar o processo
	jpeg_finish_decompress(&cinfo);

	imagem->super.largura = cinfo.output_width;
	imagem->super.altura = cinfo.output_height;
	imagem->super.componentes = cinfo.num_components;

	// Passo 8: limpar o objeto
	jpeg_destroy_decompress(&cinfo);

	// Fechando o arquivo
	fclose(infile);
}

void escreverImagemJPEG(ATImagemJPEG* imagem)
{
  if(imagem == NULL) throw(ATImagemEscreverVazioException, "O objeto da imagem não foi alocado.");
  if(imagem->super.dados == NULL) throw(ATImagemEscreverVazioException, "O objeto da imagem não foi alocado.");

  struct jpeg_compress_struct cinfo;  // Informações da imagem
  struct jpeg_error_mgr jerr;         // Tratamento de erro
  FILE* outfile;                      // Saída
  JSAMPROW row_pointer[1];            // Ponteiro para a as linhas
  int row_stride;                     // Tamanho da linha

  // Passo 1: Alocar e inicializar objeto de compressão JPEG
  cinfo.err = jpeg_std_error(&jerr);  // Definir o tratador de erro
  jpeg_create_compress(&cinfo);       // Criar o compressor

  // Passo 2: Abrir o destino
  if((outfile = fopen(imagem->super.nome, "wb")) == NULL)
  {
    throw(ATImagemEscreverException, "Não pode abrir o arquivo com esse nome.");
  }
  jpeg_stdio_dest(&cinfo, outfile);

  // Passo 3: Comprimir
  cinfo.image_width = imagem->super.largura;                        // Largura
  cinfo.image_height = imagem->super.altura;                        // Altura
  cinfo.input_components = imagem->super.componentes;                    // Número de componentes (canais)
  if(imagem->super.componentes == 1) cinfo.in_color_space = JCS_GRAYSCALE;
  else            cinfo.in_color_space = JCS_YCbCr;
  jpeg_set_defaults(&cinfo);                                  // Definir os outros parâmetros como default
  jpeg_set_quality(&cinfo, 95, TRUE);                  // Definir a qualidade

  // Passo 4: Iniciar o compressor após as infos
  jpeg_start_compress(&cinfo, TRUE);

  // Passo 5: escrever cada linha
  row_stride = imagem->super.largura * imagem->super.componentes;

  while(cinfo.next_scanline < cinfo.image_height)
  {
    row_pointer[0] = &imagem->super.dados[cinfo.next_scanline * row_stride];
    jpeg_write_scanlines(&cinfo, row_pointer, 1);
  }

  // Passo 6: Finalizar
  jpeg_finish_compress(&cinfo);   // Fechar o objeto de compressão
  // Passo 7: Limpar
  fclose(outfile);                // Fechar o arquivo
  jpeg_destroy_compress(&cinfo);  // Limpar o cinfo (muita memória)
}

void converterImagemJPEG(ATImagemJPEG* imagem, ATImagemFormato formato)
{
  if(imagem == NULL) throw(ATImagemNulaException, "Objeto vazio");
  if(imagem->super.componentes == 0 ||
     imagem->super.largura == 0 ||
     imagem->super.altura == 0) throw(ATImagemConverterException, "Não pode converter uma imagem vazia.");
  if(imagem->super.formato == AT_GRAYSCALE) throw(ATImagemConverterException, "Não pode converter grayscale em outro espaço de cores.");
}
