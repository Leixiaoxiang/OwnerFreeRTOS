#include "list.h"

List_t xList_Tree;

ListItem xList_Item1;
ListItem xList_Item2;
ListItem xList_Item3;

int main(void)
{
    UBaseType_t size_t = (UBaseType_t)0UL;

    vListInitialise(&xList_Tree);

    vListInitialiseItem(&xList_Item1);
    xList_Item1.xItemValue = (TickType_t)1U;

    vListInitialiseItem(&xList_Item2);
    xList_Item2.xItemValue = (TickType_t)2U;

    vListInitialiseItem(&xList_Item3);
    xList_Item3.xItemValue = (TickType_t)3U;

    vListInsert(&xList_Tree,&xList_Item1);

    vListInsert(&xList_Tree,&xList_Item2);

    vListInsert(&xList_Tree,&xList_Item3);

    size_t = uxListRemove(&xList_Item3);

    size_t = uxListRemove(&xList_Item1);

    size_t = uxListRemove(&xList_Item2);




    for(;;)
    {

    }
}
