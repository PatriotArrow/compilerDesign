#include <stdio.h>
#include <ctype.h>
#include <stdbool.h>
#include <string.h>

bool isIdentifier(char* str){

    char c;
    int q = 0 ;
    int len = strlen(str);

    int i = 0;
    while ( i < len )
    {
        c = str[i++];
        switch(q)
        {
            case 0:
                printf("initial state ->");
                if (isalpha(c) || c == '_') {
                    //state = Q1;
                    q = 1;
                    printf(" state 1 ->");
                } else {
                    q = -1;
                    printf(" dead state , Invalid C identifier\n");
                }
                break;
            case 1:
                if (isalnum(c) || c == '_') {
                    q = 2;
                    printf(" state 2 ->");
                }
                else {
                    q = -1 ;
                    printf(" dead state , Invalid C identifier\n");
                }
                break;
            case 2:
                if (isalnum(c) || c == '_') {
                    printf(" state 2 ->");
                } else 
                {
                    q = -1;
                    printf(" dead state , Invalid C identifier\n");
                }
                break;
            case -1:
                goto end;
                break;
        }
    }

    if (q == 2 ||  q == 1 ) {
        printf(" final state , Valid C identifier\n");
        return true;
    }
    end:
    return false;
}

void main ()
{
    char * str;
    scanf("%s",str);
    isIdentifier(str);
}
