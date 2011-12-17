/*============================================================================*/
/* Cosmic Ray [Tau] - Dominik Deak                                            */
/*                                                                            */
/*                                                                            */
/*                  Common Stack Functions for Allocated Data                 */
/*============================================================================*/

/*---------------------------------------------------------------------------
   Don't include this file if it's already defined.   
  ---------------------------------------------------------------------------*/
#ifndef __STACK_CPP__
#define __STACK_CPP__


/*---------------------------------------------------------------------------
   Include libraries and other source files needed in this file.
  ---------------------------------------------------------------------------*/
#include "../_common/std_inc.h"


/*---------------------------------------------------------------------------
  The stack class. This class can be used to perform basic stack operations.
  ---------------------------------------------------------------------------*/
class StackClass
   {
   /*==== Public Declarations ================================================*/
   public:

   /*---- Public Data --------------------------------------------------------*/
   struct StackRec
      {
      StackRec* Next;
      void*     Data;
      };

   /*-------------------------------------------------------------------------
     Allocates a StackRec data structure. Returns NULL on fail.
     -------------------------------------------------------------------------*/
   StackRec* Allocate(void)
      {
      StackRec* NewEntry = new StackRec;
      if (NewEntry == NULL) {return NULL;}
      
      NewEntry->Next = NULL;
      NewEntry->Data = NULL;

      return NewEntry;
      }

   /*-------------------------------------------------------------------------
     Pushes a data pointed by NewData onto the stack. Note that the Stack 
     pointer must be the TOP of the stack. The Stack pointer will advance to 
     the newly pushed item. Returns true on success.
     -------------------------------------------------------------------------*/
   bool Push(StackRec* &Stack, void* NewData)
      {
      if (NewData == NULL) {return false;}

      StackRec* NewEntry = Allocate();                //Allocate a StackRec structure
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
     the Stack pointer becomes NULL. Returns NULL on error, or when Stack->Data 
     is NULL. Otherwise it returns the pointer of the data.
     -------------------------------------------------------------------------*/
   void* Pop(StackRec* &Stack)
      {
      if (Stack == NULL) {return NULL;}
      void* Data = Stack->Data;                       //Retrieve the data   
            
      StackRec* NextEntry = Stack->Next;              //Temp save the next pointer
      delete Stack;                                   //Nuke the TOP of the Stack
      Stack = NextEntry;                              //Advance the Stack reference to the next pointer

      return Data;
      }

   /*-------------------------------------------------------------------------
     Pops and returns the TOP data, then pops and discards the rest of the 
     stack data. Note that the Stack pointer must be the TOP of the stack. 
     Stack becomes NULL on exit.
     -------------------------------------------------------------------------*/
   void* PopNuke(StackRec* &Stack)
      {
      if (Stack == NULL) {return NULL;}
      void* Data = Stack->Data;                       //Retrieve the data   
      
      Nuke(Stack);                                    //Remove the rest
      
      return Data;
      }

   /*-------------------------------------------------------------------------
     Pops and deletes the TOP data. Note that the Stack pointer must be the 
     TOP of the stack. After the data is retrived, the TOP is DELETED and 
     the Stack pointer will advance to Stack->Next. If Stack->Next is NULL, 
     then the Stack pointer becomes NULL.
     -------------------------------------------------------------------------*/
   void Delete(StackRec* &Stack)
      {
      if (Stack == NULL) {return;}
      if (Stack->Data != NULL) {delete Stack->Data;}

      StackRec* NextEntry = Stack->Next;              //Temp save the next pointer
      delete Stack;                                   //Nuke the TOP of the Stack
      Stack = NextEntry;                              //Advance the Stack reference to the next pointer
      }

   /*-------------------------------------------------------------------------
     Deletes an entire stack pointed by OldStack. The OldStack pointer must 
     be the TOP of the stack. OldStack becomes NULL on return.
     -------------------------------------------------------------------------*/
   void Nuke(StackRec* &OldStack)
      {
      if (OldStack == NULL) {return;}
      while (OldStack != NULL) {Delete(OldStack);}    //Loop until everyting is deleted
      }

   /*==== End Class =============================================================*/
   };


/*----------------------------------------------------------------------------
  Global Declarations.
  ----------------------------------------------------------------------------*/
typedef StackClass::StackRec StackRec;
StackClass Stack;




/*==== End of file ===========================================================*/
#endif
