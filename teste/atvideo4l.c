#include "atvideo4l.h"

static int xioctl(int fh, int request, void *arg)
{
        int r;
        do {
                r = ioctl(fh, request, arg);
        } while (-1 == r && EINTR == errno);
        return r;
}


int abrirDispositivo(DispositivoV4L* dispositivo)
{
	struct stat st;
	// Tentar identificar o dispositivo
	if(-1 == stat(dispositivo->nome, &st))
	{
		fprintf(stderr, "Não pode identificar '%s': %d %s\n", dispositivo->nome, errno, strerror(errno));
		return EXIT_FAILURE;
	}

	// Tentar verificar se é um dispositivo
	if(!S_ISCHR(st.st_mode))
	{
		fprintf(stderr, "%s não é dispositivo\n", dispositivo->nome);
		return EXIT_FAILURE;
	}

	// Tentar abrir o arquivo do dispositivo
	dispositivo->arquivo = open(dispositivo->nome, O_RDWR | O_NONBLOCK, 0);

	// Se não conseguiu, erro
	if(-1 == dispositivo->arquivo)
	{
		fprintf(stderr, "Não pode abrir '%s': %d, %s\n", dispositivo->nome, errno, strerror(errno));
		return EXIT_FAILURE;
	}
	return 0;
}
int iniciarDispositivo(DispositivoV4L* dispositivo)
{
	struct v4l2_capability cap;
	struct v4l2_cropcap cropcap;
	struct v4l2_crop crop;
	struct v4l2_format fmt;
	if(-1 == xioctl(dispositivo->arquivo, VIDIOC_QUERYCAP, &cap))
	{
		if(EINVAL == errno)
		{
			fprintf(stderr, "%s não é um dispositivo V4L2\n", dispositivo->nome);
			return EXIT_FAILURE;
		}
	}

	if(!(cap.capabilities & V4L2_CAP_VIDEO_CAPTURE))
	{
		fprintf(stderr, "%s não é um dispositivo de captura de vídeo\n", dispositivo->nome);
		return EXIT_FAILURE;
	}

	if(!(cap.capabilities & V4L2_CAP_READWRITE))
	{
		fprintf(stderr, "%s não suporta leitura e escrita\n", dispositivo->nome);
		return EXIT_FAILURE;
	}

	CLEAR(cropcap);
	cropcap.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
	if(0 == xioctl(dispositivo->arquivo, VIDIOC_CROPCAP, &cropcap))
	{
		crop.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
		crop.c = cropcap.defrect;

		if(-1 == xioctl(dispositivo->arquivo, VIDIOC_S_CROP, &crop))
		{
			switch(errno)
			{
				case EINVAL: /* cropping não suportado*/ break;
				default:     /* erros ignorados */ break;
			}
		}
	}else
	{
		/* erros ignorados */
	}

	CLEAR(fmt);

	return 0;
}
void iniciarCaptura(DispositivoV4L* dispositivo)
{
	
}
void terminarDispositivo(DispositivoV4L* dispositivo)
{
	
}
void desligarDispositivo(DispositivoV4L* dispositivo)
{

}

void capturarQuadro(DispositivoV4L* dispositivo)
{

}

// Ir para um tempo
void irPara(DispositivoV4L* dispositivo, /* tempo */)
{

}