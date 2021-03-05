#ifndef LIST_H
#define LIST_H

#include "FreeRTOS.h"

struct xLIST_ITEM
{
    TickType_t xItemValue;  /*排序*/
    struct xLIST_ITEM * pxNext;
    struct xLIST_ITEM * pxPrevious;
    void * pvOwner;
    void * pvContainer;     /*属于哪个链表*/
};

typedef struct xLIST_ITEM ListItem;

struct xMINI_LIST_ITEM
{
    TickType_t xItemValue;  /*排序*/
    struct xLIST_ITEM * pxNext;
    struct xLIST_ITEM * pxPrevious;
};

typedef struct xMINI_LIST_ITEM MiniListItem_t;

typedef struct xLIST
{
    UBaseType_t uxNumberOfItems;    /*链表节点的计数*/
    ListItem * pxIndex;             /*链表节点索引指针*/
    MiniListItem_t xListEnd;        /*链表最后一个节点*/
}List_t;


#define listSET_LIST_ITEM_OWNER(pxListItem,pxOwner)  \
        ((pxListItem)->pvOwner = (void *)(pxOwner))

#define listGET_LIST_ITEM_OWNER(pxListItem)  \
        ((pxListItem)->pvOwner)

#define listSET_LIST_ITEM_VALUE(pxListItem,xValue)  \
        ((pxListItem)-xItemValue = (TickType_t)(xValue))

#define listGET_LIST_ITEM_VALUE(pxListItem) \
        ((pxListItem)->xItemValue)

#define listGET_ITEM_VALUE_OF_HEAD_ENTRY(pxList)    \
        (((pxList)->xListEnd).pxNext->xItemValue)

#define listGET_HEAD_ENTRY(pxList)  \
        (((pxList)->xListEnd).pxNext)

#define listGET_NEXT(pxListItem)    \
        ((pxListItem)->pxNext)

#define listGET_END_MARKER(pxList)  \
        ((ListItem const *) (&((pxList)->xListEnd)))

#define listLIST_IS_EMPTY(pxList)   \
        ((pxList)->uxNumberOfItems == (UBaseType_t)0UL)

#define listCURRENT_LIST_LENGTH(pxList) \
        ((pxList)->uxNumberOfItem)

#if 0
#define listGET_OWNER_OF_NEXT_ENTRY(pxTCB,pxList)   \
{
        List_t * const pxConstList = pxList;    \
        (pxConstList)->pxIndex = (pxConstList)->pxIndex->pxNext;        \
        if((void *)(pxConstList)->pxIndex == (void *) & ((pxConstList)->xListEnd))
        {       \
                (pxConstList)->pxIndex = (pxConstList)->pxIndex->pxNext;        \
        }       \
        (pxTCB) = (pxConstList)->pxIndex->pvOwner;
}
#endif
				
#endif
