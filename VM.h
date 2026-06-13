#pragma once
#include <vector>
#include <cstdint>
#include <iostream>

enum class OpCode : uint8_t {
    LOAD_CONST, LOAD_VAR, STORE_VAR,
    ADD, SUB, MUL, DIV, NEG,
    PRINT, POP, RETURN
};

struct ByteCodeInstruction {
    OpCode opcode;
    int32_t operand;
    
    ByteCodeInstruction(OpCode op, int32_t arg = 0) : opcode(op), operand(arg) {}
};

class VirtualMachine {
private:
    vector<int32_t> stack;
    vector<int32_t> variables;
    
    void executeArithmetic(OpCode op) {
        int32_t b = stack.back(); stack.pop_back();
        int32_t a = stack.back(); stack.pop_back();
        
        switch (op) {
            case OpCode::ADD: stack.push_back(a + b); break;
            case OpCode::SUB: stack.push_back(a - b); break;
            case OpCode::MUL: stack.push_back(a * b); break;
            case OpCode::DIV: 
                if (b != 0) stack.push_back(a / b); 
                break;
            default: break;
        }
    }
    
public:
    VirtualMachine() : variables(256, 0) {}
    
    void execute(const std::vector<ByteCodeInstruction>& bytecode, 
                 const std::vector<int32_t>& constants) 
                 {
        
        cout << "EXECUTION\n";
        
        for (const auto& instr : bytecode)
         {
            switch (instr.opcode) 
            {
                case OpCode::LOAD_CONST:
                    stack.push_back(constants[instr.operand]);
                    break;
                    
                case OpCode::LOAD_VAR:
                    stack.push_back(variables[instr.operand]);
                    break;
                    
                case OpCode::STORE_VAR:
                    if (!stack.empty()) {
                        variables[instr.operand] = stack.back();
                    }
                    break;
                    
                case OpCode::ADD:
                case OpCode::SUB:
                case OpCode::MUL:
                case OpCode::DIV:
                    executeArithmetic(instr.opcode);
                    break;
                    
                case OpCode::NEG:
                    if (!stack.empty()) 
                    {
                        stack.back() = -stack.back();
                    }
                    break;
                    
                case OpCode::PRINT:
                    if (!stack.empty()) 
                    {
                        std::cout << "Result: " << stack.back() << "\n";
                        stack.pop_back();
                    }
                    break;
                    
                case OpCode::POP:
                    if (!stack.empty()) stack.pop_back();
                    break;
                    
                case OpCode::RETURN:
                    return;
            }
        }
    }
};
