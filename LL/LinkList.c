#include "LinkList.h"

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
        ? (node->u.uc = (unsigned char)value)
        : (node->u.c = (char)value);
      node->type = type;
      break;
    case LLIN_SHORT:
      type & LLIN_UNSIGNED 
        ? (node->u.us = (unsigned short)value)
        : (node->u.s = (short)value);
      node->type = type;
      break;
    case LLIN_INT:
      type & LLIN_UNSIGNED 
        ? (node->u.ui = (unsigned int)value)
        : (node->u.i = (int)value);
      node->type = type;
      break;
    case LLIN_LONG:
      type & LLIN_UNSIGNED 
        ? (node->u.ul = (unsigned long)value)
        : (node->u.l = (long)value);
      node->type = type;
      break;
    #ifdef BIG_TYPES
    case LLIN_LONG_LONG:
      type & LLIN_UNSIGNED 
        ? (node->u.ull = (unsigned MAX_INT_TYPE)value)
        : (node->u.ll = (MAX_INT_TYPE)value);
      node->type = type;
      break;
    #endif
    default:
      #ifdef BIG_TYPES
      node->u.ll = value;
      #else
      node->u.l = value;
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
      node->u.f = (float)value;
      node->type = type;
      break;
    case LLDN_DOUBLE:
      node->u.d = (double)value;
      node->type = type;
      break;
    #ifdef BIG_TYPES
    case LLDN_LONG_DOUBLE:
      node->u.ld = value;
      node->type = type;
      break;
    #endif
  }
}

void LNSetStrByType(LLStringNode *node, LLStringType type, LLVoid string)
{
  switch (type)
  {
    default:
    case LLSN_STRING:
      node->u.s = __strdup((char *)string);
      node->type = type;
      break;
    #ifdef WCHAR_SUPPORT
    case LLSN_WIDE:
      node->u.w = __wstrdup((wchar_t *)string);
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
    default:     return sizeof(LLKeyedNode);
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
    default:     return sizeof(LinkNode);
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
    dest->u.s = __strdup(source->u.s);
    break;
  #ifdef WCHAR_SUPPORT
  case LLSN_WIDE:
    dest->u.w = __wstrdup(source->u.w);
    break;
  #endif
  }

  return dest;
}


#pragma mark - Initialization Functions

/* Push methods without keyed or named values */
struct LinkList *_LLPushBool(struct LinkList *list, LLBoolean data);
struct LinkList *_LLPushChar(struct LinkList *list, char data);
struct LinkList *_LLPushUChar(struct LinkList *list, unsigned char data);
struct LinkList *_LLPushShort(struct LinkList *list, short data);
struct LinkList *_LLPushUShort(struct LinkList *list, unsigned short data);
struct LinkList *_LLPushInt(struct LinkList *list, int data);
struct LinkList *_LLPushUInt(struct LinkList *list, unsigned int data);
struct LinkList *_LLPushLong(struct LinkList *list, long data);
struct LinkList *_LLPushULong(struct LinkList *list, unsigned long data);
struct LinkList *_LLPushFloat(struct LinkList *list, float data);
struct LinkList *_LLPushDouble(struct LinkList *list, double data);
struct LinkList *_LLPushString(struct LinkList *list, char *data);
struct LinkList *_LLPushVoid(struct LinkList *list, LLVoid data);

/* Push methods with keyed or named values */
struct LinkList *_LLPushKBool(struct LinkList *list, LLKey key, LLBoolean data);
struct LinkList *_LLPushKChar(struct LinkList *list, LLKey key, char data);
struct LinkList *_LLPushKUChar(struct LinkList *list, LLKey key, unsigned char data);
struct LinkList *_LLPushKShort(struct LinkList *list, LLKey key, short data);
struct LinkList *_LLPushKUShort(struct LinkList *list, LLKey key, unsigned short data);
struct LinkList *_LLPushKInt(struct LinkList *list, LLKey key, int data);
struct LinkList *_LLPushKUInt(struct LinkList *list, LLKey key, unsigned int data);
struct LinkList *_LLPushKLong(struct LinkList *list, LLKey key, long data);
struct LinkList *_LLPushKULong(struct LinkList *list, LLKey key, unsigned long data);
struct LinkList *_LLPushKFloat(struct LinkList *list, LLKey key, float data);
struct LinkList *_LLPushKDouble(struct LinkList *list, LLKey key, double data);
struct LinkList *_LLPushKString(struct LinkList *list, LLKey key, char *data);
struct LinkList *_LLPushKVoid(struct LinkList *list, LLKey key, LLVoid data);

/* Pop methods for unnamed/unkeyed values - LIFO */
LinkNode       *_LLPop(struct LinkList *list);
LLBoolean       _LLPopBool(struct LinkList *list);
char            _LLPopChar(struct LinkList *list);
unsigned char   _LLPopUChar(struct LinkList *list);
short           _LLPopShort(struct LinkList *list);
unsigned short  _LLPopUShort(struct LinkList *list);
int             _LLPopInt(struct LinkList *list);
unsigned int    _LLPopUInt(struct LinkList *list);
long            _LLPopLong(struct LinkList *list);
unsigned long   _LLPopULong(struct LinkList *list);
float           _LLPopFloat(struct LinkList *list);
double          _LLPopDouble(struct LinkList *list);
char           *_LLPopString(struct LinkList *list);
LLVoid          _LLPopVoid(struct LinkList *list);

