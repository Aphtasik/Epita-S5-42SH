#ifndef PARSE_H
#define PARSE_H

#include "../lexer/lexer.h"
#include "../ast/ast.h"

#include <err.h>

enum parser_status
{
    PARSER_OK,
    PARSER_UNEXPECTED_TOKEN,
    PARSER_NO_COMMAND,
    PARSER_MISSING_TOKEN
};

enum parser_status parse_compound_list(struct lexer *lexer, struct ast ***res, size_t *len);
enum parser_status parse_rule_if(struct lexer *lexer, struct ast **res);
enum parser_status parse_cmd(struct lexer *lexer, struct ast **res);
enum parser_status handle_parse_error(enum parser_status status, struct ast **res);
void skip_newline(struct lexer *lexer);

#endif
