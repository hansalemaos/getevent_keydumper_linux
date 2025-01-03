#include <dirent.h>
#include <errno.h>
#include <fcntl.h>
#include <getopt.h>
#include <linux/input.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/inotify.h>
#include <sys/ioctl.h>
//// #include <sys/limits.h>
#include <algorithm>
#include <array>
#include <string>
#include <fstream>
#include <cstring>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <ranges>
#include <string>
#include <sys/poll.h>
#include <unistd.h>
#include <vector>
#include <string_view>
#include <sstream>
#if __has_include(<format>)
#include <format>
#define HAS_STD_FORMAT 1
#else
#define HAS_STD_FORMAT 0
#endif

typedef struct allkeyinfos
{
    std::string filename;
    std::string devname;
    std::string name;
    std::string location;
    std::string idstr;
    int nfds;
    int id_bustype;
    int id_vendor;
    int id_product;
    int id_version;
    std::string hex_id_bustype;
    std::string hex_id_vendor;
    std::string hex_id_product;
    std::string hex_id_version;
    std::string version;
    std::string label;
    int label_int;
    char down;
    std::string bit_label_hex;
    std::string bit_label_str;
    int bit_label_int;
    int abs_value;
    int abs_minimum;
    int abs_maximum;
    int abs_fuzz;
    int abs_flat;
    int abs_resolution;
    std::string input_props;
    std::string hdi_descriptor;

} akinfos;
using sva = std::vector<allkeyinfos>;

const std::string csv_header =
    "\"filename\",\"devname\",\"name\",\"location\",\"idstr\",\"nfds\",\"id_bustype\",\"id_vendor\",\"id_product\","
    "\"id_version\",\"hex_id_bustype\",\"hex_id_vendor\",\"hex_id_product\",\"hex_id_version\",\"version\",\"label\","
    "\"label_int\",\"down\",\"bit_label_hex\",\"bit_label_str\",\"bit_label_int\",\"abs_value\",\"abs_minimum\",\"abs_"
    "maximum\",\"abs_fuzz\",\"abs_flat\",\"abs_resolution\",\"input_props\",\"hdi_descriptor\"";

static std::string dump_struct_vector_as_csv(sva &v)
{
    std::string outputstring;
    if (!v.empty())
    {
        size_t maxsize{sizeof(v[0])};
        for (const auto &ref : v)
        {
            if (sizeof(ref) > maxsize)
            {
                maxsize = sizeof(ref);
            }
        }
        outputstring.reserve(v.size() * maxsize * 2);
        outputstring.append(csv_header);
        outputstring += '\n';
    }
    else
    {
        return outputstring;
    }
    std::string delim_csv{"\",\""};
    sva::iterator it{v.begin()};
    while (it != v.end())
    {
#if __has_include(<format>)
        outputstring.append(std::format(
            "\"{}\",\"{}\",\"{}\",\"{}\",\"{}\",\"{}\",\"{}\",\"{}\",\"{}\",\"{}\",\"{}\",\"{}\",\"{}\",\"{}\",\"{}\","
            "\"{}\",\"{}\",\"{}\",\"{}\",\"{}\",\"{}\",\"{}\",\"{}\",\"{}\",\"{}\",\"{}\",\"{}\",\"{}\",\"{}\"\n",
            it->filename, it->devname, it->name, it->location, it->idstr, it->nfds, it->id_bustype, it->id_vendor,
            it->id_product, it->id_version, it->hex_id_bustype, it->hex_id_vendor, it->hex_id_product,
            it->hex_id_version, it->version, it->label, it->label_int, it->down, it->bit_label_hex, it->bit_label_str,
            it->bit_label_int, it->abs_value, it->abs_minimum, it->abs_maximum, it->abs_fuzz, it->abs_flat,
            it->abs_resolution, it->input_props, it->hdi_descriptor));
#else
        outputstring += '"';
        outputstring.append((it->filename));
        outputstring.append(delim_csv);
        outputstring.append((it->devname));
        outputstring.append(delim_csv);
        outputstring.append((it->name));
        outputstring.append(delim_csv);
        outputstring.append((it->location));
        outputstring.append(delim_csv);
        outputstring.append((it->idstr));
        outputstring.append(delim_csv);
        outputstring.append(std::to_string(it->nfds));
        outputstring.append(delim_csv);
        outputstring.append(std::to_string(it->id_bustype));
        outputstring.append(delim_csv);
        outputstring.append(std::to_string(it->id_vendor));
        outputstring.append(delim_csv);
        outputstring.append(std::to_string(it->id_product));
        outputstring.append(delim_csv);
        outputstring.append(std::to_string(it->id_version));
        outputstring.append(delim_csv);
        outputstring.append((it->hex_id_bustype));
        outputstring.append(delim_csv);
        outputstring.append((it->hex_id_vendor));
        outputstring.append(delim_csv);
        outputstring.append((it->hex_id_product));
        outputstring.append(delim_csv);
        outputstring.append((it->hex_id_version));
        outputstring.append(delim_csv);
        outputstring.append((it->version));
        outputstring.append(delim_csv);
        outputstring.append((it->label));
        outputstring.append(delim_csv);
        outputstring.append(std::to_string(it->label_int));
        outputstring.append(delim_csv);
        outputstring.append += it->down;
        outputstring.append(delim_csv);
        outputstring.append((it->bit_label_hex));
        outputstring.append(delim_csv);
        outputstring.append((it->bit_label_str));
        outputstring.append(delim_csv);
        outputstring.append(std::to_string(it->bit_label_int));
        outputstring.append(delim_csv);
        outputstring.append(std::to_string(it->abs_value));
        outputstring.append(delim_csv);
        outputstring.append(std::to_string(it->abs_minimum));
        outputstring.append(delim_csv);
        outputstring.append(std::to_string(it->abs_maximum));
        outputstring.append(delim_csv);
        outputstring.append(std::to_string(it->abs_fuzz));
        outputstring.append(delim_csv);
        outputstring.append(std::to_string(it->abs_flat));
        outputstring.append(delim_csv);
        outputstring.append(std::to_string(it->abs_resolution));
        outputstring.append(delim_csv);
        outputstring.append((it->input_props));
        outputstring.append(delim_csv);
        outputstring.append((it->hdi_descriptor));
        outputstring += '"';
        outputstring += '\n';

#endif
        it++;
    }
    return outputstring;
}

