#include "atvideo.h"

E4C_DEFINE_EXCEPTION(ATVideoException, "Algum erro de vídeo", RuntimeException);
E4C_DEFINE_EXCEPTION(ATVideoNaoExisteException, "Não existe este vídeo", ATVideoException);
E4C_DEFINE_EXCEPTION(ATVideoInvalidoException, "Algum erro de vídeo", RuntimeException);
E4C_DEFINE_EXCEPTION(ATVideoAbrirNaoExisteException, "Não pode abrir um vídeo que não existe", ATVideoNaoExisteException);
E4C_DEFINE_EXCEPTION(ATVideoAbrirInvalidoException, "Não pode abrir um vídeo que não é um vídeo mesmo", ATVideoInvalidoException);

void iniciarContextoVideo()
{
  avcodec_register_all();
}


ATVideo* criarVideo(char* nome, ATVideoTipo tipo)
{
  ATVideo* video = malloc(sizeof(ATVideo));
  video->altura = 0;
  video->largura = 0;
  video->totalframes = 0;
  video->canais = 0;
  video->tipo = tipo;
  video->formato = AT_YUV;
  return video;
}
void abrirVideo(ATVideo* video)
{
  AVCodec* codec;
  AVCodecContext* c = NULL;
  int frame_count;
  FILE *f;
  AVFrame *frame;
  uint8_t inbuf[INBUF_SIZE];
  AVPacket avpkt;
  av_init_packet(&avpkt);

  AVOutputFormat* format = av_guess_format(NULL,video->nome,NULL);
  if(!format)
  {
    throw(ATVideoException, "Não pode achar o formato");
  }
  CodecID codecID = av_guess_codec(format, NULL, video->nome, NULL, AVMEDIA_TYPE_VIDEO);
  if(!codec)
  {
    throw(ATVideoException, "Não pode achar o codec");
  }
  c = avcodec_alloc_context3(codec);
  if(!c)
  {
    throw(ATVideoException, "Não pode abrir um contexto para o codec do vídeo");
  }


  if(codec->capabilities & )



  throw(ATVideoAbrirNaoExisteException, "O vídeo não pode ser aberto");
}

unsigned char videoEstaAberto(ATVideo* video)
{
  return 1;
}

void fecharVideo(ATVideo* video)
{

}

void destruirVideo(ATVideo* video)
{
  free(video);
}
