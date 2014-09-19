#include "unity_fixture.h"

static void TodosTestes(void)
{
    //RUN_TEST_GROUP(Imagem);
    RUN_TEST_GROUP(Video);
    //RUN_TEST_GROUP(Camera);
}

int main(int argc, char**argv)
{
    return UnityMain(argc, argv, TodosTestes);
}
