; CLW file contains information for the MFC ClassWizard

[General Info]
Version=1
LastClass=CResizableDialog
LastTemplate=CDialog
NewFileInclude1=#include "stdafx.h"
NewFileInclude2=#include "demo.h"
LastPage=0

ClassCount=4
Class1=CDemoApp
Class2=CDemoDlg
Class3=CResizableDialog
Class4=CSecondDialog

ResourceCount=3
Resource1=IDR_TOOLBAR1
Resource2=IDD_SECOND_DIALOG
Resource3=IDD_DEMO_DIALOG

[CLS:CDemoApp]
Type=0
BaseClass=CWinApp
HeaderFile=Demo.h
ImplementationFile=Demo.cpp
LastObject=CDemoApp

[CLS:CDemoDlg]
Type=0
BaseClass=CResizableDialog
HeaderFile=DemoDlg.h
ImplementationFile=DemoDlg.cpp
LastObject=CDemoDlg

[CLS:CResizableDialog]
Type=0
BaseClass=CDialog
HeaderFile=ResizableDialog.h
ImplementationFile=ResizableDialog.cpp
LastObject=CResizableDialog

[CLS:CSecondDialog]
Type=0
BaseClass=CResizableDialog
HeaderFile=SecondDialog.h
ImplementationFile=SecondDialog.cpp
Filter=D
VirtualFilter=dWC
LastObject=CSecondDialog

[DLG:IDD_DEMO_DIALOG]
Type=1
Class=CDemoDlg
ControlCount=8
Control1=IDC_LABEL1,static,1342308352
Control2=IDC_SPIN1,msctls_updown32,1342177376
Control3=IDC_EDIT1,edit,1352728644
Control4=IDC_RADIO1,button,1342373897
Control5=IDC_RADIO2,button,1342177289
Control6=IDC_GROUP1,button,1342177287
Control7=IDOK,button,1342373889
Control8=IDCANCEL,button,1342242816

[DLG:IDD_SECOND_DIALOG]
Type=1
Class=CSecondDialog
ControlCount=6
Control1=IDOK,button,1342242817
Control2=IDCANCEL,button,1342242816
Control3=IDC_LINE1,static,1342177296
Control4=IDC_PICTURE1,static,1342182926
Control5=IDC_STATIC2,static,1342362124
Control6=IDC_STATIC1,static,1342312577

[TB:IDR_TOOLBAR1]
Type=1
Class=CSecondDialog
Command1=ID_BUTTON1
Command2=ID_BUTTON2
Command3=ID_BUTTON3
Command4=ID_BUTTON4
CommandCount=4

