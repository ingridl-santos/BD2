#include <fstream>
#include <iostream>

#include <iomanip>
#include <sstream>
#include <string>

#include <vector>

#include "csvTest.cpp"
//#include "btreeplus.cpp"

int mallocando(){
    bucketArea* area = new bucketArea;

    int count = 0;
    for(int a = 0; a < BUCKETSAMOUNT; a++){
        for(int b = 0; b < BUCKETSIZE - 1; b++){
            area->buckets[a].blocks[b].head = count;
            count += 1;
        }
        area->buckets[a].intBlocks.head = count;
        count += 1;
    }

    int ofssetArea = (BUCKETSAMOUNT/8)*(BUCKETSIZE);
    offsetBlock* offset = new offsetBlock[ofssetArea];
    for(int a=0; a < ofssetArea; a++){
        offset[a].head = count;
        count += 1;
    }
    
    std::cout << "Alocando espaço..." << std::endl;
    
    std::ofstream output_file("students.data", std::ios::binary);
    output_file.write((char*) area, sizeof(bucketArea));
    output_file.write((char*) offset, sizeof(offsetBlock)*ofssetArea);
    output_file.close();
}

int main(){
    mallocando();

    std::ifstream file("artigo.csv");
    btreeplus *tree = new btreeplus();

    std::cout << "preenchendo hashing e arvore b+" << std::endl;

    if (file.is_open()) {
        std::string line;
        int count = 0;
        while(getline(file, line)) {
            if(readLine(line.c_str(), tree)){
                count += 1;
            }
            if(count > 100000) break;
        }
        std::cout << count << " Registros Salvos\n";
        tree->printar();
        tree->salvar();
        file.close();
    }
}