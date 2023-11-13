#include "List.h"
#include "GraphVizDump.h"
int main()
{
    List a;
    ListCtor(&a, 4);
    //ListDump(&a);
    ListInsertEnd(&a, 13);
    ListInsertEnd(&a, 22);
    ListInsertStart(&a, 1);

    ListRealloc(&a, 8);
    PrintList(&a);
}

