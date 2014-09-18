#include "unity.h"
#include "unity_fixture.h"
#include "atimagem.h"

TEST_GROUP(Imagem);

// Antes de testar
TEST_SETUP(Imagem)
{
  e4c_context_begin(E4C_TRUE);
}

// Após testar
TEST_TEAR_DOWN(Imagem)
{
  e4c_context_end();
}

// Testes para JPEG
TEST(Imagem, testImagem_JPEG_Abrir_Vazio)
{
  int erroCorreto = 0;
  // Criar a imagem
  ATImagemJPEG *imagemjpeg = (ATImagemJPEG*) criarImagem("naoexiste.jpg", AT_JPEG);

  // Tentar ler uma imagem vazia
  try
  {
    lerImagem((ATImagem*) imagemjpeg);
  }
  // Capturar o erro (testar aqui se o erro é correto)
  catch(ATImagemNaoExisteException)
  {
    erroCorreto = 1;
  }

  TEST_ASSERT_EQUAL(1, erroCorreto);

  // Testar se a largura, altura e número de canais continuam sendo 0
  TEST_ASSERT_EQUAL(0, imagemjpeg->super.altura);
  TEST_ASSERT_EQUAL(0, imagemjpeg->super.largura);
  TEST_ASSERT_EQUAL(0, imagemjpeg->super.componentes);
  TEST_ASSERT_EQUAL(AT_YCBCR, imagemjpeg->super.formato);
  TEST_ASSERT_EQUAL(AT_JPEG, imagemjpeg->super.tipo);

  destruirImagem((ATImagem*) imagemjpeg);
}
TEST(Imagem, testImagem_JPEG_Abrir_Invalido)
{
  int erroCorreto = 0;
  ATImagemJPEG *imagemjpeg = (ATImagemJPEG*) criarImagem("outroobjeto.dat", AT_JPEG);

  try
  {
    lerImagem((ATImagem*) imagemjpeg);
  }
  catch(ATImagemInvalidaException)
  {
    erroCorreto = 1;
  }
  TEST_ASSERT_EQUAL(1, erroCorreto);
  // Testar se a largura, altura e número de canais continuam sendo 0
  TEST_ASSERT_EQUAL(0, imagemjpeg->super.altura);
  TEST_ASSERT_EQUAL(0, imagemjpeg->super.largura);
  TEST_ASSERT_EQUAL(0, imagemjpeg->super.componentes);
  TEST_ASSERT_EQUAL(AT_YCBCR, imagemjpeg->super.formato);
  TEST_ASSERT_EQUAL(AT_JPEG, imagemjpeg->super.tipo);

  destruirImagem((ATImagem*) imagemjpeg);
}
TEST(Imagem, testImagem_JPEG_Abrir_Valido)
{
  TEST_IGNORE();
  int erroCorreto = 1;
  int largura = 64;
  int altura = 64;
  int canais = 1;
  int i;
  char nome[50];
  for(largura = 64; largura <= 1024; largura += 64)
  {
    for(altura = 64; altura <= 1024; altura += 64)
    {
      for(canais = 1; canais <= 3; canais += 2)
      {
        sprintf(nome, "imagens/%05d%05d%02d.jpg", largura, altura, canais);
        ATImagemJPEG *imagemjpeg = (ATImagemJPEG*) criarImagem(nome, AT_JPEG);
        if(canais == 1) imagemjpeg->super.formato = AT_GRAYSCALE;
        try
        {
          lerImagem((ATImagem*) imagemjpeg);
        }catch(ATImagemException)
        {
          erroCorreto = 0;
        }
        TEST_ASSERT_EQUAL(1, erroCorreto);
        TEST_ASSERT_EQUAL(altura, imagemjpeg->super.altura);
        TEST_ASSERT_EQUAL(largura, imagemjpeg->super.largura);
        TEST_ASSERT_EQUAL(canais, imagemjpeg->super.componentes);
        if(canais == 3) TEST_ASSERT_EQUAL(AT_YCBCR, imagemjpeg->super.formato);
        else      TEST_ASSERT_EQUAL(AT_GRAYSCALE, imagemjpeg->super.formato);
        TEST_ASSERT_EQUAL(AT_JPEG, imagemjpeg->super.tipo);
        destruirImagem((ATImagem*) imagemjpeg);
      }
    }
  }
}

