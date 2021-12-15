#ifndef PARSE_H
#define PARSE_H

#include <err.h>

#include "../ast/ast.h"
#include "../lexer/lexer.h"

enum parser_status
{
    PARSER_OK,
    PARSER_UNEXPECTED_TOKEN,
    PARSER_NO_COMMAND,
    PARSER_MISSING_TOKEN
};

// parse_compound_list.c
enum parser_status parse_compound_list(struct lexer *lexer, struct ast ***res,
                                       size_t *len);
void skip_newline(struct lexer *lexer);

// parse_rule_if.c
enum parser_status parse_rule_if(struct lexer *lexer, struct ast **res);

// parse_rule_while
enum parser_status parse_rule_while(struct lexer *lexer, struct ast **res);

// parse_rule_until
enum parser_status parse_rule_until(struct lexer *lexer, struct ast **res);

// parse_command.c
enum parser_status parse_fill_cmd(struct lexer *lexer, struct ast_cmd *cmd);
enum parser_status parse_cmd(struct lexer *lexer, struct ast **res);

// parse.c
enum parser_status handle_parse_error(enum parser_status status,
                                      struct ast **res);
struct ast *parse(struct lexer *lexer);

#endif
