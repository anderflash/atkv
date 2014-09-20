#include "unity.h"
#include "unity_fixture.h"

TEST_GROUP_RUNNER(Camera)
{
  RUN_TEST_CASE(Camera, testCamera_Abrir_Invalido);
  RUN_TEST_CASE(Camera, testCamera_Abrir_Valido);
  RUN_TEST_CASE(Camera, testImagem_Obter_Frame);
}
