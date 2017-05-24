#ifndef LINKED_LIST_H
#define LINKED_LIST_H

#ifdef WCHAR_SUPPORT
#include <wchar.h>
#endif

#include <stdio.h>
#include <stddef.h>

#pragma mark - Base Types

/** Base type for generic data pointers for anything going forward. */
typedef void *LLVoid;

/** Specify the text type that should be used for LinkList keys */
#ifndef LL_KEY_TYPEspoti

#define LL_KEY_TYPE char *
#endif
typedef LL_KEY_TYPE LLKey;

/** Keyed data types also have a hashing function and field */
typedef unsigned int (*LLHashFn)(LLKey key, int limit);

/** The limit for the hash key generation. Ideally the size of the array for a hash table */
typedef size_t LLHashLimit;

/* 64-bit types are hidden behind the BIG_TYPES define. The maximum size for
 * a given integer or decimal type are defined here. */
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
  #ifdef WCHAR_SUPPORT
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
  LN_VOID = 16,

  LN_KEYED = 256
} LinkNodeDataType;

typedef enum
{
  LL_FORWARD = 1,
  LL_BACKWARD = 2
} LLFindDir;

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
  union
  {
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
  } u;
  LLIntegerType type;
} LLIntegerNode;

typedef struct LLKeyedInteger
{
  LLKeyedNode keyedNode;
  LLIntegerNode integer;
} LLKeyedInteger;

typedef struct LLDecimalNode
{
  union
  {
    float f;
    double d;
    #ifdef BIG_TYPES
    long double ld;
    #endif
  } u;
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
  } u;
  LLStringType type;
} LLStringNode;

typedef struct LLKeyedString 
{
  LLKeyedNode keyedNode;
  LLStringNode string;
} LLKeyedString;

typedef struct LLVoidNode
{
  LLVoid value;
} LLVoidNode;

typedef struct LLKeyedVoid
{
  LLKeyedNode *keyedNode;
  LLVoidNode *voidNode;
} LLKeyedVoid;

typedef struct LinkNode
{
  struct LinkNode *next;
  struct LinkNode *prev;
  LLVoid value;
  LinkNodeDataType type;
} LinkNode;

