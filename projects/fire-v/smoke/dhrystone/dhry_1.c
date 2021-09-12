/*
 ****************************************************************************
 *
 *                   "DHRYSTONE" Benchmark Program
 *                   -----------------------------
 *
 *  Version:    C, Version 2.1
 *
 *  File:       dhry_1.c (part 2 of 3)
 *
 *  Date:       May 25, 1988
 *
 *  Author:     Reinhold P. Weicker
 *
 ****************************************************************************
 */

#define SHOW_PRINTF 1

#include "dhry.h"

#ifdef USE_MYSTDLIB
extern char     *malloc ();
#else
#  include <stdlib.h>
#  include <string.h>
#endif

/* Global Variables: */

Rec_Pointer     Ptr_Glob,
                Next_Ptr_Glob;
int             Int_Glob;
Boolean         Bool_Glob;
char            Ch_1_Glob,
                Ch_2_Glob;
int             Arr_1_Glob [50];
int             Arr_2_Glob [50] [50];

Enumeration     Func_1 ();
  /* forward declaration necessary since Enumeration may not simply be int */

#ifndef REG
        Boolean Reg = false;
#define REG
        /* REG becomes defined as empty */
        /* i.e. no register variables   */
#else
        Boolean Reg = true;
#endif

/* variables for time measurement: */

#ifdef IGN_TIMES
#define HZ 50000000
struct tms      time_info;
extern  time_t     times ();
                /* see library function "times" */
#define Too_Small_Time 120
                /* Measurements should last at least about 2 seconds */
#endif
#ifdef TIME
extern long     time();
#ifdef RISCV
extern long     insn();
#endif
                /* see library function "time"  */
#define Too_Small_Time 2
                /* Measurements should last at least 2 seconds */
#endif

long            Begin_Time,
                End_Time,
                User_Time;
#ifdef RISCV
long            Begin_Insn,
                End_Insn,
                User_Insn;
#endif
float           Microseconds,
                Dhrystones_Per_Second;
/* end of variables for time measurement */


main ()
/*****/

  /* main program, corresponds to procedures        */
  /* Main and Proc_0 in the Ada version             */
{
        One_Fifty       Int_1_Loc;
  REG   One_Fifty       Int_2_Loc;
        One_Fifty       Int_3_Loc;
  REG   char            Ch_Index;
        Enumeration     Enum_Loc;
        Str_30          Str_1_Loc;
        Str_30          Str_2_Loc;
  REG   int             Run_Index;
  REG   int             Number_Of_Runs;

  Rec_Type R1,R2;
 
  /* Initializations */

  /* Replacing malloc by static structures */
  Next_Ptr_Glob = &R1; // (Rec_Pointer) malloc (sizeof (Rec_Type));
  Ptr_Glob = &R2; // (Rec_Pointer) malloc (sizeof (Rec_Type));
   
  Ptr_Glob->Ptr_Comp                    = Next_Ptr_Glob;
  Ptr_Glob->Discr                       = Ident_1;
  Ptr_Glob->variant.var_1.Enum_Comp     = Ident_3;
  Ptr_Glob->variant.var_1.Int_Comp      = 40;
  strcpy (Ptr_Glob->variant.var_1.Str_Comp,
          "DHRYSTONE PROGRAM, SOME STRING");
  strcpy (Str_1_Loc, "DHRYSTONE PROGRAM, 1'ST STRING");

  Arr_2_Glob [8][7] = 10;
        /* Was missing in published program. Without this statement,    */
        /* Arr_2_Glob [8][7] would have an undefined value.             */
        /* Warning: With 16-Bit processors and Number_Of_Runs > 32000,  */
        /* overflow may occur for this array element.                   */

  set_draw_buffer(1);
  if(SHOW_PRINTF) printf ("\n");
  if(SHOW_PRINTF) printf ("Dhrystone Benchmark, Version 2.1 (Language: C)\n");
  if(SHOW_PRINTF) printf ("\n");
  if (Reg)
  {
    if(SHOW_PRINTF) printf ("Program compiled with 'register' attribute\n");
    if(SHOW_PRINTF) printf ("\n");
  }
  else
  {
    if(SHOW_PRINTF) printf ("Program compiled without 'register' attribute\n");
    if(SHOW_PRINTF) printf ("\n");
  }
  if(SHOW_PRINTF) printf ("Please give the number of runs through the benchmark: ");
  {
    // int n;
    // scanf ("%d", &n);
    Number_Of_Runs = 100;
  }
  if(SHOW_PRINTF) printf ("\n");

  if(SHOW_PRINTF) printf ("Execution starts, %d runs through Dhrystone\n", Number_Of_Runs);

  /***************/
  /* Start timer */
  /***************/

#ifdef IGN_TIMES
  times (&time_info);
  Begin_Time = (long) time_info.tms_utime;
#endif
#ifdef TIME
  Begin_Time = time ( (long *) 0);
#ifdef RISCV
  Begin_Insn = insn ( (long *) 0);
#endif
#endif

  for (Run_Index = 1; Run_Index <= Number_Of_Runs; ++Run_Index)
  {
    Proc_5();
    Proc_4();
      /* Ch_1_Glob == 'A', Ch_2_Glob == 'B', Bool_Glob == true */
    Int_1_Loc = 2;
    Int_2_Loc = 3;
    strcpy (Str_2_Loc, "DHRYSTONE PROGRAM, 2'ND STRING");
    Enum_Loc = Ident_2;
    Bool_Glob = ! Func_2 (Str_1_Loc, Str_2_Loc);
      /* Bool_Glob == 1 */
    while (Int_1_Loc < Int_2_Loc)  /* loop body executed once */
    {
      Int_3_Loc = 5 * Int_1_Loc - Int_2_Loc;
        /* Int_3_Loc == 7 */
      Proc_7 (Int_1_Loc, Int_2_Loc, &Int_3_Loc);
        /* Int_3_Loc == 7 */
      Int_1_Loc += 1;
    } /* while */
      /* Int_1_Loc == 3, Int_2_Loc == 3, Int_3_Loc == 7 */
    Proc_8 (Arr_1_Glob, Arr_2_Glob, Int_1_Loc, Int_3_Loc);
      /* Int_Glob == 5 */
    Proc_1 (Ptr_Glob);
    for (Ch_Index = 'A'; Ch_Index <= Ch_2_Glob; ++Ch_Index)
                             /* loop body executed twice */
    {
      if (Enum_Loc == Func_1 (Ch_Index, 'C'))
          /* then, not executed */
        {
        Proc_6 (Ident_1, &Enum_Loc);
        strcpy (Str_2_Loc, "DHRYSTONE PROGRAM, 3'RD STRING");
        Int_2_Loc = Run_Index;
        Int_Glob = Run_Index;
        }
    }
      /* Int_1_Loc == 3, Int_2_Loc == 3, Int_3_Loc == 7 */
    Int_2_Loc = Int_2_Loc * Int_1_Loc;
    Int_1_Loc = Int_2_Loc / Int_3_Loc;
    Int_2_Loc = 7 * (Int_2_Loc - Int_3_Loc) - Int_1_Loc;
      /* Int_1_Loc == 1, Int_2_Loc == 13, Int_3_Loc == 7 */
    Proc_2 (&Int_1_Loc);
      /* Int_1_Loc == 5 */

  } /* loop "for Run_Index" */

  /**************/
  /* Stop timer */
  /**************/

#ifdef IGN_TIMES
  times (&time_info);
  End_Time = (long) time_info.tms_utime;
#endif
#ifdef TIME
  End_Time = time ( (long *) 0);
#ifdef RISCV
  End_Insn = insn ( (long *) 0);
#endif
#endif

  if(SHOW_PRINTF) printf ("Execution ends\n");
  if(SHOW_PRINTF) printf ("\n");
  if(SHOW_PRINTF) printf ("Final values of the variables used in the benchmark:\n");
  if(SHOW_PRINTF) printf ("\n");
  if(SHOW_PRINTF) printf ("Int_Glob:            %d\n", Int_Glob);
  if(SHOW_PRINTF) printf ("        should be:   %d\n", 5);
  if(SHOW_PRINTF) printf ("Bool_Glob:           %d\n", Bool_Glob);
  if(SHOW_PRINTF) printf ("        should be:   %d\n", 1);
  if(SHOW_PRINTF) printf ("Ch_1_Glob:           %c\n", Ch_1_Glob);
  if(SHOW_PRINTF) printf ("        should be:   %c\n", 'A');
  if(SHOW_PRINTF) printf ("Ch_2_Glob:           %c\n", Ch_2_Glob);
  if(SHOW_PRINTF) printf ("        should be:   %c\n", 'B');
  if(SHOW_PRINTF) printf ("Arr_1_Glob[8]:       %d\n", Arr_1_Glob[8]);
  if(SHOW_PRINTF) printf ("        should be:   %d\n", 7);
  if(SHOW_PRINTF) printf ("Arr_2_Glob[8][7]:    %d\n", Arr_2_Glob[8][7]);
  if(SHOW_PRINTF) printf ("        should be:   Number_Of_Runs + 10\n");
  if(SHOW_PRINTF) printf ("Ptr_Glob->\n");
  if(SHOW_PRINTF) printf ("  Ptr_Comp:          %d\n", (int) Ptr_Glob->Ptr_Comp);
  if(SHOW_PRINTF) printf ("        should be:   (implementation-dependent)\n");
  if(SHOW_PRINTF) printf ("  Discr:             %d\n", Ptr_Glob->Discr);
  if(SHOW_PRINTF) printf ("        should be:   %d\n", 0);
  if(SHOW_PRINTF) printf ("  Enum_Comp:         %d\n", Ptr_Glob->variant.var_1.Enum_Comp);
  if(SHOW_PRINTF) printf ("        should be:   %d\n", 2);
  if(SHOW_PRINTF) printf ("  Int_Comp:          %d\n", Ptr_Glob->variant.var_1.Int_Comp);
  if(SHOW_PRINTF) printf ("        should be:   %d\n", 17);
  if(SHOW_PRINTF) printf ("  Str_Comp:          %s\n", Ptr_Glob->variant.var_1.Str_Comp);
  if(SHOW_PRINTF) printf ("        should be:   DHRYSTONE PROGRAM, SOME STRING\n");
  if(SHOW_PRINTF) printf ("Next_Ptr_Glob->\n");
  if(SHOW_PRINTF) printf ("  Ptr_Comp:          %d\n", (int) Next_Ptr_Glob->Ptr_Comp);
  if(SHOW_PRINTF) printf ("        should be:   (implementation-dependent), same as above\n");
  if(SHOW_PRINTF) printf ("  Discr:             %d\n", Next_Ptr_Glob->Discr);
  if(SHOW_PRINTF) printf ("        should be:   %d\n", 0);
  if(SHOW_PRINTF) printf ("  Enum_Comp:         %d\n", Next_Ptr_Glob->variant.var_1.Enum_Comp);
  if(SHOW_PRINTF) printf ("        should be:   %d\n", 1);
  if(SHOW_PRINTF) printf ("  Int_Comp:          %d\n", Next_Ptr_Glob->variant.var_1.Int_Comp);
  if(SHOW_PRINTF) printf ("        should be:   %d\n", 18);
  if(SHOW_PRINTF) printf ("  Str_Comp:          %s\n",
                                Next_Ptr_Glob->variant.var_1.Str_Comp);
  if(SHOW_PRINTF) printf ("        should be:   DHRYSTONE PROGRAM, SOME STRING\n");
  if(SHOW_PRINTF) printf ("Int_1_Loc:           %d\n", Int_1_Loc);
  if(SHOW_PRINTF) printf ("        should be:   %d\n", 5);
  if(SHOW_PRINTF) printf ("Int_2_Loc:           %d\n", Int_2_Loc);
  if(SHOW_PRINTF) printf ("        should be:   %d\n", 13);
  if(SHOW_PRINTF) printf ("Int_3_Loc:           %d\n", Int_3_Loc);
  if(SHOW_PRINTF) printf ("        should be:   %d\n", 7);
  if(SHOW_PRINTF) printf ("Enum_Loc:            %d\n", Enum_Loc);
  if(SHOW_PRINTF) printf ("        should be:   %d\n", 1);
  if(SHOW_PRINTF) printf ("Str_1_Loc:           %s\n", Str_1_Loc);
  if(SHOW_PRINTF) printf ("        should be:   DHRYSTONE PROGRAM, 1'ST STRING\n");
  if(SHOW_PRINTF) printf ("Str_2_Loc:           %s\n", Str_2_Loc);
  if(SHOW_PRINTF) printf ("        should be:   DHRYSTONE PROGRAM, 2'ND STRING\n");
  if(SHOW_PRINTF) printf ("\n");

  User_Time = End_Time - Begin_Time;

#ifdef RISCV
  User_Insn = End_Insn - Begin_Insn;

  if(SHOW_PRINTF) printf("Number_Of_Runs: %d\n", Number_Of_Runs);
  if(1) printf("User_Time: %d cycles, %d insn\n", User_Time, User_Insn);

  int Cycles_Per_Instruction_x1000 = (1000 * User_Time) / User_Insn;
  if(1) printf("Cycles_Per_Instruction: %d.%d%d%d\n", Cycles_Per_Instruction_x1000 / 1000,
		(Cycles_Per_Instruction_x1000 / 100) % 10,
		(Cycles_Per_Instruction_x1000 / 10) % 10,
		(Cycles_Per_Instruction_x1000 / 1) % 10);

  int Dhrystones_Per_Second_Per_MHz = (Number_Of_Runs * 1000000) / User_Time;
  if(SHOW_PRINTF) printf("Dhrystones_Per_Second_Per_MHz: %d\n", Dhrystones_Per_Second_Per_MHz);

   /*
    * "Another common representation of the Dhrystone benchmark is the DMIPS (Dhrystone MIPS) obtained 
    * when the Dhrystone score is divided by 1757 (the number of Dhrystones per second obtained on the 
    * VAX 11/780, nominally a 1 MIPS machine)."
    */
   
  int DMIPS_Per_MHz_x1000 = (1000 * Dhrystones_Per_Second_Per_MHz) / 1757;
  if(SHOW_PRINTF) printf("DMIPS_Per_MHz: %d.%d%d%d\n", DMIPS_Per_MHz_x1000 / 1000,
		(DMIPS_Per_MHz_x1000 / 100) % 10,
		(DMIPS_Per_MHz_x1000 / 10) % 10,
		(DMIPS_Per_MHz_x1000 / 1) % 10);
#else
  if (User_Time < Too_Small_Time)
  {
    if(SHOW_PRINTF) printf ("Measured time too small to obtain meaningful results\n");
    if(SHOW_PRINTF) printf ("Please increase number of runs\n");
    if(SHOW_PRINTF) printf ("\n");
  }
  else
  {
#ifdef TIME
    Microseconds = (float) User_Time * Mic_secs_Per_Second
                        / (float) Number_Of_Runs;
    Dhrystones_Per_Second = (float) Number_Of_Runs / (float) User_Time;
#else
    Microseconds = (float) User_Time * Mic_secs_Per_Second
                        / ((float) HZ * ((float) Number_Of_Runs));
    Dhrystones_Per_Second = ((float) HZ * (float) Number_Of_Runs)
                        / (float) User_Time;
#endif
    if(SHOW_PRINTF) printf ("Microseconds for one run through Dhrystone: ");
    if(SHOW_PRINTF) printf ("%6.1f \n", Microseconds);
    if(SHOW_PRINTF) printf ("Dhrystones per Second:                      ");
    if(SHOW_PRINTF) printf ("%6.1f \n", Dhrystones_Per_Second);
    if(SHOW_PRINTF) printf ("\n");
  }
#endif
  asm(".word 0\n"); // SL: halts CPU
}


