
.data

__true__: .asciiz "true\n"
__false__:.asciiz "false\n"
__true__print: .asciiz "true"
__false__print:.asciiz "false"
__buffer__: .space 10
__error__string__: .asciiz "run time error\n"

a: .word	0		# integer
b: .word	0		# integer
c: .word	0		# integer
x: .word	0		# integer
j: .word	0		# integer
y: .space	500		# string
f: .float	0.0		# float
r: .word	0		# boolean
d: .word	999999		# integer constant
e: .word	-20		# integer constant
z: .word	560		# integer constant
w: .word	-1120		# integer constant

.text

jal main

exit:
li $v0, 10
syscall


# function  iterativeFactorial
.globl iterativeFactorial

iterativeFactorial:
# save $s0-$s7 and $sp and $fp
move $t8, $sp

addiu $sp, $sp, -36
sw $fp, 36($sp)
sw $s0, 32($sp)
sw $s1, 28($sp)
sw $s2, 24($sp)
sw $s3, 20($sp)
sw $s4, 16($sp)
sw $s5, 12($sp)
sw $s6, 8($sp)
sw $s7, 4($sp)
move $fp, $sp
addiu $sp, $sp, -8
addiu $t8, $t8, 4
lw $t0, 0($t8)
sw $t0,0($fp)

# body
li $t0,1
addiu $sp,$sp, -4
sw $t0,4($sp)
addiu $sp, $sp, 4
lw $t0, 0($sp)
sw $t0,4($fp)

# while statment
while__label__0:
lw $t0, 0($fp) 
addiu $sp,$sp, -4 
sw $t0, 4($sp)
li $t0,0
addiu $sp,$sp, -4
sw $t0,4($sp)
lw $t0, 8($sp)
lw $t1, 4($sp)
addiu $sp, $sp, 8
sgt $t2, $t0, $t1
addiu $sp,$sp,-4
sw $t2, 4($sp)
addiu $sp, $sp, 4
lw $t0, 0($sp)
beqz $t0,end__while__label__1
lw $t0, 4($fp) 
addiu $sp,$sp, -4 
sw $t0, 4($sp)
lw $t0, 0($fp) 
addiu $sp,$sp, -4 
sw $t0, 4($sp)
lw $t0, 8($sp)
lw $t1, 4($sp)
addiu $sp, $sp, 8
mul $t2, $t0, $t1
addiu $sp,$sp,-4
sw $t2, 4($sp)
addiu $sp, $sp, 4
lw $t0, 0($sp)
sw $t0,4($fp)
lw $t0, 0($fp) 
addiu $sp,$sp, -4 
sw $t0, 4($sp)
li $t0,1
addiu $sp,$sp, -4
sw $t0,4($sp)
lw $t0, 8($sp)
lw $t1, 4($sp)
addiu $sp, $sp, 8
sub $t2, $t0, $t1
addiu $sp,$sp,-4
sw $t2, 4($sp)
addiu $sp, $sp, 4
lw $t0, 0($sp)
sw $t0,0($fp)
j while__label__0
end__while__label__1:

# return
lw $t0, 4($fp) 
addiu $sp,$sp, -4 
sw $t0, 4($sp)
addiu $sp, $sp, 4
lw $v1, 0($sp)
move $sp, $fp
# restore $fp and %sp and $s0 - $s7
addiu $sp, $sp, 36
lw $s7, -32($sp)
lw $s6, -28($sp)
lw $s5, -24($sp)
lw $s4, -20($sp)
lw $s3, -16($sp)
lw $s2, -12($sp)
lw $s1, -8($sp)
lw $s0, -4($sp)
lw $fp, 0($sp)
jr $ra

# function  absolute_value
.globl absolute_value

absolute_value:
# save $s0-$s7 and $sp and $fp
move $t8, $sp

addiu $sp, $sp, -36
sw $fp, 36($sp)
sw $s0, 32($sp)
sw $s1, 28($sp)
sw $s2, 24($sp)
sw $s3, 20($sp)
sw $s4, 16($sp)
sw $s5, 12($sp)
sw $s6, 8($sp)
sw $s7, 4($sp)
move $fp, $sp
addiu $sp, $sp, -4
addiu $t8, $t8, 4
lw $t0, 0($t8)
sw $t0,0($fp)

# body

