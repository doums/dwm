#include <X11/XF86keysym.h>

static const unsigned int borderpx  = 4;
static const unsigned int gappx     = 14;
static const int bar_pa             = 6;
static const unsigned int snap      = 32;
static const int showbar            = 1;
static const int topbar             = 0;
static const char *fonts[]          = { "JetBrainsMono Nerd Font:pixelsize=20:antialias=true" };
static const char dmenufont[]       = "JetBrains Mono:pixelsize=20:antialias=true";
static const char grey[]            = "#404040";
static const char dark_grey[]       = "#262626";
static const char near_black[]      = "#0d0d0d";
static const char stone[]           = "#8c8c8c";

static const char *colors[][3] = {
        //                      fg, bg, border
        [SchemeNorm]        = { dark_grey, dark_grey, dark_grey },
        [SchemeSel]         = { grey, grey, grey },
        //                      text, bg, unused color (cannot be empty)
        [SchemeStatus]      = { stone, dark_grey, "#000000" },
        [SchemeTagsSel]     = { stone, near_black, "#000000" },
        [SchemeTagsNorm]    = { stone, dark_grey, "#000000" },
        [SchemeInfoSel]     = { stone, near_black, "#000000" },
        [SchemeInfoNorm]    = { stone, dark_grey, "#000000" },
};
static const char *tags[] = { "󰞷", "󰈹", "󱃖", "󰒱", "󰡶", "1", "2", "3", "4" };

static const Rule rules[] = {
        { "jetbrains-toolbox", NULL, NULL,0, 1, -1 },
        { "Pavucontrol", NULL, NULL, 0, 1, -1 },
        { "feh", NULL, NULL, 0, 1, -1 },
        { "nnn", NULL, NULL, 0, 1, -1 },
        { "Galculator", NULL, NULL, 0, 1, -1 },
        { "TeamSpeak 3", NULL, NULL, 0, 1, -1 }
};

static const char *const autostart[] = {
        "start-pulseaudio-x11", NULL,
        "picom", "--config", "/home/pierre/.config/picom.conf", "-b", NULL,
        "redshift", "-c", "/home/pierre/.config/redshift/redshift.conf", NULL,
        "gnome-keyring-daemon", "--start", "-d", NULL,
        "dunst", "-c", "/home/pierre/.config/dunst/dunstrc", NULL,
        "udiskie", NULL,
        NULL // terminate
};

static const float mfact     = 0.50;
static const int nmaster     = 1;
static const int resizehints = 1;
static const int attachbelow = 1;

static const Layout layouts[] = {
        { "󰜴", tile },
        { "󰜮", bstack },
        { "󰢡", monocle },
        { "󰖯", NULL }, /* floating layout */
        { NULL, NULL },
};

#define MODKEY Mod4Mask
#define TAGKEYS(KEY,TAG) \
    { MODKEY,                       KEY,      view,           {.ui = 1 << TAG} }, \
    { MODKEY|ControlMask,           KEY,      toggleview,     {.ui = 1 << TAG} }, \
    { MODKEY|ShiftMask,             KEY,      tag,            {.ui = 1 << TAG} }, \
    { MODKEY|ControlMask|ShiftMask, KEY,      toggletag,      {.ui = 1 << TAG} },

static char dmenumon[2] = "0";
static const char *dmenucmd[]       = { "dmenu_run", "-fn", dmenufont, "-nb", near_black, "-nf", stone, "-sb", dark_grey, "-sf", stone, NULL };
static const char *termcmd[]        = { "alacritty", NULL };
static const char *rofi[]           = { "rofi", "-show", "drun", NULL };
static const char *windows[]        = { "rofi", "-show", "window", NULL };
static const char *light_up[]       = { "pral.sh", "light_up", NULL };
static const char *light_down[]     = { "pral.sh", "light_down", NULL };
static const char *sink_up[]        = { "pral.sh", "sink_up", NULL };
static const char *sink_down[]      = { "pral.sh", "sink_down", NULL };
static const char *sink_mute[]      = { "pral.sh", "sink_mute", NULL };
static const char *source_mute[]    = { "pral.sh", "source_mute", NULL };
static const char *redshift[]       = { "pkill", "-USR1", "redshift", NULL };
static const char *lock[]           = { "lock.sh", NULL };
static const char *screenshot[]     = { "screenshot.sh", NULL };
static const char *clip_shot[]      = {"clipshot.sh", NULL };
static const char *set_dp[]         = { "set_dp.sh", "-fn", dmenufont, "-nb", near_black, "-nf", stone, "-sb", dark_grey, "-sf", stone, NULL };
static const char *session[]        = { "session.sh", "-fn", dmenufont, "-nb", near_black, "-nf", stone, "-sb", dark_grey, "-sf", stone, NULL };