/* Pop methods for named/keyed values - LIFO */
LLBoolean       _LLPopKBool(struct LinkList *list, LLKey key);
char            _LLPopKChar(struct LinkList *list, LLKey key);
unsigned char   _LLPopKUChar(struct LinkList *list, LLKey key);
short           _LLPopKShort(struct LinkList *list, LLKey key);
unsigned short  _LLPopKUShort(struct LinkList *list, LLKey key);
int             _LLPopKInt(struct LinkList *list, LLKey key);
unsigned int    _LLPopKUInt(struct LinkList *list, LLKey key);
long            _LLPopKLong(struct LinkList *list, LLKey key);
unsigned long   _LLPopKULong(struct LinkList *list, LLKey key);
float           _LLPopKFloat(struct LinkList *list, LLKey key);
double          _LLPopKDouble(struct LinkList *list, LLKey key);
char           *_LLPopKString(struct LinkList *list, LLKey key);
LLVoid          _LLPopKVoid(struct LinkList *list, LLKey key);

/* Dequeue methods for unnamed/unkeyed values - FIFO */
LinkNode       *_LLDequeue(struct LinkList *list);
LLBoolean       _LLDequeueBool(struct LinkList *list);
char            _LLDequeueChar(struct LinkList *list);
unsigned char   _LLDequeueUChar(struct LinkList *list);
short           _LLDequeueShort(struct LinkList *list);
unsigned short  _LLDequeueUShort(struct LinkList *list);
int             _LLDequeueInt(struct LinkList *list);
unsigned int    _LLDequeueUInt(struct LinkList *list);
long            _LLDequeueLong(struct LinkList *list);
unsigned long   _LLDequeueULong(struct LinkList *list);
float           _LLDequeueFloat(struct LinkList *list);
double          _LLDequeueDouble(struct LinkList *list);
char           *_LLDequeueString(struct LinkList *list);
LLVoid          _LLDequeueVoid(struct LinkList *list);

/* Dequeue methods for named/keyed values - FIFO */
LLBoolean       _LLDequeueKBool(struct LinkList *list, LLKey key);
char            _LLDequeueKChar(struct LinkList *list, LLKey key);
unsigned char   _LLDequeueKUChar(struct LinkList *list, LLKey key);
short           _LLDequeueKShort(struct LinkList *list, LLKey key);
unsigned short  _LLDequeueKUShort(struct LinkList *list, LLKey key);
int             _LLDequeueKInt(struct LinkList *list, LLKey key);
unsigned int    _LLDequeueKUInt(struct LinkList *list, LLKey key);
long            _LLDequeueKLong(struct LinkList *list, LLKey key);
unsigned long   _LLDequeueKULong(struct LinkList *list, LLKey key);
float           _LLDequeueKFloat(struct LinkList *list, LLKey key);
double          _LLDequeueKDouble(struct LinkList *list, LLKey key);
char           *_LLDequeueKString(struct LinkList *list, LLKey key);
LLVoid          _LLDequeueKVoid(struct LinkList *list, LLKey key);


#ifdef BIG_TYPES
struct LinkList *_LLPushLongLong(struct LinkList *list, long long data);
struct LinkList *_LLPushULongLong(struct LinkList *list, unsigned long long data);
struct LinkList *_LLPushLongDouble(struct LinkList *list, long double data);
long long _LLPopLongLong(struct LinkList *list);
long long _LLPopKLongLong(struct LinkList *list, LLKey key);
unsigned long long _LLPopULongLong(struct LinkList *list);
unsigned long long _LLPopKULongLong(struct LinkList *list, LLKey key);
long double _LLPopLongDouble(struct LinkList *list);
long double _LLPopKLongDouble(struct LinkList *list, LLKey key);
long long _LLDequeueLongLong(struct LinkList *list);
long long _LLDequeueKLongLong(struct LinkList *list, LLKey key);
unsigned long long _LLDequeueULongLong(struct LinkList *list);
unsigned long long _LLDequeueKULongLong(struct LinkList *list, LLKey key);
long double _LLDequeueLongDouble(struct LinkList *list);
long double _LLDequeueKLongDouble(struct LinkList *list, LLKey key);
#endif

#ifdef WCHAR_SUPPORT
struct LinkList *_LLPushWString(struct LinkList *list, wchar_t *data);
struct LinkList *_LLPushKWString(struct LinkList *list, LLKey key, wchar_t *data);
wchar_t *_LLPopWString(struct LinkList *list);
wchar_t *_LLPopKWString(struct LinkList *list, LLKey key);
wchar_t *_LLDequeueWString(struct LinkList *list, LLKey key);
wchar_t *_LLDequeueKWString(struct LinkList *list, LLKey key);
#endif


