#include <stdio.h>
#include <stdlib.h>
void print_bin (short val);
int main() {
    unsigned char  PORT=0xCF ;
    print_bin(~(1<<5));
    if (PORT&(1<<6)) {
        printf("True",PORT);
    }
    else 
    printf("false");
    return 0 ;

}



void print_bin(short val)
{
    short i ;
    for (i=7;i>=0;i--)
        printf("%c",(val&(1<<i))? '1':'0');
    putchar('b');
    putchar('\n');
}