# if statment
lw $t0, 0($fp) 
addiu $sp,$sp, -4 
sw $t0, 4($sp)
li $t0,0
addiu $sp,$sp, -4
sw $t0,4($sp)
lw $t0, 8($sp)
lw $t1, 4($sp)
addiu $sp, $sp, 8
slt $t2, $t0, $t1
addiu $sp,$sp,-4
sw $t2, 4($sp)
addiu $sp, $sp, 4
lw $t0, 0($sp)
beqz $t0, if__label__2
lw $t0, 0($fp) 
addiu $sp,$sp, -4 
sw $t0, 4($sp)
lw $t0, 4($sp)
addiu $sp, $sp, 4
mul $t1, $t0,-1
addiu $sp,$sp,-4
sw $t1, 4($sp)
addiu $sp, $sp, 4
lw $t0, 0($sp)
sw $t0,0($fp)
j end__if__label__3
if__label__2:
end__if__label__3:

# return
lw $t0, 0($fp) 
addiu $sp,$sp, -4 
sw $t0, 4($sp)
addiu $sp, $sp, 4
lw $v1, 0($sp)
move $sp, $fp
# restore $fp and %sp and $s0 - $s7
addiu $sp, $sp, 36
lw $s7, -32($sp)
lw $s6, -28($sp)
lw $s5, -24($sp)
lw $s4, -20($sp)
lw $s3, -16($sp)
lw $s2, -12($sp)
lw $s1, -8($sp)
lw $s0, -4($sp)
lw $fp, 0($sp)
jr $ra

# function  calculateSum
.globl calculateSum

calculateSum:
# save $s0-$s7 and $sp and $fp
move $t8, $sp

addiu $sp, $sp, -36
sw $fp, 36($sp)
sw $s0, 32($sp)
sw $s1, 28($sp)
sw $s2, 24($sp)
sw $s3, 20($sp)
sw $s4, 16($sp)
sw $s5, 12($sp)
sw $s6, 8($sp)
sw $s7, 4($sp)
move $fp, $sp
addiu $sp, $sp, -8
addiu $t8, $t8, 4
lw $t0, 0($t8)
sw $t0,4($fp)
addiu $t8, $t8, 4
lw $t0, 0($t8)
sw $t0,0($fp)

# body

# return
lw $t0, 0($fp) 
addiu $sp,$sp, -4 
sw $t0, 4($sp)
lw $t0, 4($fp) 
addiu $sp,$sp, -4 
sw $t0, 4($sp)
lw $t0, 8($sp)
lw $t1, 4($sp)
addiu $sp, $sp, 8
add $t2, $t0, $t1
addiu $sp,$sp,-4
sw $t2, 4($sp)
addiu $sp, $sp, 4
lw $v1, 0($sp)
move $sp, $fp
# restore $fp and %sp and $s0 - $s7
addiu $sp, $sp, 36
lw $s7, -32($sp)
lw $s6, -28($sp)
lw $s5, -24($sp)
lw $s4, -20($sp)
lw $s3, -16($sp)
lw $s2, -12($sp)
lw $s1, -8($sp)
lw $s0, -4($sp)
lw $fp, 0($sp)
jr $ra

# function  calculateDifferent
.globl calculateDifferent

calculateDifferent:
# save $s0-$s7 and $sp and $fp
move $t8, $sp

addiu $sp, $sp, -36
sw $fp, 36($sp)
sw $s0, 32($sp)
sw $s1, 28($sp)
sw $s2, 24($sp)
sw $s3, 20($sp)
sw $s4, 16($sp)
sw $s5, 12($sp)
sw $s6, 8($sp)
sw $s7, 4($sp)
move $fp, $sp
addiu $sp, $sp, -12
addiu $t8, $t8, 4
lw $t0, 0($t8)
sw $t0,4($fp)
addiu $t8, $t8, 4
lw $t0, 0($t8)
sw $t0,0($fp)

# body
lw $t0, 0($fp) 
addiu $sp,$sp, -4 
sw $t0, 4($sp)
lw $t0, 4($fp) 
addiu $sp,$sp, -4 
sw $t0, 4($sp)
lw $t0, 8($sp)
lw $t1, 4($sp)
addiu $sp, $sp, 8
sub $t2, $t0, $t1
addiu $sp,$sp,-4
sw $t2, 4($sp)
addiu $sp, $sp, 4
lw $t0, 0($sp)
sw $t0,8($fp)

