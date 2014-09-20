#include "atcamera.h"

E4C_DEFINE_EXCEPTION(ATCameraException,               "Erro na câmera", RuntimeException);
E4C_DEFINE_EXCEPTION(ATCameraAbrirException,          "Erro ao abrir a câmera", ATCameraException);
E4C_DEFINE_EXCEPTION(ATCameraAbrirNaoEhV4LException,  "Dispositivo não é suportado pelo Video4Linux", ATCameraAbrirException);
E4C_DEFINE_EXCEPTION(ATCameraAbrirCapturaException,   "Erro no processo de captura da câmera", ATCameraAbrirException);
E4C_DEFINE_EXCEPTION(ATCameraAbrirLeituraEscritaException, "O dispositivo não suporta leitura/escrita", ATCameraException);
E4C_DEFINE_EXCEPTION(ATCameraAtualizarQuadroException, "Erro ao atualizar o quadro", ATCameraException);

static int xioctl(int fh, int request, void *arg)
{
  int r;

  do {
          r = ioctl(fh, request, arg);
  } while (-1 == r && EINTR == errno);

  return r;
}

static void errno_print(const char *s)
{
  fprintf(stderr, "%s error %d, %s\n", s, errno, strerror(errno));
}

void abrirCamera(char* nome, int largura, int altura, ATEspacoCores espacoCores, ATCamera** cam)
{
  // Abrir o arquivo do dispositivo
  struct stat st;

  if(-1 == stat(nome, &st)) throw(ATCameraAbrirException, "Não pode identificar o caminho");
  if(!S_ISCHR(st.st_mode))  throw(ATCameraAbrirException, "O arquivo identificado não é um dispositivo");


  ATCamera* camera = malloc(sizeof(ATCamera));
  camera->nome = nome;
  camera->fd = -1;
  camera->fd = open(camera->nome, O_RDWR /* required */ | O_NONBLOCK, 0);

  if(-1 == camera->fd)       throw(ATCameraAbrirException, "Não consigo abrir o dispositivo");

  // Iniciar o dispositivo
  struct v4l2_capability cap;
  struct v4l2_cropcap cropcap;
  struct v4l2_crop crop;
  struct v4l2_format format;
  unsigned int min;
  if(-1 == xioctl(camera->fd, VIDIOC_QUERYCAP, &cap))
  {
    if(EINVAL == errno) throw(ATCameraAbrirNaoEhV4LException, "Não é um dispositivo V4L");
    else                throw(ATCameraAbrirException, "Erro ao abrir dispositivo");
  }
  if(!(cap.capabilities & V4L2_CAP_VIDEO_CAPTURE)) throw(ATCameraAbrirCapturaException, "Não é um dispositivo de captura");
  if(!(cap.capabilities & V4L2_CAP_STREAMING))     throw(ATCameraAbrirLeituraEscritaException, "Não suporta escrita e leitura");
  //if(!(cap.capabilities & V4L2_CAP_READWRITE))     throw(ATCameraAbrirLeituraEscritaException, "Não suporta escrita e leitura");
  CLEAR(cropcap);
  cropcap.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;

  if(0 == xioctl(camera->fd, VIDIOC_CROPCAP, &cropcap))
  {
    crop.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
    crop.c = cropcap.defrect;
    if(-1 == xioctl(camera->fd, VIDIOC_S_CROP, &crop))
    {
      switch(errno)
      {
        case EINVAL: /*Não pode cortar*/ break;
        default:     /*Outros erros ignorados*/break;
      }
    }
  }
  else
  {
    /* Erros ignorados */
  }

  CLEAR(format);
  format.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
  camera->espacoCores = espacoCores;
  if(largura > 0 && altura > 0)
  {
    format.fmt.pix.width = largura;
    format.fmt.pix.height = altura;
    switch(espacoCores)
    {
      case AT_RGB: format.fmt.pix.pixelformat = V4L2_PIX_FMT_RGB32;break;
      case AT_YUV: format.fmt.pix.pixelformat = V4L2_PIX_FMT_YUV444;break;
      case AT_YCBCR: format.fmt.pix.pixelformat = V4L2_PIX_FMT_YUV444;break;
      case AT_CINZA: format.fmt.pix.pixelformat = V4L2_PIX_FMT_GREY;break;
      default: format.fmt.pix.pixelformat = V4L2_PIX_FMT_YUV444;break;
    }
    //format.fmt.pix.field = V4L2_FIELD_INTERLACED;
    if(-1 == xioctl(camera->fd, VIDIOC_S_FMT,&format)) throw(ATCameraAbrirException,"VIDIOC_S_FMT");
  }
  else
  {
    if(-1 == xioctl(camera->fd, VIDIOC_G_FMT,&format)) throw(ATCameraAbrirException,"VIDIOC_G_FMT");
  }

  /* Bruxaria nos drivers. Deveria ser:
     - 2bytes*Largura = Tamanho em bytes de uma linha  (bytesperline)
     - 2bytes*Largura*Altura = Tamanho total da imagem (sizeimage)
  */
  min = format.fmt.pix.width * 3;
  if (format.fmt.pix.bytesperline < min)
          format.fmt.pix.bytesperline = min;
  min = format.fmt.pix.bytesperline * format.fmt.pix.height;
  if (format.fmt.pix.sizeimage < min)
          format.fmt.pix.sizeimage = min;

  camera->largura = format.fmt.pix.width;
  camera->altura = format.fmt.pix.height;

  // Iniciar Streaming
  struct v4l2_requestbuffers req;

  CLEAR(req);

  req.count = 4;
  req.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
  req.memory = V4L2_MEMORY_MMAP;

  if (-1 == xioctl(camera->fd, VIDIOC_REQBUFS, &req))
    throw(ATCameraAbrirCapturaException, "Erro na requisição user point");

  if (req.count < 2) throw(ATCameraAbrirCapturaException, "Memória insuficiente");

  camera->buffer = calloc(req.count, sizeof(*camera->buffer));
  if (!camera->buffer) throw(ATCameraAbrirException, "Faltou memória");

  for (camera->n_buffers = 0; camera->n_buffers < req.count; ++camera->n_buffers) {
    struct v4l2_buffer buf;

    CLEAR(buf);

    buf.type        = V4L2_BUF_TYPE_VIDEO_CAPTURE;
    buf.memory      = V4L2_MEMORY_MMAP;
    buf.index       = camera->n_buffers;

    if (-1 == xioctl(camera->fd, VIDIOC_QUERYBUF, &buf)) throw(ATCameraAbrirException, "VIDIOC_QUERYBUF");

    camera->buffer[camera->n_buffers].length = buf.length;
    camera->buffer[camera->n_buffers].start =
            mmap(NULL /* start anywhere */,
                  buf.length,
                  PROT_READ | PROT_WRITE /* required */,
                  MAP_SHARED /* recommended */,
                  camera->fd, buf.m.offset);

    if (MAP_FAILED == camera->buffer[camera->n_buffers].start) throw(ATCameraAbrirException, "mmap");
  }

  // Iniciar a captura (ainda não captura de fato)
  unsigned int i;
  enum v4l2_buf_type type;

  for (i = 0; i < camera->n_buffers; ++i)
  {
    struct v4l2_buffer buf;

    CLEAR(buf);
    buf.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
    buf.memory = V4L2_MEMORY_MMAP;
    buf.index = i;

    if (-1 == xioctl(camera->fd, VIDIOC_QBUF, &buf)) throw(ATCameraAbrirException, "VIDIOC_QBUF");
  }
  type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
  if (-1 == xioctl(camera->fd, VIDIOC_STREAMON, &type)) throw(ATCameraAbrirException, "VIDIOC_STREAMON");



  *cam = camera;
}

