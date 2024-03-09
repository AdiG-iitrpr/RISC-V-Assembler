.data
array: .word 10 20 30 40 50
loc1: .word 0x10001000
loc2: .word 0x10002000
n: .word 5

.text
addi x10 x0 1
slli x10 x10 28
lw x13 24(x10)
lw x15 28(x10)
addi x16 x0 0
addi x17 x0 0
bne x16 x15 loop

loop:
    lw x18 0(x10)
    addi x10 x10 4
    addi x16 x16 1
    addi x19 x0 0
    beq x0 x0 count

#1'S COUNT LOGIC take and with 1 and add it and then right shift the number by 1 until 0
count:
    andi x20 x18 1
    srli x18 x18 1
    add x19 x19 x20
    bne x18 x0 count
    beq x0 x0 fallthrough
 
fallthrough:
    sw x19 0(x13)
    addi x13 x13 4
    bne x16 x15 loop
    beq x0 x0 exit

exit: