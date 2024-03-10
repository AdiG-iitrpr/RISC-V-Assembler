# Aditya Goel
# 2020MCB1259
# Note I am storing underflow and overflow outputs at x31 instead of x10
.data
max_size: .word 20
operations: .asciiz "eedesdes"
data: .byte 10 20 6 7
E: .asciiz "E"
D: .asciiz "D"
.text
auipc x10, 65536
addi x10, x10, 4
auipc x11, 65536
addi x11, x11, 5
lui x13, 65536      # Storing address
addi x13, x13, 512
lui x23, 65536      # Starting address of queue
addi x23, x23, 256
addi x15, x0, 0     # Queue size
addi x9, x0, 1      # Word size
lui x30 65536       # Message address
addi x30 x30 512

addi x14, x0, 0     # iterator for operations
addi x8, x0, 0      # iterator for data
addi x22, x0, 0     # iterator for storing output
addi x6, x0, 0      # iterator for queue
addi x20, x0, 100   # To match for d
addi x21, x0, 101   # To match for e

addi x3, x0, 69
addi x4, x0, 68

loop:
    add x16, x14, x10
    lb x17, 0(x16)
    beq x17, x0, exit
    beq x17, x21, enq
    beq x17, x20, deq
    addi x14, x14, 1
    beq x0, x0, loop
    enq:
        jal x1, enqueue
        sb x3, 0(x30)
        addi x30, x30, 1
        addi x14, x14, 1
        beq x0, x0, loop
    deq:
        jal x1, dequeue
        sb x4, 0(x30)
        addi x30, x30, 1
        addi x14, x14, 1
        beq x0, x0, loop
        
enqueue:
    beq x6, x12, jump       # Overflow
    add x24, x11, x8
    add x8, x8, x9
    addi x15, x15, 1
    lb x7, 0(x24)
    add x5, x23, x6
    sb x7, 0(x5)
    add x6, x6, x9
    jalr x0, x1, 0
    jump:
        addi x31, x0, -2
        beq x0, x0, exit
    
dequeue:
    beq x15, x0, jump2      # Underflow
    addi x15, x15, -1
    addi x6, x6, -1
    addi x26, x8,-1
    addi x27, x0, 0
    lp: 
        bge x27, x26, st
        add x28, x27, x23
        addi x28, x28, 1
        lb x29, 0(x28)
        addi x28, x28, -1
        sb x29, 0(x28)
        addi x27, x27, 1
        beq x0, x0, lp
    st:
        jalr x0, x1, 0
    jump2:
        addi x31, x0, -1
        beq x0, x0, exit
exit: