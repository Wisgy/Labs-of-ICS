/*
 * @Author       : Chivier Humber
 * @Date         : 2021-09-15 21:15:24
 * @LastEditors  : Chivier Humber
 * @LastEditTime : 2021-11-23 16:08:51
 * @Description  : file content
 */
#include "common.h"
#include "memory.h"

namespace virtual_machine_nsp {
    void memory_tp::ReadMemoryFromFile(std::string filename, int beginning_address) {
        // Read from the file
        // TO BE DONE
        std::ifstream input_file(filename);
        std::string line;
        int16_t mem;
        if(input_file.is_open()){
            while(std::getline(input_file,line)){
                for(int i=0;i<16;i++){
                    if(line[i]=='1')((mem) |= (1 << ((i) ^ 15)));
                    else ((mem) &= ~(1 << ((i) ^ 15)));
                }
                memory_tp::memory[beginning_address++]=mem;
            }
        }
        input_file.close();
    }

    int16_t memory_tp::GetContent(int address) const {
        // get the content
        // TO BE DONE
        return memory_tp::memory[address];
    }

    int16_t& memory_tp::operator[](int address) {
        // get the content
        // TO BE DONE
        int16_t& name=memory_tp::memory[address];
        return name;
    }    
}; // virtual machine namespace
