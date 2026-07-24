<h3>List of all opcode:</h3>

Data opcode<br>

LDTM: Load to memory or declare a variable with r0 for address and r1 is for value on memory.<br>
MOV: Move value from memory at r1 address to r0 address on memory.<br>

Math and bit-wise operations<br>

r0: Mean using the value of the r0 memory address.<br>
r1: Mean using the value of the r1 memory address.<br>
r2: Mean using the value of the r2 memory address.<br>

ADD: Add operation to add up two r0 and r1 together and store at r2 address on memory.<br>
SUB: Subtract operation to subtract two number r0 and r1 together and store at r2 address on memory.<br>
MUL: Mulply operation to multiply number r0 and r1 together and store at r2 address on memory.<br>
DIV: Division operation to division number r0 and r1 together and store at r2 address on memory.<br>
INC: Increment the number at r0 address on memory.<br>
DEC: Decrement the number at r0 address on memory.<br>
AND: And bit-wise operation that use r0 and r1 together and output the r0 & r1 at r2 address.<br>
OR: Or bit-wise operation that use r0 and r1 together and output the r0 | r1 at r2 address. <br>
XOR: Xor (Exclusive Or) bit-wise operation that use r0 and r1 together and output the r0 ^ r1 at r2 address.<br>
NOT: Not bit-wise operation that use r0 to output ~r0 at r1 address.<br>
SHL: Shift left bit-wise operation that use r0 value at memory and r1 value in the operand to shift left how many times and store at r2 address.<br>
SHR: Shift right bit-wise operation that use r0 value at memory and r1 value in the operand to shift right how many times and store at r2 address.<br>

Condition and Flag, Jump and routines<br>

CMP: Compare between r0 and r1 and set flag to it.<br>
JMP: Jump operation that set the program counter with r0 value.<br>
BRH: Branch operation that set the program counter with r0 value with condition that satisfied r1 mask value.<br>
CALL: Call to subroutine at r0 value as address.<br>
CALLC: Call to subroutine at r0 value as address with condition that satisfied r1 mask value.<br>
RET: Return from subroutine and also a must in a subroutine to make it return to the last + 1 current program counter value.<br>

Flag code as bit: 0001 (1) equal, 0010 (2) greater, 0100 (4) less, 0011 (3) greater or equal, 0101 (5) less or equal, 0110 (6) not equal<br>

I/O<br>
INPUT: As it name it is input but it handle by reading the keyboard input and store the character in the ASCII number to r0 address.<br>
GSCRY: An opcode to get screen height and store it to memory address using r0.<br>
GSCRX: An opcode to get screen width and store it to memory address using r0. <br>
SCR: Assign value of color by r2 memory address at r0 address as y axis and r1 address as x axis to virtual VRAM.<br>
DSCR: Flush frame into the buffer and output with character.<br>
CLS: Clear screen value and clear the output console.<br>

Other<br>
DLY: A delay operation that delay how many r0 value in miliseconds.<br>
HLT: A halt operation that halt the program.
