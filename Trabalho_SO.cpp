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

  std::string getNome() {
    return nome;
  }

  void lista(int tb) {
    if(DFT.empty()) {
      std::cout << "Diretório se encontra vazio.\n";
    } else {
      for(auto it = DFT.begin(); it != DFT.end(); it++) {
        if(it->tipo == 'D') {
          std::cout << "D " << it->nome << "\n";
        } else {
          std::cout << "A " << it->nome << it->tamanho << ((it->tamanho)/tb) << "\n";
        }
      }
    }
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
    Metadado *aux = new Metadado('D', "/");
    std::list<Metadado> pilha;
    pilha.push_back(*aux);
  }

  std::getline(entrada, std::cin);
  while(entrada != "quit") {
    if(flagSA == 1) {
      if(entrada.compare(0,5,"mkdir") == 0) {
        entrada.erase(0,6); // Consome o comando

        if(entrada.size() <= 8) {
          // Nome não sofre corte
          aux = new Metadado('D', entrada);
        } else {
          // Nome muito grande, corta
          aux = new Metadado('D', entrada.substr(0,8));
        }

        // Verifica se já existe conteúdo no diretório e se os nomes são diferentes
        if(pilha.back().DFT.empty()) {
          pilha.back().DFT.push_back(*aux); // Adiciona ao diretório atual
          std::cout << "Diretório criado com sucesso.\n"
        } else {
          for(auto it = pilha.back().DFT.begin(); it != pilha.back().DFT.end(); it++) {
            if(it->getNome() == aux->getNome()) {
              break;
            }
          }
          if(it == pilha.back().DFT.end()) {
            pilha.back().DFT.push_back(*aux); // Adiciona ao diretório atual
            std::cout << "Diretório criado com sucesso.\n"
          }
          else {
            std::cout << "ERRO! Já exite Diretório/Arquivo com esse nome.\n";
          }
        }
      }

      if(entrada.compare(0,2,"cd" == 0)) {
        entrada.erase(0,3); // Consome comando

        if(entrada.compare(0,1,'.') == 0) {
          if(pilha.back().getNome() == "/") {
            std::cout << "Usuário já se encontra no diretório raiz.\n";
          } else {
            *aux = pilha.back(); // Salva o estado do último diretório
            pilha.pop_back(); // Sobe um diretório na hierárquia
            // Atualiza o estado do antigo diretório aberto no novo
            for(auto it = pilha.back().DFT.begin(); it != pilha.back().DFT.end(); it++) {
              if(it->getNome() == aux->getNome()) {
                pilha.back().DFT.erase(it);
                pilha.back().DFT.push_back(*aux);
                break;
              }
            }
          }
        } else {
          // Verifica se existe diretório com esse nome
          for(auto it = pilha.back().DFT.begin(); it != pilha.back().DFT.end(); it++) {
            if(entrada.compare(0,it->getNome().size(),it->getNome()) == 0) {
              pilha.push_back(*it);
              break;
            }
          }
          if(it == pilha.back().DFT.end()) {
            std::cout << "ERRO! Não encontrado diretório com nome " << entrada << "\n";
          }
        }
      }

      if(entrada.compare(0,2,"ls") == 0) {
        pilha.back().listar(tb);
      }
    }

    std::getline(entrada, std::cin);
  }

  return 0;
}
