#include "atcamera.h"

ATCamera* abrirCamera(char* nome, int largura, int altura, ATEspacoCores espacoCores)
{
  // Abrir o arquivo do dispositivo
  struct stat st;
  int fd;


  if(-1 == stat(nome, &st)) throw(ATCameraAbrirException, "Não pode identificar o caminho");
  if(!S_ISCHR(st.st_mode))  throw(ATCameraAbrirException, "O arquivo identificado não é um dispositivo");
  fd = open(nome, O_RDWR | O_NONBLOCK, 0);
  if(-1 == fd)              throw(ATCameraAbrirException, "Não consigo abrir o dispositivo");

  // Iniciar o dispositivo
  struct v4l2_capability cap;
  struct v4l2_cropcap cropcap;
  struct v4l2_crop crop;
  struct v4l2_format format;
  unsigned int min;
  if(-1 == xioctl(fd, VIDIOC_QUERYCAP, &cap))
  {
    if(EINVAL == errno) throw(ATCameraAbrirNaoEhV4LException, "Não é um dispositivo V4L");
    else                throw(ATCameraAbrirException, "Erro ao abrir dispositivo");
  }
  if(!(cap.capabilities & V4L2_CAP_VIDEO_CAPTURE)) throw(ATCameraAbrirCapturaException, "Não é um dispositivo de captura");
  if(!(cap.capabilities & V4L2_CAP_READWRITE))     throw(ATCameraAbrirLeituraEscritaException, "Não suporta escrita e leitura");
  CLEAR(cropcap);
  cropcap.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;

  if(0 == xioctl(fd, VIDIOC_CROPCAP, &cropcap))
  {
    crop.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
    crop.c = cropcap.defrect
    if(-1 == xioctl(fd, VIDIOC_S_CROP, &crop))
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
  if(largura > 0 && altura > 0)
  {
    format.fmt.pix.width = largura;
    format.fmt.pix.height = altura;
    switch(espacoCores)
    {
      case AT_RGB: format.fmt.pix.pixelformat = V4L2_PIX_FMT_RGB24;break;
      case AT_YUV: format.fmt.pix.pixelformat = V4L2_PIX_FMT_YUV444;break;
      case AT_YCBCR: format.fmt.pix.pixelformat = V4L2_PIX_FMT_YUV444;break;
      default: format.fmt.pix.pixelformat = V4L2_PIX_FMT_YUV444;break;
    }
    format.fmt.pix.field = V4L2_FIELD_INTERLACED;
    if(-1 == xioctl(fd, VIDIOC_S_FMT,&format)) throw(ATCameraAbrirException,"VIDIOC_S_FMT");
  }
  else
  {
    if(-1 == xioctl(fd, VIDIOC_G_FMT,&format)) throw(ATCameraAbrirException,"VIDIOC_G_FMT");
  }

  /* Bruxaria nos drivers. Deveria ser:
     - 2bytes*Largura = Tamanho em bytes de uma linha  (bytesperline)
     - 2bytes*Largura*Altura = Tamanho total da imagem (sizeimage)
  */
  min = format.fmt.pix.width * 2;
  if (format.fmt.pix.bytesperline < min)
          format.fmt.pix.bytesperline = min;
  min = format.fmt.pix.bytesperline * format.fmt.pix.height;
  if (format.fmt.pix.sizeimage < min)
          format.fmt.pix.sizeimage = min;


  buffers = calloc(1, sizeof(*buffers));
  if(!buffers)


  format.fmt.pix.pixelformat = V4L2_PIX_FMT
  if(force)

  if(force_format)

}

void fecharCamera(ATCamera* camera)
{

}

void atualizarQuadro(ATCamera* camera)
{

}

void destruirCamera(ATCamera* camera)
{

}
