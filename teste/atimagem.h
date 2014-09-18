#ifndef ATIMAGEM_H
#define ATIMAGEM_H

typedef enum{AT_JPEG, AT_GIF, AT_TIFF} ATTipoImagem;
typedef enum{AT_RGB, AT_CMYK, AT_GRAYSCALE, AT_YUV, AT_YCBCR} ATFormatoImagem;

// Estrutura base para imagem
typedef struct
{
  int largura;
  int altura;
  int componentes;
  ATFormatoImagem formato;
  ATTipoImagem tipo;
  char* caminho;
  unsigned char* dados;
} ATImagem;

// Estrutura específica para JPEG
typedef struct
{
  ATImagem super;
} ATImagemJPEG;

// Estrutura específica para GIF
typedef struct
{
  ATImagem super;
} ATImagemGIF;

// Estrutura específica para TIFF
typedef struct
{
  ATImagem super;
} ATImagemTIFF;

// Funções Gerais
ATStatus criarImagem(ATImagem* imagem);
ATStatus lerImagem(ATImagem* imagem);
ATStatus escreverImagem(const char* saida, ATImagem* imagem);

// Funções Específicas
ATStatus lerImagemJPEG(ATImagemJPEG* imagem);
ATStatus escreverImagemJPEG(ATImagemJPEG* imagem);

ATStatus criarImagemJPEG(ATImagem *imagem);
ATStatus destruirImagemJPEG(ATImagem);




#endif // ATIMAGEM_H

