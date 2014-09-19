#include "atvideo.h"

E4C_DEFINE_EXCEPTION(ATVideoException, "Algum erro de vídeo", RuntimeException);
E4C_DEFINE_EXCEPTION(ATVideoNaoExisteException, "Não existe este vídeo", ATVideoException);
E4C_DEFINE_EXCEPTION(ATVideoInvalidoException, "Algum erro de vídeo", RuntimeException);
E4C_DEFINE_EXCEPTION(ATVideoAbrirNaoExisteException, "Não pode abrir um vídeo que não existe", ATVideoNaoExisteException);
E4C_DEFINE_EXCEPTION(ATVideoAbrirInvalidoException, "Não pode abrir um vídeo que não é um vídeo mesmo", ATVideoInvalidoException);
E4C_DEFINE_EXCEPTION(ATVideoFormatoException, "Erro sobre formato de vídeo", ATVideoException);
E4C_DEFINE_EXCEPTION(ATVideoCodecException, "Erro sobre codec de vídeo", ATVideoException);
E4C_DEFINE_EXCEPTION(ATVideoObterFrameException, "Erro ao obter o frame do vídeo", ATVideoException);

void _log_callback(void* seila, int algo, const char* outraCoisa, va_list teste)
{

}

void iniciarContextoVideo()
{
  av_register_all();
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
  video->nome = nome;
  return video;
}
void abrirVideo(ATVideo* video)
{
  FILE *f;
  if(video == NULL) throw(ATVideoException, "Objeto Vídeo Nulo");
  f = fopen(video->nome, "rb");
  if(!f)
  {
    throw(ATVideoAbrirNaoExisteException, "Não pode abrir o vídeo");
  }

  AVCodec* codec = NULL;
  AVCodecContext* c = NULL;
  int frame_count;

  AVFrame *frame;
  uint8_t inbuf[INBUF_SIZE];
  AVPacket avpkt;
  av_init_packet(&avpkt);


  AVOutputFormat* format = av_guess_format(NULL,video->nome,NULL);
  if(!format)
  {
    throw(ATVideoFormatoException, "Não pode achar o formato");
  }
  enum AVCodecID codecID = av_guess_codec(format, NULL, video->nome, NULL, AVMEDIA_TYPE_VIDEO);
  if(!codecID)
  {
    throw(ATVideoCodecException, "Não pode achar o id do codec");
  }
  codec = avcodec_find_decoder(codecID);
  if(!codec)
  {
    throw(ATVideoCodecException, "Não pode achar o codec");
  }
  c = avcodec_alloc_context3(codec);
  if(!c)
  {
    throw(ATVideoCodecException, "Não pode abrir um contexto para o codec do vídeo");
  }

  if(codec->capabilities & CODEC_CAP_TRUNCATED)
    c->flags|= CODEC_FLAG_TRUNCATED;

  if(avcodec_open2(c,codec,NULL)<0)
  {
    throw(ATVideoCodecException, "Não pode abrir o codec");
  }

  frame = av_frame_alloc();
  if(!frame)
  {
    throw(ATVideoException, "Não pode alocar o quadro do vídeo");
  }

  // Essa parte ficará no obterVideoQuadro()

  frame_count = 0;
  for(;;)
  {
    avpkt.size = fread(inbuf, 1, INBUF_SIZE, f);
    if(avpkt.size == 0) break;
    avpkt.data = inbuf;
    while(avpkt.size > 0)
    {
      // Decodificar o frame
      int got_frame, len;

      len = avcodec_decode_video2(c, frame, &got_frame, &avpkt);
      if(len < 0)
      {
        throw(ATVideoObterFrameException, "Não foi possível decodificar o frame");
      }
      if(got_frame)
      {
        // Usar o frame (Criar um ATImage com ele)

      }
      if(avpkt.data)
      {
        avpkt.size -= len;
        avpkt.data += len;
      }
    }
  }
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
