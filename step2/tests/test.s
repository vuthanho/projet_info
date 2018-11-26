# TEST_RETURN_CODE = PASS
# allons au ru
.set noreorder
.text
    ADDI $t1,$zero,8
boucle:
    BEQ $t0 , $t1 , byebye
    addi $t1 , $t1 , -1
    J boucle
byebye:
    JAL viteviteauru

.data
lunchtime:
    .word 12
    .word menu
    .asciiz "ils disent : \"au ru!\""
.bss
   .space 24
   menu : .space 5
