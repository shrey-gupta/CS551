#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//Using and printing a pointer in structure.
typedef struct {
int *a;
} tempstruct;

int main()
{
tempstruct obj;
int n = 9;
obj.a = &n;
printf("%d\n", *obj.a);

exit(1);
}
