/*============================================================================*/
/* Cosmic Ray [Tau] - Dominik Deak                                            */
/*                                                                            */
/*                     Mini Compiler and Equation Solver                      */
/*============================================================================*/

/*---------------------------------------------------------------------------
   Don't include this file if it's already defined.   
  ---------------------------------------------------------------------------*/
#ifndef __EQUSOLVER_CPP__
#define __EQUSOLVER_CPP__


/*---------------------------------------------------------------------------
   Include libraries and other source files needed in this file.
  ---------------------------------------------------------------------------*/
#include "../_common/std_inc.h"
#include "../_common/stack.h"

//Opcode masking
#define OP_PR_MASK   0xE0                       //Precedence mask
#define OP_CODE_MASK 0x1F                       //Opcode mask

//Special opcodes
#define OP_NULL      0x00                       //NOP
#define OP_ST        0x01                       //Store and pop
#define OP_LD_IMM    0x02                       //Load immediate
#define OP_LD_MEM    0x03                       //Load variable
#define OP_BROPEN    0x04                       //Start sub expression "(" (non executable)
#define OP_BRCLOSE   0x05                       //End sub expression ")" (non executable)

//Arithmetic opcodes
#define OP_NEG       0xCA                       //Negate TOP
#define OP_ADD       0x2B                       //Pop TOP and add it to TOP-1
#define OP_SUB       0x2C                       //Pop TOP and subtract TOP-1 with it
#define OP_MUL       0x4D                       //Pop and multiply TOP with TOP-1
#define OP_DIV       0x4E                       //Pop and use TOP as the divisor for TOP-1
#define OP_PWR       0xAF                       //Pop and use TOP as the power for TOP-1

#define OP_SIN       0xF0                       //Find the sine of TOP
#define OP_COS       0xF1                       //Find the cosine of TOP
#define OP_TAN       0xF2                       //Find the tangent of TOP
#define OP_ASIN      0xF3                       //Find the arc sine of TOP
#define OP_ACOS      0xF4                       //Find the arc cosine of TOP
#define OP_ATAN      0xF5                       //Find the arc tangent of TOP
#define OP_SQRT      0xF6                       //Find the sqare root of TOP
#define OP_SINH      0xF7                       //Find the hyperbolic sine of TOP
#define OP_COSH      0xF8                       //Find the hyperbolic cosine of TOP
#define OP_TANH      0xF9                       //Find the hyperbolic tangent of TOP

#define OP_EXP       0xFA                       //Find the exponetial of TOP
#define OP_LN        0xFB                       //Find logarithm of TOP
#define OP_LOG       0xFC                       //Find logarithm base 10 of TOP
#define OP_ABS       0xFD                       //Make TOP an absolute value



/*---------------------------------------------------------------------------
   This is a mini compiler that converts an infix-based equation into a 
   reverse polish notaion. It also has a small exectution unit and a debugger.
  ---------------------------------------------------------------------------*/
