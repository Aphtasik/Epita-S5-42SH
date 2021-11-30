#ifndef PARSER_H
#define PARSER_H

#include "../ast/ast.h"
#include "../lexer/lexer.h"

#include <err.h>
#include <errno.h>
#include <stdlib.h>
#include <stdio.h>

enum parser_status
{
    PARSER_OK,
    PARSER_NOMATCH,
    PARSER_MISSING_OPERAND,
    PARSER_ERROR,
};


#endif
