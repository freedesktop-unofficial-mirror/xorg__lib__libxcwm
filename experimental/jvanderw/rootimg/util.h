/* Copyright (c) 2012 Jess VanDerwalker
 *
 * util.h
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

#ifndef _UTIL_H_
#define _UTIL_H_

#include <stdlib.h>
#include <stdio.h>
#include <xcb/xcb.h>

/**
 * Return the given windows attributes reply.
 * @param conn The windows connection.
 * @param window The window.
 * @return The window attributes reply. Null if the request fails.
 */
xcb_get_window_attributes_reply_t *
GetWindowAttributes (xcb_connection_t *conn, xcb_window_t window);

/**
 * Return the geometry of the window in a geometry reply.
 * @param conn The windows connection.
 * @param window The window.
 * @return The window's geometry reply. Null if the request for reply fails.
 */
xcb_get_geometry_reply_t *
GetWindowGeometry (xcb_connection_t *conn, xcb_window_t window);

/**
 * Check the request cookie and determine if there is an error.
 * @param conn The connection the request was sent on.
 * @param cookie The cookie returned by the request.
 * @param msg the string to display if there is an error with the request.
 * @return int The number of the error code, if any. Otherwise zero.
 */
int
RequestCheck (xcb_connection_t *conn, xcb_void_cookie_t cookie,
              char *msg);

#endif  /* _UTIL_H_ */
