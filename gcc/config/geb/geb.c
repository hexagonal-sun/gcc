#include "config.h"
#include "system.h"
#include "coretypes.h"
#include "tm.h"
#include "rtl.h"
#include "regs.h"
#include "insn-config.h"
#include "insn-attr.h"
#include "recog.h"
#include "output.h"
#include "alias.h"
#include "tree.h"
#include "stringpool.h"
#include "attribs.h"
#include "varasm.h"
#include "stor-layout.h"
#include "calls.h"
#include "function.h"
#include "explow.h"
#include "memmodel.h"
#include "emit-rtl.h"
#include "reload.h"
#include "tm_p.h"
#include "target.h"
#include "target-def.h"
#include "basic-block.h"
#include "expr.h"
#include "optabs.h"
#include "bitmap.h"
#include "df.h"
#include "diagnostic.h"
#include "dbxout.h"
#include "builtins.h"
#include "predict.h"


void geb_cpu_cpp_builtins (cpp_reader *)
{
    return;
}

int geb_initial_elimination_offset(int a, int b)
{
    return a - b;
}

void
default_globalize_label (FILE *, const char *)
{
}

rtx
geb_function_value(const_tree ret_type, const_tree fn_decl)
{
    return gen_rtx_REG (TYPE_MODE(ret_type), 0);
}

bool
geb_do_move(machine_mode mode, rtx dest, rtx src)
{
    printf("Moving ");
    print_rtl(stdout, src);
    printf(" to ");
    print_rtl(stdout, dest);
    printf("\n");

    if (MEM_P (dest))
	src = force_reg(mode, src);

    printf("Moving ");
    print_rtl(stdout, src);
    printf(" to ");
    print_rtl(stdout, dest);
    printf("\n");

    if (CONST_INT_P (src) && REG_P (dest)) {
        emit_insn (gen_rtx_SET (dest, src));
        return true;
    }

    emit_insn(gen_rtx_SET (dest, src));

    printf("return true\n");

    return true;
}

static void
geb_emit_push(rtx reg_to_push)
{
    rtx x =  gen_rtx_PRE_DEC (Pmode, stack_pointer_rtx);
    x = gen_rtx_MEM (Pmode, x);
    emit_move_insn (x, reg_to_push);
}

void
geb_expand_prologue(void)
{
    geb_emit_push (frame_pointer_rtx);
    emit_insn(gen_rtx_SET (frame_pointer_rtx, stack_pointer_rtx));
}

static bool
geb_legitimate_address_p (machine_mode mode, rtx x, bool strict_p)
{
    return true;
}
#undef TARGET_LEGITIMATE_ADDRESS_P
#define TARGET_LEGITIMATE_ADDRESS_P	geb_legitimate_address_p

static void
geb_print_operand (FILE *file, rtx op, int letter)
{
  machine_mode mode = GET_MODE (op);
  enum rtx_code code = GET_CODE (op);

  printf("Printing operand: ");
  print_rtl(stdout, op);
  printf("\n");

  switch (code)
      {
      case GT:
	  fputs("gt", file);
	  break;
      case LT:
	  fputs("lt", file);
	  break;
      case LE:
	  fputs("le", file);
	  break;
      case NE:
	  fputs("ne", file);
	  break;
      case REG:
	  fputs(reg_names[REGNO (op)], file);
	  break;

      case MEM:
          output_address (mode, XEXP (op, 0));
	  break;
      default:
	  output_addr_const(file, op);
	  break;
      }

  printf("Asking for print operand of: ");
  print_rtl(stdout, op);
  printf("\n");
}

static rtx
geb_emit_int_compare(rtx op0, rtx op1)
{
    rtx x = gen_rtx_COMPARE (SImode, op0, op1);
    rtx dest = gen_reg_rtx (SImode);
    emit_insn( gen_rtx_SET (dest, x));

    return dest;
}

void
geb_do_conditional_jump (rtx target_label, rtx cmp,
			 rtx op0, rtx op1)
{
    rtx ccreg = geb_emit_int_compare(op0, op1);

    emit_jump_insn (gen_condjump (cmp, target_label));
}


#undef TARGET_PRINT_OPERAND
#define TARGET_PRINT_OPERAND geb_print_operand

static void
geb_print_operand_address (FILE *file, machine_mode mode ATTRIBUTE_UNUSED,
                           rtx x)
{
    switch(GET_CODE (x))
    {
    case REG:
        fprintf(file, "[%s]", reg_names[REGNO (x)]);
        break;
    default:
        gcc_unreachable();
    }
}
#undef TARGET_PRINT_OPERAND_ADDRESS
#define TARGET_PRINT_OPERAND_ADDRESS geb_print_operand_address

static void
geb_function_arg_advance (cumulative_args_t ca_v,
			  machine_mode mode ATTRIBUTE_UNUSED,
			  const_tree type ATTRIBUTE_UNUSED,
			  bool named ATTRIBUTE_UNUSED)
{
  CUMULATIVE_ARGS *ca = get_cumulative_args (ca_v);
}
#undef TARGET_FUNCTION_ARG_ADVANCE
#define TARGET_FUNCTION_ARG_ADVANCE geb_function_arg_advance

static rtx
geb_function_arg (cumulative_args_t ca_v,
		  machine_mode mode ATTRIBUTE_UNUSED,
		  const_tree type ATTRIBUTE_UNUSED,
		  bool named ATTRIBUTE_UNUSED)
{
    return 0;
}
#undef TARGET_FUNCTION_ARG
#define TARGET_FUNCTION_ARG geb_function_arg

struct gcc_target targetm = TARGET_INITIALIZER;
