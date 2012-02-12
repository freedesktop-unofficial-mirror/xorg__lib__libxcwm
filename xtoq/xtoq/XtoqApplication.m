/*Copyright (C) 2012 Braden Wooley, Ben Huddle
 
 Permission is hereby granted, free of charge, to any person obtaining a copy of
 this software and associated documentation files (the "Software"), to deal in
 the Software without restriction, including without limitation the rights to
 use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies
 of the Software, and to permit persons to whom the Software is furnished to do
 so, subject to the following conditions:
 
 The above copyright notice and this permission notice shall be included in all
 copies or substantial portions of the Software.
 
 THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 SOFTWARE.
 */

#import "XtoqApplication.h"

@implementation XtoqApplication

int XtoqApplicationMain(int argc, char** argv){

    char *scrn;
    scrn = findScreen(argc, argv);
    
    /*dispatch_async(dispatch_get_global_queue(DISPATCH_QUEUE_PRIORITY_DEFAULT, 0), ^{
        CFBundleRef bundle = CFBundleGetMainBundle();
        char *bundle_path = CFBundleGetDataPointerForName(bundle, CFSTR("xtoq.app"));
        
        const char *startx_path = "//opt/X11/bin/startx";
        char const *Xorg_path = "/opt/X11/bin/Xorg";
        char *xinitrc_path;
        asprintf(&xinitrc_path, "%s/Contents/Resources/X11/lib/X11/xinit/xinitrc", bundle_path);
        if (xinitrc_path == NULL) {
            //... do something
            NSLog(@"xinitrc path is null");
        }
        
        const char *argv[5];
        argv[0] = startx_path;
        argv[1] = xinitrc_path;
        argv[2] = "--";
        argv[3] = Xorg_path;
        argv[4] = '\0';
        
        pid_t child;
        int error = posix_spawnp(&child, argv[0], NULL, NULL, argv, _NSGetEnviron());
        if (error) {
            //... do something
            NSLog(@"error with posix_spawnp");
        }
        
        int retval;
        waitpid(child, &retval, 0);
        if (retval != 0) {
            //... do something
            NSLog(@"error with waitpid");
        }        
    }); */
    
    
    // initializes simple subclass
    [XtoqApplication sharedApplication];
    XtoqController *controller = [[XtoqController alloc] init];
    [controller setScreen:scrn];
    [NSApp setDelegate: controller];
    [NSApp run];

    return 1;
}


char* findScreen(int argc, char **argv) {
    int i;
    int j;
    int loc;
    char place;
    bool toBreak = false;
    
    for (i = 1; !toBreak && i < argc; ++i) {
        for (j = 0; argv[i][j] != '\0'; ++j) {
            place = argv[i][j];
            if (place == ':') {
                toBreak = true;
                loc = i;
                break;
            }
            else loc = 0;
        }
    }
    
    if ( loc != 0) {
        return argv[loc];
    }
    else return ":1";
    
}

@end
