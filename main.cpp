#include "List.h"
#include "GraphVizDump.h"
int main()
{
    List a;
    ListCtor(&a, 4);
    ListDump(&a);
    ListInsertEnd(&a, 1);
    ListInsertBefore(&a, 2, 1);
    ListInsertEnd(&a, 3);
    ListInsertEnd(&a, 4);
    PrintList(&a);
    return 0;
}