typedef struct LinkList
{
  LinkNode *head;
  LinkNode *tail;

  /* Push methods without keyed or named values */
  struct LinkList *(*pushBool)(struct LinkList *list, LLBoolean data);
  struct LinkList *(*pushChar)(struct LinkList *list, char data);
  struct LinkList *(*pushUChar)(struct LinkList *list, unsigned char data);
  struct LinkList *(*pushShort)(struct LinkList *list, short data);
  struct LinkList *(*pushUShort)(struct LinkList *list, unsigned short data);
  struct LinkList *(*pushInt)(struct LinkList *list, int data);
  struct LinkList *(*pushUInt)(struct LinkList *list, unsigned int data);
  struct LinkList *(*pushLong)(struct LinkList *list, long data);
  struct LinkList *(*pushULong)(struct LinkList *list, unsigned long data);
  struct LinkList *(*pushFloat)(struct LinkList *list, float data);
  struct LinkList *(*pushDouble)(struct LinkList *list, double data);
  struct LinkList *(*pushString)(struct LinkList *list, char *data);
  struct LinkList *(*pushVoid)(struct LinkList *list, LLVoid data);

  /* Push methods with keyed or named values */
  struct LinkList *(*pushKBool)(struct LinkList *list, LLKey key, LLBoolean data);
  struct LinkList *(*pushKChar)(struct LinkList *list, LLKey key, char data);
  struct LinkList *(*pushKUChar)(struct LinkList *list, LLKey key, unsigned char data);
  struct LinkList *(*pushKShort)(struct LinkList *list, LLKey key, short data);
  struct LinkList *(*pushKUShort)(struct LinkList *list, LLKey key, unsigned short data);
  struct LinkList *(*pushKInt)(struct LinkList *list, LLKey key, int data);
  struct LinkList *(*pushKUInt)(struct LinkList *list, LLKey key, unsigned int data);
  struct LinkList *(*pushKLong)(struct LinkList *list, LLKey key, long data);
  struct LinkList *(*pushKULong)(struct LinkList *list, LLKey key, unsigned long data);
  struct LinkList *(*pushKFloat)(struct LinkList *list, LLKey key, float data);
  struct LinkList *(*pushKDouble)(struct LinkList *list, LLKey key, double data);
  struct LinkList *(*pushKString)(struct LinkList *list, LLKey key, char *data);
  struct LinkList *(*pushKVoid)(struct LinkList *list, LLKey key, LLVoid data);

  /* Pop methods for unnamed/unkeyed values - LIFO */
  LinkNode       *(*pop)(struct LinkList *list);
  LLBoolean       (*popBool)(struct LinkList *list);
  char            (*popChar)(struct LinkList *list);
  unsigned char   (*popUChar)(struct LinkList *list);
  short           (*popShort)(struct LinkList *list);
  unsigned short  (*popUShort)(struct LinkList *list);
  int             (*popInt)(struct LinkList *list);
  unsigned int    (*popUInt)(struct LinkList *list);
  long            (*popLong)(struct LinkList *list);
  unsigned long   (*popULong)(struct LinkList *list);
  float           (*popFloat)(struct LinkList *list);
  double          (*popDouble)(struct LinkList *list);
  char           *(*popString)(struct LinkList *list);
  LLVoid          (*popVoid)(struct LinkList *list);

  /* Pop methods for named/keyed values - LIFO */
  LLBoolean       (*popKBool)(struct LinkList *list, LLKey key);
  char            (*popKChar)(struct LinkList *list, LLKey key);
  unsigned char   (*popKUChar)(struct LinkList *list, LLKey key);
  short           (*popKShort)(struct LinkList *list, LLKey key);
  unsigned short  (*popKUShort)(struct LinkList *list, LLKey key);
  int             (*popKInt)(struct LinkList *list, LLKey key);
  unsigned int    (*popKUInt)(struct LinkList *list, LLKey key);
  long            (*popKLong)(struct LinkList *list, LLKey key);
  unsigned long   (*popKULong)(struct LinkList *list, LLKey key);
  float           (*popKFloat)(struct LinkList *list, LLKey key);
  double          (*popKDouble)(struct LinkList *list, LLKey key);
  char           *(*popKString)(struct LinkList *list, LLKey key);
  LLVoid          (*popKVoid)(struct LinkList *list, LLKey key);

  /* Dequeue methods for unnamed/unkeyed values - FIFO */
  LinkNode       *(*dequeue)(struct LinkList *list);
  LLBoolean       (*dequeueBool)(struct LinkList *list);
  char            (*dequeueChar)(struct LinkList *list);
  unsigned char   (*dequeueUChar)(struct LinkList *list);
  short           (*dequeueShort)(struct LinkList *list);
  unsigned short  (*dequeueUShort)(struct LinkList *list);
  int             (*dequeueInt)(struct LinkList *list);
  unsigned int    (*dequeueUInt)(struct LinkList *list);
  long            (*dequeueLong)(struct LinkList *list);
  unsigned long   (*dequeueULong)(struct LinkList *list);
  float           (*dequeueFloat)(struct LinkList *list);
  double          (*dequeueDouble)(struct LinkList *list);
  char           *(*dequeueString)(struct LinkList *list);
  LLVoid          (*dequeueVoid)(struct LinkList *list);

  /* Dequeue methods for named/keyed values - FIFO */
  LLBoolean       (*dequeueKBool)(struct LinkList *list, LLKey key);
  char            (*dequeueKChar)(struct LinkList *list, LLKey key);
  unsigned char   (*dequeueKUChar)(struct LinkList *list, LLKey key);
  short           (*dequeueKShort)(struct LinkList *list, LLKey key);
  unsigned short  (*dequeueKUShort)(struct LinkList *list, LLKey key);
  int             (*dequeueKInt)(struct LinkList *list, LLKey key);
  unsigned int    (*dequeueKUInt)(struct LinkList *list, LLKey key);
  long            (*dequeueKLong)(struct LinkList *list, LLKey key);
  unsigned long   (*dequeueKULong)(struct LinkList *list, LLKey key);
  float           (*dequeueKFloat)(struct LinkList *list, LLKey key);
  double          (*dequeueKDouble)(struct LinkList *list, LLKey key);
  char           *(*dequeueKString)(struct LinkList *list, LLKey key);
  LLVoid          (*dequeueKVoid)(struct LinkList *list, LLKey key);


  #ifdef BIG_TYPES
  struct LinkList *(*pushLongLong)(struct LinkList *list, long long data);
  struct LinkList *(*pushULongLong)(struct LinkList *list, unsigned long long data);
  struct LinkList *(*pushLongDouble)(struct LinkList *list, long double data);
  long long (*popLongLong)(struct LinkList *list);
  long long (*popKLongLong)(struct LinkList *list, LLKey key);
  unsigned long long (*popULongLong)(struct LinkList *list);
  unsigned long long (*popKULongLong)(struct LinkList *list, LLKey key);
  long double (*popLongDouble)(struct LinkList *list);
  long double (*popKLongDouble)(struct LinkList *list, LLKey key);
  long long (*dequeueLongLong)(struct LinkList *list);
  long long (*dequeueKLongLong)(struct LinkList *list, LLKey key);
  unsigned long long (*dequeueULongLong)(struct LinkList *list);
  unsigned long long (*dequeueKULongLong)(struct LinkList *list, LLKey key);
  long double (*dequeueLongDouble)(struct LinkList *list);
  long double (*dequeueKLongDouble)(struct LinkList *list, LLKey key);
  #endif

  #ifdef WCHAR_SUPPORT
  struct LinkList *(*pushWString)(struct LinkList *list, wchar_t *data);
  struct LinkList *(*pushKWString)(struct LinkList *list, LLKey key, wchar_t *data);
  wchar_t *(*popWString)(struct LinkList *list);
  wchar_t *(*popKWString)(struct LinkList *list, LLKey key);
  wchar_t *(*dequeueWString)(struct LinkList *list, LLKey key);
  wchar_t *(*dequeueKWString)(struct LinkList *list, LLKey key);
  #endif
} LinkList;

