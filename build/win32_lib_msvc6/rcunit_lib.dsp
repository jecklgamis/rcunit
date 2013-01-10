# Microsoft Developer Studio Project File - Name="rcunit_lib" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Static Library" 0x0104

CFG=rcunit_lib - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "rcunit_lib.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "rcunit_lib.mak" CFG="rcunit_lib - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "rcunit_lib - Win32 Release" (based on "Win32 (x86) Static Library")
!MESSAGE "rcunit_lib - Win32 Debug" (based on "Win32 (x86) Static Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
RSC=rc.exe

!IF  "$(CFG)" == "rcunit_lib - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir ".\release\out"
# PROP Intermediate_Dir ".\release\obj"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_MBCS" /D "_LIB" /YX /FD /c
# ADD CPP /nologo /W3 /GX /O2 /I "..\..\src\\" /D "WIN32" /D "NDEBUG" /D "_MBCS" /D "_LIB" /YX /FD /c
# SUBTRACT CPP /Fr
# ADD BASE RSC /l 0x411 /d "NDEBUG"
# ADD RSC /l 0x411 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo /out:".\release\out\rcunit.lib"

!ELSEIF  "$(CFG)" == "rcunit_lib - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir ".\debug\out"
# PROP Intermediate_Dir ".\debug\obj"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_MBCS" /D "_LIB" /YX /FD /GZ /c
# ADD CPP /nologo /W3 /Gm /GX /ZI /Od /I "..\..\src\\" /D "WIN32" /D "_DEBUG" /D "_MBCS" /D "_LIB" /D "RCU_DEBUG" /D "HAVE_CONFIG_H" /YX /FD /GZ /c
# SUBTRACT CPP /Fr
# ADD BASE RSC /l 0x411 /d "_DEBUG"
# ADD RSC /l 0x411 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo /out:".\debug\out\rcunit.lib"

!ENDIF 

# Begin Target

# Name "rcunit_lib - Win32 Release"
# Name "rcunit_lib - Win32 Debug"
# Begin Group "rcunit"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=..\..\src\rcunit.c
# End Source File
# Begin Source File

SOURCE=..\..\src\rcunit_error.c
# End Source File
# Begin Source File

SOURCE=..\..\src\rcunit_exception.c
# End Source File
# Begin Source File

SOURCE=..\..\src\rcunit_hashtable.c
# End Source File
# Begin Source File

SOURCE=..\..\src\rcunit_io_device.c
# End Source File
# Begin Source File

SOURCE=..\..\src\rcunit_list.c
# End Source File
# Begin Source File

SOURCE=..\..\src\rcunit_log.c
# End Source File
# Begin Source File

SOURCE=..\..\src\rcunit_log_hnd_file.c
# End Source File
# Begin Source File

SOURCE=..\..\src\rcunit_log_hnd_stdout.c
# End Source File
# Begin Source File

SOURCE=..\..\src\rcunit_mem.c
# End Source File
# Begin Source File

SOURCE=..\..\src\rcunit_mtrace.c
# End Source File
# Begin Source File

SOURCE=..\..\src\rcunit_random.c
# End Source File
# Begin Source File

SOURCE=..\..\src\rcunit_report.c
# End Source File
# End Group
# End Target
# End Project
