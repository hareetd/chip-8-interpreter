#include <cstdint>

class CHIP8 
{
public:
    uint8_t memory[4096];
    uint8_t display[64*32];
    uint16_t PC;
    uint16_t I;
    uint16_t stack[12];
    uint8_t SP;
    uint8_t delayTimer;
    uint8_t soundTimer;
    uint8_t registers[16];
    uint8_t keypad[16];
    uint16_t opcode;
    uint16_t START_ADDRESS = 0x200;
private:
    void load(const char* ROM);
    uint8_t readByte(uint16_t address);
    void writeByte(uint16_t address, uint8_t val);
    uint16_t readWord(uint16_t address);
    void writeWord(uint16_t address, uint16_t val);
    void fetch();
    void decodeAndExecute();
    void OP_00E0();
    void OP_00EE();
    void OP_1NNN(uint16_t address);
    void OP_2NNN(uint16_t address);
    void OP_3XNN(uint8_t VX, uint8_t byte);
    void OP_4XNN(uint8_t VX, uint8_t byte);
    void OP_5XY0(uint8_t VX, uint8_t VY);
    void OP_6XNN(uint8_t VX, uint8_t byte);
    void OP_7XNN(uint8_t VX, uint8_t byte);
    void OP_8XY0(uint8_t VX, uint8_t VY);
    void OP_8XY1(uint8_t VX, uint8_t VY);
    void OP_8XY2(uint8_t VX, uint8_t VY);
    void OP_8XY3(uint8_t VX, uint8_t VY);
    void OP_8XY4(uint8_t VX, uint8_t VY);
    void OP_8XY5(uint8_t VX, uint8_t VY);
    void OP_8XY6(uint8_t VX, uint8_t VY);
    void OP_8XY7(uint8_t VX, uint8_t VY);
    void OP_8XYE(uint8_t VX, uint8_t VY);
    void OP_9XY0(uint8_t VX, uint8_t VY);
    void OP_ANNN(uint16_t address);
    void OP_BNNN(uint16_t address);
    void OP_CXNN(uint8_t VX, uint8_t byte);
    void OP_DXYN(uint8_t VX, uint8_t VY, uint8_t height);
    void OP_EX9E(uint8_t VX);
    void OP_EXA1(uint8_t VX);
    void OP_FX07(uint8_t VX);
    void OP_FX0A(uint8_t VX);
    void OP_FX15(uint8_t VX);
    void OP_FX18(uint8_t VX);
    void OP_FX1E(uint8_t VX);
    void OP_FX29(uint8_t VX);
    void OP_FX33(uint8_t VX);
    void OP_FX55(uint8_t VX);
    void OP_FX65(uint8_t VX);
    void generateInterrupts();
    void emulateGraphics();
    void emulateSound();
};