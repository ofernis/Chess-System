#include "map.h"
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>

/** Type for defining the nodes that contain data and key for the members of the map */
typedef struct node_t {
    MapDataElement data;
    MapKeyElement key;
    struct node_t* next;
} *Node;

/** Type for defining the map */
struct Map_t {
    int size;
    Node head;
    Node current;
    copyMapDataElements copyDataFunc;
    copyMapKeyElements copyKeyFunc;
    freeMapDataElements freeDataFunc;
    freeMapKeyElements freeKeyFunc;
    compareMapKeyElements compareKeyFunc;
};

/**
*	createNode: Creates a new node in the given map.
*
* @param map - The map for which to create a new node
* @param dataElement - The new data element to associate with the given key.
* @param keyElement - The key element which need to be assigned
* @param next - The next node in the map
* @return
* 	NULL - If the the memory allocation failed
* 	new_node - If the new node was created successfully
*/
static Node createNode(Map map, MapDataElement data, MapKeyElement key, Node next){
    Node new_node = malloc(sizeof(*new_node));
    if(new_node == NULL)
    {
        return NULL;
    }
    
    new_node->data = map->copyDataFunc(data);
    new_node->key = map->copyKeyFunc(key);
    new_node->next = next;

    return new_node;
}

Map mapCreate(copyMapDataElements copyDataElement,
              copyMapKeyElements copyKeyElement,
              freeMapDataElements freeDataElement,
              freeMapKeyElements freeKeyElement,
              compareMapKeyElements compareKeyElements){

    if(copyDataElement == NULL
     || copyKeyElement == NULL
     || freeDataElement == NULL
     || freeKeyElement == NULL
     || compareKeyElements == NULL)
    {
        return NULL;
    }

    Map map = malloc(sizeof(*map));
    if(map == NULL)
    {
        return NULL;
    }

    map->copyDataFunc = copyDataElement;
    map->copyKeyFunc = copyKeyElement;
    map->freeDataFunc = freeDataElement;
    map->freeKeyFunc = freeKeyElement;
    map->compareKeyFunc = compareKeyElements;

    map->head = createNode(map, NULL, NULL, NULL);
    if (map->head == NULL)
    {
        free(map);
        return NULL;

    }
    map->current = map->head;
    map->size = 0;

    return map;
}

void mapDestroy(Map map){
    if(map == NULL)
    {
        return;
    }
    map->current = map->head;
    Node next;

    while (map->current != NULL) 
    {
    next = map->current->next;
    map->freeDataFunc(map->current->data);
    map->freeKeyFunc(map->current->key);
    map->current->data = NULL;
    map->current->key = NULL;

    free(map->current);
    map->current = next;

    }
    free(map);
}

Map mapCopy(Map map)
{
    if (map == NULL)
    {
        return NULL;
    }

    Map map_cpy = mapCreate(map->copyDataFunc,
                            map->copyKeyFunc,
                            map->freeKeyFunc,
                            map->freeKeyFunc,
                            map->compareKeyFunc);
    if (map_cpy == NULL)
    {
        return NULL;
    }

    map->current = map->head;

    while (map->current != NULL)
    {
        mapPut(map_cpy, map->current->key, map->current->data);
        map->current = map->current->next;
    }

    map_cpy->size = map->size;
    return map_cpy;
}

int mapGetSize(Map map){
    if(map == NULL)
    {
        return -1;
    }
    return map->size;
}

bool mapContains(Map map, MapKeyElement key){
    if (map == NULL || key == NULL || mapGetSize(map) == 0)
    {
        return false;
    }

    map->current = map->head;
    while(map->current != NULL)
    {
        if (map->compareKeyFunc(map->current->key, key) == 0)
        {
            return true;
        } 
          
        map->current = map->current->next;
    }
    
    return false;
}

/**
*	mapChangeExistingData: change existing data which is associtated with the given key.
*
* @param map - The map for which to reassign the data element
* @param keyElement - The key of the relevant data
* @param dataElement - The new data element to associate with the given key.
*      A copy of the element will be inserted as supplied by the copying function
*      which is given at initialization and old data memory would be
*      deleted using the free function given at initialization.
* @return
* 	None
*/
static void mapChangeExistingData(Map map, MapKeyElement keyElement, MapDataElement dataElement){
    map->current = map->head;
    while(map->compareKeyFunc(map->current->key, keyElement) != 0) //the function is used only after verifing the given key exists in the map
    {
        map->current = map->current->next;
    }

    map->freeDataFunc(map->current->data);
    map->current->data = map->copyDataFunc(dataElement);
}

