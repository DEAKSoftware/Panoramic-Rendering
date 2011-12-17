/*============================================================================*/
/* Cosmic Ray [Tau] - Dominik Deak                                            */
/*                                                                            */
/*                                                                            */
/*                   Common Stack Functions for Integers                      */
/*============================================================================*/

/*---------------------------------------------------------------------------
   Don't include this file if it's already defined.   
  ---------------------------------------------------------------------------*/
#ifndef __STACK_INT_CPP__
#define __STACK_INT_CPP__


/*---------------------------------------------------------------------------
   Include libraries and other source files needed in this file.
  ---------------------------------------------------------------------------*/
#include "../_common/std_inc.h"


/*---------------------------------------------------------------------------
  The stack class. This class can be used to perform basic stack operations.
  ---------------------------------------------------------------------------*/
class iStackClass
   {
   /*==== Public Declarations ================================================*/
   public:

   /*---- Public Data --------------------------------------------------------*/
   struct iStackRec
      {
      iStackRec* Next;
      int        Data;
      };


   /*-------------------------------------------------------------------------
     Allocates a iStackRec data structure. Returns NULL on fail.
     -------------------------------------------------------------------------*/
   iStackRec* Allocate(void)
      {
      iStackRec* NewEntry = new iStackRec;
      if (NewEntry == NULL) {return NULL;}
      
      NewEntry->Next = NULL;
      NewEntry->Data = 0.0f;

      return NewEntry;
      }

   /*-------------------------------------------------------------------------
     Pushes a data in NewData onto the stack. Note that the Stack pointer must 
     be the TOP of the stack. The Stack pointer will advance to the newly 
     pushed item. Returns true on success.
     -------------------------------------------------------------------------*/
   bool Push(iStackRec* &Stack, int NewData)
      {
      iStackRec* NewEntry = Allocate();               //Allocate a iStackRec structure
      if (NewEntry == NULL) {return false;}
    
      if (Stack == NULL)                              //If the Stack is empty, create a new one
         {
         NewEntry->Data = NewData;                    //Assign the data pointer to this entry
         Stack = NewEntry;
         }
      else 
         {
         NewEntry->Next = Stack;                      //NewEntry becomes the head of the list
         NewEntry->Data = NewData;                    //Assign the data pointer to this entry
         Stack = NewEntry;                            //Advance the List pointer to the NewEntry
         }

      return true;
      }

   /*-------------------------------------------------------------------------
     Pops and returns the TOP data. Note that the Stack pointer must be the TOP 
     of the stack. After the data is retrived, the TOP is DELETED and the 
     Stack pointer will advance to Stack->Next. If Stack->Next is NULL, then 
     the Stack pointer becomes NULL. Returns 0 on error, or when Stack->Data 
     is 0. Otherwise it returns the the data.
     -------------------------------------------------------------------------*/
   int Pop(iStackRec* &Stack)
      {
      if (Stack == NULL) {return NULL;}
      int Data = Stack->Data;                         //Retrieve the data   
            
      iStackRec* NextEntry = Stack->Next;             //Temp save the next pointer
      delete Stack;                                   //Nuke the TOP of the Stack
      Stack = NextEntry;                              //Advance the Stack reference to the next pointer

      return Data;
      }

   /*-------------------------------------------------------------------------
     Pops and returns the TOP data, then pops and discards the rest of the 
     stack data. Note that the Stack pointer must be the TOP of the stack. 
     Stack becomes NULL on exit.
     -------------------------------------------------------------------------*/
   int PopNuke(iStackRec* &Stack)
      {
      if (Stack == NULL) {return 0.0f;}
      int Data = Stack->Data;                         //Retrieve the data   
      
      Nuke(Stack);                                    //Remove the rest
      
      return Data;
      }

   /*-------------------------------------------------------------------------
     Pops and deletes the TOP data. Note that the Stack pointer must be the 
     TOP of the stack. After the data is retrived, the TOP is DELETED and 
     the Stack pointer will advance to Stack->Next. If Stack->Next is NULL, 
     then the Stack pointer becomes NULL.
     -------------------------------------------------------------------------*/
   void Delete(iStackRec* &Stack)
      {
      if (Stack == NULL) {return;}

      iStackRec* NextEntry = Stack->Next;             //Temp save the next pointer
      delete Stack;                                   //Nuke the TOP of the Stack
      Stack = NextEntry;                              //Advance the Stack reference to the next pointer
      }

   /*-------------------------------------------------------------------------
     Deletes an entire stack pointed by OldStack. The OldStack pointer must 
     be the TOP of the stack. OldStack becomes NULL on return.
     -------------------------------------------------------------------------*/
   void Nuke(iStackRec* &OldStack)
      {
      if (OldStack == NULL) {return;}
      while (OldStack != NULL) {Delete(OldStack);}    //Loop until everyting is deleted
      }

   /*==== End Class =============================================================*/
   };


/*----------------------------------------------------------------------------
  Global Declarations.
  ----------------------------------------------------------------------------*/
typedef iStackClass::iStackRec iStackRec;
iStackClass iStack;



/*==== End of file ===========================================================*/
#endif
