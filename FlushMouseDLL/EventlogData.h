//
// EventlogData.mc
//		Copyright (C) 2023 JeffyTS
//
// No.      Date		    Name		    Reason & Document
// -------+-----------+-----------+-------------------------------------------- -
// #0000	2023/04/13  JeffyTS  	New edit.
//
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
// MessageId: Installer_CATEGORY
//
// MessageText:
//
// Installer Events
//
#define Installer_CATEGORY               ((DWORD)0x00000004L)

//
// MessageId: Shortcut_CATEGORY
//
// MessageText:
//
// Shortcut Events
//
#define Shortcut_CATEGORY                ((DWORD)0x00000005L)

//
// MessageId: POWERNOTIFICATION_CATEGORY
//
// MessageText:
//
// PowerNotification Events
//
#define POWERNOTIFICATION_CATEGORY       ((DWORD)0x00000006L)

//
// MessageId: TASKSCHEDULER_CATEGORY
//
// MessageText:
//
// TaskScheduler Events
//
#define TASKSCHEDULER_CATEGORY           ((DWORD)0x00000007L)

//
// Message definitions.
//
//
// MessageId: MSG_STARTING_FLUSHMOUSE
//
// MessageText:
//
// Starting FlushMouse.
//
#define MSG_STARTING_FLUSHMOUSE          ((DWORD)0x40000100L)

//
// MessageId: MSG_RESTART_FLUSHMOUSE_EVENT
//
// MessageText:
//
// Restart FlushMouse.
//
#define MSG_RESTART_FLUSHMOUSE_EVENT     ((DWORD)0x40000101L)

//
// MessageId: MSG_DELAYED_RESTART_FLUSHMOUSE_EVENT
//
// MessageText:
//
// Restart FlushMouse(Delayed Start).
//
#define MSG_DELAYED_RESTART_FLUSHMOUSE_EVENT ((DWORD)0x40000102L)

//
// MessageId: MSG_STARTED_FLUSHMOUSE
//
// MessageText:
//
// Started FlushMouse.
//
#define MSG_STARTED_FLUSHMOUSE           ((DWORD)0x40000103L)

//
// MessageId: MSG_QUIT_FLUSHMOUSE
//
// MessageText:
//
// Quit FlushMouse.
//
#define MSG_QUIT_FLUSHMOUSE              ((DWORD)0x00000104L)

//
// MessageId: MSG_STOPPED_FLUSHMOUSE
//
// MessageText:
//
// Stopped FlushMouse.
//
#define MSG_STOPPED_FLUSHMOUSE           ((DWORD)0x00000105L)

//
// MessageId: MSG_START_FLUSHMOUSE_EVENT
//
// MessageText:
//
// Start FlushMouse.
//
#define MSG_START_FLUSHMOUSE_EVENT       ((DWORD)0x00000106L)

//
// MessageId: MSG_QUIT_FLUSHMOUSE_EVENT
//
// MessageText:
//
// Quit FlushMouse.
//
#define MSG_QUIT_FLUSHMOUSE_EVENT        ((DWORD)0x00000107L)

//
// MessageId: MSG_THREAD_HOOK_TIMER_STARTED
//
// MessageText:
//
// Thread, Hook and Timer Started.
//
#define MSG_THREAD_HOOK_TIMER_STARTED    ((DWORD)0x40000108L)

//
// MessageId: MSG_THREAD_HOOK_TIMER_START_FAILED
//
// MessageText:
//
// Thread, Hook and Timer Start Failed.
//
#define MSG_THREAD_HOOK_TIMER_START_FAILED ((DWORD)0x40008109L)

//
// MessageId: MSG_THREAD_HOOK_TIMER_STOPPED
//
// MessageText:
//
// Thread, Hook and Timer Stopped.
//
#define MSG_THREAD_HOOK_TIMER_STOPPED    ((DWORD)0x4000010AL)

//
// MessageId: MSG_THREAD_HOOK_TIMER_RESTARTED
//
// MessageText:
//
// Thread, Hook and Timer Restarted.
//
#define MSG_THREAD_HOOK_TIMER_RESTARTED  ((DWORD)0x4000010BL)

