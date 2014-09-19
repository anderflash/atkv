#include "unity.h"
#include "unity_fixture.h"

TEST_GROUP_RUNNER(Video)
{
  RUN_TEST_CASE(Video, testVideo_Abrir_Vazio);
  RUN_TEST_CASE(Video, testVideo_Abrir_Invalido);
  RUN_TEST_CASE(Video, testVideo_Abrir_Valido);
  RUN_TEST_CASE(Video, testVideo_Gravar_Vazio);
  RUN_TEST_CASE(Video, testVideo_Gravar_Grayscale);
  RUN_TEST_CASE(Video, testVideo_Gravar_Cores);
  RUN_TEST_CASE(Video, testVideo_Gravar_Audio);
  RUN_TEST_CASE(Video, testVideo_Obter_Quadro);
  RUN_TEST_CASE(Video, testVideo_Controlar);
}