std::ostream &operator<<(std::ostream &os, sva &v)
{
    int counter = 0;
    for (auto &i : v)
    {
        os << "\n--------------------------------------------\n";
        os << "idx:            " << counter << '\n';
        os << "filename:       " << i.filename << '\n';
        os << "devname:        " << i.devname << '\n';
        os << "name:           " << i.name << '\n';
        os << "location:       " << i.location << '\n';
        os << "idstr:          " << i.idstr << '\n';
        os << "nfds:           " << i.nfds << '\n';
        os << "id_bustype:     " << i.id_bustype << '\n';
        os << "id_vendor:      " << i.id_vendor << '\n';
        os << "id_product:     " << i.id_product << '\n';
        os << "id_version:     " << i.id_version << '\n';
        os << "hex_id_bustype: " << i.hex_id_bustype << '\n';
        os << "hex_id_vendor:  " << i.hex_id_vendor << '\n';
        os << "hex_id_product: " << i.hex_id_product << '\n';
        os << "hex_id_version: " << i.hex_id_version << '\n';
        os << "version:        " << i.version << '\n';
        os << "label:          " << i.label << '\n';
        os << "label_int:      " << i.label_int << '\n';
        os << "down:           " << i.down << '\n';
        os << "bit_label_hex:  " << i.bit_label_hex << '\n';
        os << "bit_label_str:  " << i.bit_label_str << '\n';
        os << "bit_label:      " << i.bit_label_int << '\n';
        os << "abs_value:      " << i.abs_value << '\n';
        os << "abs_minimum:    " << i.abs_minimum << '\n';
        os << "abs_maximum:    " << i.abs_maximum << '\n';
        os << "abs_fuzz:       " << i.abs_fuzz << '\n';
        os << "abs_flat:       " << i.abs_flat << '\n';
        os << "abs_resolution: " << i.abs_resolution << '\n';
        os << "input_props:    " << i.input_props << '\n';
        os << "hdi_descriptor: " << i.hdi_descriptor << '\n';

        counter++;
    }
    return os;
}

