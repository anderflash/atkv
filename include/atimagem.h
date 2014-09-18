#ifndef ATIMAGEM_H
#define ATIMAGEM_H

#include "e4c.h"
#include <stdio.h>
#include <stdlib.h>
#include <setjmp.h>
#include <jpeglib.h>

METHODDEF(void)
my_error_exit (j_common_ptr cinfo);


E4C_DECLARE_EXCEPTION(ATImagemException);
E4C_DECLARE_EXCEPTION(ATImagemNaoExisteException);
E4C_DECLARE_EXCEPTION(ATImagemInvalidaException);
E4C_DECLARE_EXCEPTION(ATImagemEscreverException);
E4C_DECLARE_EXCEPTION(ATImagemEscreverVazioException);
E4C_DECLARE_EXCEPTION(ATImagemConverterException);
E4C_DECLARE_EXCEPTION(ATImagemNulaException);

typedef enum{AT_CMYK, AT_RGB, AT_YCBCR, AT_YUV, AT_GRAYSCALE} ATImagemFormato;
typedef enum{AT_JPEG, AT_PNG, AT_TIFF} ATImagemTipo;

// PARA O LIBJPEG
struct my_error_mgr {
struct jpeg_error_mgr pub;	/* "public" fields */
jmp_buf setjmp_buffer;	/* for return to caller */
};
typedef struct my_error_mgr * my_error_ptr;

typedef struct
{
    int largura;
    int altura;
    int componentes;
    ATImagemFormato formato; // Se é rgb, cmyk, grayscale...
    ATImagemTipo    tipo;    // Se é jpeg, tiff, png...
    char* nome;
    unsigned char* dados;
}ATImagem;

typedef struct
{
    ATImagem super;
}ATImagemJPEG;

ATImagem* criarImagem(const char* nome, ATImagemTipo tipo);
void lerImagem(ATImagem* imagem);
void escreverImagem(ATImagem* imagem);
void destruirImagem(ATImagem* imagem);
void converterImagem(ATImagem* imagem, ATImagemFormato formato);

// FUNÇÕES JPEG
void lerImagemJPEG(ATImagemJPEG* imagem);
void escreverImagemJPEG(ATImagemJPEG* imagem);
void converterImagemJPEG(ATImagemJPEG* imagem, ATImagemFormato formato);

#endif // ATIMAGEM_H
