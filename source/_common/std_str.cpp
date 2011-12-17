/*============================================================================*/
/* Cosmic Ray [Tau] - Dominik Deak                                            */
/*                                                                            */
/*                      Standard String Functions                             */
/*============================================================================*/

/*---------------------------------------------------------------------------
   Don't include this file if it's already defined.   
  ---------------------------------------------------------------------------*/
#ifndef __STD_STR_CPP__
#define __STD_STR_CPP__


/*---------------------------------------------------------------------------
   Include libraries and other source files needed in this file.
  ---------------------------------------------------------------------------*/
#include "../_common/std_inc.h"


/*----------------------------------------------------------------------------
   This function uses a variable-argument list (similar to printf()) to
   compose a formatted output string. The function accepts the pointer of an
   allocated output string. It is the caller's responsiblilty to allocate 
   an output string that is long enough, because there is no size checking.
   Note that the maximum number of characters that can be written is 65535, 
   including the 0 character.
  ----------------------------------------------------------------------------*/
bool __cdecl str_compose(char* String, char* FormatSpec, ...)
   {
   if ((FormatSpec == NULL) || (String == NULL)) {return false;}

   *String = 0;

   va_list  Arguments;
   va_start(Arguments, FormatSpec);                   //Access variable-argument lists
   vsprintf(String, FormatSpec, Arguments);           //Write formatted output using a pointer to a list of arguments
   va_end(Arguments);

   return true;
   }

/*----------------------------------------------------------------------------
   This function uses a variable-argument list (similar to printf()) to
   compose a formatted output string. The function returns the pointer of the 
   allocated output string. Note that the maximum number of characters that
   can be written is 65535, including the 0 character.
  ----------------------------------------------------------------------------*/
char* __cdecl str_compose_new(char* FormatSpec, ...)
   {
   if (FormatSpec == NULL) {return NULL;}

   char* String = new char[0xFFFF];                   //Allocate a temp string
   if (String == NULL) {return NULL;}
   *String = 0;

   va_list  Arguments;
   va_start(Arguments, FormatSpec);                   //Access variable-argument lists
   vsprintf(String, FormatSpec, Arguments);           //Write formatted output using a pointer to a list of arguments
   va_end(Arguments);

   char* Temp = (char*)realloc(String, strlen(String)+1); //Resize allocated block to fit string
   if (Temp == NULL) {delete[] String; return NULL;}

   return String;
   }

/*----------------------------------------------------------------------------
   This function appends Str to NewStr. If NewStr is NULL, Str will be 
   duplicated to NewStr. If NewStr is NOT NULL, Str will be appended to NewStr.
   If Str is NULL, then the function does nothing. 
   
   WARNING: NewStr must not be a static string!
  ----------------------------------------------------------------------------*/
bool inline str_append(char* &NewStr, char* Str)
   {
   //If Str is NULL exit
   if (Str == NULL) {return true;}
   
   //Get the lenght of NewStr
   dword OldStrLen = 0;
   if (NewStr != NULL) {OldStrLen = strlen(NewStr);}

   //Re-allocate (resize) NewStr (plus add one for the '\0' terminating char)
   char* Temp = (char*)realloc(NewStr, (OldStrLen + strlen(Str) + 1));
   if (Temp == NULL) {return false;}

   //Update NewStr
   NewStr            = Temp;
   NewStr[OldStrLen] = 0;
      
   //Append Str
   strcat(NewStr, Str);

   return true;
   }
 
/*----------------------------------------------------------------------------
   This function replaces NewStr with Str. NewStr will be de-allocated and the 
   contents of Str will be duplicated into NewStr.
   
   WARNING: NewStr must not be a static string!
  ----------------------------------------------------------------------------*/
bool inline str_replace(char* &NewStr, char* Str)
   {
   //If Str is NULL exit with error
   if (Str == NULL) {return false;}

   //De-allocate existing NewStr
   if (NewStr != NULL) {delete[] NewStr;}

   //Allocate a copy of Str
   NewStr = new char[strlen(Str) + 1];
   if (NewStr == NULL) {return false;}

   //Duplicate Str
   strcpy(NewStr, Str);

   return true;
   }

/*----------------------------------------------------------------------------
  Scans and skips past the all the characters listed in SkipStr (excluding 0) 
  if SkipTestChars is set. If SkipTestChars clear, all characters in Str is
  skipped, until one of characters listed in SkipStr is matched. Returns the 
  pointer where the scan has stopped. Returns NULL on fail.
  ----------------------------------------------------------------------------*/
inline char* str_skip(char* Str, char* TestChars, bool SkipTestChars)
   {
   if (Str       == NULL) {return NULL;}
   if (TestChars == NULL) {return Str;}

   while (*Str != 0) 
      {
      char* CmpChar = TestChars;
      while ((*CmpChar != *Str) && (*CmpChar != 0)) {*CmpChar++;}
      
      if (SkipTestChars) {if (*CmpChar == 0) {return Str;}}
      else {if (*CmpChar != 0) {return Str;}}
      
      Str++;
      }
  
   return Str;
   }


/*==== End of file ===========================================================*/
#endif