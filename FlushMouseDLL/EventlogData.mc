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
; // Categories of events.

MessageId=0x1
SymbolicName=APPLICATION_CATEGORY
Language=English
Application Events
.

Language=Japanese
Application Events
.

MessageId=0x2
SymbolicName=POWERNOTIFICATION_CATEGORY
Language=English
PowerNotification Events
.

Language=Japanese
PowerNotification Events
.


;// Message definitions.
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
SymbolicName=MSG_QUIT_FLUSHMOUSE
Language=English
Quit FlushMouse.
.

Language=Japanese
FlushMouseを終了します。
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
SymbolicName=MSG_PBT_APMRESUMESUSPEND
Language=English
Resumeed from Susped (APM).
.

Language=Japanese
Resumeed from Susped (APM).
.

MessageId=0x0102
Severity=Informational
Facility=Application
SymbolicName=MSG_PBT_APMPOWERSTATUSCHANGE
Language=English
Changed in the power status (APM).
.

Language=Japanese
Changed in the power status (APM).
.

MessageId=0x0103
Severity=Informational
Facility=Application
SymbolicName=MSG_PBT_APMRESUMEAUTOMATIC
Language=English
Resuming from sleep or hibernation (APM).
.

Language=Japanese
Resuming from sleep or hibernation (APM).
.

MessageId=0x0104
Severity=Informational
Facility=Application
SymbolicName=MSG_PBT_POWERSETTINGCHANGE
Language=English
Power settings changed (APM).
.

Language=Japanese
Power settings changed (APM).
.


