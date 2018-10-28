(include "predicates.md")

(define_insn "addsi3"
  [(set (match_operand:SI          0 "register_operand" "=r,r")
	(plus:SI (match_operand:SI 1 "register_operand" " r,r")
		 (match_operand:SI 2 "arith_operand"    " r,r")))]
  "TARGET_ALL"
  "add\t%0,%1,%2"
  [])
;;  Function Prologue and Epilogue
(define_expand "prologue"
  [(const_int 0)]
  ""
  {
    geb_expand_prologue ();
    DONE;
  }
)

(define_insn "*push"
  [(set (match_operand:SI 0 "push_operand" "")
	(match_operand:SI 1 "register_operand" ""))]
  ""
  "push\t%1")

(define_expand "call_value"
  [(parallel [(set (match_operand 0 "")
		   (call (match_operand 1 "")
			 (match_operand 2 "")))
	      (use (match_operand 3 ""))])]		;; next_arg_reg
  ""
{
  geb_do_call_value(operands[0], operands[1], operands[2],
                    operands[3]);
  DONE;
})

(define_insn "call_value_internal"
  [(set (match_operand 0 "" "")
	(call (mem:SI (match_operand 1 "call_insn_operand" ""))
	      (match_operand 2 "" "")))]
  ""
  "call\t%1")

(define_insn "jump"
  [(set (pc)
	(label_ref (match_operand 0 "" "")))]
  ""
  "jmp\t%l0")

(define_insn "*cbranch_internal"
  [(set (pc)
	(if_then_else
	 (match_operator 1 "order_operator"
			 [(match_operand:SI 2 "register_operand" "r")
			  (match_operand:SI 3 "const_int_operand" "r")])
	 (label_ref (match_operand 0 "" ""))
	 (pc)))]
  ""
  "b%C1\t%2,%3,%0")

(define_insn "cmpimmsi"
    [(set (match_operand:SI 0 "register_operand" "=r")
          (compare:SI
           (match_operand:SI 1 "register_operand" "r")
           (match_operand:SI 2 "const_int_operand" "")))]
    ""
    "cmp\t%1,#%2")

(define_expand "condjump"
  [(set (pc)
	(if_then_else (match_operand 0)
		      (label_ref (match_operand 1))
		      (pc)))])

(define_expand "cbranchsi4"
  [(set (pc) (if_then_else
	      (match_operator 0 "comparison_operator"
	       [(match_operand:SI 1 "register_operand" "")
	        (match_operand:SI 2 "nonmemory_operand" "")])
	      (label_ref (match_operand 3 "" ""))
	      (pc)))]
  ""
  {
      geb_do_conditional_jump(operands[3], operands[0],
                              operands[1], operands[2]);
      DONE;
  }
)

(define_insn "*strsi"
    [(set (mem:SI (match_operand:SI 0 "register_operand" "r"))
          (match_operand:SI 1 "register_operand" "r"))]
    ""
    "str\t%0,%1")

(define_insn "*ldrsi"
    [(set (match_operand:SI 1 "register_operand" "=r")
          (mem:SI (match_operand:SI 0 "register_operand" "r")))]
    ""
    "ldr\t%0,%1")

(define_insn "*movsi_imm"
    [(set (match_operand:SI 0 "register_operand" "=r,r")
          (match_operand:SI 1 "immediate_operand" ""))]
    ""
    "mov\t%0,#%1")

(define_insn "*movsi_reg"
    [(set (match_operand:SI 0 "register_operand" "=r,r")
          (match_operand:SI 1 "register_operand" ""))]
    ""
    "mov\t%0,%1")

(define_insn "*fp_si_store"
    [(set (mem:SI (plus:SI (match_operand:SI 0 "fp_reg")
                  (match_operand:SI 1 "const_int_operand")))
          (match_operand:SI 2 "register_operand" "r"))]
    ""
    "fpstr\t%1,%2")


(define_insn "*fp_si_load"
    [(set (match_operand:SI 0 "register_operand" "=r")
          (mem:SI (plus:SI (match_operand:SI 1 "fp_reg")
                  (match_operand:SI 2 "const_int_operand"))))]
    ""
    "fpldr\t%0,%2")

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
