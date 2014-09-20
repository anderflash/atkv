#include "unity.h"
#include "unity_fixture.h"
#include "atcamera.h"

TEST_GROUP(Camera);

TEST_SETUP(Camera)
{
  e4c_context_begin(E4C_TRUE);
}

TEST_TEAR_DOWN(Camera)
{
  e4c_context_end();
}

TEST(Camera, testCamera_Abrir_Invalido)
{
  int erro = 0;
  ATCamera* camera;
  try
  {
    abrirCamera("uau",0,0,AT_RGB, &camera);
    TEST_ASSERT_EQUAL(640, camera->largura);
    TEST_ASSERT_EQUAL(480, camera->altura);
    fecharCamera(camera);
  }
  catch(ATCameraAbrirLeituraEscritaException)
  {
    erro = 1;
  }
  catch(ATCameraException)
  {
    erro = 1;
  }
  TEST_ASSERT_EQUAL(1, erro);

}

TEST(Camera, testCamera_Abrir_Valido)
{
  int erro = 0;
  int largura = 1280;
  int altura = 720;
  ATCamera* camera;
  try
  {
    abrirCamera("/dev/video0",largura,altura,AT_RGB,&camera);
    TEST_ASSERT_EQUAL(largura, camera->largura);
    TEST_ASSERT_EQUAL(altura, camera->altura);
    fecharCamera(camera);
    destruirCamera(camera);
  }
  catch(ATCameraAbrirLeituraEscritaException)
  {
    erro = 1;
  }
  catch(ATCameraException)
  {
    erro = 1;
  }
  TEST_ASSERT_EQUAL(0, erro);
}

TEST(Camera, testImagem_Obter_Frame)
{
  int erro = 0;
  ATCamera* camera;
  int largura = 640;
  int altura = 480;
  try
  {
    abrirCamera("/dev/video1",largura,altura,AT_CINZA, &camera);
    TEST_ASSERT_EQUAL(largura, camera->largura);
    TEST_ASSERT_EQUAL(altura, camera->altura);
    atualizarQuadro(camera);
    fecharCamera(camera);
    destruirCamera(camera);
  }
  catch(ATCameraAtualizarQuadroException)
  {
    erro = 1;
  }
  catch(ATCameraAbrirLeituraEscritaException)
  {
    erro = 2;
  }
  catch(ATCameraException)
  {
    erro = 3;
  }
  TEST_ASSERT_EQUAL(0, erro);
}