struct label
{
    const char *name;
    int value;
};
#define LABEL(constant) {#constant, constant}
#define LABEL_END {NULL, -1}
static struct label input_prop_labels[] = {
    LABEL(INPUT_PROP_POINTER),
    LABEL(INPUT_PROP_DIRECT),
    LABEL(INPUT_PROP_BUTTONPAD),
    LABEL(INPUT_PROP_SEMI_MT),
    LABEL_END,
};
static struct label ev_labels[] = {
    LABEL(EV_SYN), LABEL(EV_KEY), LABEL(EV_REL), LABEL(EV_ABS), LABEL(EV_MSC),       LABEL(EV_SW), LABEL(EV_LED),
    LABEL(EV_SND), LABEL(EV_REP), LABEL(EV_FF),  LABEL(EV_PWR), LABEL(EV_FF_STATUS), LABEL_END,
};
static struct label syn_labels[] = {
    LABEL(SYN_REPORT), LABEL(SYN_CONFIG), LABEL(SYN_MT_REPORT), LABEL(SYN_DROPPED), LABEL_END,
};
static struct label key_labels[] = {
    LABEL(KEY_RESERVED),
    LABEL(KEY_ESC),
    LABEL(KEY_1),
    LABEL(KEY_2),
    LABEL(KEY_3),
    LABEL(KEY_4),
    LABEL(KEY_5),
    LABEL(KEY_6),
    LABEL(KEY_7),
    LABEL(KEY_8),
    LABEL(KEY_9),
    LABEL(KEY_0),
    LABEL(KEY_MINUS),
    LABEL(KEY_EQUAL),
    LABEL(KEY_BACKSPACE),
    LABEL(KEY_TAB),
    LABEL(KEY_Q),
    LABEL(KEY_W),
    LABEL(KEY_E),
    LABEL(KEY_R),
    LABEL(KEY_T),
    LABEL(KEY_Y),
    LABEL(KEY_U),
    LABEL(KEY_I),
    LABEL(KEY_O),
    LABEL(KEY_P),
    LABEL(KEY_LEFTBRACE),
    LABEL(KEY_RIGHTBRACE),
    LABEL(KEY_ENTER),
    LABEL(KEY_LEFTCTRL),
    LABEL(KEY_A),
    LABEL(KEY_S),
    LABEL(KEY_D),
    LABEL(KEY_F),
    LABEL(KEY_G),
    LABEL(KEY_H),
    LABEL(KEY_J),
    LABEL(KEY_K),
    LABEL(KEY_L),
    LABEL(KEY_SEMICOLON),
    LABEL(KEY_APOSTROPHE),
    LABEL(KEY_GRAVE),
    LABEL(KEY_LEFTSHIFT),
    LABEL(KEY_BACKSLASH),
    LABEL(KEY_Z),
    LABEL(KEY_X),
    LABEL(KEY_C),
    LABEL(KEY_V),
    LABEL(KEY_B),
    LABEL(KEY_N),
    LABEL(KEY_M),
    LABEL(KEY_COMMA),
    LABEL(KEY_DOT),
    LABEL(KEY_SLASH),
    LABEL(KEY_RIGHTSHIFT),
    LABEL(KEY_KPASTERISK),
    LABEL(KEY_LEFTALT),
    LABEL(KEY_SPACE),
    LABEL(KEY_CAPSLOCK),
    LABEL(KEY_F1),
    LABEL(KEY_F2),
    LABEL(KEY_F3),
    LABEL(KEY_F4),
    LABEL(KEY_F5),
    LABEL(KEY_F6),
    LABEL(KEY_F7),
    LABEL(KEY_F8),
    LABEL(KEY_F9),
    LABEL(KEY_F10),
    LABEL(KEY_NUMLOCK),
    LABEL(KEY_SCROLLLOCK),
    LABEL(KEY_KP7),
    LABEL(KEY_KP8),
    LABEL(KEY_KP9),
    LABEL(KEY_KPMINUS),
    LABEL(KEY_KP4),
    LABEL(KEY_KP5),
    LABEL(KEY_KP6),
    LABEL(KEY_KPPLUS),
    LABEL(KEY_KP1),
    LABEL(KEY_KP2),
    LABEL(KEY_KP3),
    LABEL(KEY_KP0),
    LABEL(KEY_KPDOT),
    LABEL(KEY_ZENKAKUHANKAKU),
    LABEL(KEY_102ND),
    LABEL(KEY_F11),
    LABEL(KEY_F12),
    LABEL(KEY_RO),
    LABEL(KEY_KATAKANA),
    LABEL(KEY_HIRAGANA),
    LABEL(KEY_HENKAN),
    LABEL(KEY_KATAKANAHIRAGANA),
    LABEL(KEY_MUHENKAN),
    LABEL(KEY_KPJPCOMMA),
    LABEL(KEY_KPENTER),
    LABEL(KEY_RIGHTCTRL),
    LABEL(KEY_KPSLASH),
    LABEL(KEY_SYSRQ),
    LABEL(KEY_RIGHTALT),
    LABEL(KEY_LINEFEED),
    LABEL(KEY_HOME),
    LABEL(KEY_UP),
    LABEL(KEY_PAGEUP),
    LABEL(KEY_LEFT),
    LABEL(KEY_RIGHT),
    LABEL(KEY_END),
    LABEL(KEY_DOWN),
    LABEL(KEY_PAGEDOWN),
    LABEL(KEY_INSERT),
    LABEL(KEY_DELETE),
    LABEL(KEY_MACRO),
    LABEL(KEY_MUTE),
    LABEL(KEY_VOLUMEDOWN),
    LABEL(KEY_VOLUMEUP),
    LABEL(KEY_POWER),
    LABEL(KEY_KPEQUAL),
    LABEL(KEY_KPPLUSMINUS),
    LABEL(KEY_PAUSE),
    LABEL(KEY_SCALE),
    LABEL(KEY_KPCOMMA),
    LABEL(KEY_HANGEUL),
    LABEL(KEY_HANGUEL),
    LABEL(KEY_HANJA),
    LABEL(KEY_YEN),
    LABEL(KEY_LEFTMETA),
    LABEL(KEY_RIGHTMETA),
    LABEL(KEY_COMPOSE),
    LABEL(KEY_STOP),
    LABEL(KEY_AGAIN),
    LABEL(KEY_PROPS),
    LABEL(KEY_UNDO),
    LABEL(KEY_FRONT),
    LABEL(KEY_COPY),
    LABEL(KEY_OPEN),
    LABEL(KEY_PASTE),
    LABEL(KEY_FIND),
    LABEL(KEY_CUT),
    LABEL(KEY_HELP),
    LABEL(KEY_MENU),
    LABEL(KEY_CALC),
    LABEL(KEY_SETUP),
    LABEL(KEY_SLEEP),
    LABEL(KEY_WAKEUP),
    LABEL(KEY_FILE),
    LABEL(KEY_SENDFILE),
    LABEL(KEY_DELETEFILE),
    LABEL(KEY_XFER),
    LABEL(KEY_PROG1),
    LABEL(KEY_PROG2),
    LABEL(KEY_WWW),
    LABEL(KEY_MSDOS),
    LABEL(KEY_COFFEE),
    LABEL(KEY_SCREENLOCK),
    LABEL(KEY_DIRECTION),
    LABEL(KEY_CYCLEWINDOWS),
    LABEL(KEY_MAIL),
    LABEL(KEY_BOOKMARKS),
    LABEL(KEY_COMPUTER),
    LABEL(KEY_BACK),
    LABEL(KEY_FORWARD),
    LABEL(KEY_CLOSECD),
    LABEL(KEY_EJECTCD),
    LABEL(KEY_EJECTCLOSECD),
    LABEL(KEY_NEXTSONG),
    LABEL(KEY_PLAYPAUSE),
    LABEL(KEY_PREVIOUSSONG),
    LABEL(KEY_STOPCD),
    LABEL(KEY_RECORD),
    LABEL(KEY_REWIND),
    LABEL(KEY_PHONE),
    LABEL(KEY_ISO),
    LABEL(KEY_CONFIG),
    LABEL(KEY_HOMEPAGE),
    LABEL(KEY_REFRESH),
    LABEL(KEY_EXIT),
    LABEL(KEY_MOVE),
    LABEL(KEY_EDIT),
    LABEL(KEY_SCROLLUP),
    LABEL(KEY_SCROLLDOWN),
    LABEL(KEY_KPLEFTPAREN),
    LABEL(KEY_KPRIGHTPAREN),
    LABEL(KEY_NEW),
    LABEL(KEY_REDO),
    LABEL(KEY_F13),
    LABEL(KEY_F14),
    LABEL(KEY_F15),
    LABEL(KEY_F16),
    LABEL(KEY_F17),
    LABEL(KEY_F18),
    LABEL(KEY_F19),
    LABEL(KEY_F20),
    LABEL(KEY_F21),
    LABEL(KEY_F22),
    LABEL(KEY_F23),
    LABEL(KEY_F24),
    LABEL(KEY_PLAYCD),
    LABEL(KEY_PAUSECD),
    LABEL(KEY_PROG3),
    LABEL(KEY_PROG4),
    LABEL(KEY_DASHBOARD),
    LABEL(KEY_SUSPEND),
    LABEL(KEY_CLOSE),
    LABEL(KEY_PLAY),
    LABEL(KEY_FASTFORWARD),
    LABEL(KEY_BASSBOOST),
    LABEL(KEY_PRINT),
    LABEL(KEY_HP),
    LABEL(KEY_CAMERA),
    LABEL(KEY_SOUND),
    LABEL(KEY_QUESTION),
    LABEL(KEY_EMAIL),
    LABEL(KEY_CHAT),
    LABEL(KEY_SEARCH),
    LABEL(KEY_CONNECT),
    LABEL(KEY_FINANCE),
    LABEL(KEY_SPORT),
    LABEL(KEY_SHOP),
    LABEL(KEY_ALTERASE),
    LABEL(KEY_CANCEL),
    LABEL(KEY_BRIGHTNESSDOWN),
    LABEL(KEY_BRIGHTNESSUP),
    LABEL(KEY_MEDIA),
    LABEL(KEY_SWITCHVIDEOMODE),
    LABEL(KEY_KBDILLUMTOGGLE),
    LABEL(KEY_KBDILLUMDOWN),
    LABEL(KEY_KBDILLUMUP),
    LABEL(KEY_SEND),
    LABEL(KEY_REPLY),
    LABEL(KEY_FORWARDMAIL),
    LABEL(KEY_SAVE),
    LABEL(KEY_DOCUMENTS),
    LABEL(KEY_BATTERY),
    LABEL(KEY_BLUETOOTH),
    LABEL(KEY_WLAN),
    LABEL(KEY_UWB),
    LABEL(KEY_UNKNOWN),
    LABEL(KEY_VIDEO_NEXT),
    LABEL(KEY_VIDEO_PREV),
    LABEL(KEY_BRIGHTNESS_CYCLE),
    LABEL(KEY_BRIGHTNESS_ZERO),
    LABEL(KEY_DISPLAY_OFF),
    LABEL(KEY_WIMAX),
    LABEL(KEY_RFKILL),
    LABEL(BTN_0),
    LABEL(BTN_1),
    LABEL(BTN_2),
    LABEL(BTN_3),
    LABEL(BTN_4),
    LABEL(BTN_5),
    LABEL(BTN_6),
    LABEL(BTN_7),
    LABEL(BTN_8),
    LABEL(BTN_9),
    LABEL(BTN_LEFT),
    LABEL(BTN_RIGHT),
    LABEL(BTN_MIDDLE),
    LABEL(BTN_SIDE),
    LABEL(BTN_EXTRA),
    LABEL(BTN_FORWARD),
    LABEL(BTN_BACK),
    LABEL(BTN_TASK),
    LABEL(BTN_JOYSTICK),
    LABEL(BTN_TRIGGER),
    LABEL(BTN_THUMB),
    LABEL(BTN_THUMB2),
    LABEL(BTN_TOP),
    LABEL(BTN_TOP2),
    LABEL(BTN_PINKIE),
    LABEL(BTN_BASE),
    LABEL(BTN_BASE2),
    LABEL(BTN_BASE3),
    LABEL(BTN_BASE4),
    LABEL(BTN_BASE5),
    LABEL(BTN_BASE6),
    LABEL(BTN_DEAD),
    LABEL(BTN_A),
    LABEL(BTN_B),
    LABEL(BTN_C),
    LABEL(BTN_X),
    LABEL(BTN_Y),
    LABEL(BTN_Z),
    LABEL(BTN_TL),
    LABEL(BTN_TR),
    LABEL(BTN_TL2),
    LABEL(BTN_TR2),
    LABEL(BTN_SELECT),
    LABEL(BTN_START),
    LABEL(BTN_MODE),
    LABEL(BTN_THUMBL),
    LABEL(BTN_THUMBR),
    LABEL(BTN_TOOL_PEN),
    LABEL(BTN_TOOL_RUBBER),
    LABEL(BTN_TOOL_BRUSH),
    LABEL(BTN_TOOL_PENCIL),
    LABEL(BTN_TOOL_AIRBRUSH),
    LABEL(BTN_TOOL_FINGER),
    LABEL(BTN_TOOL_MOUSE),
    LABEL(BTN_TOOL_LENS),
    LABEL(BTN_TOUCH),
    LABEL(BTN_STYLUS),
    LABEL(BTN_STYLUS2),
    LABEL(BTN_TOOL_DOUBLETAP),
    LABEL(BTN_TOOL_TRIPLETAP),
    LABEL(BTN_TOOL_QUADTAP),
    LABEL(BTN_GEAR_DOWN),
    LABEL(BTN_GEAR_UP),
    LABEL(KEY_OK),
    LABEL(KEY_SELECT),
    LABEL(KEY_GOTO),
    LABEL(KEY_CLEAR),
    LABEL(KEY_POWER2),
    LABEL(KEY_OPTION),
    LABEL(KEY_INFO),
    LABEL(KEY_TIME),
    LABEL(KEY_VENDOR),
    LABEL(KEY_ARCHIVE),
    LABEL(KEY_PROGRAM),
    LABEL(KEY_CHANNEL),
    LABEL(KEY_FAVORITES),
    LABEL(KEY_EPG),
    LABEL(KEY_PVR),
    LABEL(KEY_MHP),
    LABEL(KEY_LANGUAGE),
    LABEL(KEY_TITLE),
    LABEL(KEY_SUBTITLE),
    LABEL(KEY_ANGLE),
    LABEL(KEY_ZOOM),
    LABEL(KEY_MODE),
    LABEL(KEY_KEYBOARD),
    LABEL(KEY_SCREEN),
    LABEL(KEY_PC),
    LABEL(KEY_TV),
    LABEL(KEY_TV2),
    LABEL(KEY_VCR),
    LABEL(KEY_VCR2),
    LABEL(KEY_SAT),
    LABEL(KEY_SAT2),
    LABEL(KEY_CD),
    LABEL(KEY_TAPE),
    LABEL(KEY_RADIO),
    LABEL(KEY_TUNER),
    LABEL(KEY_PLAYER),
    LABEL(KEY_TEXT),
    LABEL(KEY_DVD),
    LABEL(KEY_AUX),
    LABEL(KEY_MP3),
    LABEL(KEY_AUDIO),
    LABEL(KEY_VIDEO),
    LABEL(KEY_DIRECTORY),
    LABEL(KEY_LIST),
    LABEL(KEY_MEMO),
    LABEL(KEY_CALENDAR),
    LABEL(KEY_RED),
    LABEL(KEY_GREEN),
    LABEL(KEY_YELLOW),
    LABEL(KEY_BLUE),
    LABEL(KEY_CHANNELUP),
    LABEL(KEY_CHANNELDOWN),
    LABEL(KEY_FIRST),
    LABEL(KEY_LAST),
    LABEL(KEY_AB),
    LABEL(KEY_NEXT),
    LABEL(KEY_RESTART),
    LABEL(KEY_SLOW),
    LABEL(KEY_SHUFFLE),
    LABEL(KEY_BREAK),
    LABEL(KEY_PREVIOUS),
    LABEL(KEY_DIGITS),
    LABEL(KEY_TEEN),
    LABEL(KEY_TWEN),
    LABEL(KEY_VIDEOPHONE),
    LABEL(KEY_GAMES),
    LABEL(KEY_ZOOMIN),
    LABEL(KEY_ZOOMOUT),
    LABEL(KEY_ZOOMRESET),
    LABEL(KEY_WORDPROCESSOR),
    LABEL(KEY_EDITOR),
    LABEL(KEY_SPREADSHEET),
    LABEL(KEY_GRAPHICSEDITOR),
    LABEL(KEY_PRESENTATION),
    LABEL(KEY_DATABASE),
    LABEL(KEY_NEWS),
    LABEL(KEY_VOICEMAIL),
    LABEL(KEY_ADDRESSBOOK),
    LABEL(KEY_MESSENGER),
    LABEL(KEY_DISPLAYTOGGLE),
    LABEL(KEY_SPELLCHECK),
    LABEL(KEY_LOGOFF),
    LABEL(KEY_DOLLAR),
    LABEL(KEY_EURO),
    LABEL(KEY_FRAMEBACK),
    LABEL(KEY_FRAMEFORWARD),
    LABEL(KEY_CONTEXT_MENU),
    LABEL(KEY_MEDIA_REPEAT),
    LABEL(KEY_10CHANNELSUP),
    LABEL(KEY_10CHANNELSDOWN),
    LABEL(KEY_IMAGES),
    LABEL(KEY_DEL_EOL),
    LABEL(KEY_DEL_EOS),
    LABEL(KEY_INS_LINE),
    LABEL(KEY_DEL_LINE),
    LABEL(KEY_FN),
    LABEL(KEY_FN_ESC),
    LABEL(KEY_FN_F1),
    LABEL(KEY_FN_F2),
    LABEL(KEY_FN_F3),
    LABEL(KEY_FN_F4),
    LABEL(KEY_FN_F5),
    LABEL(KEY_FN_F6),
    LABEL(KEY_FN_F7),
    LABEL(KEY_FN_F8),
    LABEL(KEY_FN_F9),
    LABEL(KEY_FN_F10),
    LABEL(KEY_FN_F11),
    LABEL(KEY_FN_F12),
    LABEL(KEY_FN_1),
    LABEL(KEY_FN_2),
    LABEL(KEY_FN_D),
    LABEL(KEY_FN_E),
    LABEL(KEY_FN_F),
    LABEL(KEY_FN_S),
    LABEL(KEY_FN_B),
    LABEL(KEY_BRL_DOT1),
    LABEL(KEY_BRL_DOT2),
    LABEL(KEY_BRL_DOT3),
    LABEL(KEY_BRL_DOT4),
    LABEL(KEY_BRL_DOT5),
    LABEL(KEY_BRL_DOT6),
    LABEL(KEY_BRL_DOT7),
    LABEL(KEY_BRL_DOT8),
    LABEL(KEY_BRL_DOT9),
    LABEL(KEY_BRL_DOT10),
    LABEL(KEY_NUMERIC_0),
    LABEL(KEY_NUMERIC_1),
    LABEL(KEY_NUMERIC_2),
    LABEL(KEY_NUMERIC_3),
    LABEL(KEY_NUMERIC_4),
    LABEL(KEY_NUMERIC_5),
    LABEL(KEY_NUMERIC_6),
    LABEL(KEY_NUMERIC_7),
    LABEL(KEY_NUMERIC_8),
    LABEL(KEY_NUMERIC_9),
    LABEL(KEY_NUMERIC_STAR),
    LABEL(KEY_NUMERIC_POUND),
    LABEL(KEY_CAMERA_FOCUS),
    LABEL(KEY_WPS_BUTTON),
    LABEL(KEY_TOUCHPAD_TOGGLE),
    LABEL(KEY_TOUCHPAD_ON),
    LABEL(KEY_TOUCHPAD_OFF),
    LABEL(KEY_CAMERA_ZOOMIN),
    LABEL(KEY_CAMERA_ZOOMOUT),
    LABEL(KEY_CAMERA_UP),
    LABEL(KEY_CAMERA_DOWN),
    LABEL(KEY_CAMERA_LEFT),
    LABEL(KEY_CAMERA_RIGHT),
    LABEL(BTN_TRIGGER_HAPPY1),
    LABEL(BTN_TRIGGER_HAPPY2),
    LABEL(BTN_TRIGGER_HAPPY3),
    LABEL(BTN_TRIGGER_HAPPY4),
    LABEL(BTN_TRIGGER_HAPPY5),
    LABEL(BTN_TRIGGER_HAPPY6),
    LABEL(BTN_TRIGGER_HAPPY7),
    LABEL(BTN_TRIGGER_HAPPY8),
    LABEL(BTN_TRIGGER_HAPPY9),
    LABEL(BTN_TRIGGER_HAPPY10),
    LABEL(BTN_TRIGGER_HAPPY11),
    LABEL(BTN_TRIGGER_HAPPY12),
    LABEL(BTN_TRIGGER_HAPPY13),
    LABEL(BTN_TRIGGER_HAPPY14),
    LABEL(BTN_TRIGGER_HAPPY15),
    LABEL(BTN_TRIGGER_HAPPY16),
    LABEL(BTN_TRIGGER_HAPPY17),
    LABEL(BTN_TRIGGER_HAPPY18),
    LABEL(BTN_TRIGGER_HAPPY19),
    LABEL(BTN_TRIGGER_HAPPY20),
    LABEL(BTN_TRIGGER_HAPPY21),
    LABEL(BTN_TRIGGER_HAPPY22),
    LABEL(BTN_TRIGGER_HAPPY23),
    LABEL(BTN_TRIGGER_HAPPY24),
    LABEL(BTN_TRIGGER_HAPPY25),
    LABEL(BTN_TRIGGER_HAPPY26),
    LABEL(BTN_TRIGGER_HAPPY27),
    LABEL(BTN_TRIGGER_HAPPY28),
    LABEL(BTN_TRIGGER_HAPPY29),
    LABEL(BTN_TRIGGER_HAPPY30),
    LABEL(BTN_TRIGGER_HAPPY31),
    LABEL(BTN_TRIGGER_HAPPY32),
    LABEL(BTN_TRIGGER_HAPPY33),
    LABEL(BTN_TRIGGER_HAPPY34),
    LABEL(BTN_TRIGGER_HAPPY35),
    LABEL(BTN_TRIGGER_HAPPY36),
    LABEL(BTN_TRIGGER_HAPPY37),
    LABEL(BTN_TRIGGER_HAPPY38),
    LABEL(BTN_TRIGGER_HAPPY39),
    LABEL(BTN_TRIGGER_HAPPY40),
    LABEL_END,
};
static struct label rel_labels[] = {
    LABEL(REL_X),      LABEL(REL_Y),    LABEL(REL_Z),     LABEL(REL_RX),   LABEL(REL_RY), LABEL(REL_RZ),
    LABEL(REL_HWHEEL), LABEL(REL_DIAL), LABEL(REL_WHEEL), LABEL(REL_MISC), LABEL_END,
};
static struct label abs_labels[] = {
    LABEL(ABS_X),
    LABEL(ABS_Y),
    LABEL(ABS_Z),
    LABEL(ABS_RX),
    LABEL(ABS_RY),
    LABEL(ABS_RZ),
    LABEL(ABS_THROTTLE),
    LABEL(ABS_RUDDER),
    LABEL(ABS_WHEEL),
    LABEL(ABS_GAS),
    LABEL(ABS_BRAKE),
    LABEL(ABS_HAT0X),
    LABEL(ABS_HAT0Y),
    LABEL(ABS_HAT1X),
    LABEL(ABS_HAT1Y),
    LABEL(ABS_HAT2X),
    LABEL(ABS_HAT2Y),
    LABEL(ABS_HAT3X),
    LABEL(ABS_HAT3Y),
    LABEL(ABS_PRESSURE),
    LABEL(ABS_DISTANCE),
    LABEL(ABS_TILT_X),
    LABEL(ABS_TILT_Y),
    LABEL(ABS_TOOL_WIDTH),
    LABEL(ABS_VOLUME),
    LABEL(ABS_MISC),
    LABEL(ABS_MT_SLOT),
    LABEL(ABS_MT_TOUCH_MAJOR),
    LABEL(ABS_MT_TOUCH_MINOR),
    LABEL(ABS_MT_WIDTH_MAJOR),
    LABEL(ABS_MT_WIDTH_MINOR),
    LABEL(ABS_MT_ORIENTATION),
    LABEL(ABS_MT_POSITION_X),
    LABEL(ABS_MT_POSITION_Y),
    LABEL(ABS_MT_TOOL_TYPE),
    LABEL(ABS_MT_BLOB_ID),
    LABEL(ABS_MT_TRACKING_ID),
    LABEL(ABS_MT_PRESSURE),
    LABEL(ABS_MT_DISTANCE),
    LABEL_END,
};
static struct label sw_labels[] = {
    LABEL(SW_LID),
    LABEL(SW_TABLET_MODE),
    LABEL(SW_HEADPHONE_INSERT),
    LABEL(SW_RFKILL_ALL),
    LABEL(SW_RADIO),
    LABEL(SW_MICROPHONE_INSERT),
    LABEL(SW_DOCK),
    LABEL(SW_LINEOUT_INSERT),
    LABEL(SW_JACK_PHYSICAL_INSERT),
    LABEL(SW_VIDEOOUT_INSERT),
    LABEL(SW_CAMERA_LENS_COVER),
    LABEL(SW_KEYPAD_SLIDE),
    LABEL(SW_FRONT_PROXIMITY),
    LABEL(SW_ROTATE_LOCK),
    LABEL_END,
};
static struct label msc_labels[] = {
    LABEL(MSC_SERIAL), LABEL(MSC_PULSELED), LABEL(MSC_GESTURE), LABEL(MSC_RAW), LABEL(MSC_SCAN), LABEL_END,
};
static struct label led_labels[] = {
    LABEL(LED_NUML),    LABEL(LED_CAPSL), LABEL(LED_SCROLLL), LABEL(LED_COMPOSE), LABEL(LED_KANA),     LABEL(LED_SLEEP),
    LABEL(LED_SUSPEND), LABEL(LED_MUTE),  LABEL(LED_MISC),    LABEL(LED_MAIL),    LABEL(LED_CHARGING), LABEL_END,
};
static struct label rep_labels[] = {
    LABEL(REP_DELAY),
    LABEL(REP_PERIOD),
    LABEL_END,
};
static struct label snd_labels[] = {
    LABEL(SND_CLICK),
    LABEL(SND_BELL),
    LABEL(SND_TONE),
    LABEL_END,
};
static struct label id_labels[] = {
    LABEL(ID_BUS), LABEL(ID_VENDOR), LABEL(ID_PRODUCT), LABEL(ID_VERSION), LABEL_END,
};
static struct label bus_labels[] = {
    LABEL(BUS_PCI),      LABEL(BUS_ISAPNP),  LABEL(BUS_USB),   LABEL(BUS_HIL),   LABEL(BUS_BLUETOOTH),
    LABEL(BUS_VIRTUAL),  LABEL(BUS_ISA),     LABEL(BUS_I8042), LABEL(BUS_XTKBD), LABEL(BUS_RS232),
    LABEL(BUS_GAMEPORT), LABEL(BUS_PARPORT), LABEL(BUS_AMIGA), LABEL(BUS_ADB),   LABEL(BUS_I2C),
    LABEL(BUS_HOST),     LABEL(BUS_GSC),     LABEL(BUS_ATARI), LABEL(BUS_SPI),   LABEL_END,
};
static struct label mt_tool_labels[] = {
    LABEL(MT_TOOL_FINGER),
    LABEL(MT_TOOL_PEN),
    LABEL(MT_TOOL_MAX),
    LABEL_END,
};
static struct label ff_status_labels[] = {
    LABEL(FF_STATUS_STOPPED),
    LABEL(FF_STATUS_PLAYING),
    LABEL(FF_STATUS_MAX),
    LABEL_END,
};
static struct label ff_labels[] = {
    LABEL(FF_RUMBLE),     LABEL(FF_PERIODIC), LABEL(FF_CONSTANT), LABEL(FF_SPRING), LABEL(FF_FRICTION),
    LABEL(FF_DAMPER),     LABEL(FF_INERTIA),  LABEL(FF_RAMP),     LABEL(FF_SQUARE), LABEL(FF_TRIANGLE),
    LABEL(FF_SINE),       LABEL(FF_SAW_UP),   LABEL(FF_SAW_DOWN), LABEL(FF_CUSTOM), LABEL(FF_GAIN),
    LABEL(FF_AUTOCENTER), LABEL_END,
};
static struct label key_value_labels[] = {
    {"UP", 0},
    {"DOWN", 1},
    {"REPEAT", 2},
    LABEL_END,
};

