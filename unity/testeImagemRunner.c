#include "unity.h"
#include "unity_fixture.h"

TEST_GROUP_RUNNER(Imagem)
{
    // Testes para JPEG
//    RUN_TEST_CASE(Imagem, testImagem_JPEG_Abrir_Vazio);
//    RUN_TEST_CASE(Imagem, testImagem_JPEG_Abrir_Invalido);
//    RUN_TEST_CASE(Imagem, testImagem_JPEG_Abrir_Valido);
//
//    RUN_TEST_CASE(Imagem, testImagem_JPEG_Escrever_Vazio);
//    RUN_TEST_CASE(Imagem, testImagem_JPEG_Escrever_TamanhoCoresVariadas);


//    RUN_TEST_CASE(Imagem, testImagem_JPEG_Converter_GrayScale_Vazio);
//    RUN_TEST_CASE(Imagem, testImagem_JPEG_Converter_GrayScale_Correto);
//    RUN_TEST_CASE(Imagem, testImagem_JPEG_Converter_Cores_Correto);
//
//    RUN_TEST_CASE(Imagem, testImagem_JPEG_Somar_Vazio);
//    RUN_TEST_CASE(Imagem, testImagem_JPEG_Somar_Correto);
//
//    RUN_TEST_CASE(Imagem, testImagem_JPEG_Subtrair_Vazio);
//    RUN_TEST_CASE(Imagem, testImagem_JPEG_Subtrair_Correto);
//
//    RUN_TEST_CASE(Imagem, testImagem_JPEG_Multiplicar_Vazio);
//    RUN_TEST_CASE(Imagem, testImagem_JPEG_Multiplicar_Correto);
//
//    RUN_TEST_CASE(Imagem, testImagem_JPEG_Dividir_Vazio);
//    RUN_TEST_CASE(Imagem, testImagem_JPEG_Dividir_Correto);
//
//    RUN_TEST_CASE(Imagem, testImagem_JPEG_Destruir_Vazio);
//    RUN_TEST_CASE(Imagem, testImagem_JPEG_Destruir_Correto);
//
//    RUN_TEST_CASE(Imagem, testImagem_JPEG_SubImagem_Vazio);
//    RUN_TEST_CASE(Imagem, testImagem_JPEG_SubImagem_Correto);
//
//    RUN_TEST_CASE(Imagem, testImagem_JPEG_Filtro_Vazio);
//    RUN_TEST_CASE(Imagem, testImagem_JPEG_Filtro_Correto);
//    RUN_TEST_CASE(Imagem, testImagem_JPEG_Correlacao_Vazio);
//    RUN_TEST_CASE(Imagem, testImagem_JPEG_Correlacao_Correto);
//    RUN_TEST_CASE(Imagem, testImagem_JPEG_Convolucao_Vazio);
//    RUN_TEST_CASE(Imagem, testImagem_JPEG_Convolucao_Correto);


    // Testes para PNG
    RUN_TEST_CASE(Imagem, testImagem_PNG_Abrir_Vazio);
    RUN_TEST_CASE(Imagem, testImagem_PNG_Abrir_Invalido);
    RUN_TEST_CASE(Imagem, testImagem_PNG_Abrir_Valido);

    RUN_TEST_CASE(Imagem, testImagem_PNG_Escrever_Vazio);
    RUN_TEST_CASE(Imagem, testImagem_PNG_Escrever_TamanhoCoresVariadas);
//
//
//    RUN_TEST_CASE(Imagem, testImagem_PNG_Converter_GrayScale_Vazio);
//    RUN_TEST_CASE(Imagem, testImagem_PNG_Converter_GrayScale_Correto);
//    RUN_TEST_CASE(Imagem, testImagem_PNG_Converter_Cores_Correto);
//
//
//    RUN_TEST_CASE(Imagem, testImagem_PNG_Somar_Vazio);
//    RUN_TEST_CASE(Imagem, testImagem_PNG_Somar_Correto);
//
//    RUN_TEST_CASE(Imagem, testImagem_PNG_Subtrair_Vazio);
//    RUN_TEST_CASE(Imagem, testImagem_PNG_Subtrair_Correto);
//
//    RUN_TEST_CASE(Imagem, testImagem_PNG_Multiplicar_Vazio);
//    RUN_TEST_CASE(Imagem, testImagem_PNG_Multiplicar_Correto);
//
//    RUN_TEST_CASE(Imagem, testImagem_PNG_Dividir_Vazio);
//    RUN_TEST_CASE(Imagem, testImagem_PNG_Dividir_Correto);
//
//    RUN_TEST_CASE(Imagem, testImagem_PNG_Destruir_Vazio);
//    RUN_TEST_CASE(Imagem, testImagem_PNG_Destruir_Correto);
//
//    RUN_TEST_CASE(Imagem, testImagem_PNG_SubImagem_Vazio);
//    RUN_TEST_CASE(Imagem, testImagem_PNG_SubImagem_Correto);
//
//    RUN_TEST_CASE(Imagem, testImagem_PNG_Filtro_Vazio);
//    RUN_TEST_CASE(Imagem, testImagem_PNG_Filtro_Correto);
//    RUN_TEST_CASE(Imagem, testImagem_PNG_Correlacao_Vazio);
//    RUN_TEST_CASE(Imagem, testImagem_PNG_Correlacao_Correto);
//    RUN_TEST_CASE(Imagem, testImagem_PNG_Convolucao_Vazio);
//    RUN_TEST_CASE(Imagem, testImagem_PNG_Convolucao_Correto);


    // Testes para TIFF
    RUN_TEST_CASE(Imagem, testImagem_TIFF_Abrir_Vazio);
    RUN_TEST_CASE(Imagem, testImagem_TIFF_Abrir_Invalido);
    RUN_TEST_CASE(Imagem, testImagem_TIFF_Abrir_Valido);

//    RUN_TEST_CASE(Imagem, testImagem_TIFF_Escrever_Vazio);
//    RUN_TEST_CASE(Imagem, testImagem_TIFF_Escrever_TamanhoCoresVariadas);
//
//
//    RUN_TEST_CASE(Imagem, testImagem_TIFF_Converter_GrayScale_Vazio);
//    RUN_TEST_CASE(Imagem, testImagem_TIFF_Converter_GrayScale_Correto);
//    RUN_TEST_CASE(Imagem, testImagem_TIFF_Converter_Cores_Correto);
//
//
//    RUN_TEST_CASE(Imagem, testImagem_TIFF_Somar_Vazio);
//    RUN_TEST_CASE(Imagem, testImagem_TIFF_Somar_Correto);
//
//    RUN_TEST_CASE(Imagem, testImagem_TIFF_Subtrair_Vazio);
//    RUN_TEST_CASE(Imagem, testImagem_TIFF_Subtrair_Correto);
//
//    RUN_TEST_CASE(Imagem, testImagem_TIFF_Multiplicar_Vazio);
//    RUN_TEST_CASE(Imagem, testImagem_TIFF_Multiplicar_Correto);
//
//    RUN_TEST_CASE(Imagem, testImagem_TIFF_Dividir_Vazio);
//    RUN_TEST_CASE(Imagem, testImagem_TIFF_Dividir_Correto);
//
//    RUN_TEST_CASE(Imagem, testImagem_TIFF_Destruir_Vazio);
//    RUN_TEST_CASE(Imagem, testImagem_TIFF_Destruir_Correto);
//
//    RUN_TEST_CASE(Imagem, testImagem_TIFF_SubImagem_Vazio);
//    RUN_TEST_CASE(Imagem, testImagem_TIFF_SubImagem_Correto);
//
//    RUN_TEST_CASE(Imagem, testImagem_TIFF_Filtro_Vazio);
//    RUN_TEST_CASE(Imagem, testImagem_TIFF_Filtro_Correto);
//    RUN_TEST_CASE(Imagem, testImagem_TIFF_Correlacao_Vazio);
//    RUN_TEST_CASE(Imagem, testImagem_TIFF_Correlacao_Correto);
//    RUN_TEST_CASE(Imagem, testImagem_TIFF_Convolucao_Vazio);
//    RUN_TEST_CASE(Imagem, testImagem_TIFF_Convolucao_Correto);

}
