;//
;// Eventlog.mc
;//		Copyright (C) 2023 JeffyTS
;//
;// No.      Date		    Name		    Reason & Document
;// -------+-----------+-----------+-------------------------------------------- -
;// #0000	2023/04/13  JeffyTS  	New edit.
;//
;//

SeverityNames=(Success=0x0:STATUS_SEVERITY_SUCCESS
Informational=0x1:STATUS_SEVERITY_INFORMATIONAL
Warning=0x2:STATUS_SEVERITY_WARNING
Error=0x3:STATUS_SEVERITY_ERROR
)

FacilityNames=(System=0x0:FACILITY_SYSTEM
Runtime=0x2:FACILITY_RUNTIME
Stubs=0x3:FACILITY_STUBS
Io=0x4:FACILITY_IO_ERROR_CODE
)

LanguageNames=(English=0x409:MSG00409)
LanguageNames=(Japanese=0x411:MSG00411)

MessageIdTypedef=DWORD

;//
;// Categories of events.
;//

MessageId=0x0
SymbolicName=APPLICATION_CATEGORY
Language=English
Application Events
.

Language=Japanese
Application Events
.

MessageId=0x1
SymbolicName=APPLICATION32_CATEGORY
Language=English
Application32 Events
.

Language=Japanese
Application32 Events
.

MessageId=0x2
SymbolicName=DLL_CATEGORY
Language=English
DLL Events
.

Language=Japanese
DLL Events
.

MessageId=0x3
SymbolicName=DLL32_CATEGORY
Language=English
DLL32 Events
.

Language=Japanese
DLL32 Events
.

MessageId=0x4
SymbolicName=Installer_CATEGORY
Language=English
Installer Events
.

Language=Japanese
Installer Events
.

MessageId=0x5
SymbolicName=Shortcut_CATEGORY
Language=English
Sortcut Events
.

Language=Japanese
Sortcut Events
.

MessageId=0x10
SymbolicName=POWERNOTIFICATION_CATEGORY
Language=English
PowerNotification Events
.

Language=Japanese
PowerNotification Events
.

MessageId=0x11
SymbolicName=TASKSCHEDULER_CATEGORY
Language=English
TaskScheduler Events
.

Language=Japanese
TaskScheduler Events
.


;//
;// Message definitions.
;//

MessageId=0x0000
Severity=Informational
Facility=Application
SymbolicName=MSG_START_SUCCEED
Language=English
Started FlushMouse.
.

Language=Japanese
FlushMouseを起動しました。
.

MessageId=0x0001
Severity=Informational
Facility=Application
SymbolicName=MSG_RESTART_EVENT
Language=English
Restart FlushMouse.
.

Language=Japanese
FlushMouseを再起動します。
.

MessageId=0x0002
Severity=Informational
Facility=Application
SymbolicName=MSG_THREAD_HOOK_TIMER_STARTED
Language=English
Thread, Hook and Timer Started.
.

Language=Japanese
Thread, Hook and Timer Started.
.

MessageId=0x8002
Severity=Informational
Facility=Application
SymbolicName=MSG_THREAD_HOOK_TIMER_START_FAILED
Language=English
Thread, Hook and Timer Start Failed.
.

Language=Japanese
Thread, Hook and Timer Start Failed.
.

MessageId=0x0003
Severity=Informational
Facility=Application
SymbolicName=MSG_THREAD_HOOK_TIMER_STOPPED
Language=English
Thread, Hook and Timer Stopped.
.

Language=Japanese
Thread, Hook and Timer Stopped.
.

MessageId=0x0004
Severity=Informational
Facility=Application
SymbolicName=MSG_THREAD_HOOK_TIMER_RESTARTED
Language=English
Thread, Hook and Timer Restarted.
.

Language=Japanese
Thread, Hook and Timer Restarted.
.

MessageId=0x8004
Severity=Informational
Facility=Application
SymbolicName=MSG_THREAD_HOOK_TIMER_RESTART_FAILED
Language=English
Thread, Hook and Timer Restart Failed.
.

