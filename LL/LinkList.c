#include "LinkList.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#pragma mark - Internal Helper Functions

unsigned int LLDefaultStringHashFn(LLKey key, int limit)
{
  unsigned int result = 0x55555555;

  while (*key) { 
      result ^= *key++;
      result = (result << 5) | (result >> ((sizeof(result) * 8) % sizeof(result)));
  }
  
  result = result % limit;

  return result;
}

#ifndef LL_DEFAULT_HASHFN
#define LL_DEFAULT_HASHFN
const LLHashFn LLDefaultHashFunction = LLDefaultStringHashFn;
#else
const LLHashFn LLDefaultHashFunction = LL_DEFAULT_HASHFN;
#endif

#ifndef LL_DEFAULT_HASHFN_LIMIT
#define LL_DEFAULT_HASHFN_LIMIT
const LLHashLimit LLDefaultHashLimit = 100;
#else
const LLHashLimit LLDefaultHashLimit = LL_DEFAULT_HASHFN_LIMIT;
#endif

char *__strdup(char *source)
{
  size_t size = strlen(source) + 1;
  char *dest = (char *)malloc(size);

  memset(dest, 0L, size);
  strcpy(dest, source);
  return dest;
}

#ifdef WCHAR_SUPPORT
wchar_t *__wstrdup(wchar_t *source)
{
  size_t size = wcslen(source) + sizeof(wint_t);
  wchar_t *dest = (wchar_t *)malloc(size);

  memset(dest, 0L, size);
  wcscpy(dest, source);
  return dest;
}
#endif

void LNSetIntByType(LLIntegerNode *node, LLIntegerType type, MAX_INT_TYPE value)
{  
  switch(type) 
  {
    case LLIN_CHAR: 
      type & LLIN_UNSIGNED 
        ? (node->uc = (unsigned char)value) 
        : (node->c = (char)type);
      node->type = type;
      break;
    case LLIN_SHORT:
      type & LLIN_UNSIGNED 
        ? (node->us = (unsigned short)value) 
        : (node->s = (short)type);
      node->type = type;
      break;
    case LLIN_INT:
      type & LLIN_UNSIGNED 
        ? (node->ui = (unsigned int)value) 
        : (node->i = (int)type);
      node->type = type;
      break;
    case LLIN_LONG:
      type & LLIN_UNSIGNED 
        ? (node->ul = (unsigned long)value) 
        : (node->l = (long)type);
      node->type = type;
      break;
    #ifdef BIG_TYPES
    case LLIN_LONG_LONG:
      type & LLIN_UNSIGNED 
        ? (node->ull = (unsigned MAX_INT_TYPE)value) 
        : (node->ll = (MAX_INT_TYPE)type);
      node->type = type;
      break;
    #endif
    default:
      #ifdef BIG_TYPES
      node->ll = value;
      #else
      node->l = value;
      #endif
      node->type = type;
      break;
  }  
}

void LNSetDecByType(LLDecimalNode *node, LLDecimalType type, MAX_DEC_TYPE value)
{
  switch (type)
  {
    case LLDN_FLOAT:
      node->f = (float)value;
      node->type = type;
      break;
    case LLDN_DOUBLE:
      node->d = (double)value;
      node->type = type;
      break;
    #ifdef BIG_TYPES
    case LLDN_LONG_DOUBLE:
      node->ld = (MAX_DEC_TYPE)value;
      node->type = type;
      break;
    #endif
  }
}

void LNSetStrByType(LLStringNode *node, LLStringType type, VoidPtr string)
{
  switch (type)
  {
    default:
    case LLSN_STRING:
      node->s = __strdup((char *)string);
      node->type = type;
      break;
    #ifdef WCHAR_SUPPORT
    case LLSN_WIDE:
      node->w = __wstrdup((wchar_t *)string);
      node->type = type;
      break;
    #endif
  }
}

#pragma mark - Utility Functions

size_t LLDataSize(LinkNode *node)
{
  if (node->type & LN_KEYED)
  {
    switch (node->type ^ LN_KEYED)
    {
      case LN_BOOLEAN: return sizeof(LLKeyedBool);
      case LN_INTEGER: return sizeof(LLKeyedInteger);
      case LN_DECIMAL: return sizeof(LLKeyedDecimal);
      case LN_STRING:  return sizeof(LLKeyedString);
      default:         return sizeof(LLKeyedNode);
    }
  }
  else 
  {
    switch (node->type)
    {
      case LN_BOOLEAN: return sizeof(LLBoolNode);
      case LN_INTEGER: return sizeof(LLIntegerNode);
      case LN_DECIMAL: return sizeof(LLDecimalNode);
      case LN_STRING:  return sizeof(LLStringNode);
      default:         return sizeof(LinkNode);
    }
  }
}