/** ForEach function pointer type */
typedef LLVoid (*LLMapFn)(LLVoid data, size_t index, LinkList *list);
typedef void (*LLForEach)(LLVoid data, size_t index, LinkList *list);
typedef LinkList *(*LLMap)(LLMapFn mapFn, LinkList *list);

#pragma mark - Constant Exports

unsigned int LLDefaultStringHashFn(LLKey key, int limit);

extern const LLHashFn LLDefaultHashFunction;
extern const LLHashLimit LLDefaultHashLimit;

#pragma mark - Utility Functions

size_t LLDataSize(LinkNode *node);
LinkNode *LLFindKeyed(LinkList *list, LLKey key);
LLStringNode *LLDuplicateStringNode(LLStringNode *source);
LinkNode *LLFindNodeOfType(LinkList *list, LinkNodeDataType type, LLFindDir dir);

#pragma mark - Initialization Functions

LinkList *LLInit(LinkList *list, LLBoolean alloc);
LinkNode *LNInit(LinkNode *node, LLBoolean alloc);

LLBoolNode *LNBInit(LLBoolNode *node, LLBoolean alloc);
LLIntegerNode *LNIInit(LLIntegerNode *node, LLBoolean alloc);
LLDecimalNode *LNDInit(LLDecimalNode *node, LLBoolean alloc);
LLStringNode *LNSInit(LLStringNode *node, LLBoolean alloc);
LLVoidNode *LNVInit(LLVoidNode *node, LLBoolean alloc);

LLKeyedNode *LNKNInit(LLKeyedNode *node, LLBoolean alloc);
LLKeyedBool *LNKBInit(LLKeyedBool *node, LLBoolean alloc);
LLKeyedInteger *LNKIInit(LLKeyedInteger *node, LLBoolean alloc);
LLKeyedDecimal *LNKDInit(LLKeyedDecimal *node, LLBoolean alloc);
LLKeyedString *LNKSInit(LLKeyedString *node, LLBoolean alloc);
LLKeyedVoid *LNKVInit(LLKeyedVoid *node, LLBoolean alloc);

#pragma mark - Creation Functions

LinkList *LLCreate(void);
LinkNode *LNCreate(LLVoid value, LinkNodeDataType type);

