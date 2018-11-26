#include <iostream>
#include <string>
#include <ctime>
#include <list>

// Caso FAT:
class Metadado {
private:
  char tipo;
  std::string nome;
  int tamanho;
  std::time_t *data;
  int pbloco;
public:

  std::list<Metadado> DFT;

  Metadado(char t, std::string n, int ta = -1, int pb = -1) {
    tipo = t;
    nome = n;
    tamanho = ta;
    *data = std::time(nullptr);
    pbloco = pb;
  }
};

int main() {

  std::string sa, entrada; // sa = Sistema de Arquivos
  int tp, tb, nb; // tp = tamanho da partição; tb = tamanho do bloco; nb = número de blocos;
  int flagSA, *bitmap; // Flag sinalizante do tipo de sistema e mapa de bits

  // Ler arquivo de config.
  std::getline(sa,std::cin);
  std::cin >> tp;
  std::cin >> tb;

  nb = tp/tb; // Cálculo do número de blocos

  // Definir o tipo de sistema de arquivos.
  if(sa == "FAT") {
    flagSA=1;
  }
  if(sa == "i-node") {
    flagSA=0;
  }

  // Aloca bitmap
  bitmap = new int[nb];

  // Seta para zero
  for(int i = 0; i < nb; i++) {
    bitmap[i] = 0;
  }

  if(flagSA == 1) {
    int tabelaFAT[nb][2]; // [][0] = busy bit; [][1] = bloco

    // Seta para zero
    for(int i = 0; i < nb; i++) {
      tabelaFAT[i][0] = 0;
      tabelaFAT[i][1] = 0;
    }

    // Cria diretório '/'
  }

  std::getline(entrada, std::cin);
  while(entrada != "quit") {

  }

  return 0;
}
