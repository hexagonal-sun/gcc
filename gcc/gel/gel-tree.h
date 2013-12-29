/* Language-dependent trees for GEL. */

#ifndef GCC_GEL_TREE_H
#define GCC_GEL_TREE_H

struct GTY(()) lang_identifier
{
    struct tree_identifier base;
    tree decl;
};

#endif /* GCC_GEL_TREE_H */
