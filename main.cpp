#include <fstream>

class CHIP8 
{
public:
    uint8_t memory[4096];
    uint8_t display[64*32];
    uint16_t PC;
    uint16_t I;
    uint16_t stack[16];
    uint8_t SP;
    uint8_t delayTimer;
    uint8_t soundTimer;
    uint8_t registers[16];
    uint8_t keypad[16];
    uint16_t opcode;
    uint16_t START_ADDRESS = 0x200;
private:
    void load(const char* ROM);
    uint8_t readByte(uint16_t addr);
    void writeByte(uint16_t addr, uint8_t val);
    uint16_t readWord(uint16_t addr);
    void writeWord(uint16_t addr, uint16_t val));
    void fetch();
    void decode();
    void generateInterrupts();
    void emulateGraphics();
    void emulateSound();
};

CHIP8::CHIP8() {
    PC = START_ADDRESS;
}

CHIP8::load(const char* ROM) {
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

CHIP8::readByte(uint16_t addr) {
    // rmbr to handle out of bounds addr
    if (addr > 0x1FF && addr < 0x1000) {
        return memory[addr];
    } else {
        return -1;
    }
} 

CHIP8::writeByte(uint16_t addr, uint8_t val) { 
    if (addr > 0x1FF && addr < 0x1000) {
        memory[addr] = val;
    }
}

CHIP8::readWord(uint16_t addr) {
    if (addr > 0x1FF && addr < 0x1000) {
        uint16_t val = readByte(addr);
        val = val << 8 | readByte(addr + 1);
        return val;
    } else {
        return -1;
    }
}

CHIP8::writeWord(uint16_t addr, uint16_t val) {
    if (addr > 0x1FF && addr < 0x1000) {
        writeByte(addr, val >> 8);
        writeByte(addr + 1, val & 0xFF);
    } else {
        return -1;
    }
}

CHIP8::fetch() {
    if (PC < 0x1000) {
        opcode = readWord(PC);
        PC += 2;
    } 
}

CHIP8::decode() {
    uint8_t group = (opcode & 0x1000) >> 3;
    uint8_t registerX = (opcode & 0x100) >> 2;
    uint8_t registerY = (opcode & 0x10) >> 1;
    uint8_t fourBitConstant = opcode & 0x1; 
    uint8_t eightBitConstant = opcode & 0x11;
    uint16_t addr = opcode & 0x111;
    
    switch(group) {
        case 0x0:
            if (registerX == 0) {
                if (fourBitConstant == 0) {
                    OP_00E0();
                } else {
                    OP_00EE();
                }
            } else {
                OP_0NNN(addr);
            }
            break;
        case 0x1:
            OP_1NNN(addr);
            break;
        case 0x2:
            OP_2NNN(addr);
            break;
        case 0x3:
            OP_3XNN(registerX, eightBitConstant);
            break;
        case 0x4:
            OP_4XNN(registerX, eightBitConstant);
            break;
        case 0x5:
            OP_5XY0(registerX, registerY);
            break;
        case 0x6:
            OP_6XNN(registerX, eightBitConstant);
            break;
        case 0x7:
            OP_7XNN(registerX, eightBitConstant);
            break;
        case 0x8:
            switch(fourBitConstant) {
                case 0x0:
                    OP_8XY0(registerX, registerY);
                    break;
                case 0x1:
                    OP_8XY1(registerX, registerY);
                    break;
                case 0x2:
                    OP_8XY2(registerX, registerY);
                    break;
                case 0x3:
                    OP_8XY3(registerX, registerY);
                    break;
                case 0x4:
                    OP_8XY4(registerX, registerY);
                    break;
                case 0x5:
                    OP_8XY5(registerX, registerY);
                    break;
                case 0x6:
                    OP_8XY6(registerX, registerY);
                    break;
                case 0x7:
                    OP_8XY7(registerX, registerY);
                    break;
                case 0xE:
                    OP_8XYE(registerX, registerY);
                    break;
            }
            break;
        case 0x9:
            OP_9XY0(registerX, registerY);
            break;
        case 0xA:
            OP_ANNN(addr);
            break;
        case 0xB;
            OP_BNNN(addr);
            break;
        case 0xC:
            OP_CXNN(registerX, eightBitConstant);
            break;
        case 0xD:
            OP_DXYN(registerX, registerY, fourBitConstant);
            break;
        case 0xE:
            if (eightBitConstant == 0x9E) {
                OP_EX9E(registerX);
            } else if (eightBitConstant == 0xA1) {
                OP_EXA1(registerX);
            }
            break;
        case 0xF:
            switch(eightBitConstant) {
                case 0x07:
                    OP_FX07(registerX);
                    break;
                case 0x0A:
                    OP_FX0A(registerX);
                    break;
                case 0x15:
                    OP_FX15(registerX);
                    break;
                case 0x1E:
                    OP_FX1E(registerX);
                    break;
                case 0x29:
                    OP_FX29(registerX);
                    break;
                case 0x33:
                    OP_FX33(registerX);
                    break;
                case 0x55:
                    OP_FX55(registerX);
                    break;
                case 0x65:
                    OP_FX65(registerX);
                    break;
            }
            break;
    }
}

