/* Copyright (c) 2011 Jess VanDerwalker
 *
 * lpxcb.h
 */

#ifndef _LPXCB_API_H_
#define _LPXCB_API_H_

#include <stdlib.h>
#include <stdio.h>
#include <xcb/xcb.h>
#include <xcb/damage.h>
#include <xcb/composite.h>


/* Holds information for a window */
typedef struct lpxcb_window_t {
    /* The connection associated with this window */
    xcb_connection_t *conn;
    /* The id of this window */
    xcb_drawable_t window;
    /* The id of this parent's window, if it has one. */
    xcb_drawable_t parent;
    /* Damage associated with this window */
    xcb_damage_damage_t damage;

    /* Should we have something for the pixmap? */
} lpxcb_window_t;



#endif  /* _LPXCB_API_H_ */
