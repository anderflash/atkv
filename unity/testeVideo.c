#include "unity.h"
#include "unity_fixture.h"
#include "atvideo.h"

TEST_GROUP(Video);

TEST_SETUP(Video)
{
  e4c_context_begin(E4C_TRUE);
  iniciarContextoVideo();
}

TEST_TEAR_DOWN(Video)
{
  e4c_context_end();
}


TEST(Video, testVideo_Abrir_Vazio)
{
  int erro = 0;
  ATVideo* video = criarVideo("vazio.avi", AT_AVI);
  try
  {
    abrirVideo(video);
  }
  catch(ATVideoAbrirNaoExisteException)
  {
    erro = 1;
  }
  catch(RuntimeException)
  {
    erro = 2;
  }
  TEST_ASSERT_EQUAL(1, erro);
  TEST_ASSERT_EQUAL(0, video->totalframes);
  TEST_ASSERT_EQUAL(0, video->largura);
  TEST_ASSERT_EQUAL(0, video->altura);
  TEST_ASSERT_EQUAL(0, video->canais);
  destruirVideo(video);
}
TEST(Video, testVideo_Abrir_Invalido)
{
  int erro = 0;
  ATVideo* video = criarVideo("outroobjeto.dat", AT_MPEG);
  try
  {
    abrirVideo(video);
  }
  catch(ATVideoAbrirInvalidoException)
  {
    erro = 1;
  }
  catch(ATVideoFormatoException)
  {
    erro = 1;
  }
  catch(ATVideoCodecException)
  {
    erro = 1;
  }
  catch(RuntimeException)
  {
    erro = 2;
  }
  TEST_ASSERT_EQUAL(1, erro);
  TEST_ASSERT_EQUAL(0, video->totalframes);
  TEST_ASSERT_EQUAL(0, video->largura);
  TEST_ASSERT_EQUAL(0, video->altura);
  TEST_ASSERT_EQUAL(0, video->canais);
  destruirVideo(video);
}
TEST(Video, testVideo_Abrir_Valido)
{
  int erro = 0;
  ATVideo* video = criarVideo("videos/video.mpg", AT_MPEG);
  try
  {
    abrirVideo(video);
    TEST_ASSERT_EQUAL(320, video->largura);
    TEST_ASSERT_EQUAL(240, video->altura);
    TEST_ASSERT_EQUAL(3, video->canais);
    TEST_ASSERT_EQUAL(AT_YUV, video->formato);
    TEST_ASSERT_EQUAL(AT_MPEG, video->tipo);
  }
  catch(RuntimeException)
  {
    erro = 1;
  }
  TEST_ASSERT_EQUAL(0, erro);
  if(videoEstaAberto(video))
  {
    fecharVideo(video);
  }
  destruirVideo(video);
}
TEST(Video, testVideo_Gravar_Vazio)
{
  TEST_IGNORE();
}
TEST(Video, testVideo_Gravar_Grayscale)
{
  TEST_IGNORE();
}
TEST(Video, testVideo_Gravar_Cores)
{
  TEST_IGNORE();
}
TEST(Video, testVideo_Gravar_Audio)
{
  TEST_IGNORE();
}
TEST(Video, testVideo_Obter_Quadro)
{
  TEST_IGNORE();
}
TEST(Video, testVideo_Controlar)
{
  TEST_IGNORE();
}
