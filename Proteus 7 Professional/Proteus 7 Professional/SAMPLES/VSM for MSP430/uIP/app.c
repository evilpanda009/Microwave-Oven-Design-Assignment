/*
 * Copyright (c) 2009, Usachev Mikhail.
 * All rights reserved. 
 *
 * Redistribution and use in source and binary forms, with or without 
 * modification, are permitted provided that the following conditions 
 * are met: 
 * 1. Redistributions of source code must retain the above copyright 
 *    notice, this list of conditions and the following disclaimer. 
 * 2. Redistributions in binary form must reproduce the above copyright 
 *    notice, this list of conditions and the following disclaimer in the 
 *    documentation and/or other materials provided with the distribution. 
 * 3. The name of the author may not be used to endorse or promote
 *    products derived from this software without specific prior
 *    written permission.  
 *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS
 * OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY
 * DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE
 * GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
 * NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.  
 *
 * This file is part of the uIP TCP/IP stack.
 *
 * $Id: app.c,v 1.10.2.4 2009/10/29 21:27:51 um Exp $
 *
 */

#include "app.h"
#include "uip.h"

struct TASK
 { unsigned short port;
   APPHANDLER handler;
 };

#define MAX_TASKS 8

struct TASK tasks[MAX_TASKS];
int n_tasks;

void init_tasks()
 { memset(tasks, 0, sizeof(tasks));
   n_tasks = 0;
 }

void register_task(APPHANDLER handler, unsigned short port)
 { if (n_tasks < MAX_TASKS)
    { tasks[n_tasks].handler = handler;
      tasks[n_tasks].port = port;
      n_tasks++;
    }
 }

void UIP_APPCALL()
 { int i;
   for (i = 0; i < n_tasks; i++)
    { if (tasks[i].port == uip_conn->lport)
       { APPHANDLER handler = tasks[i].handler;
         (*handler)();
       }
    }
 }
