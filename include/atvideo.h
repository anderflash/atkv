#ifndef ATVIDEO_H
#define ATVIDEO_H

#include "e4c.h"
#include <libavcodec/avcodec.h>
#include <libavformat/avformat.h>
#include <libswscale/swscale.h>
#include "atcores.h"

#define INBUF_SIZE 4096
#define AUDIO_INBUF_SIZE 20480
#define AUDIO_REFILL_THRESH 4096
#ifdef __cplusplus
namespace at{
extern "C"{
#endif

E4C_DECLARE_EXCEPTION(ATVideoException);
E4C_DECLARE_EXCEPTION(ATVideoNaoExisteException);
E4C_DECLARE_EXCEPTION(ATVideoFormatoException);
E4C_DECLARE_EXCEPTION(ATVideoCodecException);
E4C_DECLARE_EXCEPTION(ATVideoAbrirNaoExisteException);
E4C_DECLARE_EXCEPTION(ATVideoInvalidoException);
E4C_DECLARE_EXCEPTION(ATVideoAbrirInvalidoException);
E4C_DECLARE_EXCEPTION(ATVideoObterFrameException);

typedef enum{AT_MPEG, AT_AVI, AT_MOV, AT_FLV, AT_MP4} ATVideoTipo;

typedef struct
{
  int              largura;
  int              altura;
  int              totalframes;
  int              frameAtual;
  int64_t          duracaoTotal;
  int64_t          tempoInicial;
  int64_t          timestampAtual;
  int              bit_rate;

  ATEspacoCores    espacoCores;
  ATVideoTipo      tipo;
  int              canais;
  char*            nome;
  unsigned char*   frameBuffer;

  int              frameFinished;
  int              videoStreamIdx;
  AVFormatContext* pFormatCtx;
  AVCodecContext*  pCodecCtx;
  AVCodec*         pCodec;
  AVFrame*         pFrame;
  AVFrame*         pFrameRGB;
  AVPacket         packet;

  int              tocando;

  struct SwsContext *img_convert_ctx;
}ATVideo;

void iniciarContextoVideo();
ATVideo* criarVideo(char* nome);
void abrirVideo(ATVideo* video);
unsigned char videoEstaAberto(ATVideo* video);
void fecharVideo(ATVideo* video);
void destruirVideo(ATVideo* video);
void obterQuadro(ATVideo* video);

void tocarVideo(ATVideo* video);
void pausarVideo(ATVideo* video);
void rebobinarVideo(ATVideo* video);
void pararVideo(ATVideo* video);
void irParaTempo(ATVideo *video, int64_t timestamp);
void irParaFrame(ATVideo* video, int frame);

#ifdef __cplusplus
}}
#endif
#endif // ATVIDEO_H

