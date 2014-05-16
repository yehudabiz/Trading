/*
* File: ZLOG.h
* Title: Zamir Logger for VB
* Author: Avraham Bernstein <avraham.bernstein@yerushalayim.biz>
* Date: 2003-05-05T09:12 (original)
* Interface Version: V2.0.0
*
* :Description:
* This module is a logging utility designed to be used stand-alone or to support
* other Zamir C++/VB communication modules.
*
* :Change History:
* ::2003-05-05T09:12/V2.0.0/Abe:
* ::2006-08-24T13:30/V3.0.0/Abe
* (1) Added auto cleanup feature - see new Open call
* ::2007-11-19T22:20/V4.0.0/PB+SS
* (2) Change the time from the SystemTime to the LocalTime 
 */

#ifndef ZLOG_H
#define ZLOG_H

#define ZLOG_API

typedef unsigned char * ZLOG_CONTEXT;
typedef DWORD ZLOG_STATUS;

typedef enum 
{
                ZLOG_CHANNEL_FILE, 
                ZLOG_CHANNEL_DBGVIEW,
                ZLOG_CHANNEL_SYSLOG, 
                ZLOG_CHANNEL__LOWEST = ZLOG_CHANNEL_FILE,
                ZLOG_CHANNEL__HIGHEST = ZLOG_CHANNEL_SYSLOG
} ZLOG_CHANNEL;

typedef enum 
{
    ZLOG_LEVEL_EMERGENCY,
    ZLOG_LEVEL_ALERT,
    ZLOG_LEVEL_CRITICAL,
    ZLOG_LEVEL_ERROR,
    ZLOG_LEVEL_WARNING,
    ZLOG_LEVEL_NOTICE,
    ZLOG_LEVEL_INFO,
    ZLOG_LEVEL_DEBUG
} ZLOG_LEVEL;

typedef enum 
{
    ZLOG_FAC_LOCAL0 = 16,
    ZLOG_FAC_LOCAL1,
    ZLOG_FAC_LOCAL2,
    ZLOG_FAC_LOCAL3,
    ZLOG_FAC_LOCAL4,
    ZLOG_FAC_LOCAL5,
    ZLOG_FAC_LOCAL6,
    ZLOG_FAC_LOCAL7,
    ZLOG_FAC_DEFAULT = ZLOG_FAC_LOCAL7
} ZLOG_SYSLOG_FACILITY;

//#ifdef __cplusplus
//extern "C" 
//{
//#endif

ZLOG_API ZLOG_STATUS __stdcall ZLOG_Open
(
                OUT ZLOG_CONTEXT * context,
                IN OPTIONAL LPCSTR directory_name,
    IN OPTIONAL LPCSTR syslog_server_name,
                IN OPTIONAL DWORD syslog_port,
    IN ZLOG_SYSLOG_FACILITY default_syslog_facility,
    IN OPTIONAL LPCSTR site,
    IN LPCSTR app_name,
    IN LPCSTR app_version,
    IN BOOL abbrev_flag,
                IN long auto_cleanup_time_in_days // if val <= 0 then feature is disabled
);

ZLOG_API void __stdcall ZLOG_Close(IN ZLOG_CONTEXT context);

ZLOG_API BOOL __stdcall ZLOG_FilterByLevel
(
    IN ZLOG_CONTEXT context,
    IN ZLOG_CHANNEL channel,
    IN ZLOG_LEVEL level
);

ZLOG_API BOOL __stdcall ZLOG_Put
(
    IN ZLOG_CONTEXT context,
    IN ZLOG_SYSLOG_FACILITY syslog_facility,
    IN ZLOG_LEVEL level,
    IN DWORD src_id,
    IN DWORD msg_id,
    IN LPCSTR msg_text,
    IN int n_tags,
    IN LPCSTR * tag_names,
    IN LPCSTR * tag_values
);

///*inline*/
//ZLOG_API BOOL __stdcall ZLOG_Put3
//(
//   IN ZLOG_CONTEXT context,
//   IN ZLOG_LEVEL level,
//   IN LPCSTR text
//)
//{
//    return ZLOG_Put(context,ZLOG_FAC_DEFAULT,level,0,0,text,0,0,0);
//}


//#ifdef __cplusplus
//};  // extern "C"

/*
inline ZLOG_API BOOL ZLOG_Put3(
    IN ZLOG_CONTEXT context,
    IN ZLOG_LEVEL level,
    IN LPCSTR text
)
{
    return ZLOG_Put(context,ZLOG_FAC_DEFAULT,level,0,0,text,0,0,0);
}


inline ZLOG_API BOOL ZLOG_Put5(
    IN ZLOG_CONTEXT context,
    IN ZLOG_LEVEL level,
    IN DWORD src_id,
    IN DWORD msg_id,
    IN LPCSTR msg_text
)
{
    return ZLOG_Put(context,ZLOG_FAC_DEFAULT,level,src_id,msg_id,msg_text,0,0,0);
}

#else
*/

#define __ZLOG_Put3(context,level,text) \
                (context==NULL ? 0 : ZLOG_Put(context,ZLOG_FAC_DEFAULT,level,0,0,text,0,0,0))

#define __ZLOG_Put5(context,level,src_id,msg_id,msg_text) \
    ZLOG_Put(context,level,ZLOG_FAC_DEFAULT,src_id,msg_id,msg_text,0,0,0)


//#endif

#endif // ZLOG_H