LLKeyedNode *LNKCreate(LLKey key, LLHashFn hashFunction);
LLBoolNode *LNBCreate(LLBoolean boolean);
LLIntegerNode *LNICreate(MAX_INT_TYPE value, LLIntegerType type);
LLDecimalNode *LNDCreate(MAX_DEC_TYPE value, LLDecimalType type);
LLStringNode *LNSCreate(LLVoid string, LLStringType type);
LLVoidNode *LNVCreate(LLVoid data);
LLKeyedBool *LNKBCreate(LLKey key, LLBoolean boolean);
LLKeyedInteger *LNKICreate(LLKey key, MAX_INT_TYPE value, LLIntegerType type);
LLKeyedDecimal *LNKDCreate(LLKey key, MAX_DEC_TYPE value, LLDecimalType type);
LLKeyedString *LNKSCreate(LLKey key, LLVoid string, LLStringType type);
LLKeyedVoid *LNKVCreate(LLKey key, LLVoid data);

#pragma mark - Deallocation Functions

void LLDelete(LinkList *list);
void LNDelete(LinkNode *node);

#pragma mark - List Push Functions

LinkNode *LLPush(LinkList *list, LinkNode *node);
LinkNode *LLPushBoolean(LinkList *list, LLBoolean boolean);
LinkNode *LLPushInteger(LinkList *list, MAX_INT_TYPE value, LLIntegerType type);
LinkNode *LLPushDecimal(LinkList *list, MAX_DEC_TYPE value, LLDecimalType type);
LinkNode *LLPushString(LinkList *list, LLVoid string, LLStringType type);
LinkNode *LLPushVoid(LinkList *list, LLVoid data);

LinkNode *LLPushKey(LinkList *list, LLKeyedNode *node);
LinkNode *LLPushKeyedBoolean(LinkList *list, LLKey key, LLBoolean boolean);
LinkNode *LLPushKeyedInteger(LinkList *list, LLKey key, MAX_INT_TYPE value, LLIntegerType type);
LinkNode *LLPushKeyedDecimal(LinkList *list, LLKey key, MAX_DEC_TYPE value, LLDecimalType type);
LinkNode *LLPushKeyedString(LinkList *list, LLKey key, LLVoid string, LLStringType type);
LinkNode *LLPushKeyedVoid(LinkList *list, LLKey key, LLVoid data);

#pragma mark - List Pop Functions

LinkNode *LLPopNode(LinkList *list);
LLBoolean LLPopBoolean(LinkList *list);
LLIntegerNode *LLPopInteger(LinkList *list);
LLDecimalNode *LLPopDecimal(LinkList *list);
LLStringNode *LLPopString(LinkList *list);
LLVoidNode *LLPopVoid(LinkList *list);

LLBoolean LLPopKeyedBoolean(LinkList *list, LLKey key);
LLKeyedInteger *LLPopKeyedInteger(LinkList *list, LLKey key);
LLKeyedDecimal *LLPopKeyedDecimal(LinkList *list, LLKey key);
LLKeyedString *LLPopKeyedString(LinkList *list, LLKey key);
LLKeyedVoid *LLPopKeyedVoid(LinkList *list, LLKey key);

#pragma mark - List Dequeue Functions

LinkNode *LLDequeueNode(LinkList *list);
LLBoolean LLDequeueBoolean(LinkList *list);
LLIntegerNode *LLDequeueInteger(LinkList *list);
LLDecimalNode *LLDequeueDecimal(LinkList *list);
LLStringNode *LLDequeueString(LinkList *list);
LLVoidNode *LLDequeueVoid(LinkList *list);

LLBoolean LLDequeueKeyedBoolean(LinkList *list, LLKey key);
LLKeyedInteger *LLDequeueKeyedInteger(LinkList *list, LLKey key);
LLKeyedDecimal *LLDequeueKeyedDecimal(LinkList *list, LLKey key);
LLKeyedString *LLDequeueKeyedString(LinkList *list, LLKey key);
LLKeyedVoid *LLDequeueKeyedVoid(LinkList *list, LLKey key);

#pragma mark - List Item Removal Functions

void LLRemoveNode(LinkList *list, LinkNode *node);
void LLRemoveByKey(LinkList *list, LLKey key);
void LLRemoveByData(LinkList *list, LLVoid data);

#endif