# return
lw $t0, 8($fp) 
addiu $sp,$sp, -4 
sw $t0, 4($sp)
addiu $sp, $sp, 4
lw $v1, 0($sp)
move $sp, $fp
# restore $fp and %sp and $s0 - $s7
addiu $sp, $sp, 36
lw $s7, -32($sp)
lw $s6, -28($sp)
lw $s5, -24($sp)
lw $s4, -20($sp)
lw $s3, -16($sp)
lw $s2, -12($sp)
lw $s1, -8($sp)
lw $s0, -4($sp)
lw $fp, 0($sp)
jr $ra

# function  calculateXor
.globl calculateXor

calculateXor:
# save $s0-$s7 and $sp and $fp
move $t8, $sp

addiu $sp, $sp, -36
sw $fp, 36($sp)
sw $s0, 32($sp)
sw $s1, 28($sp)
sw $s2, 24($sp)
sw $s3, 20($sp)
sw $s4, 16($sp)
sw $s5, 12($sp)
sw $s6, 8($sp)
sw $s7, 4($sp)
move $fp, $sp
addiu $sp, $sp, -20
addiu $t8, $t8, 4
lw $t0, 0($t8)
sw $t0,4($fp)
addiu $t8, $t8, 4
lw $t0, 0($t8)
sw $t0,0($fp)

# body

# return
lw $t0, 0($fp) 
addiu $sp,$sp, -4 
sw $t0, 4($sp)
lw $t0, 4($fp) 
addiu $sp,$sp, -4 
sw $t0, 4($sp)
lw $t0, 4($sp)
addiu $sp, $sp, 4
xori $t1, $t0, 1
addiu $sp,$sp,-4
sw $t1, 4($sp)
lw $t0, 8($sp)
lw $t1, 4($sp)
addiu $sp, $sp, 8
and $t2, $t0, $t1
addiu $sp,$sp,-4
sw $t2, 4($sp)
lw $t0, 0($fp) 
addiu $sp,$sp, -4 
sw $t0, 4($sp)
lw $t0, 4($sp)
addiu $sp, $sp, 4
xori $t1, $t0, 1
addiu $sp,$sp,-4
sw $t1, 4($sp)
lw $t0, 4($fp) 
addiu $sp,$sp, -4 
sw $t0, 4($sp)
lw $t0, 8($sp)
lw $t1, 4($sp)
addiu $sp, $sp, 8
and $t2, $t0, $t1
addiu $sp,$sp,-4
sw $t2, 4($sp)
lw $t0, 8($sp)
lw $t1, 4($sp)
addiu $sp, $sp, 8
or $t2, $t0, $t1
addiu $sp,$sp,-4
sw $t2, 4($sp)
addiu $sp, $sp, 4
lw $v1, 0($sp)
move $sp, $fp
# restore $fp and %sp and $s0 - $s7
addiu $sp, $sp, 36
lw $s7, -32($sp)
lw $s6, -28($sp)
lw $s5, -24($sp)
lw $s4, -20($sp)
lw $s3, -16($sp)
lw $s2, -12($sp)
lw $s1, -8($sp)
lw $s0, -4($sp)
lw $fp, 0($sp)
jr $ra

# procedure  main
.globl main

main:
# save $s0-$s7 and $sp and $fp

addiu $sp, $sp, -36
sw $fp, 36($sp)
sw $s0, 32($sp)
sw $s1, 28($sp)
sw $s2, 24($sp)
sw $s3, 20($sp)
sw $s4, 16($sp)
sw $s5, 12($sp)
sw $s6, 8($sp)
sw $s7, 4($sp)
move $fp, $sp
addiu $sp, $sp, -24

# body

# read global integer a
li $v0, 5 
syscall
sw $v0, a


# write global integer a
lw $a0, a
li $v0, 1
syscall
# save $ra and $t0-$t9
addiu $sp, $sp, -44
sw $ra, 44($sp)
sw $t0, 40($sp)
sw $t1, 36($sp)
sw $t2, 32($sp)
sw $t3, 28($sp)
sw $t4, 24($sp)
sw $t5, 20($sp)
sw $t6, 16($sp)
sw $t7, 12($sp)
sw $t8, 8($sp)
sw $t9, 4($sp)

li $t0,12
addiu $sp,$sp, -4
sw $t0,4($sp)
li $t0,2
addiu $sp,$sp, -4
sw $t0,4($sp)
lw $t0, 8($sp)
lw $t1, 4($sp)
addiu $sp, $sp, 8
add $t2, $t0, $t1
addiu $sp,$sp,-4
sw $t2, 4($sp)
lw $t0, a
addiu $sp,$sp, -4 
sw $t0, 4($sp)
jal calculateSum

