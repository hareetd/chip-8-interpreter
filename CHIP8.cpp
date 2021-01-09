#include "CHIP8.h"
#include <fstream>
#include <string.h>

CHIP8::CHIP8() {
    PC = START_ADDRESS;
    SP = 0;
}

void CHIP8::load(const char* ROM) {
    std::ifstream file(ROM, std::ifstream::binary);

    if (file.is_open()) {
        // get length of file
        file.seekg(0, file.end);
        int length = file.tellg();
        file.seekg(0, file.beg);

        // read file data into buffer
        char* buffer = new char[length];
        file.read(buffer, length);
        file.close();   

        // load the ROM data into memory starting at address 0x200
        for (int i = 0; i < length; i++) {
            writeByte(START_ADDRESS + i, buffer[i]);
        }

        delete[] buffer;
    }
}

uint8_t CHIP8::readByte(uint16_t address) {
    // rmbr to handle out of bounds address
    if (address > 0x1FF && address < 0x1000) {
        return memory[address];
    } else {
        return -1;
    }
} 

void CHIP8::writeByte(uint16_t address, uint8_t val) { 
    if (address > 0x1FF && address < 0x1000) {
        memory[address] = val;
    }
}

uint16_t CHIP8::readWord(uint16_t address) {
    if (address > 0x1FF && address < 0x1000) {
        uint16_t val = readByte(address);
        val = val << 8 | readByte(address + 1);
        return val;
    } else {
        return -1;
    }
}

void CHIP8::writeWord(uint16_t address, uint16_t val) {
    if (address > 0x1FF && address < 0x1000) {
        writeByte(address, val >> 8);
        writeByte(address + 1, val & 0xFF);
    } 
}

void CHIP8::fetch() {
    if (PC < 0x1000) {
        opcode = readWord(PC);
        PC += 2;
    } 
}

void CHIP8::decodeAndExecute() {
    uint8_t group = (opcode & 0xF000u) >> 12;
    uint8_t regX = (opcode & 0x0F00u) >> 8;
    uint8_t regY = (opcode & 0x00F0u) >> 4;
    uint8_t fourBitConst = opcode & 0xFu; 
    uint8_t eightBitConst = opcode & 0xFFu;
    uint16_t address = opcode & 0xFFFu;
    
    switch(group) {
        case 0x0:
            if (fourBitConst == 0) {
                OP_00E0();
            } else {
                OP_00EE();
            }
            break;
        case 0x1:
            OP_1NNN(address);
            break;
        case 0x2:
            OP_2NNN(address);
            break;
        case 0x3:
            OP_3XNN(regX, eightBitConst);
            break;
        case 0x4:
            OP_4XNN(regX, eightBitConst);
            break;
        case 0x5:
            OP_5XY0(regX, regY);
            break;
        case 0x6:
            OP_6XNN(regX, eightBitConst);
            break;
        case 0x7:
            OP_7XNN(regX, eightBitConst);
            break;
        case 0x8:
            switch(fourBitConst) {
                case 0x0:
                    OP_8XY0(regX, regY);
                    break;
                case 0x1:
                    OP_8XY1(regX, regY);
                    break;
                case 0x2:
                    OP_8XY2(regX, regY);
                    break;
                case 0x3:
                    OP_8XY3(regX, regY);
                    break;
                case 0x4:
                    OP_8XY4(regX, regY);
                    break;
                case 0x5:
                    OP_8XY5(regX, regY);
                    break;
                case 0x6:
                    OP_8XY6(regX, regY);
                    break;
                case 0x7:
                    OP_8XY7(regX, regY);
                    break;
                case 0xE:
                    OP_8XYE(regX, regY);
                    break;
            }
            break;
        case 0x9:
            OP_9XY0(regX, regY);
            break;
        case 0xA:
            OP_ANNN(address);
            break;
        case 0xB:
            OP_BNNN(address);
            break;
        case 0xC:
            OP_CXNN(regX, eightBitConst);
            break;
        case 0xD:
            OP_DXYN(regX, regY, fourBitConst);
            break;
        case 0xE:
            if (eightBitConst == 0x9E) {
                OP_EX9E(regX);
            } else if (eightBitConst == 0xA1) {
                OP_EXA1(regX);
            }
            break;
        case 0xF:
            switch(eightBitConst) {
                case 0x07:
                    OP_FX07(regX);
                    break;
                case 0x0A:
                    OP_FX0A(regX);
                    break;
                case 0x15:
                    OP_FX15(regX);
                    break;
                case 0x1E:
                    OP_FX1E(regX);
                    break;
                case 0x29:
                    OP_FX29(regX);
                    break;
                case 0x33:
                    OP_FX33(regX);
                    break;
                case 0x55:
                    OP_FX55(regX);
                    break;
                case 0x65:
                    OP_FX65(regX);
                    break;
            }
            break;
    }
}

void CHIP8::OP_00E0() {
    memset(display, 0, sizeof(display));
}

void CHIP8::OP_00EE() {
    PC = stack[--SP];
}

void CHIP8::OP_1NNN(uint16_t address) {
    if (address > 0x1FF && address < 0x1000) {
        PC = address;
    }
}

void CHIP8::OP_2NNN(uint16_t address) {
    stack[SP++] = PC;
    PC = address;
}

void CHIP8::OP_3XNN(uint8_t VX, uint8_t byte) {
    if (registers[VX] == byte) {
        PC += 2;
    }
}

void CHIP8::OP_4XNN(uint8_t VX, uint8_t byte) {
    if (registers[VX] != byte) {
        PC += 2;
    }
}

void CHIP8::OP_5XY0(uint8_t VX, uint8_t VY) {
    if (registers[VX] == registers[VY]) {
        PC += 2;
    }
}

void CHIP8::OP_6XNN(uint8_t VX, uint8_t byte) {
    registers[VX] = byte;
}

void CHIP8::OP_7XNN(uint8_t VX, uint8_t byte) {
    registers[VX] += byte;
}

void CHIP8::OP_8XY0(uint8_t VX, uint8_t VY) {
    registers[VX] = registers[VY];
}

void CHIP8::OP_8XY1(uint8_t VX, uint8_t VY) {
    registers[VX] |= registers[VY];
}

void CHIP8::OP_8XY2(uint8_t VX, uint8_t VY) {
    registers[VX] &= registers[VY];
}

void CHIP8::OP_8XY3(uint8_t VX, uint8_t VY) {
    registers[VX] ^= registers[VY];
}

void CHIP8::OP_8XY4(uint8_t VX, uint8_t VY) {
    uint16_t sum = registers[VX] + registers[VY];

    if (sum > 255U) {
        registers[0xF] = 1;
    } else {
        registers[0xF] = 0;
    }

    registers[VX] = sum & 0xFFu;
}

void CHIP8::OP_8XY5(uint8_t VX, uint8_t VY) {
    if (registers[VY] > registers[VX]) {
        registers[0xF] = 0;
    } else {
        registers[0xF] = 1;
    }

    registers[VX] -= registers[VY];
}

void CHIP8::OP_8XY6(uint8_t VX, uint8_t VY) {
    registers[0xF] = registers[VY] & 0x1u;
    registers[VX] = registers[VY] >> 1;
}

void CHIP8::OP_8XY7(uint8_t VX, uint8_t VY) {
    if (registers[VX] > registers[VY]) {
        registers[0xF] = 0;
    } else {
        registers[0xF] = 1;
    }
    
    registers[VX] = registers[VY] - registers[VX];
}

void CHIP8::OP_8XYE(uint8_t VX, uint8_t VY) {
    registers[0xF] = registers[VY] & 0x1000u;
    registers[VX] = registers[VY] << 1;
}

void CHIP8::OP_9XY0(uint8_t VX, uint8_t VY) {
    if (registers[VX] != registers[VY]) {
        PC += 2;
    }
}

void CHIP8::OP_ANNN(uint16_t address) {
    I = address;
}

void CHIP8::OP_BNNN(uint16_t address) {
    uint16_t jumpAddress = address + registers[0x0];
    if  (jumpAddress > 0x1FF && jumpAddress < 0x1000) {
        PC = jumpAddress;
    }
}

void CHIP8::OP_CXNN(uint8_t VX, uint8_t byte) {

}

void CHIP8::OP_DXYN(uint8_t VX, uint8_t VY, uint8_t height) {

}

void CHIP8::OP_EX9E(uint8_t VX) {

}

void CHIP8::OP_EXA1(uint8_t VX) {

}

void CHIP8::OP_FX07(uint8_t VX) {
    registers[VX] = delayTimer;
}

void CHIP8::OP_FX0A(uint8_t VX) {

}

void CHIP8::OP_FX15(uint8_t VX) {
    delayTimer = registers[VX];
}

void CHIP8::OP_FX18(uint8_t VX) {
    soundTimer = registers[VX];
}

void CHIP8::OP_FX1E(uint8_t VX) {
    I += registers[VX];
}

void CHIP8::OP_FX29(uint8_t VX) {

}

void CHIP8::OP_FX33(uint8_t VX) {
    
}

void CHIP8::OP_FX55(uint8_t VX) {
    for (int i = 0; i <= VX; i++) {
        writeByte(I++, registers[i]);
    }
}

void CHIP8::OP_FX65(uint8_t VX) {
    for (int i = 0; i <= VX; i++) {
        registers[i] = readByte(I++);
    }
}