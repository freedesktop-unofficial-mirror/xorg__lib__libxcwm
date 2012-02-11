/* Copyright (c) 2012 Jess VanDerwalker <washu@sonic.net> All rights
 * reserved
 *
 * event_loop.c
 *
 * Permission is hereby granted, free of charge, to any person
 * obtaining a copy of this software and associated documentation
 * files (the "Software"), to deal in the Software without
 * restriction, including without limitation the rights to use, copy,
 * modify, merge, publish, distribute, sublicense, and/or sell copies
 * of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be
 * included in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 * NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS
 * BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN
 * ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
 * CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */


#include <pthread.h>
#include "xtoq.h"
#include "util.h"
 
typedef struct _connection_data {
	xcb_connection_t *conn;
	void * callback;
} _connection_data;

/* The thread that is running the event loop */
pthread_t _event_thread;

/* Functions only called within event_loop.c */
void *run_event_loop(void *thread_arg_struct);

/* Functions included in xtoq_internal.h */

int
_xtoq_start_event_loop (xcb_connection_t *conn,
						void *event_callback)
{
	_connection_data *conn_data;
    
    conn_data = malloc(sizeof(_connection_data));
    assert(conn_data);
    
	conn_data->conn = conn;
	conn_data->callback = event_callback;

	return pthread_create(&_event_thread,
						  NULL,
						  run_event_loop,
						  (void *) conn_data);
}

void *run_event_loop (void *thread_arg_struct)
{
	_connection_data *conn_data;
	xcb_connection_t *event_conn;
	xcb_generic_event_t *evt;
	xtoq_event_t return_evt;
    void (*callback_ptr) (xtoq_event_t event);

	conn_data = thread_arg_struct;
	event_conn = conn_data->conn;
	callback_ptr = conn_data->callback;
    
    free(thread_arg_struct);

	/* Start the event loop */
	
    while ((evt = xcb_wait_for_event(event_conn))) {
		if ((evt->response_type & ~0x80) == _damage_event) {
			return_evt.event_type = XTOQ_DAMAGE;
			return_evt.context = NULL;
            callback_ptr(return_evt);        
		} else {
			switch (evt->response_type & ~0x80) {
            case XCB_EXPOSE: {
                xcb_expose_event_t *exevnt = (xcb_expose_event_t *)evt;
                
                printf("Window %u exposed. Region to be redrawn at location (%d, %d), ",
                       exevnt->window, exevnt->x, exevnt->y);
                printf("with dimentions (%d, %d).\n", exevnt->width, exevnt->height);
                
                return_evt.event_type = XTOQ_EXPOSE;
                free(exevnt);
                callback_ptr(return_evt);
            }
            case XCB_CREATE_NOTIFY: {
                // Window created as child of root window
                xcb_create_notify_event_t *notify = (xcb_create_notify_event_t *)evt;
                
                return_evt.event_type = XTOQ_CREATE;
                // Create the memory for a new context - this will need to be freed when
                // the window is destroyed
                return_evt.context = malloc(sizeof(xtoq_context_t));
                return_evt.context->conn = event_conn;
                return_evt.context->window = notify->window;
                return_evt.context->parent = notify->parent;
                return_evt.context->x = notify->x;
                return_evt.context->y = notify->y;
                return_evt.context->window = notify->width;
                return_evt.context->window = notify->height;

                free(notify);
                
                // TODO: Add the context created here to the data structure
                
				callback_ptr(return_evt);

                break;
            }
            case XCB_DESTROY_NOTIFY: {
                // Window destroyed in root window
                xcb_destroy_notify_event_t *notify = (xcb_destroy_notify_event_t *)evt;
                return_evt.event_type = XTOQ_DESTROY;
                
                // Memory for context will need to be freed by caller
                // Only setting the window - other values will be garbage.
                return_evt.context = malloc(sizeof(xtoq_context_t));
                return_evt.context->conn = event_conn;
                return_evt.context->window = notify->window;
                
                free(notify);
                
				
                // TODO: Remove the window from the data structure.
                // Need to figure out where memory is freed.
                
                callback_ptr(return_evt);
                
                break;
            }
            default: {
                printf("UNKNOWN EVENT\n");
                break;
            }
			}
		}
	}
}