/**
*	mapInsertNewNode: change existing data which is associtated with the given key.
*
* @param map - The map for which to assign the new node
* @param keyElement - The key of the relevant data
* @param dataElement - The new data element to associate with the given key in the new node.
*
* @return
* 	false - If the the memory allocation failed
*   true - If data was successfully changed 
*/
static bool mapInsertNewNode(Map map, MapKeyElement keyElement, MapDataElement dataElement){

    Node new_node = createNode(map, dataElement, keyElement, NULL);
    if(new_node == NULL)
    {
        return false;
    }

    map->current = map->head;
    Node previous = NULL;

    while(map->compareKeyFunc(map->current->key, keyElement) < 0)
    {
        previous = map->current;
        map->current = map->current->next;  

        if (map->current == NULL)
        {
            break;
        } 
    }
    
    new_node->next = map->current;

    if (previous != NULL)
    {
        previous->next = new_node;
    }
    else
    {
        map->head = new_node;
    }

    return true;
}


MapResult mapPut(Map map, MapKeyElement keyElement, MapDataElement dataElement){
    if(map == NULL || dataElement == NULL || keyElement == NULL)
    {
        return MAP_NULL_ARGUMENT;
    }

    if(map->head->key == NULL)
    {
        map->head->data = map->copyDataFunc(dataElement);
        map->head->key = map->copyKeyFunc(keyElement);
        map->size = 1;
    }
    else if(mapContains(map, keyElement) == true)
    {
        mapChangeExistingData(map, keyElement, dataElement);
    }
    else
    {
        if(mapInsertNewNode(map, keyElement, dataElement) == false)
        {
            return MAP_OUT_OF_MEMORY;
        }
        map->size = map->size + 1;
    }

    return MAP_SUCCESS;
}

MapDataElement mapGet(Map map, MapKeyElement keyElement){
    if(map == NULL || keyElement == NULL)
    {
        return NULL;
    }

    if(mapContains(map, keyElement) == false)
    {
        return NULL;
    }

    map->current = map->head;
    while(map->current != NULL)
    { 
        if (map->compareKeyFunc(map->current->key, keyElement) == 0)
        {
            break;
        }
        map->current = map->current->next;
    }

    return map->current->data;   
}

MapResult mapRemove(Map map, MapKeyElement keyElement){
    if (map == NULL || keyElement == NULL)
    {
        return MAP_NULL_ARGUMENT;
    }
    
    if(mapContains(map, keyElement) == false)
    {
        return MAP_ITEM_DOES_NOT_EXIST;
    }

    map->current = map->head;
    Node previous = NULL;

    while(map->current != NULL) 
    {
        if (map->compareKeyFunc(map->current->key, keyElement) == 0)
        {
            break;
        }

        previous = map->current;
        map->current = map->current->next;
    }
    
    if (previous != NULL) //if previous is null - we remove map's head and therfore we need to set a new head
    {
        previous->next = map->current->next;
    }
    else
    {
        if (map->current->next != NULL)
        {
            map->head = map->current->next;
        }
    }
    
    map->freeDataFunc(map->current->data);
    map->freeKeyFunc(map->current->key);
    map->size = map->size - 1;

    map->current->data = NULL;
    map->current->key = NULL;

    if (map->size != 0)
    {
        free(map->current);
    }

    return MAP_SUCCESS;
}

MapKeyElement mapGetFirst(Map map){
    if(map == NULL || map->size == 0)
    {
        return NULL;
    }

    map->current = map->head;
    return map->copyKeyFunc(map->current->key);
}

MapKeyElement mapGetNext(Map map){
    if(map == NULL)
    {
        return NULL;
    }
    map->current = map->current->next;

    if(map->current == NULL)
    {
        return NULL;
    }

    return map->copyKeyFunc(map->current->key);
}

MapResult mapClear(Map map)
{
    if (map == NULL)
    {
        return MAP_NULL_ARGUMENT;
    }

    map->current = map->head;

    while(map->current != NULL)
    {
        map->freeDataFunc(map->current->data);
        map->copyKeyFunc(map->current->key);
        map->current = map->current->next;
    }

    map->size = 0;
    return MAP_SUCCESS;
}