#include "parse.h"

// skip every newline and semicolons found
void skip_newline(struct lexer *lexer)
{
    struct token *tok = lexer_peek(lexer);
    while (tok->type == TOKEN_BACKN || tok->type == TOKEN_SEMICOL)
    {
        token_free(lexer_pop(lexer));
        tok = lexer_peek(lexer);
    }
}

// fill the pointer to array res with all commands node found
// and discard useless tokens
enum parser_status parse_compound_list(struct lexer *lexer, struct ast ***res,
                                       size_t *len)
{
    skip_newline(lexer);
    struct token *tok = lexer_peek(lexer);
    if (tok->type == TOKEN_EOF)
        return PARSER_MISSING_TOKEN;

    enum parser_status p_stat;
    while (tok->type == TOKEN_WORD || tok->type == TOKEN_BACKN
           || tok->type == TOKEN_SEMICOL)
    {
        skip_newline(lexer);
        // get the current token because we want it set after skipping
        tok = lexer_peek(lexer);
        // after skipping if current token
        // is not a word, we don't want to parser it
        if (tok->type != TOKEN_WORD)
            break;
        // else fill the command node, error if no command
        struct ast_cmd *cmd = init_ast_cmd();

        // Get cmds
        p_stat = parse_fill_cmd(lexer, cmd);
        push_arr(res, len, (struct ast *)cmd);
        // no need to call handle_error cuz handled in parse_rule_if
        if (p_stat != PARSER_OK)
            return p_stat;

        tok = lexer_peek(lexer);
    }
    return PARSER_OK;
}
