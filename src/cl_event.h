/*
 * Copyright © 2012 Intel Corporation
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library. If not, see <http://www.gnu.org/licenses/>.
 *
 * Author: Benjamin Segovia <benjamin.segovia@intel.com>
 */

#ifndef __CL_EVENT_H__
#define __CL_EVENT_H__

#include <semaphore.h>

#include "cl_base_object.h"
#include "cl_driver.h"
#include "cl_enqueue.h"
#include "CL/cl.h"

#define CL_ENQUEUE_EXECUTE_IMM   0
#define CL_ENQUEUE_EXECUTE_DEFER 1

typedef struct _user_event {
  cl_event            event;   /* The user event */
  struct _user_event* next;    /* Next user event in list */
} user_event;

typedef struct _enqueue_callback {
  cl_event           event;            /* The event relative this enqueue callback */
  enqueue_data       data;             /* Hold all enqueue callback's infomation */
  cl_uint            num_events;       /* num events in wait list */
  cl_event*          wait_list;        /* All event wait list this callback wait on */
  user_event*        wait_user_events; /* The head of user event list the callback wait on */
  struct _enqueue_callback*  next;     /* The  next enqueue callback in wait list */
} enqueue_callback;

typedef void (CL_CALLBACK *EVENT_NOTIFY)(cl_event event, cl_int event_command_exec_status, void *user_data);

typedef struct _user_callback {
  cl_int            status;     /* The execution status */
  cl_bool           executed;   /* Indicat the callback function been called or not */
  EVENT_NOTIFY      pfn_notify; /* Callback function */
  void*             user_data;  /* Callback user data */
  struct _user_callback*    next;       /* Next event callback in list */
} user_callback;

struct _cl_event {
  _cl_base_object    base;
  cl_context         ctx;         /* The context associated with event */
  cl_command_queue   queue;       /* The command queue associated with event */
  cl_command_type    type;        /* The command type associated with event */
  cl_int             status;      /* The execution status */
  cl_gpgpu           gpgpu;       /* Current gpgpu, owned by this structure. */
  cl_gpgpu_event     gpgpu_event; /* The event object communicate with hardware */
  user_callback*     user_cb;     /* The event callback functions */
  enqueue_callback*  enqueue_cb;  /* This event's enqueue */
  enqueue_callback*  waits_head;  /* The head of enqueues list wait on this event */
  cl_bool            emplict;     /* Identify this event whether created by api emplict*/
  cl_ulong           timestamp[4];/* The time stamps for profiling. */
  cl_ulong	     queued_timestamp;
  cl_event   last_next, last_prev;/* We need a list to monitor untouchable api event*/
};

#define CL_OBJECT_EVENT_MAGIC 0x8324a9f810ebf90fLL
#define CL_OBJECT_IS_EVENT(obj) (((cl_base_object)obj)->magic == CL_OBJECT_EVENT_MAGIC)

/* Create a new event object */
cl_event cl_event_new(cl_context, cl_command_queue, cl_command_type, cl_bool);
/* Unref the object and delete it if no more reference on it */
void cl_event_delete(cl_event);
/* Add one more reference to this object */
void cl_event_add_ref(cl_event);
/* Register a user callback function for specific commond execution status */
cl_int cl_event_set_callback(cl_event, cl_int, EVENT_NOTIFY, void *);
/* Execute the event's callback if the event's status supersedes the callback's status. Free the callback if specified */
void cl_event_call_callback(cl_event event, cl_int status, cl_bool free_cb);
/* Check events wait list for enqueue commonds */
cl_int cl_event_check_waitlist(cl_uint, const cl_event *, cl_event *, cl_context);
/* Wait the all events in wait list complete */
cl_int cl_event_wait_events(cl_uint, const cl_event *, cl_command_queue);
/* New a enqueue suspend task */
void cl_event_new_enqueue_callback(cl_event, enqueue_data *, cl_uint, const cl_event *);
/* Set the event status and call all callbacks */
void cl_event_set_status(cl_event, cl_int);
/* Check and update event status */
void cl_event_update_status(cl_event, cl_int);
/* Create the marker event */
cl_int cl_event_marker_with_wait_list(cl_command_queue, cl_uint, const cl_event *,  cl_event*);
/* Create the barrier event */
cl_int cl_event_barrier_with_wait_list(cl_command_queue, cl_uint, const cl_event *,  cl_event*);
/* Get the cpu time */
cl_ulong cl_event_get_cpu_timestamp(cl_ulong *cpu_time);
/*Get the cpu time for queued*/
cl_int cl_event_get_queued_cpu_timestamp(cl_event event);
/*get timestamp delate between end and start*/
cl_ulong cl_event_get_timestamp_delta(cl_ulong start_timestamp,cl_ulong end_timestamp);
/*Get start time stamp*/
cl_ulong cl_event_get_start_timestamp(cl_event event);
/*Get end time stamp*/
cl_ulong cl_event_get_end_timestamp(cl_event event);
/* Do the event profiling */
cl_int cl_event_get_timestamp(cl_event event, cl_profiling_info param_name);
/* insert the user event */
cl_int cl_event_insert_user_event(user_event** p_u_ev, cl_event event);
/* remove the user event */
cl_int cl_event_remove_user_event(user_event** p_u_ev, cl_event event);
/* flush the event's pending gpgpu batch buffer and notify driver this gpgpu event has been flushed. */
cl_int cl_event_flush(cl_event event);
/* monitor or block wait all events in the last_event list */
void cl_event_update_last_events(cl_command_queue queuet, int wait);
/* insert the event into the last_event list in queue */
void cl_event_insert_last_events(cl_command_queue queue, cl_event event);
#endif /* __CL_EVENT_H__ */

