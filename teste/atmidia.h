#ifndef ATMIDIA_H
#define ATMIDIA_H

#include <stdio.h>
#include <jpeglib.h>
#include <setjmp.h>

struct my_error_mgr {
struct jpeg_error_mgr pub;	/* "public" fields */
jmp_buf setjmp_buffer;	/* for return to caller */
};
typedef struct my_error_mgr * my_error_ptr;

typedef struct ImagemJPEG
{
	int largura;
	int altura;
	int canais;
	int formato;
	JSAMPARRAY buffer;
}ImagemJPEG;

/** Estrutura simples de imagem
 * - Dados
 * - Largura
 * - Altura
 * - Formato de Cada Pixel
 * - Número de Canais
 */
// typedef struct Imagem
// {
// 	int largura;
// 	int altura;
// 	int canais;
// 	int formato;
// } Imagem;

/** Estrutura simples de vídeo
 * - Formato do vídeo
 * - FPS
 * - Largura
 * - Altura
 * - Número de Canais
 */
// typedef struct Video
// {
// 	int largura;
// 	int altura;
// 	int canais;
// 	int formato;
// } Video;


// typedef struct Camera
// {
// 	// Ver o que pode conter aqui
// } Camera;

// Verificar o tamanho da imagem


/* IO JPEG */
/** ler imagem JPEG */
int lerJPEG(const char* nome, ImagemJPEG* imagem);
/** escrever imagem em JPEG */
//void escreverJPEG(const char* nome, void *);

// // IO BITMAP
// void lerBITMAP();
// void escreverBITMAP();

// // IO PNG
// void lerPNG();
// void escreverPNG();

// // IO GIF
// void lerGIF();
// void escreverGIF();


// // Processos com Imagens
// // somar imagem com imagem
// void operator+

// // subtrair imagem com imagem
// void operator-

// // multiplicar imagem com imagem
// void operator*

// // dividir imagem com imagem
// void operator/

// Processos com Vídeos



// Conversão de Imagens



#endif