TEST(Imagem, testImagem_JPEG_Escrever_Vazio)
{
  ATImagemJPEG* imagem = NULL;
  int erroCorreto = 0;

  // Carregando o objeto mas buffer vazio
  imagem = (ATImagemJPEG*) criarImagem("imagem.jpg",AT_JPEG);
  try
  {
    escreverImagem((ATImagem*)imagem);
  }
  catch(ATImagemEscreverVazioException)
  {
    erroCorreto = 1;
  }
  TEST_ASSERT_EQUAL(1, erroCorreto);

}

TEST(Imagem, testImagem_JPEG_Escrever_TamanhoCoresVariadas)
{
  ATImagemJPEG* imagem = (ATImagemJPEG*) criarImagem("", AT_JPEG);
  int largura, altura, canais;
  char nome[50];
  int erro = 0;
  int imagensCriadas = 0;
  for(largura = 64; largura <= 1024; largura+=64)
  {
    for(altura = 64; altura <= 1024; altura+=64)
    {
      for(canais = 1; canais <= 3; canais+=2)
      {
        sprintf(nome, "variados/%05d%05d%02d.jpg", largura, altura, canais);
        try
        {
          imagem->super.nome = nome;
          imagem->super.largura = largura;
          imagem->super.altura = altura;
          imagem->super.componentes = canais;
          int tamanho = largura * altura * canais;
          int tamanhoBytes = sizeof(unsigned char) * (size_t)tamanho;
          imagem->super.dados = (unsigned char*) malloc(tamanhoBytes);
          int i;
          for(i = 0; i < tamanho; i++)
          {
            imagem->super.dados[i] = rand() % 256;
          }
          if(canais == 1) imagem->super.formato = AT_GRAYSCALE;
          else            imagem->super.formato = AT_YCBCR;
          escreverImagem((ATImagem*) imagem);
          imagensCriadas++;
        }
        catch(ATImagemEscreverException)
        {
          erro++;
        }
        free(imagem->super.dados);
      }
    }
  }
  TEST_ASSERT_EQUAL(0, erro);

  // Testando se os arquivos estão lá
  FILE* file;

  for(largura = 64; largura <= 1024; largura+=64)
  {
    for(altura = 64; altura <= 1024; altura+=64)
    {
      for(canais = 1; canais <= 3; canais+=2)
      {
        sprintf(nome, "variados/%05d%05d%02d.jpg", largura, altura, canais);
        if(file=fopen(nome,"r"))
        {
          fclose(file);
          imagensCriadas--;
          remove(nome);
        }
      }
    }
  }
  TEST_ASSERT_EQUAL(0, imagensCriadas);
}

TEST(Imagem, testImagem_JPEG_Converter_GrayScale_Vazio)
{
  int erroCorreto;

  // PONTEIRO NULO
  erroCorreto = 0;
  ATImagem* imagem2 = NULL;

  try
  {
    converterImagem(imagem2, AT_GRAYSCALE);
  }
  catch(ATImagemNulaException)
  {
    erroCorreto = 1;
  }
  TEST_ASSERT_EQUAL(1, erroCorreto);


  erroCorreto = 0;
  // PONTEIRO ALOCADO MAS IMAGEM VAZIA
  ATImagemJPEG* imagem = (ATImagemJPEG*) criarImagem("teste.jpg", AT_JPEG);
  try
  {
    // Converter o objeto sem carregar antes
    converterImagem((ATImagem*) imagem, AT_GRAYSCALE);
  }
  catch(ATImagemConverterException)
  {
    erroCorreto = 1;
  }
  TEST_ASSERT_EQUAL(1, erroCorreto);

  // Converter Grayscale em outro espaço (não dá para inferir as cores)
  erroCorreto = 0;
  imagem->super.formato = AT_GRAYSCALE;
  imagem->super.componentes = 1;
  imagem->super.largura = 1;
  imagem->super.altura = 1;
  try
  {
    converterImagem((ATImagem*) imagem, AT_GRAYSCALE);
  }
  catch(ATImagemConverterException)
  {
    erroCorreto = 1;
  }
  TEST_ASSERT_EQUAL(1, erroCorreto);

}
TEST(Imagem, testImagem_JPEG_Converter_GrayScale_Correto)
{
  // Converter de CMYK para Grayscale
  // Converter de RGB para Grayscale
  // Converter de YCbCr para Grayscale
  TEST_IGNORE();
}

TEST(Imagem, testImagem_JPEG_Converter_Cores_Correto)
{
  // Converter de YCbCr para RGB
  // Converter de RGB para YCbCr
  TEST_IGNORE();
}



