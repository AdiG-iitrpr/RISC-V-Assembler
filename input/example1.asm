#kartik kumar garg
#2020mcb1240
.data
n: .word 3
from_rod: .word 1
to_rod: .word 2
helper_rod: .word 3
location: .word 0x00001000

.text
addi x6 x0 1
slli x6 x6 28
lw x3 0(x6)
lw x4 16(x6)
addi x5 x0 0
lw x11 4(x6)
lw x12 8(x6)
lw x13 12(x6)
addi x20 x0 1
jal x1 tower
beq x0 x0 exit


tower:
addi sp sp -20
sw x1 0(sp)
sw x13 4(sp)
sw x12 8(sp)
sw x11 12(sp)
sw x3 16(sp)

bne x3 x20 recurse
addi sp sp 20
# logic for printing
add x16 x5 x0
add x17 x11 x0
slli x17 x17 4
addi x16 x16 153
add x16 x16 x17
add x16 x16 x12
add x18 x3 x0
slli x18 x18 8
add x16 x16 x18
sw x16 0(x4)
addi x4 x4 4
jalr x0 x1 0

recurse:
addi x3 x3 -1
add x14 x13 x0
add x13 x0 x12
add x12 x0 x14
jal x1 tower
beq x0 x0 rec


rec:
lw x1 0(sp)
lw x13 4(sp)
lw x12 8(sp)
lw x11 12(sp)
lw x3 16(sp)
addi sp sp 20
# logic for printing
add x16 x5 x0
add x17 x11 x0
slli x17 x17 4
addi x16 x16 153
add x16 x16 x17
add x16 x16 x12
add x18 x3 x0
slli x18 x18 8
add x16 x16 x18
sw x16 0(x4)
addi x4 x4 4

addi sp sp -20
sw x1 0(sp)
sw x13 4(sp)
sw x12 8(sp)
sw x11 12(sp)
sw x3 16(sp)

#swap again
addi x3 x3 -1
add x14 x13 x0
add x13 x11 x0
add x11 x14 x0
jal x1 tower
lw x1 0(sp)
lw x13 4(sp)
lw x12 8(sp)
lw x11 12(sp)
lw x3 16(sp)
addi sp sp 20

jalr x0 x1 0


exit: