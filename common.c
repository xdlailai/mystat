#include "common.h"

int printe(PrintType type)
{
  switch(type)
  {
    case PT_Info:
      printf("Info: %s", errorstring);
      break;
    case PT_Error:
      printf("Error: %s", errorstring);
      break;
    case PT_Config:
      printf("Config: %s", errorstring);
      break;
    case PT_Multiline:
      printf(" %s\n", errorstring);
      break;
    case PT_ShortMultiline:
      break;
    default:
      printf("%d: %s", type, errorstring);
      break;

  }
}
