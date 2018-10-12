#include "vm.h"

#define _NEXT_BYTE this->_program[++this->_registers[IP]]
#define _NEXT_SHORT ((uint16_t)_NEXT_BYTE | ((uint16_t)_NEXT_BYTE << 8))
#define _NEXT_INT ((uint32_t)_NEXT_BYTE | ((uint32_t)_NEXT_BYTE << 8) | ((uint32_t)_NEXT_BYTE << 16) | ((uint32_t)_NEXT_BYTE << 24))

VM::VM(const uint8_t *program, const uint8_t *data, uint16_t dataSize, uint16_t freeBytes)
    : _program(program), _dataSize(dataSize + freeBytes)
{
    if (this->_dataSize > 0)
    {
        this->_data = new uint8_t[this->_dataSize];
        if (dataSize > 0)
            memcpy((void *)this->_data, (void *)data, dataSize);
    }
}

VM::~VM()
{
    if (this->_dataSize > 0)
        delete[] this->_data;
}

void VM::run()
{
    this->_running = true;

    while (this->_running)
    {
        switch (this->_program[this->_registers[IP]])
        {
        case OP_MOV:
        {
            const uint8_t reg1 = _NEXT_BYTE;
            const uint8_t reg2 = _NEXT_BYTE;
            this->_registers[reg1] = this->_registers[reg2];
            break;
        }
        case OP_LCONS:
        {
            const uint8_t reg = _NEXT_BYTE;
            this->_registers[reg] = _NEXT_INT;
            break;
        }
        case OP_LCONSW:
        {
            const uint8_t reg = _NEXT_BYTE;
            this->_registers[reg] = _NEXT_SHORT;
            break;
        }
        case OP_LCONSB:
        {
            const uint8_t reg = _NEXT_BYTE;
            this->_registers[reg] = _NEXT_BYTE;
            break;
        }
        case OP_PUSH:
        {
            this->_stack[++this->_registers[SP]] = this->_registers[_NEXT_BYTE];
            break;
        }
        case OP_POP:
        {
            const uint8_t reg = _NEXT_BYTE;
            this->_registers[reg] = this->_stack[this->_registers[SP]--];
            break;
        }
        case OP_POP2:
        {
            const uint8_t reg1 = _NEXT_BYTE;
            const uint8_t reg2 = _NEXT_BYTE;
            this->_registers[reg1] = this->_stack[this->_registers[SP]--];
            this->_registers[reg2] = this->_stack[this->_registers[SP]--];
            break;
        }
        case OP_DUP:
        {
            this->_registers[SP]++;
            this->_stack[this->_registers[SP]] = this->_stack[this->_registers[SP] - 1];
            break;
        }
        case OP_CALL:
        {
            this->_registers[RA] = this->_registers[IP] + 3;
            this->_registers[IP] = _NEXT_SHORT - 1;
            break;
        }
        case OP_RET:
        {
            this->_registers[IP] = this->_registers[RA] - 1;
            break;
        }
        case OP_STOR:
        {
            const uint16_t addr = _NEXT_SHORT;
            const uint8_t reg = _NEXT_BYTE;
            *(uint32_t *)&this->_data[addr] = this->_registers[reg];
            break;
        }
        case OP_STORW:
        {
            const uint16_t addr = _NEXT_SHORT;
            const uint8_t reg = _NEXT_BYTE;
            *(uint16_t *)&this->_data[addr] = *(uint16_t *)&this->_registers[reg];
            break;
        }
        case OP_STORB:
        {
            const uint16_t addr = _NEXT_SHORT;
            const uint8_t reg = _NEXT_BYTE;
            this->_data[addr] = *(uint8_t *)&this->_registers[reg];
            break;
        }
        case OP_LOAD:
        {
            const uint8_t reg = _NEXT_BYTE;
            const uint16_t addr = _NEXT_SHORT;
            this->_registers[reg] = *(uint32_t *)&this->_data[addr];
            break;
        }
        case OP_LOADW:
        {
            const uint8_t reg = _NEXT_BYTE;
            const uint16_t addr = _NEXT_SHORT;
            this->_registers[reg] = *(uint16_t *)&this->_data[addr];
            break;
        }
        case OP_LOADB:
        {
            const uint8_t reg = _NEXT_BYTE;
            const uint16_t addr = _NEXT_SHORT;
            this->_registers[reg] = *(uint8_t *)&this->_data[addr];
            break;
        }
        case OP_MEMCPY:
        {
            const uint16_t bytes = _NEXT_SHORT;
            const uint16_t dest = _NEXT_SHORT;
            const uint16_t source = _NEXT_SHORT;
            memcpy((void *)&this->_data[dest], (void *)&this->_data[source], bytes);
            break;
        }
        case OP_INC:
        {
            const uint8_t reg = _NEXT_BYTE;
            this->_registers[reg]++;
            break;
        }
        case OP_FINC:
        {
            const uint8_t reg = _NEXT_BYTE;
            (*((float *)&this->_registers[reg]))++;
            break;
        }
        case OP_DEC:
        {
            const uint8_t reg = _NEXT_BYTE;
            this->_registers[reg]--;
            break;
        }
        case OP_FDEC:
        {
            const uint8_t reg = _NEXT_BYTE;
            (*((float *)&this->_registers[reg]))--;
            break;
        }
        case OP_ADD:
        {
            const uint8_t rreg = _NEXT_BYTE;
            const uint8_t reg1 = _NEXT_BYTE;
            const uint8_t reg2 = _NEXT_BYTE;
            this->_registers[rreg] = this->_registers[reg1] + this->_registers[reg2];
            break;
        }
        case OP_FADD:
        {
            const uint8_t rreg = _NEXT_BYTE;
            const uint8_t reg1 = _NEXT_BYTE;
            const uint8_t reg2 = _NEXT_BYTE;
            *((float *)&this->_registers[rreg]) = *((float *)&this->_registers[reg1]) + *((float *)&this->_registers[reg2]);
            break;
        }
        case OP_SUB:
        {
            const uint8_t rreg = _NEXT_BYTE;
            const uint8_t reg1 = _NEXT_BYTE;
            const uint8_t reg2 = _NEXT_BYTE;
            this->_registers[rreg] = this->_registers[reg1] - this->_registers[reg2];
            break;
        }
        case OP_FSUB:
        {
            const uint8_t rreg = _NEXT_BYTE;
            const uint8_t reg1 = _NEXT_BYTE;
            const uint8_t reg2 = _NEXT_BYTE;
            *((float *)&this->_registers[rreg]) = *((float *)&this->_registers[reg1]) - *((float *)&this->_registers[reg2]);
            break;
        }
        case OP_MUL:
        {
            const uint8_t rreg = _NEXT_BYTE;
            const uint8_t reg1 = _NEXT_BYTE;
            const uint8_t reg2 = _NEXT_BYTE;
            this->_registers[rreg] = this->_registers[reg1] * this->_registers[reg2];
            break;
        }
        case OP_IMUL:
        {
            const uint8_t rreg = _NEXT_BYTE;
            const uint8_t reg1 = _NEXT_BYTE;
            const uint8_t reg2 = _NEXT_BYTE;
            *((int32_t *)&this->_registers[rreg]) = *((int32_t *)&this->_registers[reg1]) * *((int32_t *)&this->_registers[reg2]);
            break;
        }
        case OP_FMUL:
        {
            const uint8_t rreg = _NEXT_BYTE;
            const uint8_t reg1 = _NEXT_BYTE;
            const uint8_t reg2 = _NEXT_BYTE;
            *((float *)&this->_registers[rreg]) = *((float *)&this->_registers[reg1]) * *((float *)&this->_registers[reg2]);
            break;
        }
        case OP_DIV:
        {
            const uint8_t rreg = _NEXT_BYTE;
            const uint8_t reg1 = _NEXT_BYTE;
            const uint8_t reg2 = _NEXT_BYTE;
            this->_registers[rreg] = this->_registers[reg1] / this->_registers[reg2];
            break;
        }
        case OP_IDIV:
        {
            const uint8_t rreg = _NEXT_BYTE;
            const uint8_t reg1 = _NEXT_BYTE;
            const uint8_t reg2 = _NEXT_BYTE;
            *((int32_t *)&this->_registers[rreg]) = *((int32_t *)&this->_registers[reg1]) / *((int32_t *)&this->_registers[reg2]);
            break;
        }
        case OP_FDIV:
        {
            const uint8_t rreg = _NEXT_BYTE;
            const uint8_t reg1 = _NEXT_BYTE;
            const uint8_t reg2 = _NEXT_BYTE;
            *((float *)&this->_registers[rreg]) = *((float *)&this->_registers[reg1]) / *((float *)&this->_registers[reg2]);
            break;
        }
        case OP_SHL:
        {
            const uint8_t rreg = _NEXT_BYTE;
            const uint8_t reg1 = _NEXT_BYTE;
            const uint8_t reg2 = _NEXT_BYTE;
            this->_registers[rreg] = this->_registers[reg1] << this->_registers[reg2];
            break;
        }
        case OP_SHR:
        {
            const uint8_t rreg = _NEXT_BYTE;
            const uint8_t reg1 = _NEXT_BYTE;
            const uint8_t reg2 = _NEXT_BYTE;
            this->_registers[rreg] = this->_registers[reg1] >> this->_registers[reg2];
            break;
        }
        case OP_ISHR:
        {
            const uint8_t rreg = _NEXT_BYTE;
            const uint8_t reg1 = _NEXT_BYTE;
            const uint8_t reg2 = _NEXT_BYTE;
            *((int32_t *)&this->_registers[rreg]) = *((int32_t *)&this->_registers[reg1]) >> *((int32_t *)&this->_registers[reg2]);
            break;
        }
        case OP_MOD:
        {
            const uint8_t rreg = _NEXT_BYTE;
            const uint8_t reg1 = _NEXT_BYTE;
            const uint8_t reg2 = _NEXT_BYTE;
            this->_registers[rreg] = this->_registers[reg1] % this->_registers[reg2];
            break;
        }
        case OP_IMOD:
        {
            const uint8_t rreg = _NEXT_BYTE;
            const uint8_t reg1 = _NEXT_BYTE;
            const uint8_t reg2 = _NEXT_BYTE;
            *((int32_t *)&this->_registers[rreg]) = *((int32_t *)&this->_registers[reg1]) % *((int32_t *)&this->_registers[reg2]);
            break;
        }
        case OP_AND:
        {
            const uint8_t rreg = _NEXT_BYTE;
            const uint8_t reg1 = _NEXT_BYTE;
            const uint8_t reg2 = _NEXT_BYTE;
            this->_registers[rreg] = this->_registers[reg1] & this->_registers[reg2];
            break;
        }
        case OP_OR:
        {
            const uint8_t rreg = _NEXT_BYTE;
            const uint8_t reg1 = _NEXT_BYTE;
            const uint8_t reg2 = _NEXT_BYTE;
            this->_registers[rreg] = this->_registers[reg1] | this->_registers[reg2];
            break;
        }
        case OP_XOR:
        {
            const uint8_t rreg = _NEXT_BYTE;
            const uint8_t reg1 = _NEXT_BYTE;
            const uint8_t reg2 = _NEXT_BYTE;
            this->_registers[rreg] = this->_registers[reg1] ^ this->_registers[reg2];
            break;
        }
        case OP_NOT:
        {
            const uint8_t rreg = _NEXT_BYTE;
            const uint8_t reg1 = _NEXT_BYTE;
            this->_registers[rreg] = ~this->_registers[reg1];
            break;
        }
        case OP_U2I:
        {
            const uint8_t reg = _NEXT_BYTE;
            *((int32_t *)&this->_registers[reg]) = this->_registers[reg];
            break;
        }
        case OP_I2U:
        {
            const uint8_t reg = _NEXT_BYTE;
            this->_registers[reg] = *((int32_t *)&this->_registers[reg]);
            break;
        }
        case OP_I2F:
        {
            const uint8_t reg = _NEXT_BYTE;
            *((float *)&this->_registers[reg]) = (float)this->_registers[reg];
            break;
        }
        case OP_F2I:
        {
            const uint8_t reg = _NEXT_BYTE;
            this->_registers[reg] = (uint32_t) * ((float *)&this->_registers[reg]);
            break;
        }
        case OP_JMP:
        {
            this->_registers[IP] = _NEXT_SHORT - 1;
            break;
        }
        case OP_JR:
        {
            this->_registers[IP] = this->_registers[_NEXT_BYTE] - 1;
            break;
        }
        case OP_JZ:
        {
            const uint8_t reg = _NEXT_BYTE;
            const uint16_t addr = _NEXT_SHORT;

            if (this->_registers[reg] == 0)
                this->_registers[IP] = addr - 1;
            break;
        }
        case OP_JNZ:
        {
            const uint8_t reg = _NEXT_BYTE;
            const uint16_t addr = _NEXT_SHORT;

            if (this->_registers[reg] != 0)
                this->_registers[IP] = addr - 1;
            break;
        }
        case OP_JE:
        {
            const uint8_t reg1 = _NEXT_BYTE;
            const uint8_t reg2 = _NEXT_BYTE;
            const uint16_t addr = _NEXT_SHORT;

            if (this->_registers[reg1] == this->_registers[reg2])
                this->_registers[IP] = addr - 1;
            break;
        }
        case OP_JNE:
        {
            const uint8_t reg1 = _NEXT_BYTE;
            const uint8_t reg2 = _NEXT_BYTE;
            const uint16_t addr = _NEXT_SHORT;

            if (this->_registers[reg1] != this->_registers[reg2])
                this->_registers[IP] = addr - 1;
            break;
        }
        case OP_JA:
        {
            const uint8_t reg1 = _NEXT_BYTE;
            const uint8_t reg2 = _NEXT_BYTE;
            const uint16_t addr = _NEXT_SHORT;

            if (this->_registers[reg1] > this->_registers[reg2])
                this->_registers[IP] = addr - 1;
            break;
        }
        case OP_JG:
        {
            const uint8_t reg1 = _NEXT_BYTE;
            const uint8_t reg2 = _NEXT_BYTE;
            const uint16_t addr = _NEXT_SHORT;

            if (*((int32_t *)&this->_registers[reg1]) > *((int32_t *)&this->_registers[reg2]))
                this->_registers[IP] = addr - 1;
            break;
        }
        case OP_JAE:
        {
            const uint8_t reg1 = _NEXT_BYTE;
            const uint8_t reg2 = _NEXT_BYTE;
            const uint16_t addr = _NEXT_SHORT;

            if (this->_registers[reg1] >= this->_registers[reg2])
                this->_registers[IP] = addr - 1;
            break;
        }
        case OP_JGE:
        {
            const uint8_t reg1 = _NEXT_BYTE;
            const uint8_t reg2 = _NEXT_BYTE;
            const uint16_t addr = _NEXT_SHORT;

            if (*((int32_t *)&this->_registers[reg1]) >= *((int32_t *)&this->_registers[reg2]))
                this->_registers[IP] = addr - 1;
            break;
        }
        case OP_JB:
        {
            const uint8_t reg1 = _NEXT_BYTE;
            const uint8_t reg2 = _NEXT_BYTE;
            const uint16_t addr = _NEXT_SHORT;

            if (this->_registers[reg1] < this->_registers[reg2])
                this->_registers[IP] = addr - 1;
            break;
        }
        case OP_JL:
        {
            const uint8_t reg1 = _NEXT_BYTE;
            const uint8_t reg2 = _NEXT_BYTE;
            const uint16_t addr = _NEXT_SHORT;

            if (*((int32_t *)&this->_registers[reg1]) < *((int32_t *)&this->_registers[reg2]))
                this->_registers[IP] = addr - 1;
            break;
        }
        case OP_JBE:
        {
            const uint8_t reg1 = _NEXT_BYTE;
            const uint8_t reg2 = _NEXT_BYTE;
            const uint16_t addr = _NEXT_SHORT;

            if (this->_registers[reg1] <= this->_registers[reg2])
                this->_registers[IP] = addr - 1;
            break;
        }
        case OP_JLE:
        {
            const uint8_t reg1 = _NEXT_BYTE;
            const uint8_t reg2 = _NEXT_BYTE;
            const uint16_t addr = _NEXT_SHORT;

            if (*((int32_t *)&this->_registers[reg1]) <= *((int32_t *)&this->_registers[reg2]))
                this->_registers[IP] = addr - 1;
            break;
        }
        case OP_PRINT:
        {
            const uint8_t reg = _NEXT_BYTE;
            printf("%u", this->_registers[reg]);
            break;
        }
        case OP_PRINTI:
        {
            const uint8_t reg = _NEXT_BYTE;
            printf("%d", *((int32_t *)&this->_registers[reg]));
            break;
        }
        case OP_PRINTF:
        {
            const uint8_t reg = _NEXT_BYTE;
            printf("%f", *((float *)&this->_registers[reg]));
            break;
        }
        case OP_PRINTP:
        {
            const uint32_t addr = this->_registers[_NEXT_BYTE];
            char *curChar = (char *)&this->_data[addr];
            while (*curChar != '\0')
            {
                putchar(*curChar);
                curChar++;
            }
            break;
        }
        case OP_PRINTLN:
        {
            putchar('\n');
            break;
        }
        case OP_I2S:
        {
            const uint16_t addr = _NEXT_SHORT;
            const uint8_t reg = _NEXT_BYTE;
            sprintf((char *)&this->_data[addr], "%d", this->_registers[reg]);
            break;
        }
        case OP_S2I:
        {
            const uint8_t reg = _NEXT_BYTE;
            const uint16_t addr = _NEXT_SHORT;
            sscanf((char *)&this->_data[addr], "%d", &this->_registers[reg]);
            break;
        }
        case OP_A_DR:
        {
            const uint8_t reg = _NEXT_BYTE;
            const uint8_t pin = _NEXT_BYTE;

#ifdef ARDUINO
            this->_registers[reg] = digitalRead(pin);
#else
            this->_registers[reg] = 1;
#endif

            break;
        }
        case OP_A_AR:
        {
            const uint8_t reg = _NEXT_BYTE;
            const uint8_t pin = _NEXT_BYTE;

#ifdef ARDUINO
            this->_registers[reg] = analogRead(pin);
#else
            this->_registers[reg] = 1024;
#endif

            break;
        }
        case OP_A_DW:
        {
            const uint8_t pin = _NEXT_BYTE;
            const uint8_t state = _NEXT_BYTE;

#ifdef ARDUINO
            digitalWrite(pin, state);
#else
            printf("set pin %d to D%d\n", pin, state);
#endif

            break;
        }
        case OP_A_AW:
        {
            const uint8_t pin = _NEXT_BYTE;
            const uint16_t val = _NEXT_SHORT;

#ifdef ARDUINO
            analogWrite(pin, val);
#else
            printf("set pin %d to A%d\n", pin, val);
#endif

            break;
        }
        case OP_A_DWR:
        {
            const uint8_t pin = _NEXT_BYTE;
            const uint8_t reg = _NEXT_BYTE;

#ifdef ARDUINO
            digitalWrite(pin, this->_registers[reg]);
#else
            printf("set pin %d to D%d\n", pin, this->_registers[reg]);
#endif

            break;
        }
        case OP_A_AWR:
        {
            const uint8_t pin = _NEXT_BYTE;
            const uint8_t reg = _NEXT_BYTE;

#ifdef ARDUINO
            analogWrite(pin, this->_registers[reg]);
#else
            printf("set pin %d to A%d\n", pin, this->_registers[reg]);
#endif

            break;
        }
        case OP_A_PM:
        {
            const uint8_t pin = _NEXT_BYTE;
            const uint8_t mode = _NEXT_BYTE;

#ifdef ARDUINO
            pinMode(pin, mode);
#else
            printf("set pin %d to mode %d\n", pin, mode);
#endif

            break;
        }
        case OP_HALT:
        {
            this->_running = false;
            break;
        }
        }

        this->_registers[IP]++;
    }

    this->_registers[IP] = 0;
    this->_registers[SP] = 0;
}