static struct pollfd *ufds;
static char **device_names;
static int nfds;
enum
{
    PRINT_DEVICE_ERRORS = 1U << 0,
    PRINT_DEVICE = 1U << 1,
    PRINT_DEVICE_NAME = 1U << 2,
    PRINT_DEVICE_INFO = 1U << 3,
    PRINT_VERSION = 1U << 4,
    PRINT_POSSIBLE_EVENTS = 1U << 5,
    PRINT_INPUT_PROPS = 1U << 6,
    PRINT_HID_DESCRIPTOR = 1U << 7,
    PRINT_ALL_INFO = (1U << 8) - 1,
    PRINT_LABELS = 1U << 16,
};
static const char *get_label(const struct label *labels, int value)
{
    while (labels->name && value != labels->value)
    {
        labels++;
    }
    return labels->name;
}

template <typename T> std::string int_to_hex(T i)
{
    std::stringstream stream;
    stream << std::setfill('0') << std::setw(4) << std::hex << i;
    return stream.str();
}

static int print_input_props(int fd, std::string &inpropis)
{
    uint8_t bits[INPUT_PROP_CNT / 8];
    int i, j;
    int res;
    int count;
    const char *bit_label_int;
    // printf("  input props:\n");
    res = ioctl(fd, EVIOCGPROP(sizeof(bits)), bits);
    if (res < 0)
    {
        // printf("    <not available\n");
        return 1;
    }
    count = 0;
    for (i = 0; i < res; i++)
    {
        for (j = 0; j < 8; j++)
        {
            if (bits[i] & 1 << j)
            {
                bit_label_int = get_label(input_prop_labels, i * 8 + j);
                if (bit_label_int)
                {
                    // printf("    %s\n", bit_label_int);
                    inpropis += bit_label_int;
                    inpropis += ' ';
                }

                else
                {
                    // printf("    %04x\n", i * 8 + j);
                    inpropis += int_to_hex(i * 8 + j);
                    inpropis += ' ';
                }
                count++;
            }
        }
    }
    if (!count)
    {
        // printf("    <none>\n");
        inpropis += "<none>\n";
    }
    inpropis.pop_back();
    return 0;
}

