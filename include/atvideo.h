#ifndef ATVIDEO_H
#define ATVIDEO_H

typedef struct
{
  int largura;
  int altura;
  int totalframes;
  int frameAtual;
  ATVideoFormato formato;
  ATVideoTipo tipo;
  int canais;
}ATVideo;

void iniciarContextoVideo();
void abrirVideo(ATVideo);

#endif // ATVIDEO_H
