/*============================================================================*/
/* Cosmic Ray [Tau] - Dominik Deak                                            */
/*                                                                            */
/*                                                                            */
/*                   Common Stack Functions for Floats                        */
/*============================================================================*/

/*---------------------------------------------------------------------------
   Don't include this file if it's already defined.   
  ---------------------------------------------------------------------------*/
#ifndef __STACK_FLOAT_CPP__
#define __STACK_FLOAT_CPP__


/*---------------------------------------------------------------------------
   Include libraries and other source files needed in this file.
  ---------------------------------------------------------------------------*/
#include "../_common/std_inc.h"


/*---------------------------------------------------------------------------
  The stack class. This class can be used to perform basic stack operations.
  ---------------------------------------------------------------------------*/
class fStackClass
   {
   /*==== Public Declarations ================================================*/
   public:

   /*---- Public Data --------------------------------------------------------*/
   struct fStackRec
      {
      fStackRec* Next;
      float      Data;
      };


   /*-------------------------------------------------------------------------
     Allocates a fStackRec data structure. Returns NULL on fail.
     -------------------------------------------------------------------------*/
   fStackRec* Allocate(void)
      {
      fStackRec* NewEntry = new fStackRec;
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
   bool Push(fStackRec* &Stack, float NewData)
      {
      fStackRec* NewEntry = Allocate();               //Allocate a fStackRec structure
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
     the Stack pointer becomes NULL. Returns 0.0f on error, or when Stack->Data 
     is 0.0f. Otherwise it returns the the data.
     -------------------------------------------------------------------------*/
   float Pop(fStackRec* &Stack)
      {
      if (Stack == NULL) {return 0.0f;}
      float Data = Stack->Data;                       //Retrieve the data   
            
      fStackRec* NextEntry = Stack->Next;             //Temp save the next pointer
      delete Stack;                                   //Nuke the TOP of the Stack
      Stack = NextEntry;                              //Advance the Stack reference to the next pointer

      return Data;
      }

   /*-------------------------------------------------------------------------
     Pops and returns the TOP data, then pops and discards the rest of the 
     stack data. Note that the Stack pointer must be the TOP of the stack. 
     Stack becomes NULL on exit.
     -------------------------------------------------------------------------*/
   float PopNuke(fStackRec* &Stack)
      {
      if (Stack == NULL) {return 0.0f;}
      float Data = Stack->Data;                       //Retrieve the data   
      
      Nuke(Stack);                                    //Remove the rest
      
      return Data;
      }

   /*-------------------------------------------------------------------------
     Pops and deletes the TOP data. Note that the Stack pointer must be the 
     TOP of the stack. After the data is retrived, the TOP is DELETED and 
     the Stack pointer will advance to Stack->Next. If Stack->Next is NULL, 
     then the Stack pointer becomes NULL.
     -------------------------------------------------------------------------*/
   void Delete(fStackRec* &Stack)
      {
      if (Stack == NULL) {return;}

      fStackRec* NextEntry = Stack->Next;             //Temp save the next pointer
      delete Stack;                                   //Nuke the TOP of the Stack
      Stack = NextEntry;                              //Advance the Stack reference to the next pointer
      }

   /*-------------------------------------------------------------------------
     Deletes an entire stack pointed by OldStack. The OldStack pointer must 
     be the TOP of the stack. OldStack becomes NULL on return.
     -------------------------------------------------------------------------*/
   void Nuke(fStackRec* &OldStack)
      {
      if (OldStack == NULL) {return;}
      while (OldStack != NULL) {Delete(OldStack);}    //Loop until everyting is deleted
      }

   /*==== End Class =============================================================*/
   };


/*----------------------------------------------------------------------------
  Global Declarations.
  ----------------------------------------------------------------------------*/
typedef fStackClass::fStackRec fStackRec;
fStackClass fStack;



/*==== End of file ===========================================================*/
#endif