static std::vector<allkeyinfos> print_possible_events(int fd, int print_flags, allkeyinfos activekeyinfo)
{
    std::vector<allkeyinfos> allresultsthisdevice;
    allresultsthisdevice.reserve(EV_MAX - EV_KEY + 1);
    uint8_t *bits = NULL;
    ssize_t bits_size = 0;
    const char *label;
    int i, j, k;
    int res, res2;
    struct label *bit_labels;
    const char *bit_label_int;
    // printf("  events:\n");
    for (i = EV_KEY; i <= EV_MAX; i++)
    { // skip EV_SYN since we cannot query its available codes
        int count = 0;
        while (1)
        {
            res = ioctl(fd, EVIOCGBIT(i, bits_size), bits);
            if (res < bits_size)
                break;
            bits_size = res + 16;
            bits = (uint8_t *)realloc(bits, bits_size * 2);
            if (bits == NULL)
            {
                fprintf(stderr, "failed to allocate buffer of size %d\n", (int)bits_size);
                return allresultsthisdevice;
            }
        }
        res2 = 0;
        switch (i)
        {
        case EV_KEY:
            res2 = ioctl(fd, EVIOCGKEY(res), bits + bits_size);
            label = "KEY";
            bit_labels = key_labels;
            break;
        case EV_REL:
            label = "REL";
            bit_labels = rel_labels;
            break;
        case EV_ABS:
            label = "ABS";
            bit_labels = abs_labels;
            break;
        case EV_MSC:
            label = "MSC";
            bit_labels = msc_labels;
            break;
        case EV_LED:
            res2 = ioctl(fd, EVIOCGLED(res), bits + bits_size);
            label = "LED";
            bit_labels = led_labels;
            break;
        case EV_SND:
            res2 = ioctl(fd, EVIOCGSND(res), bits + bits_size);
            label = "SND";
            bit_labels = snd_labels;
            break;
        case EV_SW:
            res2 = ioctl(fd, EVIOCGSW(bits_size), bits + bits_size);
            label = "SW ";
            bit_labels = sw_labels;
            break;
        case EV_REP:
            label = "REP";
            bit_labels = rep_labels;
            break;
        case EV_FF:
            label = "FF ";
            bit_labels = ff_labels;
            break;
        case EV_PWR:
            label = "PWR";
            bit_labels = NULL;
            break;
        case EV_FF_STATUS:
            label = "FFS";
            bit_labels = ff_status_labels;
            break;
        default:
            res2 = 0;
            label = "???";
            bit_labels = NULL;
        }
        for (j = 0; j < res; j++)
        {
            for (k = 0; k < 8; k++)
                if (bits[j] & 1 << k)
                {
                    allresultsthisdevice.emplace_back(activekeyinfo);
                    allresultsthisdevice.back().label = label;
                    allresultsthisdevice.back().label_int = i;

                    char down;
                    if (j < res2 && (bits[j + bits_size] & 1 << k))
                    {
                        down = '*';
                    }
                    else
                    {
                        down = ' ';
                    }
                    allresultsthisdevice.back().down = down;
                    // if (count == 0)
                    //{
                    // printf("    %s (%04x):", label, i);
                    //}
                    // else if ((count & (print_flags & PRINT_LABELS ? 0x3 : 0x7)) == 0 || i == EV_ABS)
                    //{
                    // printf("\n               ");
                    //}
                    // if (bit_labels && (print_flags & PRINT_LABELS))
                    //{
                    bit_label_int = get_label(bit_labels, j * 8 + k);
                    allresultsthisdevice.back().bit_label_int = j * 8 + k;
                    allresultsthisdevice.back().bit_label_hex = int_to_hex(j * 8 + k);
                    if (bit_label_int)
                    {
                        // printf(" %.20s%c%*s", bit_label_int, down, 20 - (int)strlen(bit_label_int), "");
                        allresultsthisdevice.back().bit_label_str = bit_label_int;
                    }

                    // else
                    // {
                    // printf(" %04x%c                ", j * 8 + k, down);
                    //}
                    //}
                    // else
                    //{
                    //    printf(" %04x%c", j * 8 + k, down);
                    //    allresultsthisdevice.back().bit_label_int = j * 8 + k;
                    //    allresultsthisdevice.back().bit_label_hex = int_to_hex(j * 8 + k);
                    //}
                    if (i == EV_ABS)
                    {
                        struct input_absinfo abs;
                        if (ioctl(fd, EVIOCGABS(j * 8 + k), &abs) == 0)
                        {
                            // printf(" : value %d, min %d, max %d, fuzz %d, flat %d, resolution %d", abs.value,
                            //        abs.minimum, abs.maximum, abs.fuzz, abs.flat, abs.resolution);
                            allresultsthisdevice.back().abs_value = abs.value;
                            allresultsthisdevice.back().abs_minimum = abs.minimum;
                            allresultsthisdevice.back().abs_maximum = abs.maximum;
                            allresultsthisdevice.back().abs_fuzz = abs.fuzz;
                            allresultsthisdevice.back().abs_flat = abs.flat;
                            allresultsthisdevice.back().abs_resolution = abs.resolution;
                        }
                    }
                    count++;
                }
        }
        // if (count)
        //     printf("\n");
    }
    free(bits);
    return allresultsthisdevice;
}
static void print_event(int type, int code, int value, int print_flags)
{
    const char *type_label, *code_label, *value_label;
    if (print_flags & PRINT_LABELS)
    {
        type_label = get_label(ev_labels, type);
        code_label = NULL;
        value_label = NULL;
        switch (type)
        {
        case EV_SYN:
            code_label = get_label(syn_labels, code);
            break;
        case EV_KEY:
            code_label = get_label(key_labels, code);
            value_label = get_label(key_value_labels, value);
            break;
        case EV_REL:
            code_label = get_label(rel_labels, code);
            break;
        case EV_ABS:
            code_label = get_label(abs_labels, code);
            switch (code)
            {
            case ABS_MT_TOOL_TYPE:
                value_label = get_label(mt_tool_labels, value);
            }
            break;
        case EV_MSC:
            code_label = get_label(msc_labels, code);
            break;
        case EV_LED:
            code_label = get_label(led_labels, code);
            break;
        case EV_SND:
            code_label = get_label(snd_labels, code);
            break;
        case EV_SW:
            code_label = get_label(sw_labels, code);
            break;
        case EV_REP:
            code_label = get_label(rep_labels, code);
            break;
        case EV_FF:
            code_label = get_label(ff_labels, code);
            break;
        case EV_FF_STATUS:
            code_label = get_label(ff_status_labels, code);
            break;
        }
        if (type_label)
            printf("%-12.12s", type_label);
        else
            printf("%04x        ", type);
        if (code_label)
            printf(" %-20.20s", code_label);
        else
            printf(" %04x                ", code);
        if (value_label)
            printf(" %-20.20s", value_label);
        else
            printf(" %08x            ", value);
    }
    else
    {
        printf("%04x %04x %08x", type, code, value);
    }
}
static void print_hid_descriptor(int bus, int vendor, int product, std::string &hiddescriptstri)
{
    const char *dirname = "/sys/kernel/debug/hid";
    char prefix[16];
    DIR *dir;
    struct dirent *de;
    char filename[PATH_MAX];
    FILE *file;
    char line[2048];
    snprintf(prefix, sizeof(prefix), "%04X:%04X:%04X.", bus, vendor, product);
    dir = opendir(dirname);
    if (dir == NULL)
        return;
    while ((de = readdir(dir)))
    {
        if (strstr(de->d_name, prefix) == de->d_name)
        {
            snprintf(filename, sizeof(filename), "%s/%s/rdesc", dirname, de->d_name);
            file = fopen(filename, "r");
            if (file)
            {
                // printf("  HID descriptor: %s\n\n", de->d_name);
                while (fgets(line, sizeof(line), file))
                {
                    // fputs("    ", stdout);
                    // fputs(line, stdout);
                    hiddescriptstri += line;
                }
                fclose(file);
                // puts("");
            }
        }
    }
    closedir(dir);
}

