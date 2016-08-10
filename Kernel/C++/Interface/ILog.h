#pragma once
#ifndef _ILOG_H_
#define _ILOG_H_
#include "IKernel.h"
//! base macro.
#define LOG_STREAM(id, level, file, line, log)\
do{\
	g_pKernel->log_format(id, level, file, line, log);\
} while (0)


//! fast macro
#define LOG_TRACE(id, log) LOG_STREAM(id, 0, __FILE__, __LINE__, log)
#define LOG_DEBUG(id, log) LOG_STREAM(id, 1, __FILE__, __LINE__, log)
#define LOG_INFO(id, log)  LOG_STREAM(id, 2, __FILE__, __LINE__, log)
#define LOG_WARN(id, log)  LOG_STREAM(id, 3, __FILE__, __LINE__, log)
#define LOG_ERROR(id, log) LOG_STREAM(id, 4, __FILE__, __LINE__, log)
#define LOG_ALARM(id, log) LOG_STREAM(id, 5, __FILE__, __LINE__, log)
#define LOG_FATAL(id, log) LOG_STREAM(id, 6, __FILE__, __LINE__, log)

//! super macro.
#define LOGT( log ) LOG_TRACE(0, log )
#define LOGD( log ) LOG_DEBUG(0, log )
#define LOGI( log ) LOG_INFO(0, log )
#define LOGW( log ) LOG_WARN(0, log )
#define LOGE( log ) LOG_ERROR(0, log )
#define LOGA( log ) LOG_ALARM(0, log )
#define LOGF( log ) LOG_FATAL(0, log )


//! format input log.
#ifdef WIN32
#define LOG_FORMAT(id, level, file, line, logformat, ...) \
do{ \
	g_pKernel->log_format(id, level, file, line, logformat, ##__VA_ARGS__);\
} while (0)

//!format string
#define LOGFMT_TRACE(id, fmt, ...)  LOG_FORMAT(id, 0, __FILE__, __LINE__, fmt, ##__VA_ARGS__)
#define LOGFMT_DEBUG(id, fmt, ...)  LOG_FORMAT(id, 1, __FILE__, __LINE__, fmt, ##__VA_ARGS__)
#define LOGFMT_INFO(id, fmt, ...)  LOG_FORMAT(id, 2, __FILE__, __LINE__, fmt, ##__VA_ARGS__)
#define LOGFMT_WARN(id, fmt, ...)  LOG_FORMAT(id, 3, __FILE__, __LINE__, fmt, ##__VA_ARGS__)
#define LOGFMT_ERROR(id, fmt, ...)  LOG_FORMAT(id, 4, __FILE__, __LINE__, fmt, ##__VA_ARGS__)
#define LOGFMT_ALARM(id, fmt, ...)  LOG_FORMAT(id, 5, __FILE__, __LINE__, fmt, ##__VA_ARGS__)
#define LOGFMT_FATAL(id, fmt, ...)  LOG_FORMAT(id, 6, __FILE__, __LINE__, fmt, ##__VA_ARGS__)
#define LOGFMTT( fmt, ...) LOGFMT_TRACE(0, fmt,  ##__VA_ARGS__)
#define LOGFMTD( fmt, ...) LOGFMT_DEBUG(0, fmt,  ##__VA_ARGS__)
#define LOGFMTI( fmt, ...) LOGFMT_INFO(0, fmt,  ##__VA_ARGS__)
#define LOGFMTW( fmt, ...) LOGFMT_WARN(0, fmt,  ##__VA_ARGS__)
#define LOGFMTE( fmt, ...) LOGFMT_ERROR(0, fmt,  ##__VA_ARGS__)
#define LOGFMTA( fmt, ...) LOGFMT_ALARM(0, fmt,  ##__VA_ARGS__)
#define LOGFMTF( fmt, ...) LOGFMT_FATAL(0, fmt,  ##__VA_ARGS__)
#else
inline void empty_log_format_function1(int id, const char*, ...){}
inline void empty_log_format_function2(const char*, ...){}
#define LOGFMT_TRACE empty_log_format_function1
#define LOGFMT_DEBUG LOGFMT_TRACE
#define LOGFMT_INFO LOGFMT_TRACE
#define LOGFMT_WARN LOGFMT_TRACE
#define LOGFMT_ERROR LOGFMT_TRACE
#define LOGFMT_ALARM LOGFMT_TRACE
#define LOGFMT_FATAL LOGFMT_TRACE
#define LOGFMTT empty_log_format_function2
#define LOGFMTD LOGFMTT
#define LOGFMTI LOGFMTT
#define LOGFMTW LOGFMTT
#define LOGFMTE LOGFMTT
#define LOGFMTA LOGFMTT
#define LOGFMTF LOGFMTT
#endif
#endif