/* Definition of GEB target for GNU compiler.
   Copyright (C) 2011-2018 Free Software Foundation, Inc.
   Contributed by Matthew Leach (matthew@mattleach.net).
   Based on riscv target for GNU compiler.

This file is part of GCC.

GCC is free software; you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation; either version 3, or (at your option)
any later version.

GCC is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with GCC; see the file COPYING3.  If not see
<http://www.gnu.org/licenses/>.  */

/*
  R0 - R5 GP
  R6 FP
  R7 SP
  R8 PC

 */
#ifndef GCC_GEB_H
#define GCC_GEB_H

#define FIRST_PSEUDO_REGISTER 9

enum reg_class
    {
     NO_REGS,
     GENERAL_REGS,
     FRAME_REGS,
     ALL_REGS,
     LIM_REG_CLASSES
    };


#define REG_CLASS_NAMES				\
    {							\
	"NO_REGS",						\
	"GENERAL_REGS",					\
	"FRAME_REGS",						\
	"ALL_REGS"						\
    }

#define REGISTER_NAMES				\
    {						\
	"R0",					\
	    "R1",				\
	    "R2",				\
	    "R3",				\
	    "R4",				\
	    "R6",				\
	    "R7",				\
	    "SP",				\
	    "PC"				\
	    }

#define N_REG_CLASSES (int) LIM_REG_CLASSES

#define UNITS_PER_WORD 4
#define MOVE_MAX BITS_PER_UNIT * UNITS_PER_WORD

typedef struct {
    int num_gprs;
} CUMULATIVE_ARGS;

#define INIT_CUMULATIVE_ARGS(CUM, FNTYPE, LIBNAME, INDIRECT, N_NAMED_ARGS) \
  memset (&(CUM), 0, sizeof (CUM))

#define TARGET_HARD_FLOAT 0

#define TARGET_ALL 1

#define STRICT_ALIGNMENT 0
#define BITS_BIG_ENDIAN 0
#define BYTES_BIG_ENDIAN 0
#define WORDS_BIG_ENDIAN 0
#define FUNCTION_BOUNDARY BITS_PER_WORD
#define PARM_BOUNDARY BITS_PER_WORD
#define STACK_BOUNDARY BITS_PER_WORD
#define PREFERRED_STACK_BOUNDARY STACK_BOUNDARY
#define MAX_REGS_PER_ADDRESS 1
#define Pmode word_mode
#define FUNCTION_MODE word_mode
#define BIGGEST_ALIGNMENT 16
#define TRAMPOLINE_SIZE 32
#define STACK_POINTER_REGNUM 7
#define FRAME_POINTER_REGNUM 6
#define BASE_REG_CLASS GENERAL_REGS
#define ARG_POINTER_REGNUM FRAME_POINTER_REGNUM
#define FUNCTION_ARG_REGNO_P(N) 0
#define REGNO_OK_FOR_BASE_P(regno) ((regno) < FRAME_POINTER_REGNUM)

#define FLOAT_TYPE_SIZE 32
#define DOUBLE_TYPE_SIZE 64
#define LONG_DOUBLE_TYPE_SIZE 64

#define SLOW_BYTE_ACCESS 0

#define LOCAL_LABEL_PREFIX	"."
#define USER_LABEL_PREFIX	""


#define ASM_GENERATE_INTERNAL_LABEL(LABEL,PREFIX,NUM)			\
  sprintf ((LABEL), "*%s%s%ld", (LOCAL_LABEL_PREFIX), (PREFIX), (long)(NUM))

/* This is how to output an element of a case-vector that is absolute.  */

#define ASM_OUTPUT_ADDR_VEC_ELT(STREAM, VALUE)				\
  fprintf (STREAM, "\t.word\t%sL%d\n", LOCAL_LABEL_PREFIX, VALUE)

/* This is how to output an element of a PIC case-vector. */