Language=Japanese
Thread, Hook and Timer Restart Failed.
.

MessageId=0x0005
Severity=Informational
Facility=Application
SymbolicName=MSG_QUIT_FLUSHMOUSE
Language=English
Quit FlushMouse.
.

Language=Japanese
FlushMouseを終了します。
.

MessageId=0x0006
Severity=Success
Facility=Application
SymbolicName=MSG_START_FLUSHMOUSE
Language=English
Start FlushMouse.
.

Language=Japanese
FlushMouseを起動します。
.

MessageId=0x0007
Severity=Success
Facility=Application
SymbolicName=MSG_DONE_FLUSHMOUSE
Language=English
Quit FlushMouse.
.

Language=Japanese
FlushMouseを終了します。
.

MessageId=0x0008
Severity=Success
Facility=Application
SymbolicName=MSG_TASKTRAY_REGISTERED
Language=English
Registered TaskTray.
.

Language=Japanese
Registered TaskTray.
.

MessageId=0x8008
Severity=Success
Facility=Application
SymbolicName=MSG_TASKTRAY_REGISTER_FAILD
Language=English
Register TaskTray Failed.
.

Language=Japanese
Register TaskTray Failed.
.

Language=Japanese
Registered TaskTray.
.

MessageId=0x8009
Severity=Success
Facility=Application
SymbolicName=MSG_DETECT_FLUSHMOUSE_STOP
Language=English
Detected Stopping FlushMouse Process.
.

Language=Japanese
Detected Stopping FlushMouse Process.
.

MessageId=0x0100
Severity=Informational
Facility=Application
SymbolicName=MSG_PBT_APMSUSPEND
Language=English
Entering into suspend (APM).
.

Language=Japanese
Entering into suspend (APM).
.

MessageId=0x0101
Severity=Informational
Facility=Application
SymbolicName=MSG_PBT_APMRESUMEAUTOMATIC
Language=English
Resuming from Sleep or Hibernation (APM).
.

Language=Japanese
Resuming from Sleep or Hibernation (APM).
.

MessageId=0x0102
Severity=Informational
Facility=Application
SymbolicName=MSG_PBT_APMRESUMESUSPEND
Language=English
Resumed from Suspend (APM).
.

Language=Japanese
Resumed from Suspend (APM).
.

MessageId=0x0103
Severity=Informational
Facility=Application
SymbolicName=MSG_PBT_POWERSETTINGCHANGE
Language=English
Power settings changed (APM).
.

Language=Japanese
Power settings changed (APM).
.

MessageId=0x0104
Severity=Informational
Facility=Application
SymbolicName=MSG_PBT_APMPOWERSTATUSCHANGE
Language=English
Changed the power status (APM).
.

Language=Japanese
Changed the power status (APM).
.

MessageId=0x0105
Severity=Informational
Facility=Application
SymbolicName=MSG_PBT_APMPOWERSTATUSCHANGE_AC_ON
Language=English
Changed the power status to AC Online (APM).
.

Language=Japanese
Changed the power status to AC Online (APM).
.

MessageId=0x0106
Severity=Informational
Facility=Application
SymbolicName=MSG_PBT_APMPOWERSTATUSCHANGE_AC_OFF
Language=English
Changed the power status to AC Offline (APM).
.

Language=Japanese
Changed the power status to AC Offline (APM).
.

MessageId=0x0107
Severity=Informational
Facility=Application
SymbolicName=MSG_PBT_APMPOWERSTATUSCHANGE_DISPLAY_ON
Language=English
Changed the display state to ON (APM).
.

Language=Japanese
Changed the display state to ON (APM).
.

MessageId=0x0108
Severity=Informational
Facility=Application
SymbolicName=MSG_PBT_APMPOWERSTATUSCHANGE_DISPLAY_OFF
Language=English
Changed the display state to OFF (APM).
.

Language=Japanese
Changed the display state to OFF (APM).
.


;///* = EOF = */