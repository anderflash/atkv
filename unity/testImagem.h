#include <

/* TESTES DE ABERTURA DE JPEG
   Caso 1: Nome do arquivo inválido (vazio ou arquivo inexistente)
     - Deve retornar o erro ATJPEGNOMEINVALIDO
   Caso 2: Nome do arquivo válido mas arquivo não é JPEG
     - Deve retornar o erro ATJPEGARQUIVOINVALIDO
   Caso 3: Nome do arquivo válido e arquivo correto
     - Deve retornar sucesso ATJPEGARQUIVOABERTO
     - Deve retornar o tamanho correto (largura e altura)
     - Deve retornar o espaço de cores desejado
     - Deve retornar o número de canais correto
 */

void testAbrirJPEGVazio()
{

}

void testAbrirJPEGInválido()
{

}

void testAbrirJPEGCorreto()
{

}

/* TESTES DE FECHAMENTO DE JPEG
   Caso 1: Arquivo não foi aberto
     - Deve retornar o erro ATJPEGARQUIVONAOABERTO
   Caso 2: Arquivo foi aberto anteriormente e fechado corretamente
     - Deve retornar o sucesso ATJPEGARQUIVOFECHADO
 */

void testFecharJPEGNaoAberto()
{

}

void testFecharJPEGAberto()
{

}

/* TESTES DE ESCRITA DE JPEG
   Caso 1: Escrever um buffer inválido ou inexistente
     - Deve retornar o erro ATJPEGBUFFERINVALIDO
   Caso 2: Nome do arquivo inválido (ex: vazio)
     - Deve retornar o erro ATJPEGNOMEINVALIDO
   Caso 3: Tamanho Variado
     - Deve retornar o sucesso ATJPEGESCRITO para cada buffer
   Caso 4: Espaço de Cores diferentes (RGB e YCbCr)
     - Deve retornar o sucesso ATJPEGESCRITO para cada espaço de cores
*/

/* TESTES DE OBTENCAO DE RETANGULO
   Caso 1: Escrever um buffer inválido ou inexistente
     - Deve retornar o erro ATJPEGBUFFERINVALIDO
   Caso 2: Nome do arquivo inválido (ex: vazio)
     - Deve retornar o erro ATJPEGNOMEINVALIDO
   Caso 3: Tamanho Variado
     - Deve retornar o sucesso ATJPEGESCRITO para cada buffer
   Caso 4: Espaço de Cores diferentes (RGB e YCbCr)
     - Deve retornar o sucesso ATJPEGESCRITO para cada espaço de cores
*/

/* TESTES DE CONVERSAO DE JPEG DE ARRAY OF LIST PARA LIST OF ARRAYS E VICE-VERSA
   Caso 1: Escrever um buffer inválido ou inexistente
     - Deve retornar o erro ATJPEGBUFFERINVALIDO
   Caso 2: Nome do arquivo inválido (ex: vazio)
     - Deve retornar o erro ATJPEGNOMEINVALIDO
   Caso 3: Tamanho Variado
     - Deve retornar o sucesso ATJPEGESCRITO para cada buffer
   Caso 4: Espaço de Cores diferentes (RGB e YCbCr)
     - Deve retornar o sucesso ATJPEGESCRITO para cada espaço de cores
*/

/* TESTES DE OPERAÇÕES EM CPU COM IMAGENS JPEG
   Caso 1: Somar Imagens
     - Deve retornar o erro ATJPEGBUFFERINVALIDO
   Caso 2: Subtrair Imagens
     - Deve retornar o erro ATJPEGNOMEINVALIDO
   Caso 3: Multiplicar Imagens
     - Deve retornar o sucesso ATJPEGESCRITO para cada buffer
   Caso 4: Dividir Imagens
     - Deve retornar o sucesso ATJPEGESCRITO para cada espaço de cores
*/

/* TESTES DE FILTROS EM CPU COM IMAGENS JPEG
   Caso 1: Somar Imagens
     - Deve retornar o erro ATJPEGBUFFERINVALIDO
   Caso 2: Subtrair Imagens
     - Deve retornar o erro ATJPEGNOMEINVALIDO
   Caso 3: Multiplicar Imagens
     - Deve retornar o sucesso ATJPEGESCRITO para cada buffer
   Caso 4: Dividir Imagens
     - Deve retornar o sucesso ATJPEGESCRITO para cada espaço de cores
*/

/* TESTES DE CONVOLUÇÕES EM CPU COM IMAGENS JPEG
   Caso 1: Somar Imagens
     - Deve retornar o erro ATJPEGBUFFERINVALIDO
   Caso 2: Subtrair Imagens
     - Deve retornar o erro ATJPEGNOMEINVALIDO
   Caso 3: Multiplicar Imagens
     - Deve retornar o sucesso ATJPEGESCRITO para cada buffer
   Caso 4: Dividir Imagens
     - Deve retornar o sucesso ATJPEGESCRITO para cada espaço de cores
*/

/* TESTES DE OPERAÇÕES EM CPU COM IMAGENS JPEG
   Caso 1: Somar Imagens
     - Deve retornar o erro ATJPEGBUFFERINVALIDO
   Caso 2: Subtrair Imagens
     - Deve retornar o erro ATJPEGNOMEINVALIDO
   Caso 3: Multiplicar Imagens
     - Deve retornar o sucesso ATJPEGESCRITO para cada buffer
   Caso 4: Dividir Imagens
     - Deve retornar o sucesso ATJPEGESCRITO para cada espaço de cores
*/




/* TESTES DE ABERTURA DE PNG
   Caso 1: Nome do arquivo inválido (vazio ou arquivo inexistente)
     - Deve retornar o erro ATJPEGNOMEINVALIDO
   Caso 2: Nome do arquivo válido mas arquivo não é JPEG
     - Deve retornar o erro ATJPEGARQUIVOINVALIDO
   Caso 3: Nome do arquivo válido e arquivo correto
     - Deve retornar sucesso ATJPEGARQUIVOABERTO
     - Deve retornar o tamanho correto (largura e altura)
     - Deve retornar o espaço de cores desejado
     - Deve retornar o número de canais correto
 */

void testAbrirPNGVazio()
{

}

void testAbrirPNGInválido()
{

}

void testAbrirPNGCorreto()
{

}