class EquSolverClass
   {
   /*==== Private Declarations ===============================================*/
   private:



   /*==== Public Declarations ================================================*/
   public:

   /*-------------------------------------------------------------------------
      Exectutes an equation code. If the code uses a variable, it will use the
      value passed in x. It returns false on fail.
     -------------------------------------------------------------------------*/
   bool Execute(float &Result, byte* Code, float x)
      {
      if (Code == NULL) {return false;}
      Result = 0.0f;

      //Local variables
      byte*  OpCode = Code;
      float  st[0x100];                            //Fast local stack
      float* sp = st-1;                            //The stack pointer

      //Execute the entire code
      while (*OpCode != OP_NULL)
         {
         switch (*OpCode)
            {
            case OP_ST      : if (sp < st) {printf("EquSolver : Stack underflow.\n"); return false;}
                              Result = *sp; sp--;
                              OpCode++;
                              break;

            case OP_LD_IMM  : if (sp >= st+0x100) {printf("EquSolver : Stack overflow.\n"); return false;}
                              OpCode++;
                              sp++; *sp = *(float*)OpCode;
                              OpCode += sizeof(float);
                              break;
            
            case OP_LD_MEM  : if (sp >= st+0x100) {printf("EquSolver : Stack overflow.\n"); return false;}
                              sp++; *sp = x;
                              OpCode++;
                              break;
            
            case OP_NEG     : if (sp < st) {printf("EquSolver : Stack underflow.\n"); return false;}
                              *sp = -*sp;
                              OpCode++;
                              break;

            case OP_ABS     : if (sp < st) {printf("EquSolver : Stack underflow.\n"); return false;}
                              *sp = fabs(*sp);
                              OpCode++;
                              break;

            case OP_ADD     : if (sp < st) {printf("EquSolver : Stack underflow.\n"); return false;}
                              *(sp-1) += *sp; sp--;
                              OpCode++;
                              break;
            
            case OP_SUB     : if (sp < st) {printf("EquSolver : Stack underflow.\n"); return false;}
                              *(sp-1) -= *sp; sp--;
                              OpCode++;
                              break;
            
            case OP_MUL     : if (sp < st) {printf("EquSolver : Stack underflow.\n"); return false;}
                              *(sp-1) *= *sp; sp--;
                              OpCode++;
                              break;
            
            case OP_DIV     : if (sp < st) {printf("EquSolver : Stack underflow.\n"); return false;}
                              *(sp-1) /= *sp; sp--;
                              OpCode++;
                              break;

            case OP_PWR     : if (sp < st) {printf("EquSolver : Stack underflow.\n"); return false;}
                              *(sp-1) = (float)pow(*(sp-1), *sp); sp--;
                              OpCode++;
                              break;

            case OP_SIN     : if (sp < st) {printf("EquSolver : Stack underflow.\n"); return false;}
                              *sp = sin(*sp);
                              OpCode++;
                              break;

            case OP_COS     : if (sp < st) {printf("EquSolver : Stack underflow.\n"); return false;}
                              *sp = cos(*sp);
                              OpCode++;
                              break;

            case OP_TAN     : if (sp < st) {printf("EquSolver : Stack underflow.\n"); return false;}
                              *sp = tan(*sp);
                              OpCode++;
                              break;

            case OP_ASIN    : if (sp < st) {printf("EquSolver : Stack underflow.\n"); return false;}
                              *sp = asin(*sp);
                              OpCode++;
                              break;

            case OP_ACOS    : if (sp < st) {printf("EquSolver : Stack underflow.\n"); return false;}
                              *sp = acos(*sp);
                              OpCode++;
                              break;

            case OP_ATAN    : if (sp < st) {printf("EquSolver : Stack underflow.\n"); return false;}
                              *sp = atan(*sp);
                              OpCode++;
                              break;

            case OP_SINH    : if (sp < st) {printf("EquSolver : Stack underflow.\n"); return false;}
                              *sp = sinh(*sp);
                              OpCode++;
                              break;

            case OP_COSH    : if (sp < st) {printf("EquSolver : Stack underflow.\n"); return false;}
                              *sp = cosh(*sp);
                              OpCode++;
                              break;

            case OP_TANH    : if (sp < st) {printf("EquSolver : Stack underflow.\n"); return false;}
                              *sp = tanh(*sp);
                              OpCode++;
                              break;

            case OP_SQRT    : if (sp < st) {printf("EquSolver : Stack underflow.\n"); return false;}
                              *sp = sqrt(*sp);
                              OpCode++;
                              break;

            case OP_EXP     : if (sp < st) {printf("EquSolver : Stack underflow.\n"); return false;}
                              *sp = exp(*sp);
                              OpCode++;
                              break;

            case OP_LN      : if (sp < st) {printf("EquSolver : Stack underflow.\n"); return false;}
                              *sp = log(*sp);
                              OpCode++;
                              break;

            case OP_LOG     : if (sp < st) {printf("EquSolver : Stack underflow.\n"); return false;}
                              *sp = log10(*sp);
                              OpCode++;
                              break;

            default         : return false;
            }
         }

      return true;
      }

   /*-------------------------------------------------------------------------
      This is a small disassempler, which debugs an equation code.
     -------------------------------------------------------------------------*/
   bool Debug(byte* Code)
      {
      if (Code == NULL) {return false;}
      bool ReturnFlag = true;

      
      //---- Do some quick analysis of the code ---
      printf("==== EquSolver Debug Information ====\n\n");
      dword DatCount = 0, OpCount = 0;
      byte* OpCode   = Code;
      while (*OpCode != OP_NULL)
         {
         OpCount++;
         if (*OpCode == OP_LD_IMM) {DatCount++; OpCode += (sizeof(float) + 1);}
         else {OpCode++;}
         }

      printf("Code Length (bytes)  : %u\n"
             "Instruction Count    : %u\n"
             "Immediate Data Count : %u\n\n",
             (dword)(OpCode - Code), OpCount, DatCount);


      //---- Display all opcode inforation ---
      OpCode = Code;
      while (*OpCode != OP_NULL)
         {
         //-- Display address --
         printf("%.8X: ", (int)(OpCode - Code));


         //-- Display opcodes --
         printf("%.2X", *OpCode);
         if (*OpCode == OP_LD_IMM)      {printf(" %.8X\t", *(dword*)(OpCode+1));}
         else if (*OpCode == OP_LD_MEM) {printf(" var_x\t");}
         else if (*OpCode == OP_ST)     {printf(" return\t");}
         else {printf("\t\t");}
         
         //-- Display opcode alias names --
         switch (*OpCode)
            {
            case OP_ST      : printf("fstp [result]\n"); OpCode++; break;
            case OP_LD_IMM  : OpCode++; 
                              printf("fld %.6f\n", *(float*)OpCode); 
                              OpCode += sizeof(float); 
                              break;
            case OP_LD_MEM  : printf("fld [x]\n"); OpCode++; break;
            case OP_NEG     : printf("fneg\n"); OpCode++; break;
            case OP_ABS     : printf("fabs\n"); OpCode++; break;
            case OP_ADD     : printf("faddp st(1),st(0)\n"); OpCode++; break;
            case OP_SUB     : printf("fsubp st(1),st(0)\n"); OpCode++; break;
            case OP_MUL     : printf("fmulp st(1),st(0)\n"); OpCode++; break;
            case OP_DIV     : printf("fdivp st(1),st(0)\n"); OpCode++; break;
            case OP_PWR     : printf("fpwrp st(1),st(0)\n"); OpCode++; break;
            case OP_SIN     : printf("fsin\n"); OpCode++; break;
            case OP_COS     : printf("fcos\n"); OpCode++; break;
            case OP_TAN     : printf("ftan\n"); OpCode++; break;
            case OP_ASIN    : printf("fasin\n"); OpCode++; break;
            case OP_ACOS    : printf("facos\n"); OpCode++; break;
            case OP_ATAN    : printf("fatan\n"); OpCode++; break;
            case OP_SINH    : printf("fsinh\n"); OpCode++; break;
            case OP_COSH    : printf("fcosh\n"); OpCode++; break;
            case OP_TANH    : printf("ftanh\n"); OpCode++; break;
            case OP_SQRT    : printf("fsqrt\n"); OpCode++; break;
            case OP_EXP     : printf("fexp\n"); OpCode++; break;
            case OP_LN      : printf("fln\n"); OpCode++; break;
            case OP_LOG     : printf("flog\n"); OpCode++; break;
            case OP_BROPEN  : printf("ERROR: Symbol is valid, but not executable: \"(\"\n"); OpCode++; ReturnFlag = false; break;
            case OP_BRCLOSE : printf("ERROR: Symbol is valid, but not executable: \")\"\n"); OpCode++; ReturnFlag = false; break;
            default         : printf("ERROR: Invalid opcode: %.2X\n", (int)*OpCode); OpCode++; ReturnFlag = false; break;
            }
         }

      return ReturnFlag;
      }

   /*-------------------------------------------------------------------------
      This is a mini compiler that converts an infix-based equation into a 
      reverse polish notaion, equation code. Returns an allocated code on 
      return, or NULL on error.

      Infix : Poiter to a string that contains the infix-based equation.
              Examples: 2.0 * (x + 3)^3;
                        (x + 3)^(1 + x);
     -------------------------------------------------------------------------*/
   byte* Compile(char* Infix)
      {
      if (Infix == NULL) {return NULL;}
      
      //Alloce a new code string   
      byte* Code = new byte[0xFFFF];
      if (Code == NULL) {return NULL;}
      
      //Setup some local variables
      byte* OpCode     = Code;
      byte* TmpCodePtr = NULL;
      
      iStackRec* op_st = NULL;
      char* InfixPtr   = Infix;
      byte  Operator   = OP_NULL;
      bool  ManOP      = false;
      bool  PrevOP     = true;
      
      //---- Process the entire input infix string ----
      InfixPtr = Infix;
      while (*InfixPtr != ';')
         {
         InfixPtr = str_skip(InfixPtr, " \t\n", true);      //Skip past spaces

         switch (*InfixPtr)
            {
            case '\0' : goto _ExitError; break;

            //-- Setup operator data --
            case '+'  : if (PrevOP) {goto _GetData;}        //If previous was an operator, this must be a sign specifier
                        Operator = OP_ADD; 
                        PrevOP = ManOP = true;              //Set previous operator, and operator manager flags to true
                        break;
            
            case '-'  : if (PrevOP) {goto _GetData;}        //If previous was an operator, this must be a sign specifier
                        Operator = OP_SUB; 
                        PrevOP = ManOP = true;              //Set previous operator, and operator manager flags to true
                        break;
            
            case '*'  : if (PrevOP) {goto _ExitError;} //If previous was an operator, this is an error
                        Operator = OP_MUL; 
                        PrevOP = ManOP = true;              //Set previous operator, and operator manager flags to true
                        break;
            
            case '/'  : if (PrevOP) {goto _ExitError;} //If previous was an operator, this is an error
                        Operator = OP_DIV; 
                        PrevOP = ManOP = true;              //Set previous operator, and operator manager flags to true
                        break;
            
            case '^'  : if (PrevOP) {goto _ExitError;} //If previous was an operator, this is an error
                        Operator = OP_PWR; 
                        PrevOP = ManOP = true;              //Set previous operator, and operator manager flags to true
                        break;

            case 'l'  : case 'e'  : case 's'  :
            case 'c'  : case 't'  : case 'a'  : 
                        if      (strnicmp(InfixPtr, "sinh", 4) == 0) {Operator = OP_SINH;}
                        else if (strnicmp(InfixPtr, "cosh", 4) == 0) {Operator = OP_COSH;}
                        else if (strnicmp(InfixPtr, "tanh", 4) == 0) {Operator = OP_TANH;}
                        else if (strnicmp(InfixPtr, "sin",  3) == 0) {Operator = OP_SIN;}
                        else if (strnicmp(InfixPtr, "cos",  3) == 0) {Operator = OP_COS;}
                        else if (strnicmp(InfixPtr, "tan",  3) == 0) {Operator = OP_TAN;}
                        else if (strnicmp(InfixPtr, "asin", 4) == 0) {Operator = OP_ASIN;}
                        else if (strnicmp(InfixPtr, "acos", 4) == 0) {Operator = OP_ACOS;}
                        else if (strnicmp(InfixPtr, "atan", 4) == 0) {Operator = OP_ATAN;}
                        else if (strnicmp(InfixPtr, "sqrt", 4) == 0) {Operator = OP_SQRT;}
                        else if (strnicmp(InfixPtr, "exp",  3) == 0) {Operator = OP_EXP;}
                        else if (strnicmp(InfixPtr, "ln",   2) == 0) {Operator = OP_LN;}
                        else if (strnicmp(InfixPtr, "log",  3) == 0) {Operator = OP_LOG;}
                        else if (strnicmp(InfixPtr, "abs",  3) == 0) {Operator = OP_ABS;}
                        else {goto _ExitError;}
                        PrevOP   = ManOP  = true;
                        InfixPtr = str_skip(InfixPtr, "asincotnqrexplghb \t\n", true) - 1;
                        break;
            
            
            //-- Handle brackets --
            case '('  : iStack.Push(op_st, (int)OP_BROPEN); //Push the open pracket
                        PrevOP = true;                      //Set previous operator to true
                        ManOP  = false;                     //Set operator manager flags to false
                        break;

            case ')'  : while (op_st != NULL)               //Pop while not NULL
                           {
                           //Break the loop if open bracket is encountered. The open bracket will be popped.
                           if (op_st->Data == OP_BROPEN) {iStack.Delete(op_st); break;}

                           //Store the operator from the stack in the destination string.
                           *OpCode = (byte)iStack.Pop(op_st); OpCode++;
                           }

                        PrevOP = false;                     //Set previous operator to true
                        ManOP  = false;                     //Set operator manager flags to false
                        break;
            
            //-- Handle data, variables, and unknowns --
            default    : 
               {
               _GetData:                                    //If jumped to this label, it must be a signed number

               //Get the symbol immediately after the sign
               char* Symbol = str_skip(InfixPtr, "+- \t\n", true);
               float Operand;
              
               //-- Handle signed varaibles --
               if (*Symbol == 'x')                          //Is the following char (after the sign) a variable?
                  {
                  if (*InfixPtr == '-') 
                     {
                     *OpCode = OP_LD_MEM; OpCode++;         //Set opcode to load variable
                     *OpCode = OP_NEG; OpCode++;            //Set opcode to negate variable
                     } 
                  else {*OpCode = OP_LD_MEM; OpCode++;}     //Set opcode to load variable
                           
                  PrevOP = ManOP = false;                   //Set previous operator, and operator manager flags to false
                  InfixPtr = str_skip(InfixPtr, "+- \t\n", true); //Skip past the sign
                  break;
                  }

               //-- Handle special signed operators, and sub-expressions --
               else if ((strnicmp(Symbol, "sinh", 4) == 0) ||
                        (strnicmp(Symbol, "cosh", 4) == 0) ||
                        (strnicmp(Symbol, "tanh", 4) == 0) ||
                        (strnicmp(Symbol, "sin",  3) == 0) ||
                        (strnicmp(Symbol, "cos",  3) == 0) ||
                        (strnicmp(Symbol, "tan",  3) == 0) ||
                        (strnicmp(Symbol, "asin", 4) == 0) ||
                        (strnicmp(Symbol, "acos", 4) == 0) ||
                        (strnicmp(Symbol, "atan", 4) == 0) ||
                        (strnicmp(Symbol, "sqrt", 4) == 0) ||
                        (strnicmp(Symbol, "exp",  3) == 0) ||
                        (strnicmp(Symbol, "ln",   2) == 0) ||
                        (strnicmp(Symbol, "log",  3) == 0) ||
                        (strnicmp(Symbol, "abs",  3) == 0) ||
                        (*Symbol == '('))
                  {
                  if (*InfixPtr == '-')                     //Is negation infront of a sub-expressions?
                     {
                     Operator = OP_NEG;                     //Negate opcode
                     PrevOP = false;
                     ManOP  = true;                         //Set true to push this opcode to the stack
                     }
                  else {PrevOP = ManOP = false;}            //For positive signs do nothing
                  break;
                  }

               //-- Handle numeric data ---
               else if ((*Symbol == '.') || ((*Symbol >= '0') && (*Symbol <= '9')))
                  {
                  InfixPtr = str_skip(InfixPtr, " \t\n", true); //Skip past spaces
                  Operand = (float)atof(InfixPtr);          //Get the float data
                  InfixPtr = str_skip(InfixPtr, "-+.0123456789eEdD", true); //Skip past numeric symbols
                  }
               
               //-- Handle unkown data ---
               else {goto _ExitError;}

               //-- If got here, it's a valid numeric data --
               InfixPtr--;                                  //Compensate, as GetFloat() has already advanced InfixPtr
               *OpCode = OP_LD_IMM; OpCode++;               //Save the opcode
                        
               *(float*)OpCode = Operand;                   //Save the associated data
               OpCode += sizeof(float);                     //Advance OpCode pointer
                        
               PrevOP = ManOP = false;                      //Set previous operator, and operator manager flags to false
               break;
               }
            }

         //-- If ManOP flag is set, do some operator managament --
         if (ManOP)
            {
            //If operator stack is empty, push it on stack
            if (op_st == NULL) {iStack.Push(op_st, (int)Operator);}

            //If the operator on the stack is an open bracket, push 
            // the current operator on stack.
            else if (op_st->Data == OP_BROPEN) {iStack.Push(op_st, (int)Operator);}

            //If the current operator is greater in priority than the one 
            // on the stack, push it on stack.
            else if ((Operator & OP_PR_MASK) > (op_st->Data & OP_PR_MASK)) {iStack.Push(op_st, (int)Operator);}

            //If got here, all the greater priority operator on the stack
            // must be popped off.
            else
               {
               //Pop while not NULL
               while (op_st != NULL)
                  {
                  //Break the loop if the operator on the stack has 
                  // a lower priority, OR if an open bracket is encountered.
                  if (((Operator & OP_PR_MASK) > (op_st->Data & OP_PR_MASK)) || (op_st->Data == OP_BROPEN)) {break;}

                  //Store the operator from the stack in the destination string.
                  *OpCode = (byte)iStack.Pop(op_st); OpCode++;
                  }
               
               //Finally, the current operator in pushed on the stack.
               iStack.Push(op_st, (int)Operator);
               }                                   //END else
            }                                      //END if (Operator != OP_NULL)

         InfixPtr++;
         }                                         //END while (*InfixPtr != 0)


      //-- Before exit, all the remining operators are removed from the stack --
      while (op_st != NULL) {*OpCode = (byte)iStack.Pop(op_st); OpCode++;}


      //Append a store opcode and then terminate the code string.
      *OpCode = OP_ST; OpCode++;
      *OpCode = OP_NULL; OpCode++;

      //Re-allocate the code string for efficient memory storage      
      TmpCodePtr = (byte*)realloc(Code, (int)(OpCode - Code));
      if (TmpCodePtr == NULL) {goto _ExitError;}
      Code = TmpCodePtr;
         
      //Print some information
      printf("==== EquSolver Compiler Information ====\n\n"
             "Input Equation       : %s\n"
             "Code Length (bytes)  : %u\n\n", 
             Infix, (int)(OpCode - Code)-1);      //Don't count OP_NULL


      //---- Successful exit ----
      return Code;


      //---- Exit with error ----
      _ExitError:
      if (Code != NULL) {delete[] Code;}
      iStack.Nuke(op_st);
      return NULL;
      }


   /*==== End Class =============================================================*/
   };


/*----------------------------------------------------------------------------
  Global Declarations.
  ----------------------------------------------------------------------------*/
EquSolverClass EquSolver;





/*==== End of file ===========================================================*/
#endif