#ifndef PARSE_H
#define PARSE_H

#include "../lexer/lexer.h"
#include "../ast/ast.h"

#include <err.h>

enum parser_status
{
    PARSER_OK,
    PARSER_UNEXPECTED_TOKEN,
};

struct ast **parse_compound_list(struct lexer *lexer);
enum parser_status parse_rule_if(struct lexer *lexer, struct ast **res);

#endif
