#include "csvtest.h"

#include <fstream>
#include <iostream>

#include <iomanip>
#include <sstream>
#include <string>

#include <vector>

#include "struct.h"
#include "btreeplus.cpp"

int enterBucket(int id, registry reg){
    std::fstream ifd("students.data", std::fstream::in |std::fstream::out | std::fstream::binary);
    //std::ofstream output_file("students.data", std::ios::binary | std::ios_base::beg);

    int bucketN = id % BUCKETSAMOUNT;
    int ofssetArea = BUCKETSAMOUNT*(BUCKETSIZE/4);
    block  bloquinho;
    bucket buck;
    intBlock intB;
    offsetBlock oblock;
    auto offsetAddress = sizeof(buck)*(BUCKETSAMOUNT);

        
    int size = ifd.tellg();
    ifd.seekg(0, std::ios::beg);
    ifd.seekg(sizeof(buck)*bucketN, std::ios::beg);

    //std::cout << bucketN << " bucket " << sizeof(buck) << std::endl;

    int count = 0;
    for(int a = 0; a < BUCKETSIZE; a++){
        if(a != BUCKETSIZE-1){
            ifd.read((char*)& bloquinho, sizeof(bloquinho));
            //std::cout << bloquinho.head << " " << count++ << " " << sizeof(bloquinho) << std::endl;

            //std::cout << bloquinho.head << "\n" << bloquinho.address1 << "\n" << bloquinho.address2 << "\n\n";
            
            
            if(bloquinho.address1 == 0 || bloquinho.address2 == 0){
                block newBlock = insertBlock(bloquinho, reg);

                ifd.seekp((sizeof(buck)*bucketN)+(sizeof(bloquinho)*(a)));
                ifd.seekg((sizeof(buck)*bucketN)+(sizeof(bloquinho)*(a)));
                ifd.write((char*)&newBlock, sizeof(block));
                ifd.close();

                if(bloquinho.address1 == 0 && newBlock.address1 == 1){
                    return (sizeof(buck)*bucketN)+(sizeof(bloquinho)*(a))+984;
                } else if(bloquinho.address2 == 0 && newBlock.address2 == 1){
                    return (sizeof(buck)*bucketN)+(sizeof(bloquinho)*(a))+2490;
                }
                break;
            }

            ifd.seekg(sizeof(buck)*bucketN+(sizeof(bloquinho)*(a+1)), std::ios::beg);
        } else{
            // se não tiver recordpointer
            // se tiver recordpointer
            ifd.read((char*)& intB, sizeof(intB));
            //std::cout << intB.head << " " << count++ << " " << sizeof(intB) << std::endl;

            //std::cout << intB.head << "\n" << intB.address1 << "\n" << intB.address2 << "\n" << intB.recordPointer << "\n\n"; 
            ifd.seekg(sizeof(buck)*bucketN+(sizeof(intB)*(a+1)), std::ios::beg);
        }
    }

    /**
     * ifd.seekg(offsetAddress, std::ios::beg);

    overRegistry ov;

    for(int b = 0; b < ofssetArea; b++){
        ifd.read((char*)& oblock, sizeof(oblock));
        std::cout << oblock.head << " " << count++ << " " << sizeof(intB) << std::endl;

        std::cout << oblock.head << "\n" << oblock.address1 << "\n" << oblock.address2 << "\n\n"; 
        if(oblock.address1 == 0 | oblock.address2 == 0){
            break;
        }
            
        ifd.seekg(offsetAddress+(sizeof(oblock)*(b+1)), std::ios::beg);        
    }
    **/
}


int readLine(std::string texto, btreeplus *tree){
    std::stringstream ss;
    ss << texto;
    std::vector<std::string> v;

    int fields = 0;

    while (ss >> std::ws) {
        std::string csvElement;

        if (ss.peek() == '"') {
            ss >> std::quoted(csvElement);
            std::string discard;
            std::getline(ss, discard, ';');
        }
        else {
            std::getline(ss, csvElement, ';');
        }

        fields += 1;

        v.push_back(csvElement);

    }

    

    if(fields != 8){
        //std::cout << fields << std::endl << std::endl;

        for(std::string value: v) {
            //std::cout << value << std::endl;
        }

        //std::cout << std::endl;
        return 0;
    } else{
        char titulo[300];
        strncpy(titulo, v[1].c_str(), 300);
        char autores[150];
        strncpy(autores, v[3].c_str(), 150);
        char atualizacao[16];
        strncpy(atualizacao, v[5].c_str(), 16);
        char snippet[1024];
        strncpy(snippet, v[6].c_str(), 1024);
        registry reg = writeRegistry(stoi(v[0]), titulo, stoi(v[2]), autores, stoi(v[4]), atualizacao, snippet);
        
        int address = enterBucket(stoi(v[0]), reg);
        //std::cout << address << std::endl;
        tree->inserir(stoi(v[0]), address);

        //std::cout << "-|" << v[0] << std::endl;
        v.clear();
        //for(std::string value: v) {
        //}
    }
    return 1;
}