Proc_1 (Ptr_Val_Par)
/******************/

REG Rec_Pointer Ptr_Val_Par;
    /* executed once */
{
  REG Rec_Pointer Next_Record = Ptr_Val_Par->Ptr_Comp;
                                        /* == Ptr_Glob_Next */
  /* Local variable, initialized with Ptr_Val_Par->Ptr_Comp,    */
  /* corresponds to "rename" in Ada, "with" in Pascal           */

  structassign (*Ptr_Val_Par->Ptr_Comp, *Ptr_Glob);
  Ptr_Val_Par->variant.var_1.Int_Comp = 5;
  Next_Record->variant.var_1.Int_Comp
        = Ptr_Val_Par->variant.var_1.Int_Comp;
  Next_Record->Ptr_Comp = Ptr_Val_Par->Ptr_Comp;
  Proc_3 (&Next_Record->Ptr_Comp);
    /* Ptr_Val_Par->Ptr_Comp->Ptr_Comp
                        == Ptr_Glob->Ptr_Comp */
  if (Next_Record->Discr == Ident_1)
    /* then, executed */
  {
    Next_Record->variant.var_1.Int_Comp = 6;
    Proc_6 (Ptr_Val_Par->variant.var_1.Enum_Comp,
           &Next_Record->variant.var_1.Enum_Comp);
    Next_Record->Ptr_Comp = Ptr_Glob->Ptr_Comp;
    Proc_7 (Next_Record->variant.var_1.Int_Comp, 10,
           &Next_Record->variant.var_1.Int_Comp);
  }
  else /* not executed */
    structassign (*Ptr_Val_Par, *Ptr_Val_Par->Ptr_Comp);
} /* Proc_1 */


