#include <stdio.h>

int main()
{
    FILE *fp = fopen("test.txt", "r");

    if (fp == NULL)
    {
        printf("Can't open file\n");
        return -1;
    }
    

    fclose(fp);

    return 0;
}