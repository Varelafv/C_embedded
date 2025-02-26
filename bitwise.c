#include <stdio.h>
#include <stdlib.h>
void print_bin (short val);
int main() {
    short  PORTA=0x05 , PORTB = 0x04 ;
    print_bin(PORTA);
    print_bin(PORTB);
    PORTA = PORTA & PORTB ;
    printf( " %d",PORTA);
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