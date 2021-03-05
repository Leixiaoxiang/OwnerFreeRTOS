#include "list.h"

/*链表节点的初始化*/
void vListInitialiseItem(ListItem * const pxItem)
{
    /*表示该节点并未插入任何的链表*/
    pxItem->pvContainer = NULL;
}

void vListInitialise(List_t * const pxList)
{
    pxList->pxIndex = (ListItem *) & (pxList->xListEnd);

    pxList->xListEnd.pxNext = (ListItem *) & (pxList->xListEnd);

    pxList->xListEnd.pxPrevious = (ListItem *) & (pxList->xListEnd);

    pxList->xListEnd.xItemValue = portMAX_DELAY;

    pxList->uxNumberOfItems = (UBaseType_t)0UL;
}

void vListInsertEnd(List_t * const pxList, ListItem * const pxNewListItem)
{
    ListItem * const pxIndex = pxList->pxIndex;

    pxNewListItem->pxNext = pxIndex;

    pxNewListItem->pxPrevious = pxIndex->pxPrevious;

    pxNewListItem->pvContainer = (void *)pxList;

    pxIndex->pxPrevious = pxNewListItem;

    /*这个地方是需要注意的*/
    pxIndex->pxPrevious->pxNext = pxNewListItem;

    (pxList->uxNumberOfItems)++;
}

/*按照升序进行插入，如值相等，则新节点在后*/
void vListInsert(List_t * const pxList, ListItem * const pxNewListItem)
{
    ListItem * pxIterator;

    const TickType_t xValueOfInsertion = pxNewListItem->xItemValue;

    for ( pxIterator = (ListItem * ) & (pxList->xListEnd); 
            pxIterator->pxNext->xItemValue <= xValueOfInsertion; 
            pxIterator = pxIterator->pxNext)
    {
        
    }

    pxNewListItem->pxPrevious = pxIterator;

    pxNewListItem->pxNext = pxIterator->pxNext;

    pxIterator->pxNext->pxPrevious = pxNewListItem;

    pxIterator->pxNext = pxNewListItem;

    pxNewListItem->pvContainer = (void *)pxList;

    (pxList->uxNumberOfItems)++;
}

UBaseType_t uxListRemove(ListItem * const pxItemToRemove)
{
    List_t * const pxList = (List_t *)pxItemToRemove->pvContainer;

    pxItemToRemove->pxPrevious->pxNext = pxItemToRemove->pxNext;
    
    pxItemToRemove->pxNext->pxPrevious = pxItemToRemove->pxPrevious;

    if(pxList->pxIndex == pxItemToRemove)
    {
        pxList->pxIndex = pxItemToRemove->pxPrevious;
    }

    pxItemToRemove->pvContainer = NULL;

    (pxList->uxNumberOfItems)--;

    return pxList->uxNumberOfItems;
}