LinkNode *LLFindKeyed(LinkList *list, LLKey key)
{
  LinkNode *node  = list && list->head ? list->head : NULL;
  LLKeyedNode *keyedNode;
  
  while (node) 
  {
    if (node->type & LN_KEYED) 
    {
      keyedNode = (LLKeyedNode *)node->value;
      if (keyedNode)
      {
        if (strcasecmp(key, keyedNode->key) == 0)
        {
          return node;
        }
      }
    }
    
    node = node->next;
  }
  
  return NULL;
}

LLStringNode *LLDuplicateStringNode(LLStringNode *source)
{
  LLStringNode *dest = LNSInit(NULL, Yes);

  dest->type = source->type;
  switch (dest->type)
  {
    default:
    case LLSN_STRING:
      dest->s = __strdup(source->s);
      break;
    #ifdef WCHAR_SUPPORT
    case LLSN_WIDE:
      dest->w = __wstrdup(source->w);
      break;
    #endif
  }

  return dest;
}


#pragma mark - Initialization Functions

LinkList *LLInit(LinkList *list, LLBoolean alloc)
{
    size_t size = sizeof(LinkList);
    
    if (alloc) list = (LinkList *)malloc(size);
    if (!list) return NULL;
    
    memset(list, 0L, size);

    return list;
}


LinkNode *LNInit(LinkNode *node, LLBoolean alloc)
{
  size_t size = sizeof(LinkNode);
  
  if (alloc) node = (LinkNode *)malloc(size);
  if (!node) return NULL;
  
  memset(node, 0L, size); 

  return node;
}


LLBoolNode *LNBInit(LLBoolNode *node, LLBoolean alloc)
{
  size_t size = sizeof(LLBoolNode);
  
  if (alloc) node = (LLBoolNode *)malloc(size);
  if (!node) return NULL;
  
  memset(node, 0L, size);

  return node;
}


LLIntegerNode *LNIInit(LLIntegerNode *node, LLBoolean alloc)
{
  size_t size = sizeof(LLIntegerNode);
  
  if (alloc) node = (LLIntegerNode *)malloc(size);
  if (!node) return NULL;
  
  memset(node, 0L, size);

  return node;
}


LLDecimalNode *LNDInit(LLDecimalNode *node, LLBoolean alloc)
{
  size_t size = sizeof(LLDecimalNode);
  
  if (alloc) node = (LLDecimalNode *)malloc(size);
  if (!node) return NULL;
  
  memset(node, 0L, size);

  return node;
}


LLStringNode *LNSInit(LLStringNode *node, LLBoolean alloc)
{
  size_t size = sizeof(LLStringNode);
  
  if (alloc) node = (LLStringNode *)malloc(size);
  if (!node) return NULL;
  
  memset(node, 0L, size);

  return node;
}


LLKeyedNode *LNKNInit(LLKeyedNode *node, LLBoolean alloc)
{
  size_t size = sizeof(LLKeyedNode);
  
  if (alloc) node = (LLKeyedNode *)malloc(size);
  if (!node) return NULL;
  
  memset(node, 0L, size);

  return node;
}


LLKeyedBool *LNKBInit(LLKeyedBool *node, LLBoolean alloc)
{
  size_t size = sizeof(LLKeyedBool);
  
  if (alloc) node = (LLKeyedBool *)malloc(size);
  if (!node) return NULL;
  
  memset(node, 0L, size);

  return node;
}


LLKeyedInteger *LNKIInit(LLKeyedInteger *node, LLBoolean alloc)
{
  size_t size = sizeof(LLKeyedInteger);
  
  if (alloc) node = (LLKeyedInteger *)malloc(size);
  if (!node) return NULL;
  
  memset(node, 0L, size);

  return node;
}


LLKeyedDecimal *LNKDInit(LLKeyedDecimal *node, LLBoolean alloc)
{
  size_t size = sizeof(LLKeyedDecimal);
  
  if (alloc) node = (LLKeyedDecimal *)malloc(size);
  if (!node) return NULL;
  
  memset(node, 0L, size);

  return node;
}


