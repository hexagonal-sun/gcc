%{
#include "config.h"
#include "system.h"
#include "coretypes.h"
#include "tree.h"
#include "varasm.h"
#include "errors.h"
#include "toplev.h"
#include "options.h"
#include "cgraph.h"
#include "function.h"
#include "tree-iterator.h"
#include "stringpool.h"
#include "gel-tree.h"
int yylex(void);

#define GEL_ID_DECL(ID) \
    (((struct lang_identifier *) IDENTIFIER_NODE_CHECK(ID))->decl)

static void
yyerror(const char *c)
{
    error(c);
}

 static void gel_enter_into_fn (char *name, tree return_type)
{
    tree param_list = NULL_TREE;
    tree param_type_list;
    tree fntype;
    tree fndecl;

    fntype = build_function_type(return_type, NULL_TREE);

    fndecl = build_fn_decl(name, fntype);
    tree res_decl = build_decl(input_location, RESULT_DECL,
			       NULL_TREE, integer_type_node);

    tree myblock = make_node(BLOCK);
    BLOCK_VARS (myblock) = res_decl;
    BLOCK_ABSTRACT_ORIGIN (myblock) = fndecl;

    DECL_INITIAL  (fndecl) = myblock;
    DECL_EXTERNAL (fndecl) = 0;
    TREE_PUBLIC   (fndecl) = 1;
    TREE_STATIC   (fndecl) = 1;
    DECL_SAVED_TREE(fndecl) = alloc_stmt_list();
    DECL_RESULT (fndecl) = res_decl;

    DECL_CONTEXT (DECL_RESULT (fndecl)) = fndecl;

    allocate_struct_function(fndecl, false);

    current_function_decl = fndecl;
}

void gel_finish_fn()
{
    tree fndecl = current_function_decl;
    cgraph_node::finalize_function (fndecl, false);
}

tree
gel_add_return(tree exp)
{
    tree fndecl = current_function_decl;
    tree mod_decl = build2 (MODIFY_EXPR, TREE_TYPE(exp),
			    DECL_RESULT(fndecl), exp);
    tree return_exp = build1 (RETURN_EXPR, void_type_node, mod_decl);
    return return_exp;
}

tree
gel_add_stmt (tree new_stmt)
{
    tree fndecl = current_function_decl;
    tree_stmt_iterator i;
    i = tsi_start(DECL_SAVED_TREE(fndecl));
    tsi_link_before(&i, new_stmt, TSI_CONTINUE_LINKING);
    return new_stmt;
}

tree
gel_build_decl(tree type, char *name)
{
    tree decl;
    tree decl_expr;
    tree id = get_identifier(name);
    decl = build_decl(input_location, VAR_DECL, id, type);
    decl_expr = build1(DECL_EXPR, void_type_node, decl);
    GEL_ID_DECL(id) = decl;
    return decl_expr;
}

tree
gel_build_fn_call(char *name, vec<tree, va_gc> *argument_list)
{
  tree fncall = build_call_expr_loc_vec(
    UNKNOWN_LOCATION,
    build_fn_decl(name,
                  build_function_type(void_type_node, NULL_TREE)),
    argument_list);
  return fncall;
}

tree
gel_build_fn_call_1(char *name, tree arg0_exp)
{
  tree fncall =  build_call_expr(build_fn_decl(name,
                                               build_function_type_list(
                                                 integer_type_node,
                                                 TREE_TYPE(arg0_exp),
                                                 NULL_TREE)),
                                 1, arg0_exp);
  return fncall;
}

tree
build_string_literal(char *string)
{
    tree string_node = build_string(strlen(string) + 1, string);
    TREE_TYPE(string_node) = integer_type_node;
    return build1(ADDR_EXPR, build_pointer_type(TREE_TYPE(string_node)), string_node);
}

%}
%union{
    tree exp;       //Tree node developed by us.
    int ival;       //Integer value for constants.
    char *name;     //Name of function or variables.
    vec<tree, va_gc> *exp_list;
    char *string;
}

%type <exp> exp stmt stmts id_exp
%type <exp_list> exp_list
%type <exp> type
%token <ival> NUM
%token <name> NAME
%token <string> STRING_LITERAL

%token FUNCTION RETURNS RETURN INTEGER FLOAT

%left '-' '+'
%left '*' '/'
%error-verbose

%%
accept: TLS
| TLS accept

 /* Top Level statements */
TLS: fndef fnbody

type: INTEGER           {$$ = integer_type_node;}
| FLOAT                 {$$ = float_type_node;}

fndef: FUNCTION NAME RETURNS type '(' ')'     {gel_enter_into_fn($2, $4);}
fnbody: '{' stmts '}'   {gel_finish_fn();}

stmts: stmt             {gel_add_stmt($1);}
| stmt stmts            {gel_add_stmt($1);}

stmt: exp ';'
| RETURN exp ';'        {$$ = gel_add_return($2);}
| type NAME ';'         {$$ = gel_build_decl($1, $2);}
| id_exp '=' exp ';'    {$$ = build2 (MODIFY_EXPR, integer_type_node, $1, $3);}

id_exp: NAME            {$$ = GEL_ID_DECL(get_identifier($1));};

exp: id_exp
| NAME '(' exp_list ')' {$$ = gel_build_fn_call($1, $3);}
| STRING_LITERAL        {$$ = build_string_literal(strlen($1) + 1, $1);}
| NUM                   {$$ = build_int_cst (integer_type_node, $1);}
| exp '+' exp           {$$ = build2 (PLUS_EXPR, integer_type_node, $1, $3);}
| exp '-' exp           {$$ = build2 (MINUS_EXPR, integer_type_node, $1, $3);}
| exp '*' exp           {$$ = build2 (MULT_EXPR, integer_type_node, $1, $3);}
| exp '/' exp           {$$ = build2 (TRUNC_DIV_EXPR, integer_type_node, $1, $3);}
| '(' exp ')'           {$$ = $2;}
;

exp_list:
{
  $$ = NULL;
}
| exp
{
  vec<tree, va_gc> *new_vec = NULL;
  vec_alloc(new_vec, 4);
  vec_safe_push(new_vec, $1);
  $$ = new_vec;
}
| exp_list ',' exp
{
  vec_safe_push($1, $3);
}
