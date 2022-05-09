
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/extensions/Xrandr.h>

static Display *g_disp;
static Window g_win;
static Screen *g_screen;
static int g_screenNumber;
static int g_white;
static int g_black;
static int g_rr_event_base;
static int g_rr_error_base;
static Window g_root_window;
static int g_ver_maj;
static int g_ver_min;

static int
print_randr(void)
{
    XRRScreenConfiguration *sc;
    int rr_screen;
    short rate;
    Rotation rot;
    Rotation cur_rot;
    XRRScreenSize *sizes;
    int nsizes;
    int index;
    int jndex;
    Time time;
    Time config_time;
    short *rates;
    int nrates;
    short sizeid;
    int min_width;
    int min_height;
    int max_width;
    int max_height;
    Status st;
    XRRScreenResources *sr;
    XRROutputInfo *oi;
    XRRCrtcInfo *ci;

    printf("print_randr: in\n");
    sc = XRRGetScreenInfo(g_disp, g_root_window);
    if (sc == NULL)
    {
        printf("XRRGetScreenInfo return nil\n");
    }
    else
    {
        printf("  XRRGetScreenInfo return ok\n");
        rot = XRRConfigRotations(sc, &cur_rot);
        printf("    XRRConfigRotations rotation 0x%4.4x current rotation 0x%4.4x\n", rot, cur_rot);
        time = XRRConfigTimes(sc, &config_time);
        printf("    XRRConfigTimes time 0x%8.8x config time 0x%8.8x\n", (int)time, (int)config_time);
        sizes = XRRConfigSizes(sc, &nsizes);
        printf("    XRRConfigSizes nsizes %d sizes %p\n", nsizes, sizes);
        if (sizes != NULL)
        {
            for (index = 0; index < nsizes; index++)
            {
                printf("      index %d width %d height %d mwidth %d mheight %d\n",
                       index,
                       sizes[index].width, sizes[index].height,
                       sizes[index].mwidth, sizes[index].mheight);
                rates = XRRConfigRates(sc, index, &nrates);
                printf("      XRRConfigRates nrates %d rates %p\n", nrates, rates);
                if (rates != NULL)
                {
                    for (jndex = 0; jndex < nrates; jndex++)
                    {
                        printf("        index %d rate 0x%4.4x\n", jndex, rates[jndex]);
                    }
                }
            }
        }
        sizeid = XRRConfigCurrentConfiguration(sc, &rot);
        printf("    XRRConfigCurrentConfiguration sizeid 0x%4.4x rotation 0x%4.4x\n", sizeid, rot);
        rate = XRRConfigCurrentRate(sc);
        printf("    XRRConfigCurrentRate rate 0x%4.4x\n", rate);
        XRRFreeScreenConfigInfo(sc);
    }
    rr_screen = XRRRootToScreen(g_disp, g_root_window);
    printf("  XRRRootToScreen for root window returns %d\n", rr_screen);
    rot = XRRRotations(g_disp, rr_screen, &cur_rot);
    printf("  XRRRotations rotation 0x%4.4x current rotation 0x%4.4x\n", rot, cur_rot);
    sizes = XRRSizes(g_disp, rr_screen, &nsizes);
    printf("  XRRSizes nsizes %d sizes %p\n", nsizes, sizes);
    if (sizes != NULL)
    {
        for (index = 0; index < nsizes; index++)
        {
            printf("    index %d width %d height %d mwidth %d mheight %d\n", index,
                   sizes[index].width, sizes[index].height,
                   sizes[index].mwidth, sizes[index].mheight);
            rates = XRRRates(g_disp, rr_screen, index, &nrates);
            printf("    XRRRates nrates %d rates %p\n", nrates, rates);
            if (rates != NULL)
            {
                for (jndex = 0; jndex < nrates; jndex++)
                {
                    printf("      index %d rate 0x%4.4x\n", jndex, rates[jndex]);

                }
            }
        }
    }
    time = XRRTimes(g_disp, rr_screen, &config_time);
    printf("  XRRTimes time 0x%8.8x config time 0x%8.8x\n", (int)time, (int)config_time);
    st = XRRGetScreenSizeRange(g_disp, g_root_window, &min_width, &min_height, &max_width, &max_height);
    printf("  XRRGetScreenSizeRange status %d min_width %d min_height %d, max_width %d max_height %d\n",
           st, min_width, min_height, max_width, max_height);
    sr = XRRGetScreenResources(g_disp, g_root_window);
    if (sr == NULL)
    {
        printf("XRRGetScreenResources return nil\n");
    }
    else
    {
        printf("  XRRGetScreenResources return ok time 0x%8.8x config time 0x%8.8x ncrtc %d noutput %d nmode %d\n",
               (int)sr->timestamp, (int)sr->configTimestamp, sr->ncrtc, sr->noutput, sr->nmode);
        for (index = 0; index <sr->nmode; index++)
        {
            printf("    mode index %d ID 0x%4.4x width %d height %d dotClock %ld hSyncStart %d hSyncEnd %d\n"
                   "         hTotal %d hSkew %d vSyncStart %d vSyncEnd %d vTotal %d name %s nameLength %d\n"
                   "         modeFlags 0x%8.8x\n",
                   index, (int)sr->modes[index].id,
                   sr->modes[index].width, sr->modes[index].height, sr->modes[index].dotClock,
                   sr->modes[index].hSyncStart, sr->modes[index].hSyncEnd, sr->modes[index].hTotal,
                   sr->modes[index].hSkew, sr->modes[index].vSyncStart, sr->modes[index].vSyncEnd,
                   sr->modes[index].vTotal, sr->modes[index].name, sr->modes[index].nameLength,
                   (int)sr->modes[index].modeFlags);
        }
        for (index = 0; index < sr->noutput; index++)
        {
            oi = XRRGetOutputInfo(g_disp, sr, sr->outputs[index]);
            if (oi == NULL)
            {
                printf("    XRRGetOutputInfo return nil\n");
            }
            else
            {
                printf("    XRRGetOutputInfo index %d ID 0x%4.4x time 0x%8.8x crtc 0x%4.4x name %s mm_width %d mm_height %d\n"
                       "                     connection 0x%4.4x subpixel_order 0x%4.4x ncrtc %d nclone %d nmode %d npreferred %d\n",
                       index, (int)sr->outputs[index], (int)oi->timestamp, (int)oi->crtc, oi->name,
                       (int)oi->mm_width, (int)oi->mm_height, oi->connection,
                       oi->subpixel_order, oi->ncrtc, oi->nclone, oi->nmode, oi->npreferred);
                for (jndex = 0; jndex < oi->ncrtc; jndex++)
                {
                    printf("      crtc index %d ID 0x%4.4x\n", jndex, (int)oi->crtcs[jndex]);
                }
                for (jndex = 0; jndex < oi->nclone; jndex++)
                {
                    printf("      clone index %d ID 0x%4.4x\n", jndex, (int)oi->clones[jndex]);
                }
                for (jndex = 0; jndex < oi->nmode; jndex++)
                {
                    printf("      mode index %d ID 0x%4.4x\n", jndex, (int)oi->modes[jndex]);
                }
                XRRFreeOutputInfo(oi);
            }
        }
        for (index = 0; index <sr->ncrtc; index++)
        {
            ci = XRRGetCrtcInfo(g_disp, sr, sr->crtcs[index]);
            if (ci == NULL)
            {
                printf("    XRRGetCrtcInfo return nil\n");
            }
            else
            {
                printf("    XRRGetCrtcInfo index %d ID 0x%4.4x time 0x%8.8x x %d y %d width %d height %d mode 0x%4.4x\n"
                       "                   rotation 0x%4.4x noutput %d rotations 0x%4.4x npossible %d\n",
                       index, (int)sr->crtcs[index], (int)ci->timestamp,
                       ci->x, ci->y, ci->width, ci->height, (int)ci->mode,
                       ci->rotation, ci->noutput, (int)ci->rotations, ci->npossible);
                for (jndex = 0; jndex < ci->noutput; jndex++)
                {
                    printf("      output index %d ID 0x%4.4x\n", jndex, (int)ci->outputs[jndex]);
                }
                for (jndex = 0; jndex < ci->npossible; jndex++)
                {
                    printf("      possibe index %d ID 0x%4.4x\n", jndex, (int)ci->possible[jndex]);
                }
                XRRFreeCrtcInfo(ci);
            }
        }
        XRRFreeScreenResources(sr);
    }
    printf("print_randr: out\n");
    return 0;
}

static int
process_randr(XEvent *ev)
{
    XRRScreenChangeNotifyEvent *rr_screen_change_notify;
    XRRNotifyEvent *rr_notify_event;
    XRRCrtcChangeNotifyEvent *rr_crtc_change_notify;
    XRROutputChangeNotifyEvent *rr_output_change_notify;
    XRROutputPropertyNotifyEvent *rr_output_property_notify;

    switch (ev->type - g_rr_event_base)
    {
        case RRScreenChangeNotify:
            XRRUpdateConfiguration(ev);
            rr_screen_change_notify = (XRRScreenChangeNotifyEvent *) ev;
            printf("process_randr: RRScreenChangeNotify event width %d event height %d\n",
                   rr_screen_change_notify->width,
                   rr_screen_change_notify->height);
            printf("process_randr: RRScreenChangeNotify DisplayWidth %d DisplayHeight %d\n",
                   DisplayWidth(g_disp, g_screenNumber),
                   DisplayHeight(g_disp, g_screenNumber));
            break;
        case RRNotify:
            rr_notify_event = (XRRNotifyEvent *) ev;
            printf("process_randr: RRNotify window 0x%8.8x\n", (int)rr_notify_event->window);
            switch (rr_notify_event->subtype)
            {
                case RRNotify_CrtcChange:
                    rr_crtc_change_notify = (XRRCrtcChangeNotifyEvent *) ev;
                    printf("process_randr: RRNotify_CrtcChange crtc 0x%4.4x "
                           "mode 0x%4.4x rotation 0x%4.4x x %d y %d "
                           "width %d height %d\n",
                           (int)rr_crtc_change_notify->crtc,
                           (int)rr_crtc_change_notify->mode,
                           (int)rr_crtc_change_notify->rotation,
                           rr_crtc_change_notify->x, rr_crtc_change_notify->y,
                           rr_crtc_change_notify->width, rr_crtc_change_notify->height);
                    break;
                case RRNotify_OutputChange:
                    rr_output_change_notify = (XRROutputChangeNotifyEvent *) ev;
                    printf("process_randr: RRNotify_OutputChange output 0x%4.4x crtc 0x%4.4x "
                           "mode 0x%4.4x rotation 0x%4.4x connection 0x%4.4x "
                           "subpixel_order 0x%4.4x\n",
                           (int)rr_output_change_notify->output,
                           (int)rr_output_change_notify->crtc,
                           (int)rr_output_change_notify->mode,
                           (int)rr_output_change_notify->rotation,
                           (int)rr_output_change_notify->connection,
                           (int)rr_output_change_notify->subpixel_order);
                    break;
                case RRNotify_OutputProperty:
                    rr_output_property_notify = (XRROutputPropertyNotifyEvent *) ev;
                    printf("process_randr: RRNotify_OutputProperty output 0x%4.4x "
                           "property 0x%8.8x timestamp 0x%8.8x state %d\n",
                           (int)rr_output_property_notify->output,
                           (int)rr_output_property_notify->property,
                           (int)rr_output_property_notify->timestamp,
                           rr_output_property_notify->state);
                    break;
            }
            break;
    }
    return 0;
}

