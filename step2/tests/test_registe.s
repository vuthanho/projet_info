# TEST_RETURN_CODE = PASS
# allons au ru
.set noreorder
.text
    Lw $t0 , lunchtime
    LW $6, -200($7)
    ADDI $t1,$azed5*, 0x001G
boucle:
    BEQ $t0 , $t1 , byebye
    NOP
    addi $t1 , $t1 , 1
    J boucle
    NOP
byebye:
    JAL viteviteauru

.data
lunchtime:
    .word 12
    .word menu
    .asciiz "ils disent : \"au ru!\""
.bss
menu:
    .space 24
