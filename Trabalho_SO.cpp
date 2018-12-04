#include <iostream>
#include <string>
#include <ctime>
#include <list>
#include <stdlib.h>
#include <ctype.h>

// Caso FAT:
class Metadado {
private:
  char tipo;
  std::string nome;
  int tamanho;
  std::time_t *data;
  int pbloco;
  std::string conteudo;
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

  void setConteudo(std::string txt) {
    conteudo = txt;
  }

  std::string getConteudo() {
    return conteudo;
  }

  int getPrimeiroBloco() {
    return pbloco;
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
  std::getline(std::cin,sa);
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
    int tabelaFAT[nb][2]; // [][0] = busy bit; [][1] = próximo bloco

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

  // Váriaveis usadas durante a execução
  std::string nome, tam = "0";
  int t, i, numbc, primab, numaux;
  std::list<Metadado>::iterator it;

  std::getline(std::cin, entrada);
  while(entrada != "sair") {
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
          for(it = pilha.back().DFT.begin(); it != pilha.back().DFT.end(); it++) {
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

        if(entrada.compare(0,2,'..') == 0) {
          if(pilha.back().getNome() == "/") {
            std::cout << "Usuário já se encontra no diretório raiz.\n";
          } else {
            *aux = pilha.back(); // Salva o estado do último diretório
            pilha.pop_back(); // Sobe um diretório na hierárquia
            // Atualiza o estado do antigo diretório aberto no novo
            for(it = pilha.back().DFT.begin(); it != pilha.back().DFT.end(); it++) {
              if(it->getNome() == aux->getNome()) {
                pilha.back().DFT.erase(it);
                pilha.back().DFT.push_back(*aux);
                break;
              }
            }
          }
        } else {
          // Verifica se existe diretório com esse nome
          for(it = pilha.back().DFT.begin(); it != pilha.back().DFT.end(); it++) {
            if(entrada.compare(0,it->getNome().size(),it->getNome()) == 0) {
              pilha.push_back(*it);
              break;
            }
          }
          if(it == pilha.back().DFT.end()) {
            std::cout << "ERRO! Não encontrado diretório com nome " << entrada << ".\n";
          }
        }
      }

      if(entrada.compare(0,2,"ls") == 0) {
        pilha.back().listar(tb);
      }

      if(entrada.compare(0,5,"touch") == 0) {
        entrada.erase(0,6); // Consome comando

        // Pega o nome
        if(entrada.find(".") > 7) {
          nome = entrada.substr(0,8);
        } else {
          nome = entrada.substr(0,entrada.find("."));
        }
        nome += entrada.substr(entrada.find("."), 3);
        entrada.erase(0,nome.size()+1);

        // Pega o tamanho
        for(std::string::itereator it = entrada.begin(); it != entrada.end(); it++) {
          if(isdigit(*it)) {
            tam += *it;
          }
          if(isalpha(*it)) {
            break;
          }
        }
        t = atoi(tam.c_str());
        entrada.erase(0,tam.size()+1);
        tam = "0";

        // Verifica memória disponível
        numbc = t/tb; // numbc = número de blocos ocupados
        for(i = 0; numbc>0; i++) {
          if(bitmap[i] == 0) {
            numbc--;
          }
        }
        if(numbc == 0) {
          // Se tem espaço disponível, aloca
          numbc = t/tb;
          primab = -1; // primeiro bloco do arquivo
          for(i = 0; numbc>0; i++) {
            if(bitmap[i] == 0) {
              bitmap[i] = 1;
              tabelaFAT[i][0] = 1;
              if(primab == -1) {
                primab = i;
                numaux = i;
              } else {
                tabelaFAT[numaux][1] = i;
                numaux = i;
              }
              if(numbc==1) {
                tabelaFAT[i][1] = -1;
              }
              numbc--;
            }
          }

          // Cria os metadados no diretório
          aux = new Metadado('A', nome, t, primab);
          aux->setConteudo(entrada);
          // Verifica se já existe conteúdo no diretório e se os nomes são diferentes
          if(pilha.back().DFT.empty()) {
            pilha.back().DFT.push_back(*aux); // Adiciona ao diretório atual
            std::cout << "Arquivo criado com sucesso.\n"
          } else {
            for(it = pilha.back().DFT.begin(); it != pilha.back().DFT.end(); it++) {
              if(it->getNome() == aux->getNome()) {
                break;
              }
            }
            if(it == pilha.back().DFT.end()) {
              pilha.back().DFT.push_back(*aux); // Adiciona ao diretório atual
              std::cout << "Arquivo criado com sucesso.\n"
            }
            else {
              std::cout << "ERRO! Já exite Diretório/Arquivo com esse nome.\n";
            }
          }
        } else {
          std::cout << "Sem espaço para armazenamento do arquivo \"" << nome << "\".\n";
        }
      }

      if(entrada.compare(0,2,"rm") == 0) {
        entrada.erase(0,3); // Consome o comando

        // Verifica se existe arquivo com esse nome
        for(it = pilha.back().DFT.begin(); it != pilha.back().DFT.end(); it++) {
          if(entrada.compare(0,it->getNome().size(),it->getNome()) == 0) {
            break;
          }
        }
        if(it == pilha.back().DFT.end()) { // Se não, ERRO
          std::cout << "ERRO! Não encontrado arquivo com nome \"" << entrada << "\".\n";
        } else { // Se sim, o apaga
          numaux = it->getPrimeiroBloco();
          while(numaux != -1) {
            bitmap[numaux] = 0;
            tabelaFAT[numaux][0] = 0;
            numaux = tabelaFAT[numaux][1];
          }
          pilha.back().DFT.erase(it);
          std::cout << "Arquivo excluído com sucesso.\n";
        }
      }

      if(entrada.compare(0,3,"cat") == 0) {
        entrada.erase(0,4); // Consome o comando

        // Verifica se existe arquivo com esse nome
        for(it = pilha.back().DFT.begin(); it != pilha.back().DFT.end(); it++) {
          if(entrada.compare(0,it->getNome().size(),it->getNome()) == 0) {
            break;
          }
        }
        if(it == pilha.back().DFT.end()) { // Se não, ERRO
          std::cout << "ERRO! Não encontrado arquivo com nome \"" << entrada << "\".\n";
        } else { // Se sim, mostra o conteúdo
          std::cout << it->getConteudo() << "\n";
        }
      }
    }

    if(entrada.compare(0,7,"listmap") == 0) {
      for(i=0;i<nb;i++) {
        std::cout << "bloco " << i << " = " << bitmap[i] << "\n";
      }
    }

    std::getline(std::cin, entrada);
  }

  return 0;
}