TEST(Imagem, testImagem_JPEG_Somar_Vazio)
{
  TEST_IGNORE();
}
TEST(Imagem, testImagem_JPEG_Somar_Correto)
{
  TEST_IGNORE();
}

TEST(Imagem, testImagem_JPEG_Subtrair_Vazio)
{
  TEST_IGNORE();
}
TEST(Imagem, testImagem_JPEG_Subtrair_Correto)
{
  TEST_IGNORE();
}

TEST(Imagem, testImagem_JPEG_Multiplicar_Vazio)
{
  TEST_IGNORE();
}
TEST(Imagem, testImagem_JPEG_Multiplicar_Correto)
{
  TEST_IGNORE();
}

TEST(Imagem, testImagem_JPEG_Dividir_Vazio)
{
  TEST_IGNORE();
}
TEST(Imagem, testImagem_JPEG_Dividir_Correto)
{
  TEST_IGNORE();
}

TEST(Imagem, testImagem_JPEG_Destruir_Vazio)
{
  TEST_IGNORE();
}
TEST(Imagem, testImagem_JPEG_Destruir_Correto)
{
  TEST_IGNORE();
}

TEST(Imagem, testImagem_JPEG_SubImagem_Vazio)
{
  TEST_IGNORE();
}
TEST(Imagem, testImagem_JPEG_SubImagem_Correto)
{
  TEST_IGNORE();
}

TEST(Imagem, testImagem_JPEG_Filtro_Vazio)
{
  TEST_IGNORE();
}
TEST(Imagem, testImagem_JPEG_Filtro_Correto)
{
  TEST_IGNORE();
}
TEST(Imagem, testImagem_JPEG_Correlacao_Vazio)
{
  TEST_IGNORE();
}
TEST(Imagem, testImagem_JPEG_Correlacao_Correto)
{
  TEST_IGNORE();
}
TEST(Imagem, testImagem_JPEG_Convolucao_Vazio)
{
  TEST_IGNORE();
}
TEST(Imagem, testImagem_JPEG_Convolucao_Correto)
{
  TEST_IGNORE();
}


// Testes para PNG
TEST(Imagem, testImagem_PNG_Abrir_Vazio)
{
  TEST_IGNORE();
}
TEST(Imagem, testImagem_PNG_Abrir_Invalido)
{
  TEST_IGNORE();
}
TEST(Imagem, testImagem_PNG_Abrir_Valido)
{
  TEST_IGNORE();
}

TEST(Imagem, testImagem_PNG_Escrever_Vazio)
{
  TEST_IGNORE();
}
TEST(Imagem, testImagem_PNG_Escrever_TamanhoCoresVariadas)
{
  TEST_IGNORE();
}


TEST(Imagem, testImagem_PNG_Converter_GrayScale_Vazio)
{
  TEST_IGNORE();
}
TEST(Imagem, testImagem_PNG_Converter_GrayScale_Correto)
{
  TEST_IGNORE();
}
TEST(Imagem, testImagem_PNG_Converter_Cores_Correto)
{
  TEST_IGNORE();
}

TEST(Imagem, testImagem_PNG_Somar_Vazio)
{
  TEST_IGNORE();
}
TEST(Imagem, testImagem_PNG_Somar_Correto)
{
  TEST_IGNORE();
}

TEST(Imagem, testImagem_PNG_Subtrair_Vazio)
{
  TEST_IGNORE();
}
TEST(Imagem, testImagem_PNG_Subtrair_Correto)
{
  TEST_IGNORE();
}

TEST(Imagem, testImagem_PNG_Multiplicar_Vazio)
{
  TEST_IGNORE();
}
TEST(Imagem, testImagem_PNG_Multiplicar_Correto)
{
  TEST_IGNORE();
}

TEST(Imagem, testImagem_PNG_Dividir_Vazio)
{
  TEST_IGNORE();
}
TEST(Imagem, testImagem_PNG_Dividir_Correto)
{
  TEST_IGNORE();
}

TEST(Imagem, testImagem_PNG_Destruir_Vazio)
{
  TEST_IGNORE();
}
TEST(Imagem, testImagem_PNG_Destruir_Correto)
{
  TEST_IGNORE();
}

TEST(Imagem, testImagem_PNG_SubImagem_Vazio)
{
  TEST_IGNORE();
}
TEST(Imagem, testImagem_PNG_SubImagem_Correto)
{
  TEST_IGNORE();
}