addiu $sp, $sp,8
# restore $ra and $t0 - $t9
addiu $sp, $sp, 44
lw $t9, -40($sp)
lw $t8, -36($sp)
lw $t7, -32($sp)
lw $t6, -28($sp)
lw $t5, -24($sp)
lw $t4, -20($sp)
lw $t3, -16($sp)
lw $t2, -12($sp)
lw $t1, -8($sp)
lw $t0, -4($sp)
lw $ra, 0($sp)
addiu $sp, $sp, -4
sw $v1, 4($sp)
addiu $sp, $sp, 4
lw $t0, 0($sp)
sw $t0,4($fp)

# write local integer sum
lw $a0,4($fp)
li $v0, 1
syscall
# save $ra and $t0-$t9
addiu $sp, $sp, -44
sw $ra, 44($sp)
sw $t0, 40($sp)
sw $t1, 36($sp)
sw $t2, 32($sp)
sw $t3, 28($sp)
sw $t4, 24($sp)
sw $t5, 20($sp)
sw $t6, 16($sp)
sw $t7, 12($sp)
sw $t8, 8($sp)
sw $t9, 4($sp)

lw $t0, 4($fp) 
addiu $sp,$sp, -4 
sw $t0, 4($sp)
lw $t0, w
addiu $sp,$sp, -4 
sw $t0, 4($sp)
jal calculateSum

addiu $sp, $sp,8
# restore $ra and $t0 - $t9
addiu $sp, $sp, 44
lw $t9, -40($sp)
lw $t8, -36($sp)
lw $t7, -32($sp)
lw $t6, -28($sp)
lw $t5, -24($sp)
lw $t4, -20($sp)
lw $t3, -16($sp)
lw $t2, -12($sp)
lw $t1, -8($sp)
lw $t0, -4($sp)
lw $ra, 0($sp)
addiu $sp, $sp, -4
sw $v1, 4($sp)
addiu $sp, $sp, 4
lw $t0, 0($sp)
sw $t0,4($fp)

# write local integer sum
lw $a0,4($fp)
li $v0, 1
syscall
# save $ra and $t0-$t9
addiu $sp, $sp, -44
sw $ra, 44($sp)
sw $t0, 40($sp)
sw $t1, 36($sp)
sw $t2, 32($sp)
sw $t3, 28($sp)
sw $t4, 24($sp)
sw $t5, 20($sp)
sw $t6, 16($sp)
sw $t7, 12($sp)
sw $t8, 8($sp)
sw $t9, 4($sp)

# save $ra and $t0-$t9
addiu $sp, $sp, -44
sw $ra, 44($sp)
sw $t0, 40($sp)
sw $t1, 36($sp)
sw $t2, 32($sp)
sw $t3, 28($sp)
sw $t4, 24($sp)
sw $t5, 20($sp)
sw $t6, 16($sp)
sw $t7, 12($sp)
sw $t8, 8($sp)
sw $t9, 4($sp)

li $t0,5
addiu $sp,$sp, -4
sw $t0,4($sp)
li $t0,10
addiu $sp,$sp, -4
sw $t0,4($sp)
jal calculateSum

addiu $sp, $sp,8
# restore $ra and $t0 - $t9
addiu $sp, $sp, 44
lw $t9, -40($sp)
lw $t8, -36($sp)
lw $t7, -32($sp)
lw $t6, -28($sp)
lw $t5, -24($sp)
lw $t4, -20($sp)
lw $t3, -16($sp)
lw $t2, -12($sp)
lw $t1, -8($sp)
lw $t0, -4($sp)
lw $ra, 0($sp)
addiu $sp, $sp, -4
sw $v1, 4($sp)
# save $ra and $t0-$t9
addiu $sp, $sp, -44
sw $ra, 44($sp)
sw $t0, 40($sp)
sw $t1, 36($sp)
sw $t2, 32($sp)
sw $t3, 28($sp)
sw $t4, 24($sp)
sw $t5, 20($sp)
sw $t6, 16($sp)
sw $t7, 12($sp)
sw $t8, 8($sp)
sw $t9, 4($sp)

lw $t0, 4($fp) 
addiu $sp,$sp, -4 
sw $t0, 4($sp)
li $t0,12
addiu $sp,$sp, -4
sw $t0,4($sp)
lw $t0, 8($sp)
lw $t1, 4($sp)
addiu $sp, $sp, 8
add $t2, $t0, $t1
addiu $sp,$sp,-4
sw $t2, 4($sp)
lw $t0, a
addiu $sp,$sp, -4 
sw $t0, 4($sp)
jal calculateDifferent

