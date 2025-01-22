#include <stdio.h>
#include <stdlib.h>
void print_bin (short val);
int main() {
    unsigned char  PORT=0xFF ;
    print_bin(PORT);
    PORT ^= (1<<6)  | (1<<3);
    print_bin(PORT);
    PORT ^= (1<<6)  | (1<<3);
    print_bin(PORT);
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