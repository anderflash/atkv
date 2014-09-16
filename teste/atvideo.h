#ifndef ATCAMERA_H
#define ATCAMERA_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct Camera
{
	char *caminho;
	int largura;
	int altura;

}Camera;

void abrirDispositivo(nome, Camera* camera);
void iniciarDispositivo(Camera* camera);
void iniciarCaptura(Camera* camera);
void terminarDispositivo(Camera* camera);
void desligarDispositivo(Camera* camera);

#endif