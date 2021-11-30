#include "lexer.h"

static char *lexer_get_separator(char *str)
{
    if (*str == '(' || *str == ')' || *str == '{' || *str == '}' || *str == '\n'
        || *str == ';' || *str == ' ' || *str == '\t' || *str == '<'
        || *str == '>')
        return (str + 1);
    return str;
}

static char *lexer_quotes(char *str)
{
    char type = *str;

    // If cond true, then it's not a quote
    if (type != '\'' && type != '"')
        return str;

    // Skip first quote
    str++;

    // Go the the last quote TODO: check if there is a final quote
    for (; *str != '\0' && *str != type; str++)
        ;
    return str + 1;
}

static char *lexer_comment(char *str)
{
    if (*str != '#')
        return str;

    char *cpy = str + 1;

    // move till \0 or \n
    for (; *cpy != '\0'; cpy++)
    {
        if (*cpy == '\n')
            return cpy;
    }

    return cpy;
}

static char *lexer_mafs(char *str)
{
    if (*str != '$' || *(str + 1) != '(' || *(str + 2) != '(')
        return (str);

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
                return str;
            if (count_parenthesis == 0)
                return cpy + 1;
        }
    }

    return str;
}

int get_special_token_end(char *str, char **end)
{
    int res = 0;
    // TODO: Handle: arithmetics/vars
    res =
        ((*end = lexer_get_separator(str)) != str
         || (*end = lexer_quotes(str)) != str || (*end = lexer_mafs(str)) != str
         || (*end = lexer_comment(str)) != str);

    return res;
}