static Key keys[] = {
        { MODKEY,                       XK_t,                      spawn,          {.v = termcmd } },
        { MODKEY,                       XK_exclam,                 spawn,          {.v = rofi }},
        { MODKEY,                       XK_w,                      spawn,          {.v = windows }},
        { MODKEY,                       XK_b,                      togglebar,      {0} },
        { MODKEY,                       XK_j,                      focusstack,     {.i = +1 } },
        { MODKEY,                       XK_k,                      focusstack,     {.i = -1 } },
        { MODKEY,                       XK_semicolon,              incnmaster,     {.i = +1 } },
        { MODKEY,                       XK_comma,                  incnmaster,     {.i = -1 } },
        { MODKEY,                       XK_h,                      setmfact,       {.f = -0.05} },
        { MODKEY,                       XK_l,                      setmfact,       {.f = +0.05} },
        { MODKEY,                       XK_Return,                 zoom,           {0} },
        { MODKEY,                       XK_Tab,                    view,           {0} },
        { MODKEY,                       XK_x,                      killclient,     {0} },
        { MODKEY,                       XK_s,                      setlayout,      {.v = &layouts[0]} },
        { MODKEY,                       XK_v,                      setlayout,      {.v = &layouts[1]} },
        { MODKEY,                       XK_m,                      setlayout,      {.v = &layouts[2]} },
        { MODKEY,                       XK_space,                  cyclelayout,    {.i = +1 } },
        { MODKEY|ShiftMask,             XK_space,                  cyclelayout,    {.i = -1 } },
        { MODKEY,                       XK_f,                      togglefloating, {0} },
        { MODKEY,                       XK_0,                      view,           {.ui = ~0 } },
        { MODKEY|ShiftMask,             XK_0,                      tag,            {.ui = ~0 } },
        { MODKEY,                       XK_Left,                   focusmon,       {.i = -1 } },
        { MODKEY,                       XK_Right,                  focusmon,       {.i = +1 } },
        { MODKEY|ShiftMask,             XK_h,                      tagmon,         {.i = -1 } },
        { MODKEY|ShiftMask,             XK_l,                      tagmon,         {.i = +1 } },
        { MODKEY,                       XK_q,                      spawn,          {.v = session } },
        { 0,                            XF86XK_MonBrightnessUp,    spawn,          {.v = light_up } },
        { 0,                            XF86XK_MonBrightnessDown,  spawn,          {.v = light_down } },
        { 0,                            XF86XK_AudioRaiseVolume,   spawn,          {.v = sink_up } },
        { 0,                            XF86XK_AudioLowerVolume,   spawn,          {.v = sink_down } },
        { 0,                            XF86XK_AudioMute,          spawn,          {.v = sink_mute } },
        { 0,                            XF86XK_AudioMicMute,       spawn,          {.v = source_mute } },
        { MODKEY|ShiftMask,             XK_l,                      spawn,          {.v = lock } },
        { 0,                            XK_Print,                  spawn,          {.v = screenshot } },
        { MODKEY,                       XK_c,                      spawn,          {.v = clip_shot } },
        { MODKEY,                       XK_d,                      spawn,          {.v = set_dp } },
        { MODKEY,                       XK_asterisk,               spawn,          {.v = redshift } },
        TAGKEYS(                              XK_ampersand,              0)
        TAGKEYS(                              XK_eacute,                 1)
        TAGKEYS(                              XK_quotedbl,               2)
        TAGKEYS(                              XK_apostrophe,             3)
        TAGKEYS(                              XK_parenleft,              4)
        TAGKEYS(                              XK_minus,                  5)
        TAGKEYS(                              XK_egrave,                 6)
        TAGKEYS(                              XK_underscore,             7)
        TAGKEYS(                              XK_ccedilla,               8)
};

static Button buttons[] = {
        { ClkLtSymbol,          0,              Button1,        setlayout,      {0} },
        { ClkLtSymbol,          0,              Button3,        setlayout,      {.v = &layouts[2]} },
        { ClkStatusText,        0,              Button2,        spawn,          {.v = termcmd } },
        { ClkClientWin,         MODKEY,         Button1,        movemouse,      {0} },
        { ClkClientWin,         MODKEY,         Button2,        togglefloating, {0} },
        { ClkClientWin,         MODKEY,         Button3,        resizemouse,    {0} },
        { ClkTagBar,            0,              Button1,        view,           {0} },
        { ClkTagBar,            0,              Button3,        toggleview,     {0} },
        { ClkTagBar,            MODKEY,         Button1,        tag,            {0} },
        { ClkTagBar,            MODKEY,         Button3,        toggletag,      {0} },
};