Proc_2 (Int_Par_Ref)
/******************/
    /* executed once */
    /* *Int_Par_Ref == 1, becomes 4 */

One_Fifty   *Int_Par_Ref;
{
  One_Fifty  Int_Loc;
  Enumeration   Enum_Loc;

  Int_Loc = *Int_Par_Ref + 10;
  do /* executed once */
    if (Ch_1_Glob == 'A')
      /* then, executed */
    {
      Int_Loc -= 1;
      *Int_Par_Ref = Int_Loc - Int_Glob;
      Enum_Loc = Ident_1;
    } /* if */
  while (Enum_Loc != Ident_1); /* true */
} /* Proc_2 */


Proc_3 (Ptr_Ref_Par)
/******************/
    /* executed once */
    /* Ptr_Ref_Par becomes Ptr_Glob */

Rec_Pointer *Ptr_Ref_Par;

{
  if (Ptr_Glob != Null)
    /* then, executed */
    *Ptr_Ref_Par = Ptr_Glob->Ptr_Comp;
  Proc_7 (10, Int_Glob, &Ptr_Glob->variant.var_1.Int_Comp);
} /* Proc_3 */


Proc_4 () /* without parameters */
/*******/
    /* executed once */
{
  Boolean Bool_Loc;

  Bool_Loc = Ch_1_Glob == 'A';
  Bool_Glob = Bool_Loc | Bool_Glob;
  Ch_2_Glob = 'B';
} /* Proc_4 */


Proc_5 () /* without parameters */
/*******/
    /* executed once */
{
  Ch_1_Glob = 'A';
  Bool_Glob = false;
} /* Proc_5 */


        /* Procedure for the assignment of structures,          */
        /* if the C compiler doesn't support this feature       */
#ifdef  NOSTRUCTASSIGN
memcpy (d, s, l)
register char   *d;
register char   *s;
register int    l;
{
        while (l--) *d++ = *s++;
}
#endif


