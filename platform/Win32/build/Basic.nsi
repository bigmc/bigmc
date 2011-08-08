;NSIS Modern User Interface
;Basic Example Script
;Written by Joost Verburg

;--------------------------------
;Include Modern UI

  !include "MUI2.nsh"

;--------------------------------
;General

  ;Name and file
  Name "BigMC Installer"
  OutFile "BigMC_Installer.exe"

  ;Default installation folder
  InstallDir "$PROGRAMFILES\BigMC"
  
  ;Get installation folder from registry if available
  InstallDirRegKey HKCU "Software\BigMC" ""

  ;Request application privileges for Windows Vista
  RequestExecutionLevel user

;--------------------------------
;Interface Settings

  !define MUI_ABORTWARNING

;--------------------------------
;Pages

  !insertmacro MUI_PAGE_DIRECTORY
  !insertmacro MUI_PAGE_INSTFILES
  
  !insertmacro MUI_UNPAGE_CONFIRM
  !insertmacro MUI_UNPAGE_INSTFILES
  
;--------------------------------
;Languages
 
  !insertmacro MUI_LANGUAGE "English"

;--------------------------------
;Installer Sections

Section ""

  SetOutPath "$INSTDIR"
  
  ;ADD YOUR OWN FILES HERE... 
  File BigMC-GUI.exe
  SetOutPath "$INSTDIR\bin"
  File bin\bigmc.exe
  SetOutPath "$INSTDIR\conf"
  File conf\bigmc.conf
  SetOutPath "$INSTDIR\doc\examples"
  File doc\examples\airport.bgm
  File doc\examples\airport2.bgm
  File doc\examples\ccs.bgm
  File doc\examples\ccs2.bgm
  File doc\examples\dining.bgm
  File doc\examples\dining_mutex.bgm
  File doc\examples\diverge.bgm
  File doc\examples\diverge_prop.bgm
  File doc\examples\foo.bgm
  File doc\examples\foo2.bgm
  File doc\examples\foo3.bgm
  File doc\examples\hole.bgm
  File doc\examples\link.bgm
  File doc\examples\match.bgm
  File doc\examples\nil.bgm
  File doc\examples\notify.bgm
  File doc\examples\par.bgm
  File doc\examples\par2.bgm
  File doc\examples\prop.bgm
  File doc\examples\subtree.bgm
  File doc\examples\terminal.bgm
  File doc\examples\unordered_subtree.bgm
  File doc\examples\wide.bgm
  File doc\examples\wide2.bgm
  File doc\examples\wide3.bgm
  SetOutPath "$INSTDIR\doc\theory"
  File doc\theory\match.sml
  SetOutPath "$INSTDIR\share\info"
  File share\info\bigmc.info
  SetOutPath "$INSTDIR\share\man\man1"
  File share\man\man1\bigmc.1

  ;Store installation folder
  WriteRegStr HKCU "Software\BigMC" "" $INSTDIR
  
  ;Create uninstaller
  WriteRegStr HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\BigMC" "DisplayName" "BigMC (remove only)"
  WriteRegStr HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\BigMC" "UninstallString" "$INSTDIR\Uninstall.exe"

  WriteUninstaller "$INSTDIR\Uninstall.exe"

  ; Now create shortcuts
  CreateShortCut "$SMPROGRAMS\BigMC.lnk" "$INSTDIR\BigMC-GUI.exe"

SectionEnd

;--------------------------------
;Uninstaller Section

Section "Uninstall"

  ;ADD YOUR OWN FILES HERE..

  Delete "$INSTDIR\BigMC-GUI.exe"
  
  Delete "$INSTDIR\bin\bigmc.exe"
  RMDir "$INSTDIR\bin"

  Delete "$INSTDIR\conf\bigmc.conf"
  RMDir "$INSTDIR\conf"

  Delete "$INSTDIR\doc\examples\airport.bgm"
  Delete "$INSTDIR\doc\examples\airport2.bgm"
  Delete "$INSTDIR\doc\examples\ccs.bgm"
  Delete "$INSTDIR\doc\examples\ccs2.bgm"
  Delete "$INSTDIR\doc\examples\dining.bgm"
  Delete "$INSTDIR\doc\examples\dining_mutex.bgm"
  Delete "$INSTDIR\doc\examples\diverge.bgm"
  Delete "$INSTDIR\doc\examples\diverge_prop.bgm"
  Delete "$INSTDIR\doc\examples\foo.bgm"
  Delete "$INSTDIR\doc\examples\foo2.bgm"
  Delete "$INSTDIR\doc\examples\foo3.bgm"
  Delete "$INSTDIR\doc\examples\hole.bgm"
  Delete "$INSTDIR\doc\examples\link.bgm"
  Delete "$INSTDIR\doc\examples\match.bgm"
  Delete "$INSTDIR\doc\examples\nil.bgm"
  Delete "$INSTDIR\doc\examples\notify.bgm"
  Delete "$INSTDIR\doc\examples\par.bgm"
  Delete "$INSTDIR\doc\examples\par2.bgm"
  Delete "$INSTDIR\doc\examples\prop.bgm"
  Delete "$INSTDIR\doc\examples\subtree.bgm"
  Delete "$INSTDIR\doc\examples\terminal.bgm"
  Delete "$INSTDIR\doc\examples\unordered_subtree.bgm"
  Delete "$INSTDIR\doc\examples\wide.bgm"
  Delete "$INSTDIR\doc\examples\wide2.bgm"
  Delete "$INSTDIR\doc\examples\wide3.bgm"
  RMDir "$INSTDIR\doc\examples"

  Delete "$INSTDIR\doc\theory\match.sml"
  RMDir "$INSTDIR\doc\theory"
  RMDir "$INSTDIR\doc"

  Delete "$INSTDIR\share\info\bigmc.info"
  RMDir "$INSTDIR\share\info"

  Delete "$INSTDIR\share\man\man1\bigmc.1"

  RMDir "$INSTDIR\share\man\man1"
  RMDir "$INSTDIR\share\man"

  RMDir "$INSTDIR\share"

  Delete "$SMPROGRAMS\BigMC.lnk"

  Delete "$INSTDIR\Uninstall.exe"

  RMDir "$INSTDIR"
  DeleteRegKey HKEY_LOCAL_MACHINE "SOFTWARE\BigMC"
  DeleteRegKey HKEY_LOCAL_MACHINE "SOFTWARE\Microsoft\Windows\CurrentVersion\Uninstall\BigMC"

  DeleteRegKey /ifempty HKCU "Software\BigMC"

SectionEnd