31
ADDI I 3 rs rt Imm func 0x8
ADD R 3 rs rt rd opcode 0x0 func 0x20 sa 0x0
BEQ I 3 rs rt Rel func 0x4
AND R 3 rs rt rd opcode 0x0 func 0x24 sa 0x0
BLEZ I 2 rs Rel func 0x6 rt 0x0
BGTZ I 2 rs Rel func 0x7 rt 0x0
DIV R 2 rs rt opcode 0x0 func 0x1A sa 0x0 rd 0x0
BNE I 3 rs rt Rel func 0x5
J J 1 Abs func 0x2
JR R 1 rs opcode 0x0 func 0x8 sa 0x0 rt 0x0 rd 0x0
JAL J 1 Abs func 0x3
LUI I 2 rt Imm func 0xF rs 0x0
MFHI R 1 rd opcode 0x0 func 0x10 sa 0x0 rs 0x0 rt 0x0
LW I 2 rt Bas func 0x23
MULT R 2 rs rt opcode 0x0 func 0x18 sa 0x0 rd 0x0
MFLO R 1 rd opcode 0x0 func 0x12 sa 0x0 rs 0x0 rt 0x0
OR R 3 rs rt rd opcode 0x0 func 0x25 sa 0x0
NOP P 0
SLL R 3 rt rd sa opdcode 0x0 func 0x0 rs 0x0
ROTR R 3 rt rd sa opcode 0x0 func 0x2 rs 0x0
SRL R 3 rt rd sa opcode 0x0 func 0x2 rs 0x0
SLT R 3 rs rt rd opcode 0x0 func 0x2A sa 0x0
SW I 2 rt Bas func 0x2B
SUB R 3 rs rt rd opcode 0x0 func 0x22 sa 0x0
XOR R 3 rs rt rd opcode 0x0 func 0x26 sa 0x0
MOVE P 2 Reg Reg
NEG P 2 Reg Reg
LI P 2 Reg Imm
BLT P 3 Reg Reg Abs
LW P 2 Reg Abs
SW P 2 Reg Abs