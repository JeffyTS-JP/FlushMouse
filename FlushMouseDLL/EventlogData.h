// Eventlog.mc
//		Copyright (C) 2023 JeffyTS
//
// No.      Date		    Name		    Reason & Document
// -------+-----------+-----------+-------------------------------------------- -
// #0000	2023/04/13  JeffyTS  	New edit.
//
//
// Categories of events.
//
//
//  Values are 32 bit values laid out as follows:
//
//   3 3 2 2 2 2 2 2 2 2 2 2 1 1 1 1 1 1 1 1 1 1
//   1 0 9 8 7 6 5 4 3 2 1 0 9 8 7 6 5 4 3 2 1 0 9 8 7 6 5 4 3 2 1 0
//  +---+-+-+-----------------------+-------------------------------+
//  |Sev|C|R|     Facility          |               Code            |
//  +---+-+-+-----------------------+-------------------------------+
//
//  where
//
//      Sev - is the severity code
//
//          00 - Success
//          01 - Informational
//          10 - Warning
//          11 - Error
//
//      C - is the Customer code flag
//
//      R - is a reserved bit
//
//      Facility - is the facility code
//
//      Code - is the facility's status code
//
//
// Define the facility codes
//
#define FACILITY_SYSTEM                  0x0
#define FACILITY_RUNTIME                 0x2
#define FACILITY_STUBS                   0x3
#define FACILITY_IO_ERROR_CODE           0x4


//
// Define the severity codes
//
#define STATUS_SEVERITY_SUCCESS          0x0
#define STATUS_SEVERITY_INFORMATIONAL    0x1
#define STATUS_SEVERITY_WARNING          0x2
#define STATUS_SEVERITY_ERROR            0x3


//
// MessageId: APPLICATION_CATEGORY
//
// MessageText:
//
// Application Events
//
#define APPLICATION_CATEGORY             ((DWORD)0x00000000L)

//
// MessageId: APPLICATION32_CATEGORY
//
// MessageText:
//
// Application32 Events
//
#define APPLICATION32_CATEGORY           ((DWORD)0x00000001L)

//
// MessageId: DLL_CATEGORY
//
// MessageText:
//
// DLL Events
//
#define DLL_CATEGORY                     ((DWORD)0x00000002L)

//
// MessageId: DLL32_CATEGORY
//
// MessageText:
//
// DLL32 Events
//
#define DLL32_CATEGORY                   ((DWORD)0x00000003L)

//
// MessageId: POWERNOTIFICATION_CATEGORY
//
// MessageText:
//
// PowerNotification Events
//
#define POWERNOTIFICATION_CATEGORY       ((DWORD)0x00000010L)

//
// Message definitions.
//
//
// MessageId: MSG_START_SUCCEED
//
// MessageText:
//
// Started FlushMouse.
//
#define MSG_START_SUCCEED                ((DWORD)0x40000000L)

//
// MessageId: MSG_RESTART_EVENT
//
// MessageText:
//
// Restart FlushMouse.
//
#define MSG_RESTART_EVENT                ((DWORD)0x40000001L)

//
// MessageId: MSG_THREAD_HOOK_TIMER_STARTED
//
// MessageText:
//
// Thread, Hook and Timer Started.
//
#define MSG_THREAD_HOOK_TIMER_STARTED    ((DWORD)0x40000002L)

//
// MessageId: MSG_THREAD_HOOK_TIMER_STOPPED
//
// MessageText:
//
// Thread, Hook and Timer Stopped.
//
#define MSG_THREAD_HOOK_TIMER_STOPPED    ((DWORD)0x40000003L)

//
// MessageId: MSG_THREAD_HOOK_TIMER_RESTARTED
//
// MessageText:
//
// Thread, Hook and Timer Restarted.
//
#define MSG_THREAD_HOOK_TIMER_RESTARTED  ((DWORD)0x40000005L)

//
// MessageId: MSG_QUIT_FLUSHMOUSE
//
// MessageText:
//
// Quit FlushMouse.
//
#define MSG_QUIT_FLUSHMOUSE              ((DWORD)0x40000006L)

//
// MessageId: MSG_PBT_APMSUSPEND
//
// MessageText:
//
// Entering into suspend (APM).
//
#define MSG_PBT_APMSUSPEND               ((DWORD)0x40000100L)

//
// MessageId: MSG_PBT_APMRESUMEAUTOMATIC
//
// MessageText:
//
// Resuming from sleep or hibernation (APM).
//
#define MSG_PBT_APMRESUMEAUTOMATIC       ((DWORD)0x40000101L)

//
// MessageId: MSG_PBT_APMRESUMESUSPEND
//
// MessageText:
//
// Resumeed from Susped (APM).
//
#define MSG_PBT_APMRESUMESUSPEND         ((DWORD)0x40000102L)

//
// MessageId: MSG_PBT_POWERSETTINGCHANGE
//
// MessageText:
//
// Power settings changed (APM).
//
#define MSG_PBT_POWERSETTINGCHANGE       ((DWORD)0x40000103L)

//
// MessageId: MSG_PBT_APMPOWERSTATUSCHANGE
//
// MessageText:
//
// Changed the power status (APM).
//
#define MSG_PBT_APMPOWERSTATUSCHANGE     ((DWORD)0x40000104L)

//
// MessageId: MSG_PBT_APMPOWERSTATUSCHANGE_AC_ON
//
// MessageText:
//
// Changed the power status to AC Online (APM).
//
#define MSG_PBT_APMPOWERSTATUSCHANGE_AC_ON ((DWORD)0x40000105L)

//
// MessageId: MSG_PBT_APMPOWERSTATUSCHANGE_AC_OFF
//
// MessageText:
//
// Changed the power status to AC Offline (APM).
//
#define MSG_PBT_APMPOWERSTATUSCHANGE_AC_OFF ((DWORD)0x40000106L)

//
// MessageId: MSG_PBT_APMPOWERSTATUSCHANGE_DISPLAY_ON
//
// MessageText:
//
// Changed the display state to ON (APM).
//
#define MSG_PBT_APMPOWERSTATUSCHANGE_DISPLAY_ON ((DWORD)0x40000107L)

//
// MessageId: MSG_PBT_APMPOWERSTATUSCHANGE_DISPLAY_OFF
//
// MessageText:
//
// Changed the display state to OFF (APM).
//
#define MSG_PBT_APMPOWERSTATUSCHANGE_DISPLAY_OFF ((DWORD)0x40000108L)

///* = EOF = */