LLKeyedString *LNKSInit(LLKeyedString *node, LLBoolean alloc)
{
  size_t size = sizeof(LLKeyedString);
  
  if (alloc) node = (LLKeyedString *)malloc(size);
  if (!node) return NULL;
  
  memset(node, 0L, size);

  return node;
}

#pragma mark - Creation Functions

LinkList *LLCreate(void)
{
  LinkList *list = LLInit(NULL, Yes);
  return list;
}

LinkNode *LNCreate(VoidPtr value, LinkNodeDataType type)
{
  LinkNode *node = LNInit(NULL, Yes);

  if (value) {
    node->value = value;
    node->type = type;
  }
  return node;
}

LLKeyedNode *LNKCreate(LLKey key, LLHashFn hashFunction)
{
  LLHashFn hashMe = hashFunction ? hashFunction : LLDefaultHashFunction;
  LLKeyedNode *node = LNKNInit(NULL, Yes);

  node->key = __strdup(key);
  node->hashValue = hashMe(key, LLDefaultHashLimit);
  return node;  
}

LLBoolNode *LNBCreate(LLBoolean boolean)
{
  LLBoolNode *node = LNBInit(NULL, Yes);

  node->boolean = boolean;
  return node;
}

LLIntegerNode *LNICreate(MAX_INT_TYPE value, LLIntegerType type)
{
  LLIntegerNode *node = LNIInit(NULL, Yes);

  LNSetIntByType(node, type, value);
  return node;
}

LLDecimalNode *LNDCreate(MAX_DEC_TYPE value, LLDecimalType type)
{
  LLDecimalNode *node = LNDInit(NULL, Yes);

  LNSetDecByType(node, type, value);
  return node;
}

LLStringNode *LNSCreate(VoidPtr string, LLStringType type)
{
  LLStringNode *node = LNSInit(NULL, Yes);

  LNSetStrByType(node, type, string);
  return node;
}

LLKeyedBool *LNKBCreate(LLKey key, LLBoolean boolean)
{
  LLKeyedNode *keyNode = LNKCreate(key, LLDefaultHashFunction);
  LLKeyedBool *node = LNKBInit(NULL, Yes);

  memcpy(&node->keyedNode, keyNode, sizeof(LLKeyedNode));
  node->boolean = boolean;
  return node;
}


LLKeyedInteger *LNKICreate(LLKey key, MAX_INT_TYPE value, LLIntegerType type)
{
  LLKeyedNode *keyNode = LNKCreate(key, LLDefaultHashFunction);
  LLIntegerNode *data = LNICreate(value, type);
  LLKeyedInteger *node = LNKIInit(NULL, Yes);

  memcpy(&node->keyedNode, keyNode, sizeof(LLKeyedNode));
  memcpy(&node->integer, data, sizeof(LLIntegerNode));
  return node;
}


LLKeyedDecimal *LNKDCreate(LLKey key, MAX_DEC_TYPE value, LLDecimalType type)
{
  LLKeyedNode *keyNode = LNKCreate(key, LLDefaultHashFunction);
  LLDecimalNode *data = LNDCreate(value, type);
  LLKeyedDecimal *node = LNKDInit(NULL, Yes);

  memcpy(&node->keyedNode, keyNode, sizeof(LLKeyedNode));
  memcpy(&node->decimal, data, sizeof(LLDecimalNode));
  return node;
}


LLKeyedString *LNKSCreate(LLKey key, VoidPtr string, LLStringType type)
{
  LLKeyedNode *keyNode = LNKCreate(key, LLDefaultHashFunction);
  LLStringNode *data = LNSCreate(string, type);
  LLKeyedString *node = LNKSInit(NULL, Yes);
  
  memcpy(&node->keyedNode, keyNode, sizeof(LLKeyedNode));
  memcpy(&node->string, data, sizeof(LLStringNode));
  return node;
}

#pragma mark - Deallocation Functions

void LLDelete(LinkList *list)
{
  LinkNode *node = list->head, *next;
  while (node) 
  {
    next = node->next;
    if (node) LNDelete(node);
    node = next;
  }
  
  free(list);
}

