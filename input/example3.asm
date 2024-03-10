# ROHAN KUMAR 
# 2020MCB1247

# Q2 Implementing a queue

.data 

queue_address: .word 0x10000100
string_E: .asciiz "ESUC"
string_D: .asciiz "DSUC"
statement_address: .word 0x10000200
arr: .word 10 20 30 40
operations: .asciiz "EEEEEDDDDS"

.text 
# Running a loop to take a character from operations string
# storing 'E', 'D', 'S' in x21, x22, x23
addi x21, x0, 69
addi x22, x0, 68
addi x23, x0, 83

# la x5, operations # x5 - address of operations
# auipc x5 65536
# addi x5 x5 22

# la x6, arr # address of the array
# auipc x6 65536
# addi x6 x6 -2

# la x7, queue_address
auipc x7 65536
addi x7 x7 -28

lw x7, 0(x7) # address of queue

addi x8, x0, 4 # INPUT n here 

# la x27, string_E
auipc x27 65536
addi x27 x27 -40

lw x27, 0(x27)

# la x28, string_D
auipc x28 65536
addi x28 x28 -47

lw x28, 0(x28)

# la x29, statement_address
auipc x29 65536
addi x29 x29 -54

lw x29, 0(x29)
addi x30, x0, 0 # statement counter


addi x20, x0, 0 # x20 - size of the queue
addi x11, x0, 0 # x11 - i starting from 0
loop: # reading the current character 
add x12, x11, x5 # address of current character 
lb x16, 0(x12) # x16 - current character
beq x16, x0, exit_loop # if character is null then exit loop
# compare with E, then D, then S 
beq x16, x21, enqueue
beq x16, x22, dequeue
beq x16, x23, size

loop_end: 
addi x11, x11, 1 # i += 1
beq x0, x0, loop # go back to start of loop


exit_loop:
beq x0, x0, exit


enqueue:
    blt x20, x8, normal1
    beq x0, x0, overflow
    normal1:
    # first calculate arr[i] in x24
    add x24, x0, x11
    # slli x24, x24, 2
    add x24, x24, x6
    lw x24, 0(x24) # x24 - arr[i]
    # now we calculate address of queue[i]
    add x25, x0, x11
    # slli x25, x25, 2
    add x25, x25, x7 # x25 is address of queue[i]
    # now we store arr[i] at address of queue[i]
    sw x24, 0(x25)
    # now we increase size by 1
    addi x20, x20, 1
    # printing statement 
    add x18, x0, x30
    # slli x18, x18, 2
    add x18, x18, x29
    sw x27, 0(x18)
    addi x30, x30, 1
    
    beq x0, x0, loop_end
    

dequeue:
    addi x24, x0, 1 # x24 - 1
    bge x20, x24, normal2 # normal if size >= 1
    beq x0, x0, underflow
    normal2:
    # shift queue address ahead by 4 bytes 
    addi x7, x7, 4
    # decrease queue size by 1
    addi x20, x20, -1
    # printing statement 
    add x18, x0, x30
    # slli x18, x18, 2
    add x18, x18, x29
    sw x28, 0(x18)
    addi x30, x30, 1
    
    beq x0, x0, loop_end

size:
    add x15, x20, x0 # as x20 was keeping track of the size
    beq x0, x0, loop_end
    
overflow:
    addi x10, x0, -2
    beq x0, x0, exit
    
underflow:
    addi x10, x0, -1
    beq x0, x0, exit

exit: