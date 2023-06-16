#include <stdio.h>
#include <X11/Xlib.h>

/*
 * https://stackoverflow.com/questions/14553435/how-to-listen-for-mouse-events-in-linux
 * logging mouse position, clicks and releases:
 *
 * https://tronche.com/gui/x/xlib/
 * https://tronche.com/gui/x/xlib/events/keyboard-pointer/keyboard-pointer.html
 *
 * gcc mouse.c -o mouse -lX11
 */

/* using xev to figure out */
char *buttons[] = {
    "Button1 (left)",
    "Button2 (wheel press)",
    "Button3 (righ)",
    "Button4 (wheel forward)",
    "Button5 (wheel backward)"
};

int main(int argc, char *argv[])
{
    Display *display;
    XEvent xevent;
    Window window;

    if ((display = XOpenDisplay(NULL)) == NULL)
	{
		printf("Try set DISPLAY enviroment variable\nDISPLAY=:0 %s\n", argv[0]);
        return -1;
	}

    window = DefaultRootWindow(display);
    XAllowEvents(display, AsyncBoth, CurrentTime);
    XGrabPointer(display,
                 window,
                 1,
                 PointerMotionMask | ButtonPressMask | ButtonReleaseMask ,
                 GrabModeAsync,
                 GrabModeAsync,
                 None,
                 None,
                 CurrentTime);

    while (1) {
        XNextEvent(display, &xevent);
        switch (xevent.type) {
            case MotionNotify:
                printf("Mouse move      : [%d, %d]\n", xevent.xmotion.x_root, xevent.xmotion.y_root);
                break;
            case ButtonPress:
                printf("%s pressed\n", buttons[xevent.xbutton.button - 1]);
                break;
            case ButtonRelease:
                printf("%s released\n", buttons[xevent.xbutton.button - 1]);
                break;
        }
    }

    return 0;
}