void LNDelete(LinkNode *node)
{
  LLBoolean isKeyed = node->type & LN_KEYED ? Yes : No;
  int unkeyedType = isKeyed ? node->type ^ LN_KEYED : node->type;
  VoidPtr data = node->value;

  LLBoolNode *boolNode;
  LLIntegerNode *intNode;
  LLDecimalNode *decNode;
  LLStringNode *strNode;

  LLKeyedBool *keyedBool;
  LLKeyedInteger *keyedInt;
  LLKeyedDecimal *keyedDec;
  LLKeyedString *keyedStr;

  if (node->type) 
  {
    switch (unkeyedType)
    {
      case LN_USER:
        // Can't know about custom types; skip freeing mem. Up to user

        break;
      case LN_BOOLEAN:
        keyedBool = isKeyed ? (LLKeyedBool *)data : NULL;
        boolNode = isKeyed ? NULL : (LLBoolNode *)data;

        if (keyedBool) {
          if (keyedBool->keyedNode.key) free(keyedBool->keyedNode.key);
          free(keyedBool);
        }
        if (boolNode) free(boolNode);

        break;
      case LN_INTEGER:
        keyedInt = isKeyed ? (LLKeyedInteger *)data : NULL;
        intNode = isKeyed ? NULL : (LLIntegerNode *)data;

        if (keyedInt) {
          if (keyedInt->keyedNode.key) free(keyedInt->keyedNode.key);
          free(keyedInt);
        }
        if (intNode) free(intNode);

        break;
      case LN_DECIMAL:
        keyedDec = isKeyed ? (LLKeyedDecimal *)data : NULL;
        decNode = isKeyed ? NULL : (LLDecimalNode *)data;

        if (keyedDec) {
          if (keyedDec->keyedNode.key) free(keyedDec->keyedNode.key);
          free(keyedDec);
        }
        if (decNode) free(decNode);

        break;
      case LN_STRING:
        keyedStr = isKeyed ? (LLKeyedString *)data : NULL;
        strNode = isKeyed ? NULL : (LLStringNode *)data;

        if (keyedStr) {
          if (keyedStr->keyedNode.key) free(keyedStr->keyedNode.key);
          if (keyedStr->string.s) free(keyedStr->string.s);
          free(keyedStr);
        }
        if (strNode) {
          if (strNode->s) free(strNode->s);
          free(strNode);
        }

        break;
    }
  }
  
  free(node);
}

#pragma mark - List Push Functions

LinkNode *LLPush(LinkList *list, LinkNode *node)
{
  if (!list->tail) 
  {
      if (!list->head)
      {
        list->head = node;
      }
      list->tail = node;
  }
  else 
  {
    list->tail->next = node;
    node->prev = list->tail;
    list->tail = node;
    node->next = NULL;
  }
  
  return node;
}


LinkNode *LLPushBoolean(LinkList *list, LLBoolean boolean)
{
  LLBoolNode *data = LNBCreate(boolean);
  LinkNode *node = LNCreate(data, LN_BOOLEAN);
  LLPush(list, node);
  return node;
}


LinkNode *LLPushInteger(LinkList *list, MAX_INT_TYPE value, LLIntegerType type)
{
  LLIntegerNode *data = LNICreate(value, type);
  LinkNode *node = LNCreate(data, LN_INTEGER);
  LLPush(list, node);  
  return node;
}


LinkNode *LLPushDecimal(LinkList *list, MAX_DEC_TYPE value, LLDecimalType type)
{
  LLDecimalNode *data = LNDCreate(value, type);
  LinkNode *node = LNCreate(data, LN_DECIMAL);
  LLPush(list, node);  
  return node;
}


LinkNode *LLPushString(LinkList *list, VoidPtr string, LLStringType type)
{
  LLStringNode *data = LNSCreate(string, type);
  LinkNode *node = LNCreate(data, LN_STRING);
  LLPush(list, node);
  return node;
}


LinkNode *LLPushKey(LinkList *list, LLKeyedNode *node)
{
  LinkNode *linkNode = LNCreate(node, LN_USER);
  LLPush(list, linkNode);
  return linkNode;
}


LinkNode *LLPushKeyedBoolean(LinkList *list, LLKey key, LLBoolean boolean)
{
  LLKeyedBool *data = LNKBCreate(key, boolean);
  LinkNode *node = LNCreate(data, LN_BOOLEAN | LN_KEYED);
  LLPush(list, node);
  return node;
}


LinkNode *LLPushKeyedInteger(
  LinkList *list, 
  LLKey key, 
  MAX_INT_TYPE value, 
  LLIntegerType type
) 
{
  LLKeyedInteger *data = LNKICreate(key, value, type);
  LinkNode *node = LNCreate(data, LN_INTEGER | LN_KEYED);
  LLPush(list, node);
  return node;
}


