.section    .text
# .global __udivsi3
# .align  2
# __udivsi3:
#   li a0, 4
#   jr ra


# .global __mulsi3
# .align 2
# __mulsi3:
#   lui a0, 0x3d09
#   jr ra

# .global __umodsi3
# .align 1
# __umodsi3:
#   lui a0, 0x3d09
#   jr ra


# uint32_t my_umodsi3(uint32_t dividend, uint32_t divisor) {
#
#   while (dividend >= divisor) {
#     volatile uint32_t tmp = divisor;
#     volatile uint32_t quotient = 1;
#     while (tmp <= dividend) {
#       tmp <<= 1;
#       quotient <<= 1;
#     }
#     dividend -= tmp >> 1;
#   }
#   return dividend;
# }
# .align 2
# .global __umodsi3
# // uint32_t __umodsi3(uint32_t a, uint32_t b)
# // return a%b
#   .type __umodsi3, @function
# __umodsi3:
#   bne a1, zero, 4f
#   mv a0, a1
#   ret
# 4:
#   bltu a0, a1, 0f
#   mv t0, a1 #tmp = divisor
#   li t1, 1 #qutient = 1
# 3:
#    sll t0, t0, 1 # tmp <<= 1
#    sll t1, t1, 1 # quotient << 1
# 2:
#   bltu t0, a0, 3b
#   srl t0, t0, 1
#   sub a0, a0, t0
#   j 4b
#
# 0:
#   ret