#define ASM_OUTPUT_ADDR_DIFF_ELT(STREAM, BODY, VALUE, REL)		\
  fprintf (STREAM, "\t.word\t%sL%d-%sL%d\n",				\
	   LOCAL_LABEL_PREFIX, VALUE, LOCAL_LABEL_PREFIX, REL)

/* The argument pointer always points to the first argument.  */
#define FIRST_PARM_OFFSET(FNDECL) 0

#define CASE_VECTOR_MODE SImode

/* This is how to output an assembler line
   that says to advance the location counter
   to a multiple of 2**LOG bytes.  */

#define ASM_OUTPUT_ALIGN(STREAM,LOG)					\
  fprintf (STREAM, "\t.align\t%d\n", (LOG))


static const enum reg_class regno_to_class[] = {
	GENERAL_REGS,    // R0
	GENERAL_REGS,    // R1
	GENERAL_REGS,    // R2
	GENERAL_REGS,    // R3
	GENERAL_REGS,    // R4
	GENERAL_REGS,    // R5
	FRAME_REGS,      // R6  FP
	FRAME_REGS,      // SP
	FRAME_REGS,      // PC
};


#ifndef ASM_APP_ON
#define ASM_APP_ON " #APP\n"
#endif

/* Output to assembler file text saying following lines
   no longer contain unusual constructs.  */

#ifndef ASM_APP_OFF
#define ASM_APP_OFF " #NO_APP\n"
#endif


/* All the work done in PROFILE_HOOK, but still required.  */
#define FUNCTION_PROFILER(STREAM, LABELNO) do { } while (0)
#define INDEX_REG_CLASS NO_REGS
#define REGNO_OK_FOR_INDEX_P(REGNO) 0

#define FIXED_REGISTERS				\
    {							\
	0, 0, 0, 0, 0, 0, 1, 1, 1			\
    }

#define CALL_USED_REGISTERS			\
    {						\
	1, 1, 1, 1, 1, 1, 1, 1, 1		\
    }
#define REG_CLASS_CONTENTS			\
{						\
  { 0x00 },	/* NO_REGS */			\
  { 0x7f },	/* GENERAL_REGS_REGS */	\
  { 0xc0 },	/* FRAME_REGS */		\
  { 0xff }	/* ALL_REG */			\
}


extern int  geb_initial_elimination_offset (int, int);
#define INITIAL_ELIMINATION_OFFSET(FROM, TO, OFFSET) \
  (OFFSET) = geb_initial_elimination_offset (FROM, TO)



#define REGNO_REG_CLASS(REGNO) regno_to_class[ (REGNO) ]

#define ELIMINABLE_REGS							\
{{ ARG_POINTER_REGNUM,   STACK_POINTER_REGNUM},				\
 { FRAME_POINTER_REGNUM, STACK_POINTER_REGNUM}}


void geb_cpu_cpp_builtins (cpp_reader *);

#define TARGET_CPU_CPP_BUILTINS() geb_cpu_cpp_builtins(pfile)


#define ASM_OUTPUT_COMMON(FILE, NAME, SIZE, ROUNDED)	\
( fputs ("\t.comm ", (FILE)),				\
  assemble_name ((FILE), (NAME)),			\
  fprintf ((FILE), ",%lu\n", (unsigned long)(SIZE)))


#define ASM_OUTPUT_LOCAL(FILE, NAME, SIZE, ROUNDED)	\
( fputs ("\t.local ", (FILE)),				\
  assemble_name ((FILE), (NAME)),			\
  fprintf ((FILE), ",%lu\n", (unsigned long)(SIZE)))

#define ASM_OUTPUT_SKIP(STREAM, NBYTES) \
  fprintf (STREAM, "\t.zero\t%u\n", (int)(NBYTES))

rtx
geb_function_value(const_tree ret_type, const_tree fn_decl);

#define FUNCTION_VALUE(RET_TYPE, FN_DECL)	\
    geb_function_value (RET_TYPE, FN_DECL)

#define DEFAULT_SIGNED_CHAR 1

bool
geb_do_move(machine_mode mode, rtx dest, rtx src);

#endif /* GCC_GEB_H */
