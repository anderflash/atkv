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
        case AT_TIFF: imagem = (ATImagem*) malloc(sizeof(ATImagemTIFF)); break;
        case AT_PNG: imagem = (ATImagem*) malloc(sizeof(ATImagemPNG)); break;
    }
    imagem->tipo = tipo;
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
        case AT_JPEG: lerImagemJPEG((ATImagemJPEG*)imagem);break;
        case AT_PNG: lerImagemPNG((ATImagemPNG*)imagem);break;
    }
}

void escreverImagem(ATImagem* imagem)
{
  if(imagem == NULL) throw(ATImagemNulaException, "Objeto vazio");
  switch(imagem->tipo)
  {
    case AT_JPEG: escreverImagemJPEG((ATImagemJPEG*)imagem);
    case AT_PNG: escreverImagemPNG((ATImagemPNG*)imagem);
  }
}

void converterImagem(ATImagem* imagemEntrada, ATImagem* imagemSaida)
{
  if(imagemEntrada == NULL) throw(ATImagemNulaException, "Objeto vazio");
  if(imagemSaida == NULL) throw(ATImagemNulaException, "Objeto vazio");
  switch(imagemEntrada->tipo)
  {
    case AT_JPEG: converterImagemJPEG((ATImagemJPEG*)imagemEntrada, (ATImagemJPEG*)imagemSaida);
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

void converterImagemJPEG(ATImagemJPEG* imagemEntrada, ATImagemJPEG* imagemSaida)
{
  if(imagemEntrada == NULL) throw(ATImagemNulaException, "Objeto vazio");
  if(imagemSaida == NULL) throw(ATImagemNulaException, "Objeto vazio");
  if(imagemEntrada->super.componentes == 0 ||
     imagemEntrada->super.largura == 0 ||
     imagemEntrada->super.altura == 0) throw(ATImagemConverterException, "Não pode converter uma imagem vazia.");
  if(imagemEntrada->super.formato == AT_GRAYSCALE) throw(ATImagemConverterException, "Não pode converter grayscale em outro espaço de cores.");
}

void lerImagemPNG(ATImagemPNG* imagem)
{
  char header[8];    // 8 is the maximum size that can be checked

  /* open file and test for it being a png */
  FILE *fp = fopen(imagem->super.nome, "rb");
  if (!fp) throw(ATImagemNaoExisteException, "A imagem não existe");
  fread(header, 1, 8, fp);
  if (png_sig_cmp(header, 0, 8))
    throw(ATImagemInvalidaException, "Não é uma imagem válida");


  /* initialize stuff */
  png_structp png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);

  if (!png_ptr)
    throw(RuntimeException, "[read_png_file] png_create_read_struct failed");

  png_infop info_ptr = png_create_info_struct(png_ptr);
  if (!info_ptr)
    throw(RuntimeException, "[read_png_file] png_create_info_struct failed");

  if (setjmp(png_jmpbuf(png_ptr)))
    throw(RuntimeException, "[read_png_file] Error during init_io");

  png_init_io(png_ptr, fp);
  png_set_sig_bytes(png_ptr, 8);

  png_read_info(png_ptr, info_ptr);

  png_set_scale_16(png_ptr);

  imagem->super.largura = png_get_image_width(png_ptr, info_ptr);
  imagem->super.altura = png_get_image_height(png_ptr, info_ptr);

  imagem->color_type = png_get_color_type(png_ptr, info_ptr);
  switch(imagem->color_type)
  {
    case PNG_COLOR_TYPE_RGB:imagem->super.formato=AT_RGB;imagem->super.componentes = 3;break;
    case PNG_COLOR_TYPE_RGBA:imagem->super.formato=AT_RGBA;imagem->super.componentes = 4;break;
    case PNG_COLOR_TYPE_GRAY:imagem->super.formato=AT_GRAYSCALE;imagem->super.componentes = 1;break;
    case PNG_COLOR_TYPE_GA:imagem->super.formato=AT_GRAYSCALE_ALPHA;imagem->super.componentes = 2;break;
  }
  imagem->bit_depth = png_get_bit_depth(png_ptr, info_ptr);

  int number_of_passes = png_set_interlace_handling(png_ptr);
  png_read_update_info(png_ptr, info_ptr);


  /* read file */
  if (setjmp(png_jmpbuf(png_ptr)))
    throw(RuntimeException, "[read_png_file] Error during read_image");

  size_t rowBytes = png_get_rowbytes(png_ptr,info_ptr);
  imagem->super.dados = (png_byte*) malloc(rowBytes * imagem->super.altura);
  png_bytep* row_pointers = (png_bytep*) malloc(sizeof(png_bytep) * imagem->super.altura);

  int y;
  for (y=0; y<imagem->super.altura; y++)
    row_pointers[y] = &imagem->super.dados[y*rowBytes];


  png_read_image(png_ptr, row_pointers);

  fclose(fp);
}

void escreverImagemPNG(ATImagemPNG* imagem)
{
  if(imagem->super.largura == 0 ||
     imagem->super.altura == 0 ||
     imagem->super.componentes == 0 ||
     imagem->bit_depth == 0 ||
     imagem->super.dados == NULL)
    throw(ATImagemEscreverVazioException, "Imagem vazia");

  /* create file */
  FILE *fp = fopen(imagem->super.nome, "wb");
  if (!fp)
  {
    throw(ATImagemException, "[write_png_file] Arquivo não pode ser aberto para escrita");
  }


  /* initialize stuff */
  png_structp png_ptr = png_create_write_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);

  if (!png_ptr)
    throw(ATImagemException, "[write_png_file] png_create_write_struct failed");

  png_infop info_ptr = png_create_info_struct(png_ptr);
  if (!info_ptr)
    throw(ATImagemException, "[write_png_file] png_create_info_struct failed");

  if (setjmp(png_jmpbuf(png_ptr)))
    throw(ATImagemException, "[write_png_file] Error during init_io");

  png_init_io(png_ptr, fp);


  /* write header */
  if (setjmp(png_jmpbuf(png_ptr)))
    throw(ATImagemException, "[write_png_file] Error during writing header");


  if(imagem->super.componentes == 1) imagem->color_type = PNG_COLOR_TYPE_GRAY;
  else if(imagem->super.componentes == 2) imagem->color_type = PNG_COLOR_TYPE_GRAY_ALPHA;
  else if(imagem->super.componentes == 3) imagem->color_type = PNG_COLOR_TYPE_RGB;
  else if(imagem->super.componentes == 4) imagem->color_type = PNG_COLOR_TYPE_RGBA;

  png_set_IHDR(png_ptr, info_ptr, imagem->super.largura, imagem->super.altura,
               imagem->bit_depth, imagem->color_type, PNG_INTERLACE_NONE,
               PNG_COMPRESSION_TYPE_BASE, PNG_FILTER_TYPE_BASE);

  png_write_info(png_ptr, info_ptr);


  /* write bytes */
  if (setjmp(png_jmpbuf(png_ptr)))
    throw(ATImagemException, "[write_png_file] Error during writing bytes");

  png_bytep* row_pointers = (png_bytep*) malloc(sizeof(png_bytep) * imagem->super.altura);

  size_t rowBytes = png_get_rowbytes(png_ptr,info_ptr);
  int y;
  for (y=0; y<imagem->super.altura; y++)
          row_pointers[y] = &imagem->super.dados[y*rowBytes];

  png_write_image(png_ptr, row_pointers);

  /* end write */
  if (setjmp(png_jmpbuf(png_ptr)))
    throw(ATImagemException, "[write_png_file] Error during end of write");

  png_write_end(png_ptr, NULL);

  /* cleanup heap allocation */
  //for (y=0; y<imagem->super.altura; y++)
  //        free(row_pointers[y]);

  free(row_pointers);

  fclose(fp);
}

void lerImagemTIFF(ATImagemTIFF* imagem)
{

}

void escreverImagemTIFF(ATImagemTIFF* imagem)
{

}