int
main(int argc, char **argv)
{
    int eventMask;
    XEvent ev;
    int width;
    int height;

    g_disp = XOpenDisplay(NULL);
    if (g_disp == NULL)
    {
        printf("error opening display\n");
        return 1;
    }
    g_screenNumber = DefaultScreen(g_disp);
    printf("DefaultScreen %d\n", g_screenNumber);
    g_white = WhitePixel(g_disp, g_screenNumber);
    g_black = BlackPixel(g_disp, g_screenNumber);
    g_screen = ScreenOfDisplay(g_disp, g_screenNumber);
    printf("ScreenOfDisplay %p\n", g_screen);
    g_root_window = RootWindowOfScreen(g_screen);
    printf("RootWindowOfScreen 0x%8.8x\n", (int)g_root_window);
    /* so we get root window events */
    eventMask = StructureNotifyMask;
    XSelectInput(g_disp, g_root_window, eventMask);
    /* create window */
    g_win = XCreateSimpleWindow(g_disp, g_root_window, 50, 50, 250, 250,
                                0, g_black, g_white);
    printf("XCreateSimpleWindow 0x%8.8x\n", (int)g_win);
    eventMask = KeyPressMask | KeyReleaseMask | ButtonPressMask |
                ButtonReleaseMask | VisibilityChangeMask |
                FocusChangeMask | StructureNotifyMask |
                PointerMotionMask | ExposureMask | PropertyChangeMask;
    XSelectInput(g_disp, g_win, eventMask);
    XMapWindow(g_disp, g_win);
    if (!XRRQueryExtension(g_disp, &g_rr_event_base, &g_rr_error_base))
    {
        printf("error randr\n");
        return 1;
    }
    printf("client randr version major %d minor %d\n", RANDR_MAJOR, RANDR_MINOR);
    XRRQueryVersion(g_disp, &g_ver_maj, &g_ver_min);
    printf("server randr version major %d minor %d\n", g_ver_maj, g_ver_min);
    XRRSelectInput(g_disp, g_win, RRScreenChangeNotifyMask |
                   RRCrtcChangeNotifyMask | RROutputChangeNotifyMask |
                   RROutputPropertyNotifyMask);
    printf("click in window to dump RANDR info\n");
    for (;;)
    {
        XNextEvent(g_disp, &ev);
        switch (ev.type)
        {
            case ButtonRelease:
                print_randr();
                break;
            case ConfigureNotify:
                if (ev.xconfigure.window == g_root_window)
                {
                    XRRUpdateConfiguration(&ev);
                    printf("ConfigureNotify for root window "
                           "width %d height %d\n",
                           ev.xconfigure.width, ev.xconfigure.height);
                    width = DisplayWidth(g_disp, DefaultScreen(g_disp));
                    height = DisplayHeight(g_disp, DefaultScreen(g_disp));
                    printf("  DisplayWidth %d DisplayHeight %d\n", width, height);
                }
                break;
            default:
                if ((ev.type >= g_rr_event_base) &&
                    (ev.type < g_rr_event_base + RRNumberEvents))
                {
                    process_randr(&ev);
                }
                break;

        }
    }
    return 0;
}
