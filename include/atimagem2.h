#ifndef ATIMAGEM_H
#define ATIMAGEM_H
typedef struct
{
  char* nome;
  int largura;
  int altura;
}ATImagem;

typedef struct 
{
  ATImagem super;
}ATImagemJPEG;

void lerImagem(ATImagem* imagem);

#endif // ATIMAGEM_H
