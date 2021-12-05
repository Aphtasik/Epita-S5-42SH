#include "parse.h"

// fill the command struct node
enum parser_status parse_fill_cmd(struct lexer *lexer, struct ast_cmd *cmd)
{
    struct token *tok = lexer_peek(lexer);
    // Not interrested by this token
    if (tok->type != TOKEN_WORD)
        return PARSER_OK;

    // in order to check if there is a command
    int i = 0;
    while ((tok = lexer_peek(lexer))->type == TOKEN_WORD)
    {
        // alloc an arg
        cmd->args = realloc(cmd->args, sizeof(char *) * (cmd->nb_args + 1));
        // set it to the value found
        cmd->args[cmd->nb_args++] = strdup(tok->value);
        // go to next token
        token_free(lexer_pop(lexer));

        i++;
    }
    if (i == 0)
        return PARSER_NO_COMMAND;
    // unrecognized token so just return ok
    return PARSER_OK;
}

// create a ast_cmd node and fill it if interrested
// return PARSER_OK otherwise
enum parser_status parse_cmd(struct lexer *lexer, struct ast **res)
{
    struct token *tok = lexer_peek(lexer);
    // Not interrested by this token
    if (tok->type != TOKEN_WORD)
        return PARSER_OK;

    // fill cmd node
    struct ast_cmd *a_cmd = init_ast_cmd();
    // place cmd node as child of his parent
    *res = (struct ast *)a_cmd;

    enum parser_status p_stat = parse_fill_cmd(lexer, a_cmd);

    return p_stat;
}
