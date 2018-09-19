(include "predicates.md")

(define_insn "addsi3"
  [(set (match_operand:SI          0 "register_operand" "=r,r")
	(plus:SI (match_operand:SI 1 "register_operand" " r,r")
		 (match_operand:SI 2 "arith_operand"    " r,r")))]
  "TARGET_ALL"
  "add\t%0,%1,%2"
  [])

(define_insn "*push"
  [(set (match_operand:SI 0 "push_operand" "")
	(match_operand:SI 1 "register_operand" ""))]
  ""
  "push\t%1")

(define_insn "*movsi_mem"
    [(set (mem:SI (match_operand:SI 0 "register_operand" "r"))
          (match_operand:SI 1 "register_operand" "r"))]
    ""
    "str\t%0,%1")

(define_insn "*movsi_imm"
    [(set (match_operand:SI 0 "register_operand" "=r,r")
          (match_operand:SI 1 "immediate_operand" ""))]
    ""
    "mov\t%0,#%1")

(define_expand "movsi"
  [(set (match_operand:SI 0 "nonimmediate_operand" "")
        (match_operand:SI 1 "general_operand" ""))]
  ""
{
    if (geb_do_move(SImode, operands[0], operands[1]))
        DONE;
})

(define_insn "nop"
  [(const_int 0)]
  ""
  "nop"
  [])