static int open_device(const char *device, int print_flags, sva &vecresults)
{
    int version;
    int fd;
    struct pollfd *new_ufds;
    char **new_device_names;
    char name[80];
    char location[80];
    char idstr[80];
    struct input_id id;
    allkeyinfos infx;
    // if (vecresults != nullptr)
    //{
    //     infx = *(allkeyinfos *)vecresults;
    // }
    // else
    //{
    //     infx = allkeyinfos();
    // }
    fd = open(device, O_RDWR);
    // if (fd < 0)
    //{
    //     fd = open(device, O_RDONLY);
    // }
    if (fd < 0)
    {
        if (print_flags & PRINT_DEVICE_ERRORS)
            fprintf(stderr, "could not open %s, %s\n", device, strerror(errno));
        return -1;
    }

    if (ioctl(fd, EVIOCGVERSION, &version))
    {
        if (print_flags & PRINT_DEVICE_ERRORS)
            fprintf(stderr, "could not get driver version for %s, %s\n", device, strerror(errno));
        return -1;
    }
    if (ioctl(fd, EVIOCGID, &id))
    {
        if (print_flags & PRINT_DEVICE_ERRORS)
            fprintf(stderr, "could not get driver id for %s, %s\n", device, strerror(errno));
        return -1;
    }
    name[sizeof(name) - 1] = '\0';
    location[sizeof(location) - 1] = '\0';
    idstr[sizeof(idstr) - 1] = '\0';

    if (ioctl(fd, EVIOCGNAME(sizeof(name) - 1), &name) < 1)
    {
        fprintf(stderr, "could not get device name for %s, %s\n", device, strerror(errno));
        name[0] = '\0';
    }
    if (ioctl(fd, EVIOCGPHYS(sizeof(location) - 1), &location) < 1)
    {
        fprintf(stderr, "could not get location for %s, %s\n", device, strerror(errno));
        location[0] = '\0';
    }
    if (ioctl(fd, EVIOCGUNIQ(sizeof(idstr) - 1), &idstr) < 1)
    {
        fprintf(stderr, "could not get idstring for %s, %s\n", device, strerror(errno));
        idstr[0] = '\0';
    }
    vecresults.back().name = name;
    vecresults.back().location = location;
    vecresults.back().idstr = idstr;

    new_ufds = (struct pollfd *)realloc(ufds, sizeof(ufds[0]) * (nfds + 1));
    if (new_ufds == NULL)
    {
        fprintf(stderr, "out of memory\n");
        return -1;
    }
    ufds = new_ufds;
    new_device_names = (char **)realloc(device_names, sizeof(device_names[0]) * (nfds + 1));
    if (new_device_names == NULL)
    {
        fprintf(stderr, "out of memory\n");
        return -1;
    }
    device_names = new_device_names;
    // printf("add device %d: %s\n", nfds, device);
    vecresults.back().nfds = nfds;

    // printf("  bus:      %04x\n"
    //        "  vendor    %04x\n"
    //        "  product   %04x\n"
    //        "  version   %04x\n",
    //        id.bustype, id.vendor, id.product, id.version);

    vecresults.back().id_bustype = id.bustype;
    vecresults.back().id_vendor = id.vendor;
    vecresults.back().id_product = id.product;
    vecresults.back().id_version = id.version;
    vecresults.back().hex_id_bustype = int_to_hex(id.bustype);
    vecresults.back().hex_id_vendor = int_to_hex(id.vendor);
    vecresults.back().hex_id_product = int_to_hex(id.product);
    vecresults.back().hex_id_version = int_to_hex(id.version);

    // printf("  name:     \"%s\"\n", name);
    // printf("  location: \"%s\"\n"
    //        "  id:       \"%s\"\n",
    //        location, idstr);
    // printf("  version:  %d.%d.%d\n", version >> 16, (version >> 8) & 0xff, version & 0xff);
    char versiontmpchar[64]{};
    int writtenbtyes{snprintf(versiontmpchar, 64, "%d.%d.%d", version >> 16, (version >> 8) & 0xff, version & 0xff)};
    if (writtenbtyes > 0)
    {
        vecresults.back().version = versiontmpchar;
    }

    allkeyinfos activekeyinfo = vecresults.back();

    // if (print_flags & PRINT_INPUT_PROPS)

    // std::cout << "print_input_props----------------------------\n";
    print_input_props(fd, activekeyinfo.input_props);
    // std::cout << "print_input_props<<<<<<<<<<<<<<<<<<<<<<<<<<<<\n";

    // if (print_flags & PRINT_HID_DESCRIPTOR)

    // std::cout << "print_hid_descriptor----------------------------\n";
    print_hid_descriptor(id.bustype, id.vendor, id.product, activekeyinfo.hdi_descriptor);
    // std::cout << "print_hid_descriptor<<<<<<<<<<<<<<<<<<<<<<<<<<\n";

    auto _tmpresultvec = print_possible_events(fd, print_flags, activekeyinfo);

    vecresults.pop_back();
    for (auto &i : _tmpresultvec)
    {
        vecresults.emplace_back(i);
    }

    ufds[nfds].fd = fd;
    ufds[nfds].events = POLLIN;
    device_names[nfds] = strdup(device);
    nfds++;
    return 0;
}
int close_device(const char *device, int print_flags)
{
    int i;
    for (i = 1; i < nfds; i++)
    {
        if (strcmp(device_names[i], device) == 0)
        {
            int count = nfds - i - 1;
            // if (print_flags & PRINT_DEVICE)
            //  printf("remove device %d: %s\n", i, device);
            free(device_names[i]);
            memmove(device_names + i, device_names + i + 1, sizeof(device_names[0]) * count);
            memmove(ufds + i, ufds + i + 1, sizeof(ufds[0]) * count);
            nfds--;
            return 0;
        }
    }
    if (print_flags & PRINT_DEVICE_ERRORS)
        fprintf(stderr, "remote device: %s not found\n", device);
    return -1;
}

