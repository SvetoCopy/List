#include "List.h"
#include "GraphVizDump.h"
int main()
{
    List a;
    ListCtor(&a, 4);
    ListDump(&a);
    ListInsertEnd(&a, 1);
    ListDump(&a);
    ListInsertBefore(&a, 2, 1);
    ListDump(&a);
    ListInsertEnd(&a, 3);
    ListDump(&a);
    ListInsertEnd(&a, 4);
    ListDump(&a);
    PrintList(&a);
    return 0;
}

