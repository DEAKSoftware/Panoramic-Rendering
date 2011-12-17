/*============================================================================*/
/* Cosmic Ray [Tau] - Dominik Deak                                            */
/*                                                                            */
/*                                                                            */
/*                        Common Linked List Functions                        */
/*============================================================================*/

/*---------------------------------------------------------------------------
   Don't include this file if it's already defined.   
  ---------------------------------------------------------------------------*/
#ifndef __LIST_CPP__
#define __LIST_CPP__


/*---------------------------------------------------------------------------
   Include libraries and other source files needed in this file.
  ---------------------------------------------------------------------------*/
#include "../_common/std_inc.h"


/*---------------------------------------------------------------------------
  The linked list class. This class can be used to perform basic linked list
  operations.
  ---------------------------------------------------------------------------*/
class LinkedListClass
   {
   /*==== Public Declarations ================================================*/
   public:

   /*---- Public Data --------------------------------------------------------*/
   struct LinkedListRec
      {
      LinkedListRec* Prev;
      LinkedListRec* Next;
      void*          Data;
      };


   /*-------------------------------------------------------------------------
     Allocates a LinkedListRec data structure. Returns NULL on fail.
     -------------------------------------------------------------------------*/
   LinkedListRec* Allocate(void)
      {
      LinkedListRec* NewEntry = new LinkedListRec;
      if (NewEntry == NULL) {return NULL;}
      
      NewEntry->Prev = NULL;
      NewEntry->Next = NULL;
      NewEntry->Data = NULL;

      return NewEntry;
      }

   /*-------------------------------------------------------------------------
     Inserts a data pointed by NewData into the linked list, pointed by List.
     Note that the pointer List must be the FIRST entry (the head) of the 
     actual list. The List ptr will advance to the newly inserted item. 
     Returns true on success.
     -------------------------------------------------------------------------*/
   bool Insert(LinkedListRec* &List, void* NewData)
      {
      if (NewData == NULL) {return false;}

      LinkedListRec* NewEntry = Allocate();     //Allocate a LinkedListRec structure
      if (NewEntry == NULL) {return false;}
    
      if (List == NULL)                         //If the List is empty, create a new one
         {
         NewEntry->Data = NewData;              //Assign the data pointer to this entry
         List = NewEntry;
         }
      else 
         {
         List->Prev = NewEntry;                 //Point prev ptr of List to NewEntry
         NewEntry->Next = List;                 //NewEntry becomes the head of the list
         NewEntry->Data = NewData;              //Assign the data pointer to this entry
         List = NewEntry;                       //Advance the List pointer to the NewEntry
         }

      return true;
      }
   
   /*-------------------------------------------------------------------------
     Merges two lists together. MergedList will contain the resulting merged 
     data. OldList will return NULL as it is no longer valid. If both parameters
     are NULL, the function does nothing. If OldList is NULL, the function does 
     nothing.
     -------------------------------------------------------------------------*/
   void Merge(LinkedListRec* &MergedList, LinkedListRec* &OldList)
      {
      if (OldList == NULL) {return;}            //If OldList is NULL, there is no need to merge.
      if (MergedList == NULL)                   //If MergedList is already NULL, simply copy the OldList ptr.
         {
         MergedList = OldList;                  //Copy the pointer over
         OldList = NULL;                        //Zero the OldList
         return;
         }

      LinkedListRec* MergedListEnd = MergedList;
      while (MergedListEnd->Next != NULL)       //Find the end of MergedList
         {
         MergedListEnd = MergedListEnd->Next;
         } 

      MergedListEnd->Next = OldList;            //Append OldList
      OldList->Prev = MergedListEnd;            //Link prev ptr of OldList to the MergedListEnd.
      OldList = NULL;                           //Zero the OldList
      }

   /*-------------------------------------------------------------------------
     Retrieves the data stored in OldEntry. The OldEntry pointer can point
     to any entry within the list. After the data is retrived, the OldEntry
     is DELETED and the OldEntry pointer will advance to OldEntry->Next by
     default. If OldEntry->Next is NULL, then the OldEntry pointer will 
     advance to OldEntry->Prev. Returns NULL on error, or when OldEntry->Data 
     is NULL. Otherwise it returns the pointer of the data.

     Calling Rules:

         DataPtr = LinkedListClass::Retrieve(List->Next);   <- That's a no no!

         ListEntryPtr = List->Next;                         <- Correct.
         DataPtr = LinkedListClass::Retrieve(ListEntryPtr);
     -------------------------------------------------------------------------*/
   void* Retrieve(LinkedListRec* &OldEntry)
      {
      if (OldEntry == NULL) {return NULL;}

      void* OldData = OldEntry->Data;           //Retrieve the data   
            
      LinkedListRec* PrevEntry = OldEntry->Prev; //Link the previous and next entries together
      LinkedListRec* NextEntry = OldEntry->Next;
      delete OldEntry;                          //Nuke the OldEntry
      if (NextEntry != NULL) {NextEntry->Prev = PrevEntry;} //If NextEntry exist, link it's prev ptr to PrevEntry
      if (PrevEntry != NULL) {PrevEntry->Next = NextEntry;} //If PrevEntry exist, link it's next ptr to NextEntry

      if (NextEntry != NULL) {OldEntry = NextEntry;} //Advance the OldEntry reference to the nearest pointer
      else {OldEntry = PrevEntry;}

      return OldData;
      }

   /*-------------------------------------------------------------------------
     Deletes the data stored in OldEntry. The OldEntry pointer can point to 
     any entry within the list. After the data is deleted, the OldEntry is also
     deleted and the OldEntry pointer will advance to OldEntry->Next by 
     default. If OldEntry->Next is NULL, then the OldEntry pointer will advance
     to OldEntry->Prev.

     Calling Rules: Same as in LinkedListClass::Retrieve();
     -------------------------------------------------------------------------*/
   void Delete(LinkedListRec* &OldEntry)
      {
      if (OldEntry == NULL) {return;}

      if (OldEntry->Data != NULL) {delete OldEntry->Data;} //Nuke the data   
            
      LinkedListRec* PrevEntry = OldEntry->Prev; //Link the previous and next entries together
      LinkedListRec* NextEntry = OldEntry->Next;
      delete OldEntry;                          //Nuke the OldEntry
      if (NextEntry != NULL) {NextEntry->Prev = PrevEntry;} //If NextEntry exist, link it's prev ptr to PrevEntry
      if (PrevEntry != NULL) {PrevEntry->Next = NextEntry;} //If PrevEntry exist, link it's next ptr to NextEntry

      if (NextEntry != NULL) {OldEntry = NextEntry;} //Advance the OldEntry reference to the nearest pointer
      else {OldEntry = PrevEntry;}
      }

   /*-------------------------------------------------------------------------
     Deletes an entire list and its data pointed by OldList. The OldList 
     pointer can point to any entry within the list. OldList becomes NULL on 
     return.

     Calling Rules: Same as in LinkedListClass::Retrieve();
     -------------------------------------------------------------------------*/
   void Nuke(LinkedListRec* &OldList)
      {
      if (OldList == NULL) {return;}
      while (OldList != NULL) {Delete(OldList);} //Loop until everyting is deleted
      }

   /*-------------------------------------------------------------------------
     Deletes an entire list chain pointed by OldList. This function does not 
     delete the data present in the list. Use this wisely, as NukeListChain( ) 
     can produce memory leaks if the allocated data is 'forgotten'. The 
     OldList pointer can point to any entry within the list. OldList becomes 
     NULL on return. 

     Calling Rules: Same as in LinkedListClass::Retrieve();
     -------------------------------------------------------------------------*/
   void NukeListChain(LinkedListRec* &OldList)
      {
      if (OldList == NULL) {return;}
      while (OldList != NULL) {Retrieve(OldList);} //Loop until everyting is deleted
      }

   /*==== End Class =============================================================*/
   };


/*----------------------------------------------------------------------------
  Global Declarations.
  ----------------------------------------------------------------------------*/
typedef LinkedListClass::LinkedListRec ListRec;
LinkedListClass LinkedList;




/*==== End of file ===========================================================*/
#endif
