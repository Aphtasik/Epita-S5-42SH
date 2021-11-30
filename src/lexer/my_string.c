#include "my_string.h"

struct my_string *my_string_init(void)
{
    struct my_string *mystr = malloc(sizeof(struct my_string));
    if (!mystr)
        return NULL;

    // Max len is two to stock \0 and 1 char
    mystr->max_len = 2;
    mystr->len = 0;
    mystr->str = malloc(mystr->max_len);
    mystr->str[0] = '\0';

    return mystr;
}

void my_string_resize(struct my_string *mystr)
{
    if (mystr == NULL)
        return;

    mystr->max_len *= 2;
    mystr->str = realloc(mystr->str, mystr->max_len);
}

void my_string_push(struct my_string *mystr, char c)
{
    if (!mystr)
        return;

    if (mystr->len + 1 == mystr->max_len)
        my_string_resize(mystr);

    mystr->str[mystr->len] = c;
    mystr->str[mystr->len + 1] = '\0';
    mystr->len++;
}

void my_string_free(struct my_string *mystr)
{
    free(mystr->str);
    free(mystr);
}
