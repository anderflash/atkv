#ifndef ATVIDEO4L_H
#define ATVIDEO4L_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <sys/stat.h>

#include <fcntl.h>
#include <sys/ioctl.h>

#include <linux/videodev2.h>

#define CLEAR(x) memset(&(x), 0, sizeof(x))

typedef struct ATDispositivoV4L
{
	char *nome;
	int largura;
	int altura;
	int arquivo;

}ATDispositivoV4L;

int abrirDispositivo(ATDispositivoV4L* dispositivo);
int iniciarDispositivo(ATDispositivoV4L* dispositivo);
void iniciarCaptura(ATDispositivoV4L* dispositivo);
void capturarQuadro(ATDispositivoV4L* dispositivo);
void terminarDispositivo(ATDispositivoV4L* dispositivo);
void desligarDispositivo(ATDispositivoV4L* dispositivo);

#endif
