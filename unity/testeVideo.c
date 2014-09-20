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
  ATVideo* video;
  try
  {
    video = criarVideo("vazio.avi");
    abrirVideo(video);
  }
  catch(ATVideoAbrirNaoExisteException)
  {
    erro = 1;
  }
  catch(ATVideoAbrirInvalidoException)
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
  ATVideo* video;
  try
  {
    video = criarVideo("outroobjeto.dat");
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
  int erro = 0;ATVideo* video;
  try
  {
    video = criarVideo("videos/video.mpg");
    abrirVideo(video);
    TEST_ASSERT_EQUAL(320, video->largura);
    TEST_ASSERT_EQUAL(240, video->altura);
    TEST_ASSERT_EQUAL(3, video->canais);
    TEST_ASSERT_EQUAL(AT_RGB, video->espacoCores);
    TEST_ASSERT_EQUAL(AT_MPEG, video->tipo);
    int i;
    for(i=0; i < 100; i++)//video->largura * video->altura / 2 * ; i++)
    {
      printf("%d ", video->frameBuffer[i]);
    }
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
  int erro = 0;
  ATVideo* video;
  try
  {
    video = criarVideo("videos/video3.3gp");
    abrirVideo(video);
    obterQuadro(video);
  }
  catch(ATVideoException)
  {
    erro = 1;
  }
  TEST_ASSERT_EQUAL(0, erro);
  TEST_ASSERT_EQUAL(640, video->largura);
  TEST_ASSERT_EQUAL(360, video->altura);
  TEST_ASSERT_EQUAL(3, video->canais);
  TEST_ASSERT_EQUAL(AT_RGB, video->espacoCores);
  TEST_ASSERT_EQUAL(AT_MPEG, video->tipo);

}
TEST(Video, testVideo_Controlar)
{
  // Abrir o vídeo
  ATVideo* video = criarVideo("videos/video3.3gp");
  abrirVideo(video);

  // Tocar
  tocarVideo(video);

  // Pausar
  pausarVideo(video);

  // Rebobinar
  rebobinarVideo(video);

  // Parar (Pausar e Rebobinar)
  pararVideo(video);

  // Ir para um ponto específico
  int frame = 200;
  irParaFrame(video, frame);
  int64_t timestamp = 3400000;
  irParaTempo(video, timestamp);
}
