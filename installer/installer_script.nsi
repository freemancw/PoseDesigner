;NSIS Modern User Interface
;Basic Example Script
;Written by Joost Verburg

;--------------------------------
;Include Modern UI

  !include "MUI2.nsh"

;--------------------------------
;General

  ;Name and file
  Name "PoseDesigner"
  OutFile "posedesigner-setup-0.2.exe"
  BrandingText " "
  !define MUI_ICON "pd.ico"
  !define MUI_HEADERIMAGE_BITMAP "test.bmp"
  !define MUI_BGCOLOR d9e9ff
  !define MUI_HEADERIMAGE_BITMAP_NOSTRETCH
  !define MUI_HEADERIMAGE
  !define MUI_HEADERIMAGE_RIGHT

  ;Default installation folder
  InstallDir "$PROGRAMFILES\PoseDesigner"
  
  ;Get installation folder from registry if available
  InstallDirRegKey HKCU "Software\PoseDesigner" ""

  ;Request application privileges for Windows Vista
  RequestExecutionLevel user

;--------------------------------
;Interface Settings

  !define MUI_ABORTWARNING

;--------------------------------
;Pages

  !insertmacro MUI_PAGE_LICENSE "gpl.txt"
  !insertmacro MUI_PAGE_COMPONENTS
  !insertmacro MUI_PAGE_DIRECTORY
  !insertmacro MUI_PAGE_INSTFILES
  
  !insertmacro MUI_UNPAGE_CONFIRM
  !insertmacro MUI_UNPAGE_INSTFILES

Function finishpageaction
CreateShortcut "$desktop\foo.lnk" "$instdir\foo.exe"
FunctionEnd

!define MUI_FINISHPAGE_SHOWREADME ""
!define MUI_FINISHPAGE_SHOWREADME_NOTCHECKED
!define MUI_FINISHPAGE_SHOWREADME_TEXT "Create Desktop Shortcut"
!define MUI_FINISHPAGE_SHOWREADME_FUNCTION finishpageaction
  
;--------------------------------
;Languages
 
  !insertmacro MUI_LANGUAGE "English"

;--------------------------------
;Installer Sections

Section -SETTINGS

  SetOutPath "$INSTDIR"

  ;Store installation folder
  WriteRegStr HKCU "Software\PoseDesigner" "" $INSTDIR
  
  ;Create uninstaller
  WriteUninstaller "$INSTDIR\Uninstall.exe"

SectionEnd

Section "OpenNI" SecOpenNI
  
  Call ConnectInternet ;Make an internet connection (if no connection available)
  
  StrCpy $2 "$TEMP\OpenNI-Win32-1.3.2.3-Dev.msi"
  NSISdl::download http://www.freemancw.com/files/posedesigner/OpenNI-Win32-1.3.2.3-Dev.msi $2
  Pop $0
  StrCmp $0 success success
    SetDetailsView show
    DetailPrint "download failed: $0"
    Abort
  success:
  skip:

  ExecWait '"msiexec" /i "$TEMP\OpenNI-Win32-1.3.2.3-Dev.msi"'
  Delete "$TEMP\OpenNI-Win32-1.3.2.3-Dev.msi"

SectionEnd

Section "NITE" SecNITE

  Call ConnectInternet ;Make an internet connection (if no connection available)
  
  StrCpy $2 "$TEMP\NITE-Win32-1.4.1.2-Dev.msi"
  NSISdl::download http://www.freemancw.com/files/posedesigner/NITE-Win32-1.4.1.2-Dev.msi $2
  Pop $0
  StrCmp $0 success success
    SetDetailsView show
    DetailPrint "download failed: $0"
    Abort
  success:
  skip:

  ExecWait '"msiexec" /i "$TEMP\NITE-Win32-1.4.1.2-Dev.msi"'
  Delete "$TEMP\NITE-Win32-1.4.1.2-Dev.msi"

SectionEnd

Section "Kinect Driver" SecSensorKinect

  Call ConnectInternet ;Make an internet connection (if no connection available)
  
  StrCpy $2 "$TEMP\SensorKinect-Win-OpenSource32-5.0.3.4.msi"
  NSISdl::download http://www.freemancw.com/files/posedesigner/SensorKinect-Win-OpenSource32-5.0.3.4.msi $2
  Pop $0
  StrCmp $0 success success
    SetDetailsView show
    DetailPrint "download failed: $0"
    Abort
  success:
  skip:

  ExecWait '"msiexec" /i "$TEMP\SensorKinect-Win-OpenSource32-5.0.3.4.msi"'
  Delete "$TEMP\SensorKinect-Win-OpenSource32-5.0.3.4.msi"

SectionEnd

Section "PoseDesigner" SecPoseDesigner

  File "PoseDesigner.exe"
  File "QtCore4.dll"
  File "QtGui4.dll"
  File "QtOpenGL4.dll"
  File "QtXml4.dll"
  File "OpenNI.dll"
  SetOutPath "$INSTDIR\data"
  File "data\config.xml"
  SetOutPath "$INSTDIR"
  createShortCut "$SMPROGRAMS\PoseDesigner.lnk" "$INSTDIR\PoseDesigner.exe"

SectionEnd

Function ConnectInternet

  Push $R0
    
    ClearErrors
    Dialer::AttemptConnect
    IfErrors noie3
    
    Pop $R0
    StrCmp $R0 "online" connected
      MessageBox MB_OK|MB_ICONSTOP "Cannot connect to the internet."
      Quit
    
    noie3:
  
    ; IE3 not installed
    MessageBox MB_OK|MB_ICONINFORMATION "Please connect to the internet now."
    
    connected:
  
  Pop $R0
  
FunctionEnd

;--------------------------------
;Descriptions

  ;Language strings
  LangString DESC_SecOpenNI ${LANG_ENGLISH} "OpenNI library."
  LangString DESC_SecNITE ${LANG_ENGLISH} "NITE middleware."
  LangString DESC_SecSensorKinect ${LANG_ENGLISH} "Kinect Driver."
  LangString DESC_SecPoseDesigner ${LANG_ENGLISH} "PoseDesigner."

  ;Assign language strings to sections
  !insertmacro MUI_FUNCTION_DESCRIPTION_BEGIN
    !insertmacro MUI_DESCRIPTION_TEXT ${SecOpenNI} $(DESC_SecOpenNI)
    !insertmacro MUI_DESCRIPTION_TEXT ${SecNITE} $(DESC_SecNITE)
    !insertmacro MUI_DESCRIPTION_TEXT ${SecSensorKinect} $(DESC_SecSensorKinect)
    !insertmacro MUI_DESCRIPTION_TEXT ${SecPoseDesigner} $(DESC_SecPoseDesigner)
  !insertmacro MUI_FUNCTION_DESCRIPTION_END

;--------------------------------
;Uninstaller Section

Section "Uninstall"

  ;ADD YOUR OWN FILES HERE...

  Delete "$INSTDIR\Uninstall.exe"

  RMDir "$INSTDIR"

  DeleteRegKey /ifempty HKCU "Software\PoseDesigner"

SectionEnd