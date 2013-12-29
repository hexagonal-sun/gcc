/* gel backend interface */

#include "config.h"
#include "system.h"
#include "coretypes.h"
#include "flag-types.h"
#include "flags.h"
#include "tm.h"
#include "tree-core.h"
#include "tree.h"
#include "stringpool.h"
#include "stor-layout.h"
#include "target.h"
#include "langhooks.h"
#include "langhooks-def.h"
#include "debug.h"
#include "function.h"
#include "gel-tree.h"
#include "tree-inline.h"
#include "basic-block.h"
#include "tree-ssa-alias.h"
#include "internal-fn.h"
#include "gimple-expr.h"
#include "is-a.h"
#include "gimple.h"
#include "diagnostic-core.h"
#include "toplev.h"
#include "opts.h"

#include "gel-parser.h"
#include "gel-tree.h"

extern FILE *yyin;

enum gel_tree_node_structure_enum {
				   TS_GEL_GENERIC
};


static enum gel_tree_node_structure_enum
gel_tree_node_structure (union lang_tree_node *t ATTRIBUTE_UNUSED)
{
    return TS_GEL_GENERIC;
}

struct GTY((variable_size)) lang_decl
{
    int dummy;  /* Added because ggc does not like empty structs.  */
};

struct GTY((variable_size)) lang_type
{
    int dummy;  /* Added because ggc does not like empty structs.  */
};

struct GTY(()) language_function
{
    int dummy;  /* Added because ggc does not like empty structs.  */
};

union GTY((desc ("gel_tree_node_structure (&%h)"),
	   chain_next ("CODE_CONTAINS_STRUCT (TREE_CODE (&%h.generic), TS_COMMON) ? ((union lang_tree_node *) TREE_CHAIN (&%h.generic)) : NULL")))
lang_tree_node
{
    union tree_node GTY ((tag ("TS_GEL_GENERIC"),
			  desc ("tree_node_structure (&%h)"))) generic;
};

static void
gel_do_parse(FILE *f)
{
    yyin = f;
    yyparse();
}

static void
gel_parse_file (void)
{
    FILE *f;

    if (num_in_fnames != 0)
	f = fopen(*in_fnames, "r");
    else
	f = stdin;

    gel_do_parse (f);
}


/* Initialize everything.  */

static bool
gel_init (void)
{
    /* Create the basic integer types.  */
    build_common_tree_nodes (false);
    tree in = integer_type_node;
    return true;
}


static void
gel_finish (void)
{
    return;
}

/* No scoping as of yet. */

bool
global_bindings_p (void)
{
    return true;
}

/* We don't support decls yet. */

tree
pushdecl(tree decl)
{
    return decl;
}


tree
gel_get_type_for_size(unsigned int bits ATTRIBUTE_UNUSED, int unsignedp ATTRIBUTE_UNUSED)
{
    return unsigned_intSI_type_node;
}

tree
gel_get_type_for_mode(enum machine_mode mode, int unsignedp ATTRIBUTE_UNUSED)
{
    if ( SCALAR_INT_MODE_P (mode)) {
        return gel_get_type_for_size(0, 0);
    } else
	return NULL_TREE;
}

/* Table of machine-independent attributes supported in GIMPLE.  */
const struct attribute_spec gel_attribute_table[] =
    {
     { NULL,                     0, 0, false, false, false, NULL, false }
    };


/* Give the specifications for the format attributes, used by C and
   all descendants.  */
const struct attribute_spec gel_format_attribute_table[] =
    {
     { NULL,                     0, 0, false, false, false, NULL, false }
    };



#undef LANG_HOOKS_NAME
#undef LANG_HOOKS_IDENTIFIER_SIZE
#undef LANG_HOOKS_INIT
#undef LANG_HOOKS_FINISH
#undef LANG_HOOKS_PARSE_FILE
#undef LANG_HOOKS_GETDECLS
#undef LANG_HOOKS_TYPE_FOR_MODE
#undef LANG_HOOKS_TYPE_FOR_SIZE

/* Define lang hooks.  */
#define LANG_HOOKS_NAME                 "gel"
#define LANG_HOOKS_IDENTIFIER_SIZE       sizeof(struct lang_identifier)
#define LANG_HOOKS_INIT                 gel_init
#define LANG_HOOKS_FINISH               gel_finish
#define LANG_HOOKS_PARSE_FILE           gel_parse_file
#define LANG_HOOKS_GETDECLS             hook_tree_void_null
#define LANG_HOOKS_TYPE_FOR_MODE        gel_get_type_for_mode
#define LANG_HOOKS_TYPE_FOR_SIZE        gel_get_type_for_size

/* Attribute hooks.  */
#undef LANG_HOOKS_COMMON_ATTRIBUTE_TABLE
#define LANG_HOOKS_COMMON_ATTRIBUTE_TABLE gel_attribute_table
#undef LANG_HOOKS_FORMAT_ATTRIBUTE_TABLE
#define LANG_HOOKS_FORMAT_ATTRIBUTE_TABLE gel_format_attribute_table


struct lang_hooks lang_hooks = LANG_HOOKS_INITIALIZER;

/* Language hooks that are not part of lang_hooks.  */

tree
convert (tree type ATTRIBUTE_UNUSED, tree expr ATTRIBUTE_UNUSED)
{
    gcc_unreachable ();
}

/* Tree walking support.  */

#include "ggc.h"
#include "gtype-gel.h"
#include "gt-gel-gel-lang.h"
