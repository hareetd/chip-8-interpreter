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
        val == val << 8 | readByte(addr + 1);
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