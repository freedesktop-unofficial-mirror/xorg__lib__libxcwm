/* Copyright (c) 2012 David Snyder, Ben Carr, Jess VanDerwalker
 *
 * init.c
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

#include <string.h>
#include "xtoq_internal.h"

int _damage_event = 0;

xcb_query_extension_reply_t *
_xtoq_init_extension (xcb_connection_t *conn, char *extension_name)
{
    xcb_query_extension_cookie_t cookie = xcb_query_extension(conn, strlen(extension_name), extension_name);
	xcb_query_extension_reply_t *reply = xcb_query_extension_reply(conn, cookie, NULL);
	if (!reply->present) {
		free(reply);
        printf("%s extension not present\n", extension_name);
        exit(1);
	} else {
        printf("%s extension present\n", extension_name);
    }
    
	return reply;
}

void
_xtoq_init_damage(xtoq_context_t *contxt)
{
    
    xcb_query_extension_reply_t *reply =_xtoq_init_extension(contxt->conn, "DAMAGE");
    
    xcb_damage_query_version_cookie_t version_cookie = 
    xcb_damage_query_version(contxt->conn, 
                             XCB_DAMAGE_MAJOR_VERSION,
                             XCB_DAMAGE_MINOR_VERSION);
	xcb_damage_query_version_reply_t* version_reply = xcb_damage_query_version_reply(contxt->conn, version_cookie, NULL);
    
    _damage_event = reply->first_event + XCB_DAMAGE_NOTIFY;
    
	free(version_reply);
	free(reply);
    
    xcb_damage_damage_t damage = xcb_generate_id(contxt->conn);
    
    // Refer to the Damage Protocol. level = 0 corresponds to the level
    // DamageReportRawRectangles.  Another level may be more appropriate.
    uint8_t level = XCB_DAMAGE_REPORT_LEVEL_BOUNDING_BOX;
    xcb_void_cookie_t cookie = xcb_damage_create(contxt->conn,
                                                 damage, contxt->window, level);
    
    /* Assign this damage object to the roots window's context */
    contxt->damage = damage;
    
}

void
_xtoq_init_xfixes (xtoq_context_t *contxt)
{
    xcb_xfixes_query_version_cookie_t cookie = 
    xcb_xfixes_query_version(contxt->conn, 4, 0);
    
    xcb_xfixes_query_version_reply_t *reply = 
    xcb_xfixes_query_version_reply(contxt->conn, cookie, NULL);
    
    free(reply);
}