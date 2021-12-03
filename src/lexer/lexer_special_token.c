#include "lexer.h"

static char *lexer_get_separator(char *str, enum token_type *type)
{
    if (*str == '<' && *(str + 1) == '<' && *(str + 2) == '-')
    {
        *type = TOKEN_REDIR;
        return (str + 3);
    }

    if ((*str == '>' || *str == '<') && *(str + 1) == '&')
    {
        *type = TOKEN_REDIR;
        return (str + 2);
    }

    if (*str == '>' && *(str + 1) == '|')
    {
        *type = TOKEN_REDIR;
        return (str + 2);
    }

    if (*str == '<' && *(str + 1) == '>')
    {
        *type = TOKEN_REDIR;
        return (str + 2);
    }

    if (*str == '>' && *(str + 1) == '>')
    {
        *type = TOKEN_REDIR;
        return (str + 2);
    }

    if (*str == '<' && *(str + 1) == '<')
    {
        *type = TOKEN_REDIR;
        return (str + 2);
    }

    if (*str == ';' && *(str + 1) == ';')
    {
        *type = TOKEN_REDIR;
        return (str + 2);
    }

    if (*str == '(' || *str == ')' || *str == '{' || *str == '}' || *str == '\n'
        || *str == ';' || *str == ' ' || *str == '\t' || *str == '<'
        || *str == '>')
    {
        if (*str == ';')
            *type = TOKEN_SEMICOL;
        else
            *type = TOKEN_REDIR;

        //TODO: Handle this shit trash nigga ass bitch
        return (str + 1);
    }

    *type = 0;
    return str;
}

static char *lexer_quotes(char *str, enum token_type *type)
{
    char cpy = *str;

    // If cond true, then it's not a quote
    if (cpy != '\'' && cpy != '"')
    {
        *type = 0;
        return str;
    }

    // Skip first quote
    str++;

    // Go the the last quote TODO: check if there is a final quote
    for (; *str != '\0' && *str != cpy; str++)
        ;
    *type = TOKEN_QT;
    return str + 1;
}

static char *lexer_comment(char *str, enum token_type *type)
{
    if (*str != '#')
    {
        *type = 0;
        return str;
    }

    char *cpy = str + 1;

    // move till \0 or \n
    for (; *cpy != '\0'; cpy++)
    {
        if (*cpy == '\n')
        {
            *type = TOKEN_COMMENT;
            return cpy;
        }
    }

    return cpy;
}

static char *lexer_mafs(char *str, enum token_type *type)
{
    if (*str != '$' || *(str + 1) != '(' || *(str + 2) != '(')
    {
        *type = 0;
        return str;
    }

    char *cpy = str + 1;
    int count_parenthesis = 0;

    for (; *cpy != '\0'; cpy++)
    {
        if (*cpy == '(')
            count_parenthesis++;

        if (*cpy == ')')
        {
            count_parenthesis--;

            if (count_parenthesis < 0)
            {
                *type = 0;
                return str;
            }
            if (count_parenthesis == 0)
            {
                *type = TOKEN_ARITH;
                return cpy + 1;
            }
        }
    }

    *type = -1;
    return str;
}

static char *lexer_variables(char *str, enum token_type *type)
{
    if (*str != '$' || *(str + 1) != '{')
    {
        *type = 0;
        return str;
    }

    char *cpy = str;
    for (; *cpy != '\0'; cpy++)
    {
        if (*cpy == '}')
        {
            *type = TOKEN_VAR;
            return cpy + 1;
        }
    }

    *type = 0;
    return str;
}

int get_special_token_end(char *str, char **end, enum token_type *type)
{
    int res = 0;
    res =
        ((*end = lexer_get_separator(str, type)) != str
         || (*end = lexer_quotes(str, type)) != str || (*end = lexer_mafs(str, type)) != str
         || (*end = lexer_comment(str, type)) != str
         || (*end = lexer_variables(str, type)) != str);

    return res;
}
