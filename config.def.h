/* See LICENSE file for copyright and license details. */

#include <X11/XF86keysym.h>

/* appearance */
static const unsigned int borderpx  = 1;        /* border pixel of windows */
static const unsigned int snap      = 32;       /* snap pixel */
static const int showbar            = 1;        /* 0 means no bar */
static const int topbar             = 1;        /* 0 means bottom bar */
static const char *fonts[]          = { "monospace:size=10" };
static const char dmenufont[]       = "monospace:size=10";

/*
 * Base16 dwm template by Daniel Mulford
 * Tomorrow Night scheme by Chris Kempson (http://chriskempson.com)
 */
static const char col_base00[]      = "#1d1f21";
static const char col_base01[]      = "#282a2e";
static const char col_base02[]      = "#373b41";
static const char col_base04[]      = "#b4b7b4";
static const char col_base0C[]      = "#8abeb7";
static const char col_base0D[]      = "#81a2be";
static const char *colors[][3]      = {
	/*               fg          bg          border   */
	[SchemeNorm] = { col_base04, col_base01, col_base02 },
	[SchemeSel]  = { col_base00, col_base0D, col_base0C },
};

/* tagging */
static const char *tags[] = { "1", "2", "3", "4", "5", "6", "7", "8", "9" };

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
static const float mfact     = 0.55; /* factor of master area size [0.05..0.95] */
static const int nmaster     = 1;    /* number of clients in master area */
static const int resizehints = 1;    /* 1 means respect size hints in tiled resizals */
static const int lockfullscreen = 1; /* 1 will force focus on the fullscreen window */

static const Layout layouts[] = {
	/* symbol     arrange function */
	{ "[]=",      tile },    /* first entry is default */
	{ "><>",      NULL },    /* no layout function means floating behavior */
	{ "[M]",      monocle },
};

/* key definitions */
#define MODKEY Mod1Mask
#define TAGKEYS(KEY,TAG) \
	{ MODKEY,                       KEY,      view,           {.ui = 1 << TAG} }, \
	{ MODKEY|ControlMask,           KEY,      toggleview,     {.ui = 1 << TAG} }, \
	{ MODKEY|ShiftMask,             KEY,      tag,            {.ui = 1 << TAG} }, \
	{ MODKEY|ControlMask|ShiftMask, KEY,      toggletag,      {.ui = 1 << TAG} },

/* helper for spawning shell commands in the pre dwm-5.0 fashion */
#define SHCMD(cmd) { .v = (const char*[]){ "/bin/sh", "-c", cmd, NULL } }

/* commands */
static char dmenumon[2] = "0"; /* component of dmenucmd, manipulated in spawn() */
static const char *dmenucmd[] = { "dmenu_run", "-m", dmenumon, "-fn", dmenufont, "-nb", col_base01, "-nf", col_base04, "-sb", col_base0D, "-sf", col_base00, NULL };
static const char *termcmd[]  = { "alacritty", NULL };