void fecharCamera(ATCamera* camera)
{
  int i;
  enum v4l2_buf_type type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
  if (-1 == xioctl(camera->fd, VIDIOC_STREAMOFF, &type)) throw(ATCameraException, "Não consigo desligar a câmera");


  for (i = 0; i < camera->n_buffers; ++i)
    if (-1 == munmap(camera->buffer[i].start, camera->buffer[i].length)) throw(ATCameraException, "Não consigo liberar os buffers da câmera");

  free(camera->buffer);
  if (-1 == close(camera->fd)) throw(ATCameraException, "Erro ao fechar o arquivo do dispositivo");
  camera->fd = -1;
}

void atualizarQuadro(ATCamera* camera)
{
  int obteveQuadro = 0;
  while(obteveQuadro < 70)
  {
    fd_set fds;
    struct timeval tv;
    int r;
    FD_ZERO(&fds);
    FD_SET(camera->fd, &fds);

    /* Timeout. */
    tv.tv_sec = 2;
    tv.tv_usec = 0;

    r = select(camera->fd + 1, &fds, NULL, NULL, &tv);

    if(r == 0) throw(ATCameraAtualizarQuadroException, "");


    if(-1 == r)
    {
      if(EINTR != errno) throw(ATCameraAtualizarQuadroException, "");
    }
    else
    {
      struct v4l2_buffer buf;
      unsigned int i;

      CLEAR(buf);

      buf.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
      buf.memory = V4L2_MEMORY_MMAP;

      if (-1 == xioctl(camera->fd, VIDIOC_DQBUF, &buf))
      {

        if(errno != EAGAIN) throw(ATCameraAtualizarQuadroException, "Erro ao atualizar o quadro da câmera");
      }
      else
      {
        assert(buf.index < camera->n_buffers);

        //process_image(buffers[buf.index].start, buf.bytesused);
        obteveQuadro++;
        printf("%d\n",buf.bytesused);
        fflush(stderr);
        // fwrite(camera->buffer[buf.index].start, buf.bytesused, 1, stdout);
        fprintf(stderr, "uau");
        fflush(stdout);

        if (-1 == xioctl(camera->fd, VIDIOC_QBUF, &buf)) throw(ATCameraException, "Erro ao atualizar o quadro da câmera");
      }
    }
  }
}

void destruirCamera(ATCamera* camera)
{
  free(camera);
}
