/*============================================================================*/
/* Cosmic Ray [Tau] - Dominik Deak                                            */
/*                                                                            */
/*                         World Related Functions                            */
/*============================================================================*/

/*---------------------------------------------------------------------------
   Don't include this file if it's already defined.   
  ---------------------------------------------------------------------------*/
#ifndef __WORLD_CPP__
#define __WORLD_CPP__


/*---------------------------------------------------------------------------
   Include libraries and other source files needed in this file.
  ---------------------------------------------------------------------------*/
#include "../_common/std_inc.h"
#include "../_common/std_3d.h"
#include "../_common/list.cpp"
#include "../math/colorrec.h"
#include "../mem_data/entity.cpp"
#include "../mem_data/light.cpp"


/*---------------------------------------------------------------------------
  The World class.
  ---------------------------------------------------------------------------*/
class WorldRec
   {
   /*==== Public Declarations ================================================*/
   public:
   
   /*---- Public Data --------------------------------------------------------*/
   PointRec VOrigin;                            //The view origin
   PointRec VVelocity;                          //The view velocity
   PointRec VOrientation;                       //The view orientation
   PointRec VRotation;                          //The view rotation (angular velocity)

   ListRec* EntityList;                         //List of entities in the world
   ListRec* LightList;                          //List of lisghts in the world
   ColorRec AmbLight;                           //The world's ambient light level (subject to chage)
   
   /*---- Constructor --------------------------------------------------------*/
   WorldRec(void) 
      {
      EntityList     = NULL;
      LightList      = NULL;
      AmbLight       = 0;

      VOrigin        = 0.0f;
      VVelocity      = 0.0f;
      VOrientation   = 0.0f;
      VRotation      = 0.0f;
      }

   /*---- Destructor ---------------------------------------------------------*/
   ~WorldRec(void) 
      {
      Nuke();
      }

   /*-------------------------------------------------------------------------
      This function will nuke the entire world... mmmwhahahahaha!
     ------------------------------------------------------------------------*/
   void Nuke(void)
      {
      //Delete all the Enities and sub-Entities (this will recursively call the 
      // destructor, ~EntityRec( )).
      while (EntityList != NULL) {delete (EntityRec*)LinkedList.Retrieve(EntityList);}

      //Delete all the Lights
      while (LightList != NULL)  {delete (LightRec*)LinkedList.Retrieve(LightList);}
      }

   /*-------------------------------------------------------------------------
      This function performs an automated batch processing on a list of 
      Entitites. The processing include rotation, translation, etc. This 
      function should be called once at every main loop. Returns true on 
      success.
   
      SubEntityList : List of Entities. Can be NULL for empty lists.
     -------------------------------------------------------------------------*/
   bool BatchProcess(ListRec* SubEntityList)
      {
      //-- Batch process the entire (sub)Entity list --
      ListRec* EntityNode = SubEntityList;
      while (EntityNode != NULL)
         {
         #define Entity ((EntityRec*)EntityNode->Data)
         if (Entity == NULL) {return false;}

         //Reset the shade flag
         Entity->Flags &= ~ENTITY_SHADE;


         //-- Batch process all the sub-Entities first. This will set
         //   the shade flags correctly. --
         if (!BatchProcess(Entity->EntityList)) {return false;}


         //-- Scale Entity if necessary --
         if ((Entity->ScaleConst.X != 1.0f) || 
             (Entity->ScaleConst.Y != 1.0f) || 
             (Entity->ScaleConst.Z != 1.0f))
            {
            if (!Entity->Scale(&Entity->ScaleConst, &Entity->Centroid)) {return false;}
            }

         //-- Translate Entity if necessary --
         if ((Entity->Velocity.X != 0.0f) || 
             (Entity->Velocity.Y != 0.0f) || 
             (Entity->Velocity.Z != 0.0f))
            {
            if (!Entity->Translate(&Entity->Velocity)) {return false;}
            }

         //-- Rotate Entity if necessary --
         if ((Entity->Rotation.X != 0.0f) || 
             (Entity->Rotation.Y != 0.0f) || 
             (Entity->Rotation.Z != 0.0f))
            {
            if (!Entity->Rotate(&Entity->Rotation, &Entity->Centroid)) {return false;}
            }


         //Advance to the next Entity node
         EntityNode = EntityNode->Next;
         #undef Entity
         }

      return true;
      }

   /*-------------------------------------------------------------------------
      Same as above, but this time the entire world is processed.
     -------------------------------------------------------------------------*/
   bool BatchProcess(void)
      {
      //Process the view origin and orientation
      VOrigin      += VVelocity;
      VOrientation += VRotation;

      return BatchProcess(EntityList);
      }
   
   /*==== End of Class =======================================================*/
   };


/*----------------------------------------------------------------------------
  Global Declarations.
  ----------------------------------------------------------------------------*/
WorldRec World;


/*==== End of File ===========================================================*/
#endif