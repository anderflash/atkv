#ifndef ATVIDEO_H
#define ATVIDEO_H

#include "e4c.h"
#include <libavcodec/avcodec.h>
#include <libavformat/avformat.h>

#define INBUF_SIZE 4096
#define AUDIO_INBUF_SIZE 20480
#define AUDIO_REFILL_THRESH 4096

E4C_DECLARE_EXCEPTION(ATVideoException);
E4C_DECLARE_EXCEPTION(ATVideoNaoExisteException);
E4C_DECLARE_EXCEPTION(ATVideoFormatoException);
E4C_DECLARE_EXCEPTION(ATVideoCodecException);
E4C_DECLARE_EXCEPTION(ATVideoAbrirNaoExisteException);
E4C_DECLARE_EXCEPTION(ATVideoInvalidoException);
E4C_DECLARE_EXCEPTION(ATVideoAbrirInvalidoException);
E4C_DECLARE_EXCEPTION(ATVideoObterFrameException);

typedef enum{AT_YUV, AT_RGB, AT_GRAYSCALE} ATVideoFormato;
typedef enum{AT_MPEG, AT_AVI, AT_MOV, AT_FLV, AT_MP4} ATVideoTipo;

typedef struct
{
  int largura;
  int altura;
  int totalframes;
  int frameAtual;
  ATVideoFormato formato;
  ATVideoTipo tipo;
  int canais;
  char* nome;
}ATVideo;

void iniciarContextoVideo();
ATVideo* criarVideo(char* nome, ATVideoTipo tipo);
void abrirVideo(ATVideo* video);
unsigned char videoEstaAberto(ATVideo* video);
void fecharVideo(ATVideo* video);
void destruirVideo(ATVideo* video);

#endif // ATVIDEO_H

