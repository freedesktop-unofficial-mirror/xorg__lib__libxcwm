# TODO: configure.ac these options
APPLE_APPLICATIONS_DIR=/Applications
APPLE_APPLICATION_NAME=XtoQ

SUBDIRS = bundle

AM_CFLAGS = $(XCB_CFLAGS) $(BASE_CFLAGS)
AM_OBJCFLAGS = $(XCB_CFLAGS) $(BASE_CFLAGS)

INCLUDES = -I$(top_srcdir)/include

xtoqappdir = $(APPLE_APPLICATIONS_DIR)/$(APPLE_APPLICATION_NAME).app/Contents/MacOS

xtoqapp_PROGRAMS = xtoq

xtoq_SOURCES = \
	XtoqApplication.m \
	XtoqController.m \
	XtoqImageRep.m \
	XtoqView.m \
	XtoqWindow.m \
	main.m

xtoq_LDFLAGS =  \
	$(XCB_LIBS) \
        -Wl,-framework,AppKit

xtoq_LDADD = \
        $(top_builddir)/src/libxcwm/libxcwm.la

bin_PROGRAMS = keymap
keymap_SOURCES = \
	keymap_main.c \
	keymap.c \
	keysym2ucs.c
keymap_LDFLAGS = \
	$(XCB_LIBS) \
	-Wl,-framework,Cocoa \
	-Wl,-framework,Carbon \
	-Wl,-framework,IOKit

