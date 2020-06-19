#include <X11/XF86keysym.h>

/* appearance */
static const unsigned int borderpx  = 4;        /* border pixel of windows */
static const unsigned int gappx     = 14;        /* gaps between windows */
static const int bar_pa             = 6;        /* pixel vertical padding of bar */
static const unsigned int snap      = 32;       /* snap pixel */
static const int showbar            = 1;        /* 0 means no bar */
static const int topbar             = 0;        /* 0 means bottom bar */
static const char *fonts[]          = { "JetBrains Mono:pixelsize=22:antialias=true" };
static const char dmenufont[]       = "Inconsolata:pixelsize=22:antialias=true";
static const char dark[]            = "#262018";
static const char green[]           = "#374029";
static const char green_light[]     = "#425931";
static const char white[]           = "#f2f2f0";
static const char *colors[][3]      = {
	/*               fg         bg         border   */
	[SchemeNorm] = {white, green, green },
	[SchemeSel]  = {white, green, green_light  },
    [SchemeStatus]  = {white, green, "#000000"  }, // Statusbar right {text,background,not used but cannot be empty}
    [SchemeTagsSel]  = {white, dark, "#000000"  }, // Tagbar left selected {text,background,not used but cannot be empty}
    [SchemeTagsNorm]  = {white, green, "#000000"  }, // Tagbar left unselected {text,background,not used but cannot be empty}
    [SchemeInfoSel]  = {white, green, "#000000"  }, // infobar middle  selected {text,background,not used but cannot be empty}
    [SchemeInfoNorm]  = {white, green, "#000000"  }, // infobar middle  unselected {text,background,not used but cannot be empty}
};

/* tagging */
static const char *tags[] = { "$", "www", "IDE", "slack", "1", "2", "3", "4", "5" };

static const Rule rules[] = {
	/* xprop(1):
	 *	WM_CLASS(STRING) = instance, class
	 *	WM_NAME(STRING) = title
	 */
	/* class      instance    title       tags mask     isfloating   monitor */
	{ "Gimp",     NULL,       NULL,       0,            1,           -1 },
	{ "Firefox",  NULL,       NULL,       1 << 8,       0,           -1 },
};

/* layout(s) */
static const float mfact     = 0.50; /* factor of master area size [0.05..0.95] */
static const int nmaster     = 1;    /* number of clients in master area */
static const int resizehints = 1;    /* 1 means respect size hints in tiled resizals */
static const int attachbelow = 1;    /* 1 means attach after the currently active window */

static const Layout layouts[] = {
	/* symbol     arrange function */
	{ "[]=",      tile },    /* first entry is default */
	{ "><>",      NULL },    /* no layout function means floating behavior */
	{ "[M]",      monocle },
    { NULL,       NULL },
};

/* key definitions */
#define MODKEY Mod4Mask
#define TAGKEYS(KEY,TAG) \
	{ MODKEY,                       KEY,      view,           {.ui = 1 << TAG} }, \
	{ MODKEY|ControlMask,           KEY,      toggleview,     {.ui = 1 << TAG} }, \
	{ MODKEY|ShiftMask,             KEY,      tag,            {.ui = 1 << TAG} }, \
	{ MODKEY|ControlMask|ShiftMask, KEY,      toggletag,      {.ui = 1 << TAG} },

/* commands */
static char dmenumon[2] = "0"; /* component of dmenucmd, manipulated in spawn() */
static const char *dmenucmd[] = {"dmenu_run", "-m", dmenumon, "-fn", dmenufont, "-nb", dark, "-nf", green, "-sb", green, "-sf", white, NULL };
static const char *termcmd[]  = { "alacritty", NULL };
static const char *rofi[] = { "rofi", "-show", "drun", NULL };
static const char *windows[] = { "rofi", "-show", "window", NULL };
static const char *light_up[] = { "light", "-A", "5", NULL };
static const char *light_down[] = { "light", "-U", "5", NULL };
static const char *sink_up[] = { "pactl", "set-sink-volume", "@DEFAULT_SINK@", "+5%", NULL };
static const char *sink_down[] = { "pactl", "set-sink-volume", "@DEFAULT_SINK@", "-5%", NULL };
static const char *sink_mute[] = { "pactl", "set-sink-mute", "@DEFAULT_SINK@", "toggle", NULL };
static const char *source_mute[] = { "pactl", "set-source-mute", "1", "toggle", NULL };
static const char *redshift[] = { "pkill", "-USR1", "redshift", NULL };
static const char *lock[] = { "lock.sh", NULL };
static const char *screenshot[] = { "screenshot.sh", NULL };
static const char *clip_shot[] = {"clipshot.sh", NULL };
static const char *set_dp[] = { "alacritty", "--class", "sDP", "-d", "13", "6", "--command", "sDP.sh", NULL };

