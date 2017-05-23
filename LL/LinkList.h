#ifndef LINKED_LIST_H
#define LINKED_LIST_H

#ifdef WCHAR_SUPPORT
#include <wchar.h>
#endif

#include <stdio.h>

#pragma mark - Base Types

typedef void *VoidPtr;

#ifndef LL_KEY_TYPE
#define LL_KEY_TYPE char *
#endif

typedef LL_KEY_TYPE LLKey;

typedef unsigned int (*LLHashFn)(LLKey key, int limit);
typedef size_t LLHashLimit;

#ifndef BIG_TYPES
#define MAX_INT_TYPE long
#define MAX_DEC_TYPE double
#else
#define MAX_INT_TYPE long long
#define MAX_DEC_TYPE long double
#endif

#pragma mark - Enums

typedef enum
{
  False = 0,
  No = 0,
  True = 1,
  Yes = 1
} LLBoolean;

typedef enum 
{
  LLIN_CHAR = 0,
  LLIN_SHORT = 2,
  LLIN_INT = 4,
  LLIN_LONG = 8,
  #ifdef BIG_TYPES
  LLIN_LONG_LONG = 16,
  #endif
  LLIN_UNSIGNED = 256
} LLIntegerType;

typedef enum
{
  LLDN_FLOAT = 1,
  LLDN_DOUBLE = 2
  #ifdef BIG_TYPES
  , LLDN_LONG_DOUBLE = 4
  #endif
} LLDecimalType;

typedef enum
{
  #ifndef WCHAR_SUPPORT
  LLSN_WIDE = 1,
  #endif
  LLSN_STRING = 0
} LLStringType;

typedef enum 
{
  LN_USER = 0,
  LN_BOOLEAN = 1,
  LN_INTEGER = 2,
  LN_DECIMAL = 4,
  LN_STRING = 8,
  
  LN_KEYED = 256
} LinkNodeDataType;

#pragma mark - Structures

typedef struct LLKeyedNode
{
  LLKey key;
  unsigned int hashValue;
} LLKeyedNode;

typedef struct LLBoolNode
{
  LLBoolean boolean;
} LLBoolNode;

typedef struct LLKeyedBool
{
  LLKeyedNode keyedNode;
  LLBoolean boolean;
} LLKeyedBool;

typedef struct LLIntegerNode
{
  union {
    char c;
    unsigned char uc;
    short s;
    unsigned short us;
    int i;
    unsigned int ui;
    long l;
    unsigned long ul;
    #ifdef BIG_TYPES
    long long ll;
    unsigned long long ull;
    #endif
  };
  LLIntegerType type;
} LLIntegerNode;

typedef struct LLKeyedInteger
{
  LLKeyedNode keyedNode;
  LLIntegerNode integer;
} LLKeyedInteger;

typedef struct LLDecimalNode
{
  union {
    float f;
    double d;
    #ifdef BIG_TYPES
    long double ld;
    #endif
  };
  LLDecimalType type;
} LLDecimalNode;

typedef struct LLKeyedDecimal
{
  LLKeyedNode keyedNode;
  LLDecimalNode decimal;
} LLKeyedDecimal;

typedef struct LLStringNode
{
  union
  {
    char *s;
    #ifdef WCHAR_SUPPORT
    wchar_t *w;
    #endif
  };
  LLStringType type;
} LLStringNode;

typedef struct LLKeyedString 
{
  LLKeyedNode keyedNode;
  LLStringNode string;
} LLKeyedString;

typedef struct LinkNode
{
  struct LinkNode *next;
  struct LinkNode *prev;
  VoidPtr value;
  LinkNodeDataType type;
} LinkNode;

typedef struct LinkList
{
  LinkNode *head;
  LinkNode *tail;
} LinkList;

#pragma mark - Constant Exports

unsigned int LLDefaultStringHashFn(LLKey key, int limit);

extern const LLHashFn LLDefaultHashFunction;
extern const LLHashLimit LLDefaultHashLimit;

#pragma mark - Utility Functions

size_t LLDataSize(LinkNode *node);
LinkNode *LLFindKeyed(LinkList *list, LLKey key);
LLStringNode *LLDuplicateStringNode(LLStringNode *source);

