#include <stdio.h>
#include "LinkList.h"

int main(int argc, char **argv) 
{
  LinkList *list = LLCreate();

  LLPushKeyedString(list, "name", "Brielle", 0L);
  LLPushKeyedString(list, "age", "MYOB", 0L);
  
  printf("Name: %s\n", LLPopKeyedString(list, "name")->string.s);
  printf("Age : %s\n", LLPopKeyedString(list, "age")->string.s);
  
  LLDelete(list);
  
  return 0;
}