static Key keys[] = {
	/* modifier                           key                        function        argument */
	{ MODKEY,                       XK_t,                      spawn,          {.v = termcmd } },
    { MODKEY,                       XK_exclam,                 spawn,          {.v = rofi }},
    { MODKEY,                       XK_w,                      spawn,          {.v = windows }},
	{ MODKEY,                       XK_b,                      togglebar,      {0} },
	{ MODKEY,                       XK_j,                      focusstack,     {.i = +1 } },
	{ MODKEY,                       XK_k,                      focusstack,     {.i = -1 } },
	{ MODKEY,                       XK_i,                      incnmaster,     {.i = +1 } },
	{ MODKEY,                       XK_d,                      incnmaster,     {.i = -1 } },
	{ MODKEY,                       XK_h,                      setmfact,       {.f = -0.05} },
	{ MODKEY,                       XK_l,                      setmfact,       {.f = +0.05} },
	{ MODKEY,                       XK_Return,                 zoom,           {0} },
	{ MODKEY,                       XK_Tab,                    view,           {0} },
	{ MODKEY,                       XK_x,                      killclient,     {0} },
	{ MODKEY,                       XK_s,                      setlayout,      {.v = &layouts[0]} },
	{ MODKEY,                       XK_m,                      setlayout,      {.v = &layouts[2]} },
	{ MODKEY,                       XK_space,                  cyclelayout,    {.i = +1 } },
    { MODKEY|ShiftMask,             XK_space,                  cyclelayout,    {.i = -1 } },
	{ MODKEY,                       XK_f,                      togglefloating, {0} },
	{ MODKEY,                       XK_0,                      view,           {.ui = ~0 } },
	{ MODKEY|ShiftMask,             XK_0,                      tag,            {.ui = ~0 } },
	{ MODKEY,                       XK_comma,                  focusmon,       {.i = -1 } },
	{ MODKEY,                       XK_period,                 focusmon,       {.i = +1 } },
	{ MODKEY|ShiftMask,             XK_comma,                  tagmon,         {.i = -1 } },
	{ MODKEY|ShiftMask,             XK_period,                 tagmon,         {.i = +1 } },
    { MODKEY|ShiftMask,             XK_q,                      quit,           {0} },
    { 0,                            XF86XK_MonBrightnessUp,    spawn,          {.v = light_up } },
    { 0,                            XF86XK_MonBrightnessDown,  spawn,          {.v = light_down } },
    { 0,                            XF86XK_AudioRaiseVolume,   spawn,          {.v = sink_up } },
    { 0,                            XF86XK_AudioLowerVolume,   spawn,          {.v = sink_down } },
    { 0,                            XF86XK_AudioMute,          spawn,          {.v = sink_mute } },
    { 0,                            XF86XK_AudioMicMute,       spawn,          {.v = source_mute } },
    { MODKEY|ShiftMask,             XK_l,                      spawn,          {.v = lock } },
    { MODKEY,                       XK_Print,                  spawn,          {.v = screenshot } },
    { MODKEY,                       XK_c,                      spawn,          {.v = clip_shot } },
    { MODKEY,                       XK_y,                      spawn,          {.v = set_dp } },
    { MODKEY,                       XK_asterisk,               spawn,          {.v = redshift } },
	TAGKEYS(                        XK_ampersand,              0)
	TAGKEYS(                        XK_eacute,                 1)
	TAGKEYS(                        XK_quotedbl,               2)
	TAGKEYS(                        XK_apostrophe,             3)
	TAGKEYS(                        XK_parenleft,              4)
	TAGKEYS(                        XK_minus,                  5)
	TAGKEYS(                        XK_egrave,                 6)
	TAGKEYS(                        XK_underscore,             7)
	TAGKEYS(                        XK_ccedilla,               8)
};

/* button definitions */
/* click can be ClkTagBar, ClkLtSymbol, ClkStatusText, ClkWinTitle, ClkClientWin, or ClkRootWin */
static Button buttons[] = {
	/* click                event mask      button          function        argument */
	{ ClkLtSymbol,          0,              Button1,        setlayout,      {0} },
	{ ClkLtSymbol,          0,              Button3,        setlayout,      {.v = &layouts[2]} },
	{ ClkWinTitle,          0,              Button2,        zoom,           {0} },
	{ ClkStatusText,        0,              Button2,        spawn,          {.v = termcmd } },
	{ ClkClientWin,         MODKEY,         Button1,        movemouse,      {0} },
	{ ClkClientWin,         MODKEY,         Button2,        togglefloating, {0} },
	{ ClkClientWin,         MODKEY,         Button3,        resizemouse,    {0} },
	{ ClkTagBar,            0,              Button1,        view,           {0} },
	{ ClkTagBar,            0,              Button3,        toggleview,     {0} },
	{ ClkTagBar,            MODKEY,         Button1,        tag,            {0} },
	{ ClkTagBar,            MODKEY,         Button3,        toggletag,      {0} },
};
