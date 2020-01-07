/*
*********************************************************************************************************
*                                                uC/OS-II
*                                          The Real-Time Kernel
*
*                          (c) Copyright 1992-2002, Jean J. Labrosse, Weston, FL
*                                           All Rights Reserved
*
* File : uCOS_II.C
* By   : Jean J. Labrosse
*********************************************************************************************************
*/

#define  OS_GLOBALS                           /* Declare GLOBAL variables                              */
#include "includes.h"


#define  OS_MASTER_FILE                       /* Prevent the following files from including includes.h */
#include "\source\os_core.c"
#include "\source\os_flag.c"
#include "\source\os_mbox.c"
#include "\source\os_mem.c"
#include "\source\os_mutex.c"
#include "\source\os_q.c"
#include "\source\os_sem.c"
#include "\source\os_task.c"
#include "\source\os_time.c"
