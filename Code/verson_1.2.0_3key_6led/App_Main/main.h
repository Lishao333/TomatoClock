/*!
    \file    main.h
    \brief   the header file of main 
*/

#ifndef __MAIN_H
#define __MAIN_H

#define SOFT_VER      "TomatoClock_1.1.0"
#define ISSUE_DATA    "2022-10-14"

#define DEBUG_ENABLE    1
#if DEBUG_ENABLE
	#define DEB_LOG(format, ...)    printf(format, ##__VA_ARGS__)
#else
	#define DEB_LOG(format, ...)    
#endif

#endif /* __MAIN_H */