addiu $sp, $sp,8
# restore $ra and $t0 - $t9
addiu $sp, $sp, 44
lw $t9, -40($sp)
lw $t8, -36($sp)
lw $t7, -32($sp)
lw $t6, -28($sp)
lw $t5, -24($sp)
lw $t4, -20($sp)
lw $t3, -16($sp)
lw $t2, -12($sp)
lw $t1, -8($sp)
lw $t0, -4($sp)
lw $ra, 0($sp)
addiu $sp, $sp, -4
sw $v1, 4($sp)
jal calculateDifferent

addiu $sp, $sp,8
# restore $ra and $t0 - $t9
addiu $sp, $sp, 44
lw $t9, -40($sp)
lw $t8, -36($sp)
lw $t7, -32($sp)
lw $t6, -28($sp)
lw $t5, -24($sp)
lw $t4, -20($sp)
lw $t3, -16($sp)
lw $t2, -12($sp)
lw $t1, -8($sp)
lw $t0, -4($sp)
lw $ra, 0($sp)
addiu $sp, $sp, -4
sw $v1, 4($sp)
addiu $sp, $sp, 4
lw $t0, 0($sp)
sw $t0,8($fp)

# write local integer def
lw $a0,8($fp)
li $v0, 1
syscall

# for statment
li $t0,0
addiu $sp,$sp, -4
sw $t0,4($sp)
addiu $sp, $sp, 4
lw $t1, 0($sp)
sw $t1,12($fp)
for__label__4 :
li $t0,20
addiu $sp,$sp, -4
sw $t0,4($sp)
addiu $sp, $sp, 4
lw $t0, 0($sp)
ble $t1,$t0,statr__for__label__5
j end__for__label__6
statr__for__label__5 :

# write local integer limit
lw $a0,12($fp)
li $v0, 1
syscall
lw $t1,12($fp)
addiu $t1,$t1,1
sw $t1,12($fp)
j for__label__4
end__for__label__6 :

# read global integer a
li $v0, 5 
syscall
sw $v0, a

# save $ra and $t0-$t9
addiu $sp, $sp, -44
sw $ra, 44($sp)
sw $t0, 40($sp)
sw $t1, 36($sp)
sw $t2, 32($sp)
sw $t3, 28($sp)
sw $t4, 24($sp)
sw $t5, 20($sp)
sw $t6, 16($sp)
sw $t7, 12($sp)
sw $t8, 8($sp)
sw $t9, 4($sp)

lw $t0, a
addiu $sp,$sp, -4 
sw $t0, 4($sp)
jal absolute_value

addiu $sp, $sp,4
# restore $ra and $t0 - $t9
addiu $sp, $sp, 44
lw $t9, -40($sp)
lw $t8, -36($sp)
lw $t7, -32($sp)
lw $t6, -28($sp)
lw $t5, -24($sp)
lw $t4, -20($sp)
lw $t3, -16($sp)
lw $t2, -12($sp)
lw $t1, -8($sp)
lw $t0, -4($sp)
lw $ra, 0($sp)
addiu $sp, $sp, -4
sw $v1, 4($sp)
addiu $sp, $sp, 4
lw $t0, 0($sp)
la $t1, a
sw $t0, 0($t1)

# write global integer a
lw $a0, a
li $v0, 1
syscall

# read local integer n
li $v0, 5 
syscall
sw $v0, 0($fp)

# save $ra and $t0-$t9
addiu $sp, $sp, -44
sw $ra, 44($sp)
sw $t0, 40($sp)
sw $t1, 36($sp)
sw $t2, 32($sp)
sw $t3, 28($sp)
sw $t4, 24($sp)
sw $t5, 20($sp)
sw $t6, 16($sp)
sw $t7, 12($sp)
sw $t8, 8($sp)
sw $t9, 4($sp)

lw $t0, 0($fp) 
addiu $sp,$sp, -4 
sw $t0, 4($sp)
jal iterativeFactorial

addiu $sp, $sp,4
# restore $ra and $t0 - $t9
addiu $sp, $sp, 44
lw $t9, -40($sp)
lw $t8, -36($sp)
lw $t7, -32($sp)
lw $t6, -28($sp)
lw $t5, -24($sp)
lw $t4, -20($sp)
lw $t3, -16($sp)
lw $t2, -12($sp)
lw $t1, -8($sp)
lw $t0, -4($sp)
lw $ra, 0($sp)
addiu $sp, $sp, -4
sw $v1, 4($sp)
addiu $sp, $sp, 4
lw $t0, 0($sp)
la $t1, a
sw $t0, 0($t1)

