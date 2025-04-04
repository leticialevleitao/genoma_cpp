#include <iostream>
#include <fstream>
#include <string>
using namespace std;

// estrutura node da tabela hash
struct Node {
    int key;         // key numerica que representa o bloco
    int count;       // qtd de ocorrencias
    string block;    // bloco das 6 bases
    Node* next;
};

int main(){
    //  tamanho da tabela hash (n° primo grande para evitar probelmas)
    const int TABLE_SIZE = 5003;
    Node* table[TABLE_SIZE];
    for (int i = 0; i < TABLE_SIZE; i++) {
        table[i] = nullptr;
    }

    ifstream infile("sequences.fasta.txt");
    string line;

    while(getline(infile, line)){
        // ignora cabeçalhos e linhas com tamanho diferente de 60
        if(line.empty() || line[0] == '>' || line.size() != 60) continue;
        // divide a linha em blocos de 6 caracteres
        for (int i = 0; i < 60; i += 6) {
            string block = line.substr(i, 6);
            // converte o bloco em uma key usando base 4
            int key = 0;
            for (int j = 0; j < 6; j++){
                key = key * 4;
                char c = block[j];
                if(c == 'A') key += 0;
                else if(c == 'C') key += 1;
                else if(c == 'G') key += 2;
                else if(c == 'T') key += 3;
            }
            // def index na tabela hash
            int index = key % TABLE_SIZE;
            Node* cur = table[index];
            bool found = false;
            // verifica se o bloco ja tem na hash
            while(cur){
                if(cur->key == key){
                    cur->count++;
                    found = true;
                    break;
                }
                cur = cur->next;
            }
            // se o bloco n foi encontrado, insere um novo node na lista
            if(!found){
                Node* newNode = new Node;
                newNode->key = key;
                newNode->count = 1;
                newNode->block = block;
                newNode->next = table[index];
                table[index] = newNode;
            }
        }
    }
    infile.close();

    // aescreve no arquivo output as ocorrencias
    ofstream outfile("output.txt");
    for (int i = 0; i < TABLE_SIZE; i++){
        Node* cur = table[i];
        while(cur){
            outfile << cur->block << " " << cur->count << "\n";
            cur = cur->next;
        }
    }
    outfile.close();

    // libera a memoria alocada da tabela hash
    for (int i = 0; i < TABLE_SIZE; i++){
        Node* cur = table[i];
        while(cur){
            Node* tmp = cur;
            cur = cur->next;
            delete tmp;
        }
    }
    return 0;
}
