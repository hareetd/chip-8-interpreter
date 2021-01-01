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
    uint16_t opcode;
private:
    void load(const char* ROM);
};

CHIP8::load(const char* ROM) {
    int START_ADDRESS = 512;
    std::ifstream file(ROM, std::ifstream::binary);

    if (file.is_open()) {
        // get length of file
        file.seekg(0, file.end);
        int length = file.tellg();
        file.seekg(0, file.beg);

        // load the ROM data into memory starting at address 0x200
        file.read(&memory[START_ADDRESS], length);

        // close the file
        file.close();   
    }
}