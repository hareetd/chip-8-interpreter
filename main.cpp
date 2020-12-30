

class Chip8
{
public:
    uint8_t memory[4096];
    uint8_t display[64*32];
    uint16_t PC;
    uint16_t index;
    uint16_t stack[16];
    uint8_t SP;
    uint8_t delayTimer;
    uint8_t soundTimer;
    uint8_t registers[16];
    uint16_t opcode;
private:
    void 
};