//
// MessageId: MSG_THREAD_HOOK_TIMER_RESTART_FAILED
//
// MessageText:
//
// Thread, Hook and Timer Restart Failed.
//
#define MSG_THREAD_HOOK_TIMER_RESTART_FAILED ((DWORD)0x40008100L)

//
// MessageId: MSG_TASKTRAY_REGISTERED
//
// MessageText:
//
// Registered TaskTray.
//
#define MSG_TASKTRAY_REGISTERED          ((DWORD)0x00008101L)

//
// MessageId: MSG_TASKTRAY_REGISTER_FAILD
//
// MessageText:
//
// Register TaskTray Failed.
//
#define MSG_TASKTRAY_REGISTER_FAILD      ((DWORD)0x00008102L)

//
// MessageId: MSG_DETECT_FLUSHMOUSE_STOP
//
// MessageText:
//
// Detected Stopping FlushMouse Process.
//
#define MSG_DETECT_FLUSHMOUSE_STOP       ((DWORD)0x00008103L)

//
// MessageId: MSG_FLUSHMOUSE_ALREADY_RUN
//
// MessageText:
//
// FlushMouse already run.
//
#define MSG_FLUSHMOUSE_ALREADY_RUN       ((DWORD)0x00008104L)

//
// MessageId: MSG_PBT_APMSUSPEND
//
// MessageText:
//
// Entering into suspend (APM).
//
#define MSG_PBT_APMSUSPEND               ((DWORD)0x40000200L)

//
// MessageId: MSG_PBT_APMRESUMEAUTOMATIC
//
// MessageText:
//
// Resuming from Sleep or Hibernation (APM).
//
#define MSG_PBT_APMRESUMEAUTOMATIC       ((DWORD)0x40000201L)

//
// MessageId: MSG_PBT_APMRESUMESUSPEND
//
// MessageText:
//
// Resumed from Suspend (APM).
//
#define MSG_PBT_APMRESUMESUSPEND         ((DWORD)0x40000202L)

//
// MessageId: MSG_PBT_POWERSETTINGCHANGE
//
// MessageText:
//
// Power settings changed (APM).
//
#define MSG_PBT_POWERSETTINGCHANGE       ((DWORD)0x40000203L)

//
// MessageId: MSG_PBT_APMPOWERSTATUSCHANGE
//
// MessageText:
//
// Changed the power status (APM).
//
#define MSG_PBT_APMPOWERSTATUSCHANGE     ((DWORD)0x40000204L)

//
// MessageId: MSG_PBT_APMPOWERSTATUSCHANGE_AC_ON
//
// MessageText:
//
// Changed the power status to AC Online (APM).
//
#define MSG_PBT_APMPOWERSTATUSCHANGE_AC_ON ((DWORD)0x40000205L)

//
// MessageId: MSG_PBT_APMPOWERSTATUSCHANGE_AC_OFF
//
// MessageText:
//
// Changed the power status to AC Offline (APM).
//
#define MSG_PBT_APMPOWERSTATUSCHANGE_AC_OFF ((DWORD)0x40000206L)

//
// MessageId: MSG_PBT_APMPOWERSTATUSCHANGE_DISPLAY_ON
//
// MessageText:
//
// Changed the display state to ON (APM).
//
#define MSG_PBT_APMPOWERSTATUSCHANGE_DISPLAY_ON ((DWORD)0x40000207L)

//
// MessageId: MSG_PBT_APMPOWERSTATUSCHANGE_DISPLAY_OFF
//
// MessageText:
//
// Changed the display state to OFF (APM).
//
#define MSG_PBT_APMPOWERSTATUSCHANGE_DISPLAY_OFF ((DWORD)0x40000208L)

//
// MessageId: MSG_WTSSESSION_CHANGE_SESSION_LOCK
//
// MessageText:
//
// Changed session to Lock (WTS).
//
#define MSG_WTSSESSION_CHANGE_SESSION_LOCK ((DWORD)0x40000209L)

//
// MessageId: MSG_WTSSESSION_CHANGE_SESSION_UNLOCK
//
// MessageText:
//
// Changed session to Unlock (WTS).
//
#define MSG_WTSSESSION_CHANGE_SESSION_UNLOCK ((DWORD)0x4000020AL)

///* = EOF = */
