# Microsoft Developer Studio Generated NMAKE File, Based on cosmic_ray_tau.dsp
!IF "$(CFG)" == ""
CFG=cosmic_ray_tau - Win32 Debug
!MESSAGE No configuration specified. Defaulting to cosmic_ray_tau - Win32 Debug.
!ENDIF 

!IF "$(CFG)" != "cosmic_ray_tau - Win32 Release" && "$(CFG)" != "cosmic_ray_tau - Win32 Debug" && "$(CFG)" != "cosmic_ray_tau - Win32 Release_pentium_mmx"
!MESSAGE Invalid configuration "$(CFG)" specified.
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "cosmic_ray_tau.mak" CFG="cosmic_ray_tau - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "cosmic_ray_tau - Win32 Release" (based on "Win32 (x86) Console Application")
!MESSAGE "cosmic_ray_tau - Win32 Debug" (based on "Win32 (x86) Console Application")
!MESSAGE "cosmic_ray_tau - Win32 Release_pentium_mmx" (based on "Win32 (x86) Console Application")
!MESSAGE 
!ERROR An invalid configuration is specified.
!ENDIF 

!IF "$(OS)" == "Windows_NT"
NULL=
!ELSE 
NULL=nul
!ENDIF 

CPP=cl.exe
RSC=rc.exe

!IF  "$(CFG)" == "cosmic_ray_tau - Win32 Release"

OUTDIR=.\Release
INTDIR=.\Release
# Begin Custom Macros
OutDir=.\Release
# End Custom Macros

ALL : "$(OUTDIR)\cosmic_ray_tau.exe"


CLEAN :
	-@erase "$(INTDIR)\cosmic_ray_tau.obj"
	-@erase "$(INTDIR)\vc60.idb"
	-@erase "$(OUTDIR)\cosmic_ray_tau.exe"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP_PROJ=/nologo /G6 /Gr /Zp16 /MT /W3 /GX /O2 /Ob2 /D "WIN32" /D "NDEBUG" /D "_CONSOLE" /D "_MBCS" /Fp"$(INTDIR)\cosmic_ray_tau.pch" /YX /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\cosmic_ray_tau.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
LINK32_FLAGS=ddraw.lib dxguid.lib dinput.lib glut32.lib glu32.lib opengl32.lib glaux.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /incremental:no /pdb:"$(OUTDIR)\cosmic_ray_tau.pdb" /machine:I386 /out:"$(OUTDIR)\cosmic_ray_tau.exe" 
LINK32_OBJS= \
	"$(INTDIR)\cosmic_ray_tau.obj"

"$(OUTDIR)\cosmic_ray_tau.exe" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ELSEIF  "$(CFG)" == "cosmic_ray_tau - Win32 Debug"

OUTDIR=.\Debug
INTDIR=.\Debug
# Begin Custom Macros
OutDir=.\Debug
# End Custom Macros

ALL : "$(OUTDIR)\cosmic_ray_tau.exe" "$(OUTDIR)\cosmic_ray_tau.bsc"


CLEAN :
	-@erase "$(INTDIR)\cosmic_ray_tau.obj"
	-@erase "$(INTDIR)\cosmic_ray_tau.sbr"
	-@erase "$(INTDIR)\vc60.idb"
	-@erase "$(INTDIR)\vc60.pdb"
	-@erase "$(OUTDIR)\cosmic_ray_tau.bsc"
	-@erase "$(OUTDIR)\cosmic_ray_tau.exe"
	-@erase "$(OUTDIR)\cosmic_ray_tau.ilk"
	-@erase "$(OUTDIR)\cosmic_ray_tau.pdb"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP_PROJ=/nologo /Zp16 /MTd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /D "_MBCS" /FR"$(INTDIR)\\" /Fp"$(INTDIR)\cosmic_ray_tau.pch" /YX /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /GZ /c 
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\cosmic_ray_tau.bsc" 
BSC32_SBRS= \
	"$(INTDIR)\cosmic_ray_tau.sbr"

"$(OUTDIR)\cosmic_ray_tau.bsc" : "$(OUTDIR)" $(BSC32_SBRS)
    $(BSC32) @<<
  $(BSC32_FLAGS) $(BSC32_SBRS)
<<

LINK32=link.exe
LINK32_FLAGS=ddraw.lib dxguid.lib dinput.lib glut32.lib glu32.lib opengl32.lib glaux.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /incremental:yes /pdb:"$(OUTDIR)\cosmic_ray_tau.pdb" /debug /machine:I386 /out:"$(OUTDIR)\cosmic_ray_tau.exe" /pdbtype:sept 
LINK32_OBJS= \
	"$(INTDIR)\cosmic_ray_tau.obj"

"$(OUTDIR)\cosmic_ray_tau.exe" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ELSEIF  "$(CFG)" == "cosmic_ray_tau - Win32 Release_pentium_mmx"

OUTDIR=.\Release_pentium_mmx
INTDIR=.\Release_pentium_mmx
# Begin Custom Macros
OutDir=.\Release_pentium_mmx
# End Custom Macros

ALL : "$(OUTDIR)\cosmic_ray_tau.exe"


CLEAN :
	-@erase "$(INTDIR)\cosmic_ray_tau.obj"
	-@erase "$(INTDIR)\vc60.idb"
	-@erase "$(OUTDIR)\cosmic_ray_tau.exe"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP_PROJ=/nologo /G5 /Gr /Zp16 /MT /W3 /GX /O2 /Ob2 /D "WIN32" /D "NDEBUG" /D "_CONSOLE" /D "_MBCS" /Fp"$(INTDIR)\cosmic_ray_tau.pch" /YX /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\cosmic_ray_tau.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
LINK32_FLAGS=ddraw.lib dxguid.lib dinput.lib glut32.lib glu32.lib opengl32.lib glaux.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /incremental:no /pdb:"$(OUTDIR)\cosmic_ray_tau.pdb" /machine:I386 /out:"$(OUTDIR)\cosmic_ray_tau.exe" 
LINK32_OBJS= \
	"$(INTDIR)\cosmic_ray_tau.obj"

"$(OUTDIR)\cosmic_ray_tau.exe" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ENDIF 

.c{$(INTDIR)}.obj::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cpp{$(INTDIR)}.obj::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cxx{$(INTDIR)}.obj::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.c{$(INTDIR)}.sbr::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cpp{$(INTDIR)}.sbr::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cxx{$(INTDIR)}.sbr::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<


!IF "$(NO_EXTERNAL_DEPS)" != "1"
!IF EXISTS("cosmic_ray_tau.dep")
!INCLUDE "cosmic_ray_tau.dep"
!ELSE 
!MESSAGE Warning: cannot find "cosmic_ray_tau.dep"
!ENDIF 
!ENDIF 


!IF "$(CFG)" == "cosmic_ray_tau - Win32 Release" || "$(CFG)" == "cosmic_ray_tau - Win32 Debug" || "$(CFG)" == "cosmic_ray_tau - Win32 Release_pentium_mmx"
SOURCE=.\source\cosmic_ray_tau.cpp

!IF  "$(CFG)" == "cosmic_ray_tau - Win32 Release"


"$(INTDIR)\cosmic_ray_tau.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "cosmic_ray_tau - Win32 Debug"


"$(INTDIR)\cosmic_ray_tau.obj"	"$(INTDIR)\cosmic_ray_tau.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "cosmic_ray_tau - Win32 Release_pentium_mmx"


"$(INTDIR)\cosmic_ray_tau.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 


!ENDIF 