# write global integer a
lw $a0, a
li $v0, 1
syscall

# read local boolean q
addiu $sp, $sp , -4
sw $ra, 4($sp)

jal read____boolean

sw $v0, 16($fp)
addiu $sp, $sp , 4
lw $ra, 0($sp)



# read global boolean r
addiu $sp, $sp , -4
sw $ra, 4($sp)

jal read____boolean

sw $v0, r
addiu $sp, $sp , 4
lw $ra, 0($sp)


# write global boolean r
lw $t0, r
addiu $sp, $sp , -4
sw $ra, 4($sp)

jal write_____boolean
addiu $sp, $sp , 4
lw $ra, 0($sp)


# write local boolean q
lw $t0,16($fp)
addiu $sp, $sp , -4
sw $ra, 4($sp)

jal write_____boolean
addiu $sp, $sp , 4
lw $ra, 0($sp)

# save $ra and $t0-$t9
addiu $sp, $sp, -44
sw $ra, 44($sp)
sw $t0, 40($sp)
sw $t1, 36($sp)
sw $t2, 32($sp)
sw $t3, 28($sp)
sw $t4, 24($sp)
sw $t5, 20($sp)
sw $t6, 16($sp)
sw $t7, 12($sp)
sw $t8, 8($sp)
sw $t9, 4($sp)

lw $t0, 16($fp) 
addiu $sp,$sp, -4 
sw $t0, 4($sp)
lw $t0, r
addiu $sp,$sp, -4 
sw $t0, 4($sp)
jal calculateXor

addiu $sp, $sp,8
# restore $ra and $t0 - $t9
addiu $sp, $sp, 44
lw $t9, -40($sp)
lw $t8, -36($sp)
lw $t7, -32($sp)
lw $t6, -28($sp)
lw $t5, -24($sp)
lw $t4, -20($sp)
lw $t3, -16($sp)
lw $t2, -12($sp)
lw $t1, -8($sp)
lw $t0, -4($sp)
lw $ra, 0($sp)
addiu $sp, $sp, -4
sw $v1, 4($sp)
addiu $sp, $sp, 4
lw $t0, 0($sp)
sw $t0,20($fp)

# write local boolean d
lw $t0,20($fp)
addiu $sp, $sp , -4
sw $ra, 4($sp)

jal write_____boolean
addiu $sp, $sp , 4
lw $ra, 0($sp)


# return
move $sp, $fp
# restore $fp and %sp and $s0 - $s7
addiu $sp, $sp, 36
lw $s7, -32($sp)
lw $s6, -28($sp)
lw $s5, -24($sp)
lw $s4, -20($sp)
lw $s3, -16($sp)
lw $s2, -12($sp)
lw $s1, -8($sp)
lw $s0, -4($sp)
lw $fp, 0($sp)
jr $ra

write_____boolean :
beq $t0, $zero, _F_a_l_s_e_
la $a0, __true__print
j p_r_i_n_t
_F_a_l_s_e_:
la $a0, __false__print
p_r_i_n_t:
li $v0, 4
syscall
jr $ra

read____boolean:
li $v0, 8
la $a0, __buffer__
li $a1, 10
syscall
la $t0, __true__
li $t3,1
compare____true:
lb $t1, ($t0)
lb $t2, ($a0)
beqz $t1, i_t_s_t_r_u_e
beqz $t2, nـoـt_tـrـuـe
beq $t1, $t2, continue___compare___true
j nـoـt_tـrـuـe
continue___compare___true:
addi $t3, $t3,1
addi $t0, $t0, 1
addi $a0, $a0, 1
j compare____true
nـoـt_tـrـuـe:
la $t0, __false__
compare____false:
lb $t1, ($t0)
lb $t2, ($a0)
beqz $t1, i_t_s_f_a_l_s_e
beqz $t2, _e_r_r_o_r_
beq $t1, $t2, continue___compare___false
j _e_r_r_o_r_
continue___compare___false:
addi $t0, $t0, 1
addi $a0, $a0, 1
j compare____false
i_t_s_t_r_u_e:
beq $t3,5, nـoـt_tـrـuـe
li $v0, 1
jr $ra
i_t_s_f_a_l_s_e:
li $v0,0
jr $ra

_e_r_r_o_r_:
la $a0 , __error__string__
li $v0, 4
syscall
li $v0, 10
syscall
