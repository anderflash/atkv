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


ATVideo* criarVideo(char* nome)
{
  AVOutputFormat* format = av_guess_format(NULL,nome,NULL);
  if(!format)
  {
    throw(ATVideoFormatoException, "Não pode achar o formato");
  }

  ATVideo* video = malloc(sizeof(ATVideo));
  video->altura = 0;
  video->largura = 0;
  video->totalframes = 0;
  video->canais = 0;
  //video->tipo = tipo;
  video->espacoCores = AT_YUV;
  video->nome = nome;
  video->bit_rate = 0;
  video->duracaoTotal = 0;
  video->frameAtual = 0;
  video->frameBuffer = NULL;
  video->img_convert_ctx = NULL;
  video->pCodec = NULL;
  video->pCodecCtx = NULL;
  video->pFormatCtx = NULL;
  video->pFrame = NULL;
  return video;
}

//int decode_interrupe
//const AVIOInterruptCB int_cb = {decode_interrupt_cb, NULL};

void abrirVideo(ATVideo* video)
{
  video->videoStreamIdx = -1;

  // Consegue abrir o vídeo?
  if(avformat_open_input(&video->pFormatCtx, video->nome, NULL,NULL))
    throw(ATVideoAbrirInvalidoException, "Não pode abrir o vídeo");
  // Consegue achar informações do stream?
  if(avformat_find_stream_info(video->pFormatCtx, NULL) < 0)
    throw(ATVideoAbrirInvalidoException, "Não tem nenhuma stream no arquivo");
  // Exiba mensagens
  av_dump_format(video->pFormatCtx, 0, video->nome, 0);

  video->bit_rate = video->pFormatCtx->bit_rate;
  video->duracaoTotal = video->pFormatCtx->duration;
  video->tempoInicial = video->pFormatCtx->start_time;
  video->frameAtual = 0;

  // Procurando stream de vídeo
  int i;
  for(i = 0; i < video->pFormatCtx->nb_streams; i++)
  {
    if(video->pFormatCtx->streams[i]->codec->codec_type == AVMEDIA_TYPE_VIDEO)
    {
      video->videoStreamIdx = i;
      break;
    }
  }
  if(video->videoStreamIdx == -1)
    throw(ATVideoAbrirInvalidoException, "Não há stream de vídeo");

  video->pCodecCtx = video->pFormatCtx->streams[video->videoStreamIdx]->codec;

  // Procurando o codec do stream de vídeo
  video->pCodec = avcodec_find_decoder(video->pCodecCtx->codec_id);
  if(video->pCodec == NULL)
    throw(ATVideoCodecException, "Não pode achar o codec do vídeo");

  // Abrindo o codec
  if(avcodec_open2(video->pCodecCtx, video->pCodec, NULL) < 0)
    throw(ATVideoCodecException, "Não pode abrir o codec do vídeo");

  video->pFrame = av_frame_alloc();
  video->pFrameRGB = av_frame_alloc();

  int numBytes = avpicture_get_size(AV_PIX_FMT_RGB24, video->pCodecCtx->width, video->pCodecCtx->height);

  video->frameBuffer = (unsigned char*) av_malloc(numBytes * sizeof(unsigned char));

  avpicture_fill((AVPicture*) video->pFrameRGB, video->frameBuffer, AV_PIX_FMT_RGB24, video->pCodecCtx->width, video->pCodecCtx->height);

  video->largura = video->pCodecCtx->width;
  video->altura = video->pCodecCtx->height;
  video->img_convert_ctx = sws_getCachedContext(NULL,
                                                video->largura, video->altura,
                                                video->pCodecCtx->pix_fmt,
                                                video->largura, video->altura,
                                                AV_PIX_FMT_RGB24,SWS_BICUBIC, NULL, NULL, NULL);
  video->canais = 3;
  video->espacoCores = AT_RGB;
}


//  int i = 0;
//  while((av_read_frame(pFormatCtx, &packet) >= 0) && (i < 1))
//  {
//
//  }
//
//
//
//
//
//  pFormatCtx = avformat_alloc_context();
//
//
//  pFormatCtx->interrupt_callback = int_cb;
//  if(avio_open2(&pFormatCtx->pb, video->nome, AVIO_FLAG_READ, &pFormatCtx->interrupt_callback, NULL))
//
//  avio_open2()

