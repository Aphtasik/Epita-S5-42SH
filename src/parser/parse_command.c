#include "parse.h"

/**
 * @brief parse a redirection
 * @redirection: [IONUMBER] '>' WORD
 *             | [IONUMBER] '<' WORD
 *             | [IONUMBER] '>>' WORD
 *             | [IONUMBER] '<<' HEREDOC
 *             | [IONUMBER] '<<-' HEREDOC
 *             | [IONUMBER] '>&' WORD
 *             | [IONUMBER] '<&' WORD
 *             | [IONUMBER] '>|' WORD
 *             | [IONUMBER] '<>' WORD
 */
enum parser_status parse_redirection(struct lexer *lexer, struct ast **ast)
{
    struct token *tok = lexer_peek(lexer);
    // not interested
    if (tok->type != TOKEN_REDIR)
        return PARSER_OK;
    struct ast_op *a_op = init_ast_op(AST_REDIR);
    a_op->value = strdup(tok->value);

    // attach the left child
    a_op->leftc = *ast;

    // parse the word at the right
    token_free(lexer_pop(lexer));
    tok = lexer_peek(lexer);
    // create the node that will be attached to the node
    struct ast_word *word = init_ast_word();
    word->value = strdup(tok->value);

    // attach everything before checking type to simplify ast_free job
    a_op->rightc = (struct ast *)word;
    *ast = (struct ast *)a_op;

    if (tok->type != TOKEN_WORD)
        return PARSER_UNEXPECTED_TOKEN;

    return PARSER_OK;
}

/**
 * @brief parse a simple element
 * @element: WORD
 *           | redirection
 */
enum parser_status parse_element(struct lexer *lexer, struct ast **ast)
{
    struct token *tok = lexer_peek(lexer);
    if (tok->type != TOKEN_WORD)
        return PARSER_OK;
    // create the word node
    struct ast_word *word = init_ast_word();
    word->value = strdup(tok->value);
    // TODO: maybe have to realloc the *ast node in order to make the element
    // fit inside attach the node to the ast base
    *ast = (struct ast *)word;
    // discard the WORD token
    token_free(lexer_pop(lexer));
    // check if we have a redirection;
    enum parser_status p_stat = parse_redirection(lexer, ast);
    if (p_stat != PARSER_OK)
        return p_stat;
    return PARSER_OK;
}

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
// handle the "simple_command" rule of the grammar
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