TEST(Imagem, testImagem_PNG_Filtro_Vazio)
{
  TEST_IGNORE();
}
TEST(Imagem, testImagem_PNG_Filtro_Correto)
{
  TEST_IGNORE();
}
TEST(Imagem, testImagem_PNG_Correlacao_Vazio)
{
  TEST_IGNORE();
}
TEST(Imagem, testImagem_PNG_Correlacao_Correto)
{
  TEST_IGNORE();
}
TEST(Imagem, testImagem_PNG_Convolucao_Vazio)
{
  TEST_IGNORE();
}
TEST(Imagem, testImagem_PNG_Convolucao_Correto)
{
  TEST_IGNORE();
}


// Testes para TIFF
TEST(Imagem, testImagem_TIFF_Abrir_Vazio)
{
  TEST_IGNORE();
}
TEST(Imagem, testImagem_TIFF_Abrir_Invalido)
{
  TEST_IGNORE();
}
TEST(Imagem, testImagem_TIFF_Abrir_Valido)
{
  TEST_IGNORE();
}

TEST(Imagem, testImagem_TIFF_Escrever_Vazio)
{
  TEST_IGNORE();
}
TEST(Imagem, testImagem_TIFF_Escrever_TamanhoCoresVariadas)
{
  TEST_IGNORE();
}

TEST(Imagem, testImagem_TIFF_Converter_GrayScale_Vazio)
{
  TEST_IGNORE();
}
TEST(Imagem, testImagem_TIFF_Converter_GrayScale_Correto)
{
  TEST_IGNORE();
}
TEST(Imagem, testImagem_TIFF_Converter_Cores_Correto)
{
  TEST_IGNORE();
}

TEST(Imagem, testImagem_TIFF_Somar_Vazio)
{
  TEST_IGNORE();
}
TEST(Imagem, testImagem_TIFF_Somar_Correto)
{
  TEST_IGNORE();
}

TEST(Imagem, testImagem_TIFF_Subtrair_Vazio)
{
  TEST_IGNORE();
}
TEST(Imagem, testImagem_TIFF_Subtrair_Correto)
{
  TEST_IGNORE();
}

TEST(Imagem, testImagem_TIFF_Multiplicar_Vazio)
{
  TEST_IGNORE();
}
TEST(Imagem, testImagem_TIFF_Multiplicar_Correto)
{
  TEST_IGNORE();
}

TEST(Imagem, testImagem_TIFF_Dividir_Vazio)
{
  TEST_IGNORE();
}
TEST(Imagem, testImagem_TIFF_Dividir_Correto)
{
  TEST_IGNORE();
}

TEST(Imagem, testImagem_TIFF_Destruir_Vazio)
{
  TEST_IGNORE();
}
TEST(Imagem, testImagem_TIFF_Destruir_Correto)
{
  TEST_IGNORE();
}

TEST(Imagem, testImagem_TIFF_SubImagem_Vazio)
{
  TEST_IGNORE();
}
TEST(Imagem, testImagem_TIFF_SubImagem_Correto)
{
  TEST_IGNORE();
}

TEST(Imagem, testImagem_TIFF_Filtro_Vazio)
{
  TEST_IGNORE();
}
TEST(Imagem, testImagem_TIFF_Filtro_Correto)
{
  TEST_IGNORE();
}
TEST(Imagem, testImagem_TIFF_Correlacao_Vazio)
{
  TEST_IGNORE();
}
TEST(Imagem, testImagem_TIFF_Correlacao_Correto)
{
  TEST_IGNORE();
}
TEST(Imagem, testImagem_TIFF_Convolucao_Vazio)
{
  TEST_IGNORE();
}
TEST(Imagem, testImagem_TIFF_Convolucao_Correto)
{
  TEST_IGNORE();
}


//void teste_ler_imagem()
//{
//  ATImagemJPEG imagemjpeg;                // Criar uma imagem
//  imagemjpeg.super.nome = "teste.jpg";          // A imagem tem tamanho 130x170
//  lerImagem((ATImagem*) &imagemjpeg);           // Fazer o processo de leitura
//  TEST_ASSERT_EQUAL_INT(imagemjpeg.super.largura, 150);   // Testar Largura
//  TEST_ASSERT_EQUAL_INT(imagemjpeg.super.altura, 170);  // Testar Altura
//}
