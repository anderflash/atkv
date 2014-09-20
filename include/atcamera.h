#ifndef ATCAMERA_H
#define ATCAMERA_H

#include <stdio.h>

#include "atcores.h"
#include "e4c.h"

#include <fcntl.h>
#include <string.h>

#include <errno.h>
#include <sys/stat.h>
#include <unistd.h>
#include <sys/mman.h>
#include <sys/ioctl.h>
#include <sys/time.h>
#include <linux/videodev2.h>

#define CLEAR(x) memset(&(x), 0, sizeof(x))

#ifdef __cplusplus
namespace at{
extern "C"{
#endif

E4C_DECLARE_EXCEPTION(ATCameraException);
E4C_DECLARE_EXCEPTION(ATCameraAbrirException);
E4C_DECLARE_EXCEPTION(ATCameraAbrirNaoEhV4LException);
E4C_DECLARE_EXCEPTION(ATCameraAbrirCapturaException);
E4C_DECLARE_EXCEPTION(ATCameraAbrirLeituraEscritaException);
E4C_DECLARE_EXCEPTION(ATCameraAtualizarQuadroException);

struct buffer {
  void   *start;
  size_t  length;
};

typedef struct
{
  int largura;
  int altura;
  int fps;
  ATEspacoCores espacoCores;
  char* nome;
  struct buffer *buffer;
  int n_buffers;
  int fd;
}ATCamera;


void abrirCamera(char* nome, int largura, int altura, ATEspacoCores espacoCores, ATCamera** cam);
void fecharCamera(ATCamera* camera);
void atualizarQuadro(ATCamera* camera);
void destruirCamera(ATCamera* camera);
#ifdef __cplusplus
}}
#endif

#endif // ATCAMERA_H
