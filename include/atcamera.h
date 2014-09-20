#ifndef ATCAMERA_H
#define ATCAMERA_H
#include "atcores.h"

#include <fcntl.h>

#include <sys/stat.h>
#include <errno.h>
#include <sys/ioctl.h>
#include <linux/videodev2.h>

#define CLEAR(x) memset(&(x), 0, sizeof(x))

typedef Struct
{
  int largura;
  int altura;
  int fps;
  ATEspacoCores espacoCores;
  char* nome;
  unsigned char* buffer;
}ATCamera;


ATCamera* abrirCamera(char* nome);
void fecharCamera(ATCamera* camera);
void atualizarQuadro(ATCamera* camera);
void destruirCamera(ATCamera* camera);


#endif // ATCAMERA_H