LinkNode *LLPushKeyedDecimal(LinkList *list, LLKey key, MAX_DEC_TYPE value, LLDecimalType type)
{
  LLKeyedDecimal *data = LNKDCreate(key, value, type);
  LinkNode *node = LNCreate(data, LN_DECIMAL | LN_KEYED);
  LLPush(list, node);
  return node;  
}


LinkNode *LLPushKeyedString(LinkList *list, LLKey key, VoidPtr string, LLStringType type)
{
  LLKeyedString *data = LNKSCreate(key, string, type);
  LinkNode *node = LNCreate(data, LN_STRING | LN_KEYED);
  LLPush(list, node);
  return node;    
}

#pragma mark - List Pop Functions

LinkNode *LLPopNode(LinkList *list)
{
  LinkNode *node = list && list->tail ? list->tail : NULL;

  if (!node || !list) return NULL;
  list->tail = node->prev;

  if (list->head == node->prev) {
    list->head = node->prev;
    list->head->next = NULL;
    list->head->prev = NULL;
  }
  
  node->next = NULL;
  node->prev = NULL;
  
  return node;
}

LLBoolean LLPopBoolean(LinkList *list)
{
  LinkNode *node = LLPopNode(list);
  LLBoolNode *data;
  
  if (!node) return No;
  else {
    data = (LLBoolNode *)node->value;
    return data->boolean;
  }
}

LLIntegerNode *LLPopInteger(LinkList *list)
{
  LinkNode *node = LLPopNode(list);
  LLIntegerNode *data;
  
  if (!node) return NULL;
  else {
    data = (LLIntegerNode*)node->value;
    return data;
  }  
}


LLDecimalNode *LLPopDecimal(LinkList *list)
{
  LinkNode *node = LLPopNode(list);
  LLDecimalNode *data;
  
  if (!node) return NULL;
  else {
    data = (LLDecimalNode*)node->value;
    return data;
  }  
}


LLStringNode *LLPopString(LinkList *list)
{
  LinkNode *node = LLPopNode(list);
  LLStringNode *data;
  
  if (!node) return NULL;
  else {
    data = (LLStringNode*)node->value;
    return data;
  }    
}


LLBoolean LLPopKeyedBoolean(LinkList *list, LLKey key)
{
  LinkNode *node = LLFindKeyed(list, key);
  LLKeyedBool *data = node && node->value ? (LLKeyedBool *)node->value : 0L;
  
  return data ? data->boolean : No;
}


LLKeyedInteger *LLPopKeyedInteger(LinkList *list, LLKey key)
{
  LinkNode *node = LLFindKeyed(list, key);
  LLKeyedInteger *data = node && node->value 
    ? (LLKeyedInteger *)node->value 
    : 0L;
  
  return data ? data : NULL;  
}


LLKeyedDecimal *LLPopKeyedDecimal(LinkList *list, LLKey key)
{
  LinkNode *node = LLFindKeyed(list, key);
  LLKeyedDecimal *data = node && node->value 
    ? (LLKeyedDecimal *)node->value 
    : 0L;
  
  return data ? data : NULL;  
}


LLKeyedString *LLPopKeyedString(LinkList *list, LLKey key)
{
  LinkNode *node = LLFindKeyed(list, key);
  LLKeyedString *data = node && node->value ? (LLKeyedString *)node->value : 0L;
  
  return data ? data : NULL;  

}

#pragma mark - List Item Removal Functions

void LLRemoveNode(LinkList *list, LinkNode *node)
{
  if (list->head == node) {
    if (node->next) {
      list->head = node->next;
    }
  }
  
  if (list->tail == node) {
    if (node->prev) 
    {
      list->tail = node->prev;
    }
  }
  
  if (node->prev) 
  {
    node->prev->next = node->next;
  }
  
  if (node->next)
  {
    node->next->prev = node->prev;
  }
}


void LLRemoveByKey(LinkList *list, LLKey key)
{
  LinkNode *node = LLFindKeyed(list, key);
  LLRemoveNode(list, node);
}


void LLRemoveByData(LinkList *list, VoidPtr data)
{
  LinkNode *node = list && list->head ? list->head : NULL;
  LinkNode *inspect;
  
  while (node) 
  {
    inspect = node;
    
    if (inspect->value && inspect->value == data) {
      LLRemoveNode(list, inspect);
    }
    
    node = node->next;
  }
}