static int scan_dir(const char *dirname, int print_flags, sva &vecresults)
{
    char devname[PATH_MAX];
    char *filename;
    DIR *dir;
    struct dirent *de;
    dir = opendir(dirname);
    if (dir == NULL)
        return -1;
    strcpy(devname, dirname);
    filename = devname + strlen(devname);
    *filename++ = '/';
    while ((de = readdir(dir)))
    {
        if (de->d_name[0] == '.' && (de->d_name[1] == '\0' || (de->d_name[1] == '.' && de->d_name[2] == '\0')))
            continue;
        strcpy(filename, de->d_name);
        // std::cout << "filename C++: " << filename << std::endl;
        // std::cout << "devname C++: " << devname << std::endl;

        vecresults.emplace_back(allkeyinfos{});
        size_t vecsize = vecresults.size();
        vecresults.at(vecsize - 1).filename = filename;
        vecresults.at(vecsize - 1).devname = devname;

        open_device(devname, print_flags, vecresults);
    }
    closedir(dir);
    return 0;
}

static void usage(int argc, char *argv[])
{
}

int main(int argc, char *argv[])
{
    int c;
    int i;
    int res;
    int pollres;
    int get_time = 0;
    int print_device = 0;
    char _newline = '\n';
    char *newline = &_newline;
    const char *_empty = "";
    uint16_t get_switch = 0;
    struct input_event event;
    int version;
    int print_flags = 0;
    int print_flags_set = 0;
    int dont_block = -1;
    int event_count = 0;
    int sync_rate = 0;
    int64_t last_sync_time = 0;
    const char *device = NULL;
    const char *device_path = "/dev/input";
    print_flags |= PRINT_DEVICE_ERRORS | PRINT_DEVICE | PRINT_DEVICE_NAME | PRINT_POSSIBLE_EVENTS | PRINT_INPUT_PROPS;
    print_flags_set = 1;
    nfds = 1;
    ufds = (struct pollfd *)calloc(1, sizeof(ufds[0]));
    ufds[0].fd = inotify_init();
    ufds[0].events = POLLIN;
    // std::cout << "print flags: " << ufds << '\n';
    res = inotify_add_watch(ufds[0].fd, device_path, IN_DELETE | IN_CREATE);
    if (res < 0)
    {
        fprintf(stderr, "could not add watch for %s, %s\n", device_path, strerror(errno));
        return 1;
    }
    std::vector<allkeyinfos> vecresults;
    res = scan_dir(device_path, print_flags, vecresults);
    // std::cout << vecresults;
    std::cout << dump_struct_vector_as_csv(vecresults);
    return 0;
}
