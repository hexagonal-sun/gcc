(define_predicate "arith_operand"
  (ior (match_code "const_int")
       (match_operand 0 "register_operand")))