static Key keys[] = {
	/* modifier                     key        function        argument */
	{ MODKEY,                       XK_p,      spawn,          {.v = dmenucmd } },
	{ MODKEY|ShiftMask,             XK_Return, spawn,          {.v = termcmd } },
	{ MODKEY,                       XK_b,      togglebar,      {0} },
	{ MODKEY,                       XK_j,      focusstack,     {.i = +1 } },
	{ MODKEY,                       XK_k,      focusstack,     {.i = -1 } },
	{ MODKEY,                       XK_i,      incnmaster,     {.i = +1 } },
	{ MODKEY,                       XK_d,      incnmaster,     {.i = -1 } },
	{ MODKEY,                       XK_h,      setmfact,       {.f = -0.05} },
	{ MODKEY,                       XK_l,      setmfact,       {.f = +0.05} },
	{ MODKEY,                       XK_Return, zoom,           {0} },
	{ MODKEY,                       XK_Tab,    view,           {0} },
	{ MODKEY|ShiftMask,             XK_c,      killclient,     {0} },
	{ MODKEY,                       XK_t,      setlayout,      {.v = &layouts[0]} },
	{ MODKEY,                       XK_f,      setlayout,      {.v = &layouts[1]} },
	{ MODKEY,                       XK_m,      setlayout,      {.v = &layouts[2]} },
	{ MODKEY,                       XK_space,  setlayout,      {0} },
	{ MODKEY|ShiftMask,             XK_space,  togglefloating, {0} },
	{ MODKEY|ShiftMask,             XK_f,      togglefullscr,  {0} },
	{ MODKEY,                       XK_0,      view,           {.ui = ~0 } },
	{ MODKEY|ShiftMask,             XK_0,      tag,            {.ui = ~0 } },
	{ MODKEY,                       XK_comma,  focusmon,       {.i = -1 } },
	{ MODKEY,                       XK_period, focusmon,       {.i = +1 } },
	{ MODKEY|ShiftMask,             XK_comma,  tagmon,         {.i = -1 } },
	{ MODKEY|ShiftMask,             XK_period, tagmon,         {.i = +1 } },
	TAGKEYS(                        XK_1,                      0)
	TAGKEYS(                        XK_2,                      1)
	TAGKEYS(                        XK_3,                      2)
	TAGKEYS(                        XK_4,                      3)
	TAGKEYS(                        XK_5,                      4)
	TAGKEYS(                        XK_6,                      5)
	TAGKEYS(                        XK_7,                      6)
	TAGKEYS(                        XK_8,                      7)
	TAGKEYS(                        XK_9,                      8)
	{ MODKEY|ShiftMask,             XK_q,      quit,           {0} },

	{ 0, XF86XK_AudioLowerVolume, spawn, SHCMD("amixer set Master 5%-; kill -RTMIN+1 \"$(cat \"$XDG_CACHE_HOME/pidofbar\")\"") },
	{ 0, XF86XK_AudioMute, spawn, SHCMD("amixer set Master toggle; kill -RTMIN+1 \"$(cat \"$XDG_CACHE_HOME/pidofbar\")\"") },
	{ 0, XF86XK_AudioNext, spawn, SHCMD("playerctl next") },
	{ 0, XF86XK_AudioPlay, spawn, SHCMD("playerctl play-pause") },
	{ 0, XF86XK_AudioPrev, spawn, SHCMD("playerctl previous") },
	{ 0, XF86XK_AudioRaiseVolume, spawn, SHCMD("amixer set Master 5%+; kill -RTMIN+1 \"$(cat \"$XDG_CACHE_HOME/pidofbar\")\"") },
	{ 0, XK_Caps_Lock,spawn, SHCMD("sleep 0.2; kill -RTMIN+3 \"$(cat \"$XDG_CACHE_HOME/pidofbar\")\"") },
	{ 0, XK_Print, spawn, SHCMD("ffmpeg -f x11grab -i \"$DISPLAY\" -vframes 1 \"$XDG_PICTURES_DIR/screenshots/$(time.uuid).png\"") },
	{ MODKEY, XF86XK_AudioMute, spawn, SHCMD("amixer set Capture toggle; kill -RTMIN \"$(cat \"$XDG_CACHE_HOME/pidofbar\")\"") },
	{ MODKEY, XK_Print, spawn, SHCMD("xrectsel '%w %h %x %y' | xargs sh -c 'ffmpeg -f x11grab -s \"$1x$2\" -i \"$DISPLAY+$3,$4\" -vframes 1 \"$XDG_PICTURES_DIR/screenshots/$(time.uuid).png\"' shell") },
	{ MODKEY, XK_n, spawn, SHCMD("dunstctl close") },
	{ MODKEY, XK_r, spawn, SHCMD("xsel -ob | rev | xsel -ib") },
	{ MODKEY, XK_s, spawn, SHCMD("shufwall") },
	{ MODKEY, XK_w, spawn, SHCMD("$BROWSER") },
	{ MODKEY, XK_y, spawn, SHCMD("mpvy -s") },
	{ MODKEY|ControlMask, XK_y,	spawn, SHCMD("tsp mpvy -A -u \"$(xsel -ob)\"") },
	{ MODKEY|ShiftMask, XK_Print, spawn, SHCMD("reco") },
	{ MODKEY|ShiftMask, XK_n, spawn, SHCMD("alarm 12 'Boiling Water!'") },
	{ MODKEY|ShiftMask, XK_w, spawn, SHCMD("mullvad-exclude $BROWSER") },
	{ MODKEY|ShiftMask, XK_y, spawn, SHCMD("mpvy -a") },

	//{ MODKEY, XK_q, spawn, SHCMD("mullvad connect -w; kill -RTMIN+4 \"$(cat \"$XDG_CACHE_HOME/pidofbar\")\"; qbittorrent") },
	//{ MODKEY, XK_v, spawn, SHCMD("if mullvad status | grep -q 'Disconnect'; then mullvad connect -w; else killall qbittorrent weechat; mullvad disconnect -w; fi; kill -RTMIN+4 \"$(cat \"$XDG_CACHE_HOME/pidofbar\")\"") },
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