//  AVCodec* codec = NULL;
//  AVCodecContext* c = NULL;
//  int frame_count;
//  AVFrame *frame;
//  uint8_t inbuf[INBUF_SIZE];
//  AVPacket avpkt;
//
//
//  // Arquivo existe?
//  FILE *f;
//  if(video == NULL) throw(ATVideoException, "Objeto Vídeo Nulo");
//  f = fopen(video->nome, "rb");
//  if(!f)
//  {
//    throw(ATVideoAbrirNaoExisteException, "Não pode abrir o vídeo");
//  }
//
//  // Setar valores padrão no pacote
//  av_init_packet(&avpkt);
//
//  // Descobrir a extensão do arquivo
//  // Se não for um vídeo, é aqui que capturamos o erro
//  AVOutputFormat* format = av_guess_format(NULL,video->nome,NULL);
//  if(!format)
//  {
//    throw(ATVideoFormatoException, "Não pode achar o formato");
//  }
//
//  // Agora precisamos saber o codec que trabalha com o formato
//  // Primeiro, o identificador
//  enum AVCodecID codecID = av_guess_codec(format, NULL, video->nome, NULL, AVMEDIA_TYPE_VIDEO);
//  if(!codecID)
//  {
//    throw(ATVideoCodecException, "Não pode achar o id do codec");
//  }
//  // Depois o próprio codec
//  codec = avcodec_find_decoder(codecID);
//  if(!codec)
//  {
//    throw(ATVideoCodecException, "Não pode achar o codec");
//  }
//  // E agora as informações de execução do codec (o contexto)
//  c = avcodec_alloc_context3(codec);
//  if(!c)
//  {
//    throw(ATVideoCodecException, "Não pode abrir um contexto para o codec do vídeo");
//  }
//  // Alguns codecs não mandam frames completos
//  // O algoritmo precisa saber disso
//  if(codec->capabilities & CODEC_CAP_TRUNCATED)
//    c->flags|= CODEC_FLAG_TRUNCATED;
//
//  // Agora precisamos abrir o Codec
//  if(avcodec_open2(c,codec,NULL)<0)
//  {
//    throw(ATVideoCodecException, "Não pode abrir o codec");
//  }
//
//  // Criando memória para o objeto frame (os dados ainda não)
//  frame = av_frame_alloc();
//  if(!frame)
//  {
//    throw(ATVideoException, "Não pode alocar o quadro do vídeo");
//  }
//
//  // Precisamos capturar o primeiro frame para saber informações
//  // (largura, altura, frame rate, bit rate... será que duração e totalframes também?)
//  int got_frame = 0;
//
//  for(;;)
//  {
//  }
//
//
//  // Essa parte ficará no obterVideoQuadro()
//
//  frame_count = 0;
//  for(;;)
//  {
//    avpkt.size = fread(inbuf, 1, INBUF_SIZE, f);
//    if(avpkt.size == 0) break;
//    avpkt.data = inbuf;
//    while(avpkt.size > 0)
//    {
//      // Decodificar o frame
//      int got_frame, len;
//
//      len = avcodec_decode_video2(c, frame, &got_frame, &avpkt);
//      if(len < 0)
//      {
//        throw(ATVideoObterFrameException, "Não foi possível decodificar o frame");
//      }
//      if(got_frame)
//      {
//        // Usar o frame (Criar um ATImage com ele)
//
//      }
//      if(avpkt.data)
//      {
//        avpkt.size -= len;
//        avpkt.data += len;
//      }
//    }
//  }
//  throw(ATVideoAbrirNaoExisteException, "O vídeo não pode ser aberto");

unsigned char videoEstaAberto(ATVideo* video)
{
  return 1;
}

void fecharVideo(ATVideo* video)
{

}

void destruirVideo(ATVideo* video)
{
  av_free(video->frameBuffer);
  av_free(video->pFrame);
  avcodec_close(video->pCodecCtx);
  avformat_close_input(&video->pFormatCtx);
  free(video);
}

void obterQuadro(ATVideo* video)
{
  int i = 0;
  while((av_read_frame(video->pFormatCtx, &video->packet)>=0)&&(i < 10000))
  {
    if(video->packet.stream_index == video->videoStreamIdx)
    {
      avcodec_decode_video2(video->pCodecCtx, video->pFrame, &video->frameFinished, &video->packet);
      if(video->frameFinished)
      {
        i++;
        sws_scale(video->img_convert_ctx,
                  (const uint8_t * const *)video->pFrame->data,
                  video->pFrame->linesize,
                  0,
                  video->pCodecCtx->height,
                  video->pFrameRGB->data,
                  video->pFrameRGB->linesize);
      }
    }
    av_free_packet(&video->packet);
  }
}


void tocarVideo(ATVideo* video)
{
  video->tocando = 1;
}
void pausarVideo(ATVideo* video)
{
  video->tocando = 0;
}
void rebobinarVideo(ATVideo* video)
{
  irParaFrame(video, 0);
}
void pararVideo(ATVideo* video)
{
  pausarVideo(video);
  rebobinarVideo(video);
}
void irParaTempo(ATVideo *video, int64_t timestamp)
{
  //av_seek_frame(video->pFormatCtx, video->videoStreamIdx, timestamp, (timestamp < 0)?AVSEEK_FLAG_BACKWARD:0)
}
void irParaFrame(ATVideo* video, int frame)
{
  //
}
