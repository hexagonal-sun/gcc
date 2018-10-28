(define_predicate "arith_operand"
  (ior (match_code "const_int")
       (match_operand 0 "register_operand")))

(define_predicate "fp_reg"
  (and (match_code "reg")
       (match_test "REGNO (op) == FRAME_POINTER_REGNUM || REGNO (op) == VIRTUAL_STACK_VARS_REGNUM")))

(define_predicate "order_operator"
  (match_code "eq,ne,lt,ltu,le,leu,ge,geu,gt,gtu"))

(define_predicate "absolute_symbolic_operand"
  (match_code "const,symbol_ref,label_ref"))

(define_predicate "call_insn_operand"
  (ior (match_operand 0 "absolute_symbolic_operand")
       (match_operand 0 "register_operand")))
