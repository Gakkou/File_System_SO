#include <iostream>
#include <string>

int flagSA, *bitmap;

// Caso FAT:
class DiretorioF {

};

void inicializaSistema(std::string sa, int nb) {
  // Definir o tipo de sistema de arquivos.
  if(sa == "FAT") {
    flagSA=0;
  }
  if(sa == "i-node") {
    flagSA=1;
  }

  //malloc bitmap


}

int main() {

  std::string sa, entrada; // sa = Sistema de Arquivos
  int tp, tb, nb; // tp = tamanho da partição; tb = tamanho do bloco; nb = número de blocos;
  // Ler arquivo de config.
  std::getline(sa,std::cin);
  std::cin >> tp;
  std::cin >> tb;

  nb = tp/tb; // Cálculo do número de blocos

  inicializaSistema(sa,nb);

  std::getline(entrada, std::cin);
  while(entrada != "quit") {

  }

  return 0;
}