#pragma mark - Initialization Functions

LinkList *LLInit(LinkList *list, LLBoolean alloc);
LinkNode *LNInit(LinkNode *node, LLBoolean alloc);

LLBoolNode *LNBInit(LLBoolNode *node, LLBoolean alloc);
LLIntegerNode *LNIInit(LLIntegerNode *node, LLBoolean alloc);
LLDecimalNode *LNDInit(LLDecimalNode *node, LLBoolean alloc);
LLStringNode *LNSInit(LLStringNode *node, LLBoolean alloc);

LLKeyedNode *LNKNInit(LLKeyedNode *node, LLBoolean alloc);
LLKeyedBool *LNKBInit(LLKeyedBool *node, LLBoolean alloc);
LLKeyedInteger *LNKIInit(LLKeyedInteger *node, LLBoolean alloc);
LLKeyedDecimal *LNKDInit(LLKeyedDecimal *node, LLBoolean alloc);
LLKeyedString *LNKSInit(LLKeyedString *node, LLBoolean alloc);

#pragma mark - Creation Functions

LinkList *LLCreate(void);
LinkNode *LNCreate(VoidPtr value, LinkNodeDataType type);

LLKeyedNode *LNKCreate(LLKey key, LLHashFn hashFunction);
LLBoolNode *LNBCreate(LLBoolean boolean);
LLIntegerNode *LNICreate(MAX_INT_TYPE value, LLIntegerType type);
LLDecimalNode *LNDCreate(MAX_DEC_TYPE value, LLDecimalType type);
LLStringNode *LNSCreate(VoidPtr string, LLStringType type);
LLKeyedBool *LNKBCreate(LLKey key, LLBoolean boolean);
LLKeyedInteger *LNKICreate(LLKey key, MAX_INT_TYPE value, LLIntegerType type);
LLKeyedDecimal *LNKDCreate(LLKey key, MAX_DEC_TYPE value, LLDecimalType type);
LLKeyedString *LNKSCreate(LLKey key, VoidPtr string, LLStringType type);

#pragma mark - Deallocation Functions

void LLDelete(LinkList *list);
void LNDelete(LinkNode *node);

#pragma mark - List Push Functions

LinkNode *LLPush(LinkList *list, LinkNode *node);
LinkNode *LLPushBoolean(LinkList *list, LLBoolean boolean);
LinkNode *LLPushInteger(LinkList *list, MAX_INT_TYPE value, LLIntegerType type);
LinkNode *LLPushDecimal(LinkList *list, MAX_DEC_TYPE value, LLDecimalType type);
LinkNode *LLPushString(LinkList *list, VoidPtr string, LLStringType type);

LinkNode *LLPushKey(LinkList *list, LLKeyedNode *node);
LinkNode *LLPushKeyedBoolean(LinkList *list, LLKey key, LLBoolean boolean);
LinkNode *LLPushKeyedInteger(LinkList *list, LLKey key, MAX_INT_TYPE value, LLIntegerType type);
LinkNode *LLPushKeyedDecimal(LinkList *list, LLKey key, MAX_DEC_TYPE value, LLDecimalType type);
LinkNode *LLPushKeyedString(LinkList *list, LLKey key, VoidPtr string, LLStringType type);

#pragma mark - List Pop Functions

LinkNode *LLPopNode(LinkList *list);
LLBoolean LLPopBoolean(LinkList *list);
LLIntegerNode *LLPopInteger(LinkList *list);
LLDecimalNode *LLPopDecimal(LinkList *list);
LLStringNode *LLPopString(LinkList *list);

LLBoolean LLPopKeyedBoolean(LinkList *list, LLKey key);
LLKeyedInteger *LLPopKeyedInteger(LinkList *list, LLKey key);
LLKeyedDecimal *LLPopKeyedDecimal(LinkList *list, LLKey key);
LLKeyedString *LLPopKeyedString(LinkList *list, LLKey key);

#pragma mark - List Item Removal Functions

void LLRemoveNode(LinkList *list, LinkNode *node);
void LLRemoveByKey(LinkList *list, LLKey key);
void LLRemoveByData(LinkList *list, VoidPtr data);

#endif