LinkList *LLInit(LinkList *list, LLBoolean alloc)
{
  size_t size = sizeof(LinkList);
  
  if (alloc) list = (LinkList *)malloc(size);
  if (!list) return NULL;
  
  memset(list, 0L, size);

  list->pushBool = _LLPushBool;
  list->pushChar = _LLPushChar;
  list->pushUChar = _LLPushUChar;
  list->pushShort = _LLPushShort;
  list->pushUShort = _LLPushUShort;
  list->pushInt = _LLPushInt;
  list->pushUInt = _LLPushUInt;
  list->pushLong = _LLPushLong;
  list->pushULong = _LLPushULong;
  #ifdef BIG_TYPES
  list->pushLongLong = _LLPushLongLong;
  list->pushULongLong = _LLPushULongLong;
  #endif

  list->pushFloat = _LLPushFloat;
  list->pushDouble = _LLPushDouble;
  #ifdef BIG_TYPES
  list->pushLongDouble = _LLPushLongDouble;
  #endif

  list->pushVoid = _LLPushVoid;
  list->pushString = _LLPushString;
  #ifdef WCHAR_SUPPORT
  list->pushWString = _LLPushWString;
  #endif

  list->pushKBool = _LLPushKBool;
  list->pushKChar = _LLPushKChar;
  list->pushKUChar = _LLPushKUChar;
  list->pushKShort = _LLPushKShort;
  list->pushKUShort = _LLPushKUShort;
  list->pushKInt = _LLPushKInt;
  list->pushKUInt = _LLPushKUInt;
  list->pushKLong = _LLPushKLong;
  list->pushKULong = _LLPushKULong;
  #ifdef BIG_TYPES
  list->pushKLongLong = _LLPushKLongLong;
  list->pushKULongLong = _LLPushKULongLong;
  #endif

  list->pushKFloat = _LLPushKFloat;
  list->pushKDouble = _LLPushKDouble;
  #ifdef BIG_TYPES
  list->pushKLongDouble = _LLPushKLongDouble;
  #endif

  list->pushKVoid = _LLPushKVoid;
  list->pushKString = _LLPushKString;
  #ifdef WCHAR_SUPPORT
  list->pushKWString = _LLPushKWString;
  #endif
  
  list->pop = _LLPop;
  list->popBool = _LLPopBool;
  list->popChar = _LLPopChar;
  list->popUChar = _LLPopUChar;
  list->popShort = _LLPopShort;
  list->popUShort = _LLPopUShort;
  list->popInt = _LLPopInt;
  list->popUInt = _LLPopUInt;
  list->popLong = _LLPopLong;
  list->popULong = _LLPopULong;
  #ifdef BIG_TYPES
  list->popLongLong = _LLPopLongLong;
  list->popULongLong = _LLPopULongLong;
  #endif
  
  list->popFloat = _LLPopFloat;
  list->popDouble = _LLPopDouble;
  #ifdef BIG_TYPES
  list->popLongDouble = _LLPopLongDouble;
  #endif
  
  list->popString = _LLPopString;
  list->popVoid = _LLPopVoid;
  #ifdef WCHAR_SUPPORT
  list->popWString = _LLPopWString;
  #endif

  list->popKBool = _LLPopKBool;
  list->popKChar = _LLPopKChar;
  list->popKUChar = _LLPopKUChar;
  list->popKShort = _LLPopKShort;
  list->popKUShort = _LLPopKUShort;
  list->popKInt = _LLPopKInt;
  list->popKUInt = _LLPopKUInt;
  list->popKLong = _LLPopKLong;
  list->popKULong = _LLPopKULong;
  #ifdef BIG_TYPES
  list->popKLongLong = _LLPopKLongLong;
  list->popKULongLong = _LLPopKULongLong;
  #endif

  list->popKFloat = _LLPopKFloat;
  list->popKDouble = _LLPopKDouble;
  #ifdef BIG_TYPES
  list->popKLongDouble = _LLPopKLongDouble;
  #endif

  list->popKString = _LLPopKString;
  list->popKVoid = _LLPopKVoid;
  #ifdef WCHAR_SUPPORT
  list->popKWString = _LLPopKWString;
  #endif

  list->dequeue = _LLDequeue;
  list->dequeueBool = _LLDequeueBool;
  list->dequeueChar = _LLDequeueChar;
  list->dequeueUChar = _LLDequeueUChar;
  list->dequeueShort = _LLDequeueShort;
  list->dequeueUShort = _LLDequeueUShort;
  list->dequeueInt = _LLDequeueInt;
  list->dequeueUInt = _LLDequeueUInt;
  list->dequeueLong = _LLDequeueLong;
  list->dequeueULong = _LLDequeueULong;
  #ifdef BIG_TYPES
  list->dequeueLongLong = _LLDequeueLongLong;
  list->dequeueULongLong = _LLDequeueULongLong;
  #endif

  list->dequeueFloat = _LLDequeueFloat;
  list->dequeueDouble = _LLDequeueDouble;
  #ifdef BIG_TYPES
  list->dequeueLongDouble = _LLDequeueLongDouble;
  #endif

  list->dequeueString = _LLDequeueString;
  list->dequeueVoid = _LLDequeueVoid;
  #ifdef WCHAR_SUPPORT
  list->dequeueWString = _LLDequeueWString;
  #endif

  list->dequeueKBool = _LLDequeueKBool;
  list->dequeueKChar = _LLDequeueKChar;
  list->dequeueKUChar = _LLDequeueKUChar;
  list->dequeueKShort = _LLDequeueKShort;
  list->dequeueKUShort = _LLDequeueKUShort;
  list->dequeueKInt = _LLDequeueKInt;
  list->dequeueKUInt = _LLDequeueKUInt;
  list->dequeueKLong = _LLDequeueKLong;
  list->dequeueKULong = _LLDequeueKULong;
  #ifdef BIG_TYPES
  list->dequeueKLongLong = _LLDequeueKLongLong;
  list->dequeueKULongLong = _LLDequeueKULongLong;
  #endif

  list->dequeueKFloat = _LLDequeueKFloat;
  list->dequeueKDouble = _LLDequeueKDouble;
  #ifdef BIG_TYPES
  list->dequeueKLongDouble = _LLDequeueKLongDouble;
  #endif

  list->dequeueKString = _LLDequeueKString;
  list->dequeueKVoid = _LLDequeueKVoid;
  #ifdef WCHAR_SUPPORT
  list->dequeueKWString = _LLDequeueKWString;
  #endif

  /* TODO make dequeue keyed values start from a different end */

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


LLVoidNode *LNVInit(LLVoidNode *node, LLBoolean alloc)
{
  size_t size = sizeof(LLVoidNode);

  if (alloc) node = (LLVoidNode *)malloc(size);
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


LLKeyedVoid *LNKVInit(LLKeyedVoid *node, LLBoolean alloc)
{
  size_t size = sizeof(LLKeyedVoid);

  if (alloc) node = (LLKeyedVoid *)malloc(size);
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

LinkNode *LNCreate(LLVoid value, LinkNodeDataType type)
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

LLStringNode *LNSCreate(LLVoid string, LLStringType type)
{
  LLStringNode *node = LNSInit(NULL, Yes);

  LNSetStrByType(node, type, string);
  return node;
}

LLVoidNode *LNVCreate(LLVoid data)
{
  LLVoidNode *node = LNVInit(NULL, Yes);

  node->value = data;
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


LLKeyedString *LNKSCreate(LLKey key, LLVoid string, LLStringType type)
{
  LLKeyedNode *keyNode = LNKCreate(key, LLDefaultHashFunction);
  LLStringNode *data = LNSCreate(string, type);
  LLKeyedString *node = LNKSInit(NULL, Yes);
  
  memcpy(&node->keyedNode, keyNode, sizeof(LLKeyedNode));
  memcpy(&node->string, data, sizeof(LLStringNode));
  return node;
}


LLKeyedVoid *LNKVCreate(LLKey key, LLVoid value)
{
  LLKeyedNode *keyNode = LNKCreate(key, LLDefaultHashFunction);
  LLVoidNode *data = LNVCreate(value);
  LLKeyedVoid *node = LNKVInit(NULL, Yes);

  memcpy(&node->keyedNode, keyNode, sizeof(LLKeyedNode));
  memcpy(&node->voidNode, data, sizeof(LLVoidNode));
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
  LLVoid data = node->value;

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
        /* Can't know about custom types; skip freeing mem. Up to user */

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
          if (keyedStr->string.u.s) free(keyedStr->string.u.s);
          free(keyedStr);
        }
        if (strNode) {
          if (strNode->u.s) free(strNode->u.s);
          free(strNode);
        }

        break;
      case LN_VOID:
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


LinkNode *LLPushString(LinkList *list, LLVoid string, LLStringType type)
{
  LLStringNode *data = LNSCreate(string, type);
  LinkNode *node = LNCreate(data, LN_STRING);
  LLPush(list, node);
  return node;
}


LinkNode *LLPushVoid(LinkList *list, LLVoid value)
{
  LLVoidNode *data = LNVCreate(value);
  LinkNode *node = LNCreate(data, LN_VOID);
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


LinkNode *LLPushKeyedString(LinkList *list, LLKey key, LLVoid string, LLStringType type)
{
  LLKeyedString *data = LNKSCreate(key, string, type);
  LinkNode *node = LNCreate(data, LN_STRING | LN_KEYED);
  LLPush(list, node);
  return node;  
}


LinkNode *LLPushKeyedVoid(LinkList *list, LLKey key, LLVoid value)
{
  LLKeyedVoid *data = LNKVCreate(key, value);
  LinkNode *node = LNCreate(data, LN_VOID | LN_KEYED);
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


LLVoidNode *LLPopVoid(LinkList *list)
{
  LinkNode *node = LLPopNode(list);
  LLVoidNode *data;

  if (!node) return NULL;
  else {
    data = (LLVoidNode*)node->value;
    return data;
  }
}


LLBoolean LLPopKeyedBoolean(LinkList *list, LLKey key)
{
  LinkNode *node = LLFindKeyed(list, key);
  LLKeyedBool *data = node && node->value ? (LLKeyedBool *)node->value : 0L;

  if (node) LLRemoveNode(list, node);
  
  return data ? data->boolean : No;
}


LLKeyedInteger *LLPopKeyedInteger(LinkList *list, LLKey key)
{
  LinkNode *node = LLFindKeyed(list, key);
  LLKeyedInteger *data = node && node->value 
    ? (LLKeyedInteger *)node->value
    : 0L;

  if (node) LLRemoveNode(list, node);

  return data ? data : NULL;
}


LLKeyedDecimal *LLPopKeyedDecimal(LinkList *list, LLKey key)
{
  LinkNode *node = LLFindKeyed(list, key);
  LLKeyedDecimal *data = node && node->value 
    ? (LLKeyedDecimal *)node->value
    : 0L;

  if (node) LLRemoveNode(list, node);

  return data ? data : NULL;  
}


LLKeyedString *LLPopKeyedString(LinkList *list, LLKey key)
{
  LinkNode *node = LLFindKeyed(list, key);
  LLKeyedString *data = node && node->value ? (LLKeyedString *)node->value : 0L;

  if (node) LLRemoveNode(list, node);
  
  return data ? data : NULL;
}


LLKeyedVoid *LLPopKeyedVoid(LinkList *list, LLKey key)
{
  LinkNode *node = LLFindKeyed(list, key);
  LLKeyedVoid *data = node && node->value ? (LLKeyedVoid *)node->value : 0L;

  if (node) LLRemoveNode(list, node);
  
  return data ? data : NULL;
}

#pragma mark - List Dequeue Functions

LinkNode *LLDequeueNode(LinkList *list)
{
  LinkNode *node = list && list->head ? list->head : NULL;

  if (!node || !list) return NULL;
  list->head = node->next;
  list->head->prev = NULL;

  if (list->tail == list->head) {
    list->tail->next = NULL;
    list->tail->prev = NULL;
  }

  node->next = NULL;
  node->prev = NULL;

  return node;
}

LLBoolean LLDequeueBoolean(LinkList *list)
{
  LinkNode *node = LLDequeueNode(list);
  LLBoolNode *data;

  if (!node) return No;
  else {
    data = (LLBoolNode *)node->value;
    return data->boolean;
  }
}

LLIntegerNode *LLDequeueInteger(LinkList *list)
{
  LinkNode *node = LLDequeueNode(list);
  LLIntegerNode *data;

  if (!node) return NULL;
  else {
    data = (LLIntegerNode*)node->value;
    return data;
  }
}


LLDecimalNode *LLDequeueDecimal(LinkList *list)
{
  LinkNode *node = LLDequeueNode(list);
  LLDecimalNode *data;

  if (!node) return NULL;
  else {
    data = (LLDecimalNode*)node->value;
    return data;
  }
}


LLStringNode *LLDequeueString(LinkList *list)
{
  LinkNode *node = LLDequeueNode(list);
  LLStringNode *data;

  if (!node) return NULL;
  else {
    data = (LLStringNode*)node->value;
    return data;
  }
}


LLVoidNode *LLDequeueVoid(LinkList *list)
{
  LinkNode *node = LLDequeueNode(list);
  LLVoidNode *data;

  if (!node) return NULL;
  else {
    data = (LLVoidNode *)node->value;
    return data;
  }
}


LLBoolean LLDequeueKeyedBoolean(LinkList *list, LLKey key)
{
  LinkNode *node = LLFindKeyed(list, key);
  LLKeyedBool *data = node && node->value ? (LLKeyedBool *)node->value : 0L;

  return data ? data->boolean : No;
}


LLKeyedInteger *LLDequeueKeyedInteger(LinkList *list, LLKey key)
{
  LinkNode *node = LLFindKeyed(list, key);
  LLKeyedInteger *data = node && node->value
                         ? (LLKeyedInteger *)node->value
                         : 0L;

  return data ? data : NULL;
}


LLKeyedDecimal *LLDequeueKeyedDecimal(LinkList *list, LLKey key)
{
  LinkNode *node = LLFindKeyed(list, key);
  LLKeyedDecimal *data = node && node->value
                         ? (LLKeyedDecimal *)node->value
                         : 0L;

  return data ? data : NULL;
}


LLKeyedString *LLDequeueKeyedString(LinkList *list, LLKey key)
{
  LinkNode *node = LLFindKeyed(list, key);
  LLKeyedString *data = node && node->value ? (LLKeyedString *)node->value : NULL;

  return data ? data : NULL;
}


LLKeyedVoid *LLDequeueKeyedVoid(LinkList *list, LLKey key)
{
  LinkNode *node = LLFindKeyed(list, key);
  LLKeyedVoid *data = node && node->value ? (LLKeyedVoid *)node->value : NULL;

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


void LLRemoveByData(LinkList *list, LLVoid data)
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

#pragma mark - Link List "Instance" Methods

/* Push methods without keyed or named values */
struct LinkList *_LLPushBool(struct LinkList *list, LLBoolean data)
{
  LLPushBoolean(list, data);
  return list;
}

struct LinkList *_LLPushChar(struct LinkList *list, char data)
{
  LLPushInteger(list, data, LLIN_CHAR);
  return list;
}

struct LinkList *_LLPushUChar(struct LinkList *list, unsigned char data)
{
  LLPushInteger(list, data, LLIN_CHAR | LLIN_UNSIGNED);
  return list;
}

struct LinkList *_LLPushShort(struct LinkList *list, short data)
{
  LLPushInteger(list, data, LLIN_SHORT);
  return list;
}

struct LinkList *_LLPushUShort(struct LinkList *list, unsigned short data)
{
  LLPushInteger(list, data, LLIN_SHORT | LLIN_UNSIGNED);
  return list;
}

struct LinkList *_LLPushInt(struct LinkList *list, int data)
{
  LLPushInteger(list, data, LLIN_INT);
  return list;
}

struct LinkList *_LLPushUInt(struct LinkList *list, unsigned int data)
{
  LLPushInteger(list, data, LLIN_INT | LLIN_UNSIGNED);
  return list;
}

struct LinkList *_LLPushLong(struct LinkList *list, long data)
{
  LLPushInteger(list, data, LLIN_LONG);
  return list;
}

struct LinkList *_LLPushULong(struct LinkList *list, unsigned long data)
{
  LLPushInteger(list, data, LLIN_LONG | LLIN_UNSIGNED);
  return list;
}

struct LinkList *_LLPushFloat(struct LinkList *list, float data)
{
  LLPushDecimal(list, data, LLDN_FLOAT);
  return list;
}

struct LinkList *_LLPushDouble(struct LinkList *list, double data)
{
  LLPushDecimal(list, data, LLDN_DOUBLE);
  return list;
}

struct LinkList *_LLPushString(struct LinkList *list, char *data)
{
  LLPushString(list, data, LLSN_STRING);
  return list;
}

struct LinkList *_LLPushVoid(struct LinkList *list, LLVoid data)
{
  LLPushVoid(list, data);
  return list;
}


/* Push methods with keyed or named values */
struct LinkList *_LLPushKBool(struct LinkList *list, LLKey key, LLBoolean data)
{
  LLPushKeyedBoolean(list, key, data);
  return list;
}

struct LinkList *_LLPushKChar(struct LinkList *list, LLKey key, char data)
{
  LLPushKeyedInteger(list, key, data, LLIN_CHAR);
  return list;
}

struct LinkList *_LLPushKUChar(struct LinkList *list, LLKey key, unsigned char data)
{
  LLPushKeyedInteger(list, key, data, LLIN_CHAR | LLIN_UNSIGNED);
  return list;
}

struct LinkList *_LLPushKShort(struct LinkList *list, LLKey key, short data)
{
  LLPushKeyedInteger(list, key, data, LLIN_SHORT);
  return list;
}

struct LinkList *_LLPushKUShort(struct LinkList *list, LLKey key, unsigned short data)
{
  LLPushKeyedInteger(list, key, data, LLIN_SHORT | LLIN_UNSIGNED);
  return list;
}

struct LinkList *_LLPushKInt(struct LinkList *list, LLKey key, int data)
{
  LLPushKeyedInteger(list, key, data, LLIN_INT);
  return list;
}

struct LinkList *_LLPushKUInt(struct LinkList *list, LLKey key, unsigned int data)
{
  LLPushKeyedInteger(list, key, data, LLIN_INT | LLIN_UNSIGNED);
  return list;
}

struct LinkList *_LLPushKLong(struct LinkList *list, LLKey key, long data)
{
  LLPushKeyedInteger(list, key, data, LLIN_LONG);
  return list;
}

struct LinkList *_LLPushKULong(struct LinkList *list, LLKey key, unsigned long data)
{
  LLPushKeyedInteger(list, key, data, LLIN_LONG | LLIN_UNSIGNED);
  return list;
}

struct LinkList *_LLPushKFloat(struct LinkList *list, LLKey key, float data)
{
  LLPushKeyedDecimal(list, key, data, LLDN_FLOAT);
  return list;
}

struct LinkList *_LLPushKDouble(struct LinkList *list, LLKey key, double data)
{
  LLPushKeyedDecimal(list, key, data, LLDN_DOUBLE);
  return list;
}

struct LinkList *_LLPushKString(struct LinkList *list, LLKey key, char *data)
{
  LLPushKeyedString(list, key, data, LLSN_STRING);
  return list;
}

struct LinkList *_LLPushKVoid(struct LinkList *list, LLKey key, LLVoid data)
{
  LLPushKeyedVoid(list, key, data);
  return list;
}


/* Pop methods for unnamed/unkeyed values - LIFO */
LinkNode *_LLPop(struct LinkList *list)
{
  return LLPopNode(list);
}

LLBoolean _LLPopBool(struct LinkList *list)
{
  return LLPopBoolean(list);
}

char _LLPopChar(struct LinkList *list)
{
  return LLPopInteger(list)->u.c;
}

unsigned char _LLPopUChar(struct LinkList *list)
{
  return LLPopInteger(list)->u.uc;
}

short _LLPopShort(struct LinkList *list)
{
  return LLPopInteger(list)->u.s;
}

unsigned short _LLPopUShort(struct LinkList *list)
{
  return LLPopInteger(list)->u.us;
}

int _LLPopInt(struct LinkList *list)
{
  return LLPopInteger(list)->u.i;
}

unsigned int _LLPopUInt(struct LinkList *list)
{
  return LLPopInteger(list)->u.ui;
}

long _LLPopLong(struct LinkList *list)
{
  return LLPopInteger(list)->u.l;
}

unsigned long _LLPopULong(struct LinkList *list)
{
  return LLPopInteger(list)->u.ul;
}

float _LLPopFloat(struct LinkList *list)
{
  return LLPopDecimal(list)->u.f;
}

double _LLPopDouble(struct LinkList *list)
{
  return LLPopDecimal(list)->u.f;
}

char *_LLPopString(struct LinkList *list)
{
  return LLPopString(list)->u.s;
}

LLVoid _LLPopVoid(struct LinkList *list)
{
  return LLPopVoid(list)->value;
}


/* Pop methods for named/keyed values - LIFO */
LLBoolean _LLPopKBool(struct LinkList *list, LLKey key)
{
  return LLPopKeyedBoolean(list, key);
}

char _LLPopKChar(struct LinkList *list, LLKey key)
{
  return LLPopKeyedInteger(list, key)->integer.u.c;
}

unsigned char _LLPopKUChar(struct LinkList *list, LLKey key)
{
  return LLPopKeyedInteger(list, key)->integer.u.uc;
}

short _LLPopKShort(struct LinkList *list, LLKey key)
{
  return LLPopKeyedInteger(list, key)->integer.u.s;
}

unsigned short _LLPopKUShort(struct LinkList *list, LLKey key)
{
  return LLPopKeyedInteger(list, key)->integer.u.us;
}

int _LLPopKInt(struct LinkList *list, LLKey key)
{
  return LLPopKeyedInteger(list, key)->integer.u.i;
}

unsigned int _LLPopKUInt(struct LinkList *list, LLKey key)
{
  return LLPopKeyedInteger(list, key)->integer.u.ui;
}

long _LLPopKLong(struct LinkList *list, LLKey key)
{
  return LLPopKeyedInteger(list, key)->integer.u.l;
}

unsigned long _LLPopKULong(struct LinkList *list, LLKey key)
{
  return LLPopKeyedInteger(list, key)->integer.u.ul;
}

float _LLPopKFloat(struct LinkList *list, LLKey key)
{
  return LLPopKeyedDecimal(list, key)->decimal.u.f;
}

double _LLPopKDouble(struct LinkList *list, LLKey key)
{
  return LLPopKeyedDecimal(list, key)->decimal.u.d;
}

char *_LLPopKString(struct LinkList *list, LLKey key)
{
  return LLPopKeyedString(list, key)->string.u.s;
}

LLVoid _LLPopKVoid(struct LinkList *list, LLKey key)
{
  return LLPopKeyedVoid(list, key)->voidNode->value;
}


/* Dequeue methods for unnamed/unkeyed values - FIFO */
LinkNode *_LLDequeue(struct LinkList *list)
{
  return LLDequeueNode(list);
}

LLBoolean _LLDequeueBool(struct LinkList *list)
{
  return LLDequeueBoolean(list);
}

char _LLDequeueChar(struct LinkList *list)
{
  return LLDequeueInteger(list)->u.c;
}

unsigned char _LLDequeueUChar(struct LinkList *list)
{
  return LLDequeueInteger(list)->u.uc;
}

short _LLDequeueShort(struct LinkList *list)
{
  return LLDequeueInteger(list)->u.s;
}

unsigned short _LLDequeueUShort(struct LinkList *list)
{
  return LLDequeueInteger(list)->u.us;
}

int _LLDequeueInt(struct LinkList *list)
{
  return LLDequeueInteger(list)->u.i;
}

unsigned int _LLDequeueUInt(struct LinkList *list)
{
  return LLDequeueInteger(list)->u.ui;
}

long _LLDequeueLong(struct LinkList *list)
{
  return LLDequeueInteger(list)->u.l;
}

unsigned long _LLDequeueULong(struct LinkList *list)
{
  return LLDequeueInteger(list)->u.ul;
}

float _LLDequeueFloat(struct LinkList *list)
{
  return LLDequeueDecimal(list)->u.f;
}

double _LLDequeueDouble(struct LinkList *list)
{
  return LLDequeueDecimal(list)->u.d;
}

char *_LLDequeueString(struct LinkList *list)
{
  return LLDequeueString(list)->u.s;
}

LLVoid _LLDequeueVoid(struct LinkList *list)
{
  return LLDequeueVoid(list)->value;
}


/* Dequeue methods for named/keyed values - FIFO */
LLBoolean _LLDequeueKBool(struct LinkList *list, LLKey key)
{
  return LLDequeueKeyedBoolean(list, key);
}

char _LLDequeueKChar(struct LinkList *list, LLKey key)
{
  return LLDequeueKeyedInteger(list, key)->integer.u.c;
}

unsigned char _LLDequeueKUChar(struct LinkList *list, LLKey key)
{
  return LLDequeueKeyedInteger(list, key)->integer.u.uc;
}

short _LLDequeueKShort(struct LinkList *list, LLKey key)
{
  return LLDequeueKeyedInteger(list, key)->integer.u.s;
}

unsigned short _LLDequeueKUShort(struct LinkList *list, LLKey key)
{
  return LLDequeueKeyedInteger(list, key)->integer.u.us;
}

int _LLDequeueKInt(struct LinkList *list, LLKey key)
{
  return LLDequeueKeyedInteger(list, key)->integer.u.i;
}

unsigned int _LLDequeueKUInt(struct LinkList *list, LLKey key)
{
  return LLDequeueKeyedInteger(list, key)->integer.u.ui;
}

long _LLDequeueKLong(struct LinkList *list, LLKey key)
{
  return LLDequeueKeyedInteger(list, key)->integer.u.l;
}

unsigned long _LLDequeueKULong(struct LinkList *list, LLKey key)
{
  return LLDequeueKeyedInteger(list, key)->integer.u.ul;
}

float _LLDequeueKFloat(struct LinkList *list, LLKey key)
{
  return LLDequeueKeyedDecimal(list, key)->decimal.u.f;
}

double _LLDequeueKDouble(struct LinkList *list, LLKey key)
{
  return LLDequeueKeyedDecimal(list, key)->decimal.u.d;
}

char *_LLDequeueKString(struct LinkList *list, LLKey key)
{
  return LLDequeueKeyedString(list, key)->string.u.s;
}

LLVoid _LLDequeueKVoid(struct LinkList *list, LLKey key)
{
  return LLDequeueKeyedVoid(list, key)->voidNode->value;
}


#ifdef BIG_TYPES
struct LinkList *_LLPushLongLong(struct LinkList *list, long long data)
{
  LLPushInteger(list, data, LLIN_LONG_LONG);
  return list;
}

struct LinkList *_LLPushULongLong(struct LinkList *list, unsigned long long data)
{
  LLPushInteger(list, data, LLIN_LONG_LONG | LLIN_UNSIGNED);
  return list;
}

struct LinkList *_LLPushLongDouble(struct LinkList *list, long double data)
{
  LLPushDecimal(list, data, LLDN_LONG_DOUBLE);
  return list;
}

long long _LLPopLongLong(struct LinkList *list)
{
  return LLPopInteger(list)->u.ll;
}

long long _LLPopKLongLong(struct LinkList *list, LLKey key)
{
  return LLPopKeyedInteger(list, key)->integer.u.ll;
}

unsigned long long _LLPopULongLong(struct LinkList *list)
{
  return LLPopInteger(list)->u.ull;
}

unsigned long long _LLPopKULongLong(struct LinkList *list, LLKey key)
{
  return LLPopKeyedInteger(list, key)->integer.u.ull;
}

long double _LLPopLongDouble(struct LinkList *list)
{
  return LLPopDecimal(list)->u.ld;
}

long double _LLPopKLongDouble(struct LinkList *list, LLKey key)
{
  return LLPopKeyedDecimal(list, key)->decimal.u.ld;
}

long long _LLDequeueLongLong(struct LinkList *list)
{
  return LLDequeueInteger(list)->u.ll;
}

long long _LLDequeueKLongLong(struct LinkList *list, LLKey key)
{
  return LLDequeueKeyedInteger(list, key)->integer.u.ll;
}

unsigned long long _LLDequeueULongLong(struct LinkList *list)
{
  return LLDequeueInteger(list)->u.ull;
}

unsigned long long _LLDequeueKULongLong(struct LinkList *list, LLKey key)
{
  return LLDequeueKeyedInteger(list, key)->integer.u.ull;
}

long double _LLDequeueLongDouble(struct LinkList *list)
{
  return LLDequeueDecimal(list)->u.ld;
}

long double _LLDequeueKLongDouble(struct LinkList *list, LLKey key)
{
  return LLDequeueKeyedDecimal(list, key)->decimal.u.ld;
}

#endif

#ifdef WCHAR_SUPPORT
struct LinkList *_LLPushWString(struct LinkList *list, wchar_t *data)
{
  LLPushString(list, data, LLSN_WIDE);
  return list;
}

struct LinkList *_LLPushKWString(struct LinkList *list, LLKey key, wchar_t *data)
{
  LLPushKeyedString(list, key, data, LLSN_WIDE);
  return list;
}

wchar_t *_LLPopWString(struct LinkList *list)
{
  return LLPopString(list)->u.w;
}

wchar_t *_LLPopKWString(struct LinkList *list, LLKey key)
{
  return LLPopKeyedString(list, key)->string.u.w;
}

wchar_t *_LLDequeueWString(struct LinkList *list, LLKey key)
{
  return LLDequeueString(list)->u.w;
}

wchar_t *_LLDequeueKWString(struct LinkList *list, LLKey key)
{
  return LLDequeueKeyedString(list, key)->string.u.w;
}

#endif

