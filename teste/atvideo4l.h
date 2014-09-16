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

typedef struct DispositivoV4L
{
	char *nome;
	int largura;
	int altura;
	int arquivo;

}DispositivoV4L;

int abrirDispositivo(DispositivoV4L* dispositivo);
int iniciarDispositivo(DispositivoV4L* dispositivo);
void iniciarCaptura(DispositivoV4L* dispositivo);
void capturarQuadro(DispositivoV4L* dispositivo);
void terminarDispositivo(DispositivoV4L* dispositivo);
void desligarDispositivo(DispositivoV4L* dispositivo);

#endif