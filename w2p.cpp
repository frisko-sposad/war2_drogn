#include <windows.h>
#include <stdint.h>
#include <ctype.h>
#include <stdio.h>
#include <math.h>
#include "patch.h" //stole that file from Fois plugins sources because why not
#include "defs.h"  //some addresses and constants
#include "names.h" //some constants

byte a_custom = 0;              // custom game
byte reg[4] = {0};              // region
int *unit[1610];                // array for units
int *unitt[1610];               // temp array for units
int *capt[1610];                // array for captured units
int units = 0;                  // number of units in array
int captk = 0;                  // number of units in captured array
byte ua[255] = {255};           // attackers array
byte ut[255] = {255};           // targets array     only unit ua[i] can deal damage to unit ut[i]
byte runes[9] = {0};            // runestone special abilities flags
bool first_step = false;        // first step of trigger
bool table = false;             // show win/lose table
bool agr = false;               // allied comps aggro if attacked
bool cpt = false;               // can buildings be captured on low hp
bool pcpt = false;              // only peons can capture low hp build
bool thcpt = false;             // if th captured on low hp, capture all units of that player
bool ucpt = false;              // units capture
bool steal = false;             // if can steal resources
bool aport = false;             // only allies can teleport in portal
bool mport = false;             // portal can teleport only if have mage(or dk) nearby
bool b3rune = false;            // can build runestone
bool b3port = false;            // can build portal
bool b3cirl = false;            // can build circle of power
bool b3mine = false;            // can build gold mine
bool b3forest = false;          // can build forest
bool apn = false;               // can build attack peons
bool manaburn = false;          // if demons have manaburn
byte heros[16] = {0};           // heroes ids
bool herosb[16] = {false};      // heroes buildable ids
char hhero[180] = {0};          // buttons for build human heroes array
char ohero[180] = {0};          // buttons for build orc heroes array
bool A_runestone = false;       // runestone activated
bool A_portal = false;          // portals activated
bool A_transport = false;       // transport activated
bool A_autoheal = false;        // paladins autoheal activated
bool blood_f = false;           // blood fixed
bool more_res = false;          // can get more resources
bool path_fixed = false;        // path fix
bool ai_fixed = false;          // ai fix
bool peon_steal = false;        // peons can steal from other peons
bool saveload_fixed = false;    // saveload ai break bug fix
byte m_slow_aura[255] = {255};  // units that have slow aura
byte m_death_aura[255] = {255}; // units that have death aura
byte m_sneak[255] = {255};      // units that have hide ability
byte m_devotion[255] = {255};   // units that have defence aura
byte m_vampire[255] = {255};    // units that have vampire aura
byte m_prvnt[255] = {255};      // units that have prevent loss

byte all_kills[16 * 16 * U_OWALL] = {0};

void *tbl_credits;
void *tbl_end;
void *tbl_task1;
void *tbl_task2;
void *tbl_task3;
void *tbl_task4;
void *tbl_task5;
void *tbl_task6;
void *tbl_task7;
char tbl_task_secret[] = "Kill 5000 orcs!";
void *tbl_task8;

void *tbl_title1;
void *tbl_title2;
void *tbl_title3;
void *tbl_title4;
void *tbl_title5;
void *tbl_title6;
void *tbl_title7;
void *tbl_title8;
void *tbl_title9;

void *tbl_name1;
void *tbl_name2;
void *tbl_name3;
void *tbl_name4;
void *tbl_name5;
void *tbl_name6;
void *tbl_name7;
void *tbl_name8;
void *tbl_name9;
void *tbl_name10;
void *tbl_name11;

void *tbl_brif1;
void *tbl_brif2;
void *tbl_brif3;
void *tbl_brif4;
void *tbl_brif5;
void *tbl_brif6;
void *tbl_brif7;
char tbl_brif_secret[] = "%!^@#(&> !*@&$ &$)()!*# ((#*$ {}!@ #))( #|}!@# (*$&*?><!$  !>!>}!@ |}!#<#@? !@#%|#@%} !#@!<> |!@$|}|";
char tbl_brif8[] = " ";

// название наций при победе или поражении
void *tbl_nations;

// units
void *grg_hugh;
void *grg_hugh_icon;
void *grg_pirat;
void *grg_old_lothar;
void *grg_castellan;
void *grg_foot_shield;

// icons
void *grp_drogn_icons;

// sound
void *def_name = NULL;
void *def_sound = NULL;
void *def_name_seq = NULL;
void *def_sound_seq = NULL;

char hugh1_name[] = "Drogn\\speech\\hugh\\hughpisd1.wav\x0";
void *hugh1_sound = NULL;
char hugh2_name[] = "Drogn\\speech\\hugh\\hughpisd2.wav\x0";
void *hugh2_sound = NULL;
char hugh3_name[] = "Drogn\\speech\\hugh\\hughpisd3.wav\x0";
void *hugh3_sound = NULL;
char hugh4_name[] = "Drogn\\speech\\hugh\\hughwhat1.wav\x0";
void *hugh4_sound = NULL;
char hugh5_name[] = "Drogn\\speech\\hugh\\hughwhat2.wav\x0";
void *hugh5_sound = NULL;
char hugh6_name[] = "Drogn\\speech\\hugh\\hughwhat3.wav\x0";
void *hugh6_sound = NULL;
char hugh7_name[] = "Drogn\\speech\\hugh\\hughyessr1.wav\x0";
void *hugh7_sound = NULL;
char hugh8_name[] = "Drogn\\speech\\hugh\\hughyessr2.wav\x0";
void *hugh8_sound = NULL;
char hugh9_name[] = "Drogn\\speech\\hugh\\hughyessr3.wav\x0";
void *hugh9_sound = NULL;
char *hugh_names[] = {hugh1_name, hugh2_name, hugh3_name, hugh4_name, hugh5_name, hugh6_name, hugh7_name, hugh8_name, hugh9_name};
void *hugh_sounds[] = {hugh1_sound, hugh2_sound, hugh3_sound, hugh4_sound, hugh5_sound, hugh6_sound, hugh7_sound, hugh8_sound, hugh9_sound};
bool w_sounds_e = true;

struct Vizs
{
    byte x = 0;
    byte y = 0;
    byte p = 0;
    byte s = 0;
};
Vizs vizs_areas[2000];
int vizs_n = 0;

char sheep_build[] = "\x0\x0\x73\x0\xf0\x40\x44\x0\xd0\xe6\x40\x0\x1\x39\x28\x1\x0\x0\x0\x0";
char demon_build[] = "\x0\x0\x25\x0\xf0\x40\x44\x0\xd0\xe6\x40\x0\x1\x38\x18\x1\x0\x0\x0\x0";
char build_3[] = "\x0\x0\x52\x0\xf0\x40\x44\x0\x80\x6b\x43\x0\x0\x66\x31\x1\x0\x0\x0\x0\x1\x0\x50\x0\xf0\x40\x44\x0\x80\x6b\x43\x0\x0\x65\x3e\x1\x0\x0\x0\x0\x2\x0\x51\x0\xf0\x40\x44\x0\x80\x6b\x43\x0\x0\x64\x46\x1\x0\x0\x0\x0\x3\x0\x4A\x0\xf0\x40\x44\x0\x80\x6b\x43\x0\x0\x5c\x38\x1\x0\x0\x0\x0\x4\x0\x67\x0\xf0\x40\x44\x0\x80\x6b\x43\x0\x0\x5c\x91\x1\x0\x0\x0\x0\x8\x0\x5b\x0\xf0\x40\x44\x0\x70\xa6\x44\x0\x0\x6e\x97\x1\x0\x0\x0\x0";
char peon_build3[] = "\x0\x0\x54\x0\xf0\x40\x44\x0\x90\x66\x43\x0\x0\x0\x88\x1\x2\x0\x0\x0\x1\x0\xa7\x0\xf0\x40\x44\x0\xf0\x61\x43\x0\x0\x0\x89\x1\x1\x0\x0\x0\x2\x0\x77\x0\xf0\x40\x44\x0\x40\x69\x43\x0\x0\x0\x8a\x1\x8\x0\x0\x0\x3\x0\x55\x0\xf0\x40\x44\x0\x70\x6a\x43\x0\x0\x0\x8b\x1\x0\x1\x0\x0\x4\x0\x56\x0\x60\x51\x44\x0\x10\x6a\x43\x0\x0\x0\x8c\x1\x40\x0\x0\x0\x5\x0\x5a\x0\xa0\x51\x44\x0\x30\x6b\x43\x0\x0\x0\x91\x1\x80\x0\x0\x0\x6\x0\x57\x0\xe0\x4d\x44\x0\x70\xa6\x44\x0\x0\x71\x8e\x1\x0\x0\x0\x0\x7\x0\x58\x0\x50\x50\x44\x0\x70\xa6\x44\x0\x0\x72\x8f\x1\x0\x0\x0\x0\x8\x0\x6d\x0\xf0\x40\x44\x0\x70\xa6\x44\x0\x0\x6d\x38\x1\x0\x0\x0\x0";
char oth_build[] = "\x0\x0\x1\x0\xa0\x44\x44\x0\xd0\xe6\x40\x0\x1\x3\xd\x1\x0\x0\x0\x0\x1\x0\x43\x0\xb0\x42\x44\x0\x10\xe7\x40\x0\x0\x59\x3e\x1\x0\x0\x0\x0\x1\x0\x45\x0\xb0\x42\x44\x0\x10\xe7\x40\x0\x1\x5b\x3c\x1\x0\x0\x0\x0\x2\x0\x4f\x0\xf0\x40\x44\x0\x70\xa6\x44\x0\x0\x6c\x90\x1\x0\x0\x0\x0\x3\x0\xb5\x0\xf0\x40\x44\x0\xd0\xe6\x40\x0\x1\x11\x8a\x1\x0\x0\x0\x0";
char hth_build[] = "\x2\x0\x4f\x0\xf0\x40\x44\x0\x70\xa6\x44\x0\x0\x6b\x90\x1\x0\x0\x0\x0\x3\x0\xb4\x0\xf0\x40\x44\x0\xd0\xe6\x40\x0\x1\x10\x8a\x1\x0\x0\x0\x0";
char churc[] = "\x0\x0\xa\x0\xc0\x43\x44\x0\x90\xe6\x40\x0\x14\x21\x72\x1\x0\x0\x0\x0\x0\x0\x6b\x0\x10\x44\x44\x0\x90\xe6\x40\x0\x1\x23\x81\x1\x0\x0\x0\x0\x1\x0\x6e\x0\x10\x44\x44\x0\x90\xe6\x40\x0\x3\x24\x83\x1\x0\x0\x0\x0\x3\x0\x6d\x0\xf0\x40\x44\x0\xf0\x40\x44\x0\x0\x0\x6c\x1\x0\x0\x0\x0";

void show_message(byte time, char *text)
{
    ((void (*)(char *, int, int))F_MAP_MSG)(text, 15, time * 10); // original war2 show msg func
}

void clear_chat()
{
    for (int i = 0; i < 12; i++)
    {
        int *p = (int *)MULTPL_CHAT;
        p = (int *)(*((int *)((uintptr_t)p)));
        p -= 100 / 4 * i;
        char buf[] = "\x0"; // just set 0 in all messages to empty
        PATCH_SET((char *)p, buf);
    }
}

int get_val(int adress, int player)
{
    return (int)(*(WORD *)(adress + player * 2)); // player*2 cause all vals is WORD
}

bool cmp_args(byte m, byte v, byte c)
{ // compare bytes
    bool f = false;
    switch (m)
    {
    case CMP_EQ:
        f = (v == c);
        break;
    case CMP_NEQ:
        f = (v != c);
        break;
    case CMP_BIGGER_EQ:
        f = (v >= c);
        break;
    case CMP_SMALLER_EQ:
        f = (v <= c);
        break;
    case CMP_BIGGER:
        f = (v > c);
        break;
    case CMP_SMALLER:
        f = (v < c);
        break;
    default:
        f = false;
        break;
    }
    return f;
}

bool cmp_args2(byte m, WORD v, WORD c)
{ // compare words
    bool f = false;
    switch (m)
    {
    case CMP_EQ:
        f = (v == c);
        break;
    case CMP_NEQ:
        f = (v != c);
        break;
    case CMP_BIGGER_EQ:
        f = (v >= c);
        break;
    case CMP_SMALLER_EQ:
        f = (v <= c);
        break;
    case CMP_BIGGER:
        f = (v > c);
        break;
    case CMP_SMALLER:
        f = (v < c);
        break;
    default:
        f = false;
        break;
    }
    return f;
}

bool cmp_args4(byte m, int v, int c)
{ // comapre 4 bytes (for resources)
    bool f = false;
    switch (m)
    {
    case CMP_EQ:
        f = (v == c);
        break;
    case CMP_NEQ:
        f = (v != c);
        break;
    case CMP_BIGGER_EQ:
        f = (v >= c);
        break;
    case CMP_SMALLER_EQ:
        f = (v <= c);
        break;
    case CMP_BIGGER:
        f = (v > c);
        break;
    case CMP_SMALLER:
        f = (v < c);
        break;
    default:
        f = false;
        break;
    }
    return f;
}

void lose(bool t)
{
    if (t == true)
    {
        char buf[] = "\x0"; // if need to show table
        PATCH_SET((char *)LOSE_SHOW_TABLE, buf);
    }
    else
    {
        char buf[] = "\x3b";
        PATCH_SET((char *)LOSE_SHOW_TABLE, buf);
    }
    if (!first_step)
    {
        char l[] = "\x2";
        PATCH_SET((char *)(ENDGAME_STATE + (*(byte *)LOCAL_PLAYER)), l);
        ((void (*)())F_LOSE)(); // original lose func
    }
    else
    {
        patch_setdword((DWORD *)0x004C0D38, (DWORD)F_LOSE);
    }
}

void win(bool t)
{
    if (t == true)
    {
        char buf[] = "\xEB"; // if need to show table
        PATCH_SET((char *)WIN_SHOW_TABLE, buf);
    }
    else
    {
        char buf[] = "\x74";
        PATCH_SET((char *)WIN_SHOW_TABLE, buf);
    }
    if (!first_step)
    {
        char l[] = "\x3";
        PATCH_SET((char *)(ENDGAME_STATE + (*(byte *)LOCAL_PLAYER)), l);
        ((void (*)())F_WIN)(); // original win func
    }
    else
    {
        patch_setdword((DWORD *)0x004C0D38, (DWORD)F_WIN);
    }
}

void lose2(bool t, byte vid)
{
    lose(t);
    ((void (*)(int, byte))F_VIDEO)(vid, 1); // original war2 func that show video by id
}

void win2(bool t, byte vid)
{
    win(t);
    ((void (*)(int, byte))F_VIDEO)(vid, 1); // original war2 func that show video by id
}

bool check_ally(byte p1, byte p2)
{
    // check allied table
    return ((*(byte *)(ALLY + p1 + 16 * p2) != 0) && (*(byte *)(ALLY + p2 + 16 * p1) != 0));
}

bool check_av(byte p1, byte p2)
{
    if (check_ally(p1, p2))
    {
        byte av1 = *(byte *)AVED; // AV byte
        byte av2 = av1;
        av1 = av1 & (1 << p1);
        av2 = av2 & (1 << p2);
        return ((av1 != 0) && (av2 != 0));
    }
    return false;
}

void tile_remove_trees(int x, int y)
{
    ((void (*)(int, int))F_TILE_REMOVE_TREES)(x, y);
}

void tile_remove_rocks(int x, int y)
{
    ((void (*)(int, int))F_TILE_REMOVE_ROCKS)(x, y);
}

void tile_remove_walls(int x, int y)
{
    ((void (*)(int, int))F_TILE_REMOVE_WALLS)(x, y);
}

byte get_all_kills(byte id, byte pla, byte plt)
{
    if ((id <= U_OWALL) && (pla <= 15) && (plt <= 15))
        return all_kills[pla * 16 * U_OWALL + plt * U_OWALL + id];
    else
        return 0;
}

bool stat_byte(byte s)
{ // chech if unit stat is 1 or 2 byte
    bool f = (s == S_DRAW_X) || (s == S_DRAW_Y) || (s == S_X) || (s == S_Y) || (s == S_HP) || (s == S_INVIZ) || (s == S_SHIELD) || (s == S_BLOOD) || (s == S_HASTE) || (s == S_AI_SPELLS) || (s == S_NEXT_FIRE) || (s == S_LAST_HARVEST_X) || (s == S_LAST_HARVEST_Y) || (s == S_BUILD_PROGRES) || (s == S_BUILD_PROGRES_TOTAL) || (s == S_RESOURCES) || (s == S_ORDER_X) || (s == S_ORDER_Y) || (s == S_RETARGET_X1) || (s == S_RETARGET_Y1) || (s == S_RETARGET_X2) || (s == S_RETARGET_Y2);
    return !f;
}

bool cmp_stat(int *p, int v, byte pr, byte cmp)
{
    // p - unit
    // v - value
    // pr - property
    // cmp - compare method
    bool f = false;
    if (stat_byte(pr))
    {
        byte ob = v % 256;
        char buf[3] = {0};
        buf[0] = ob;
        buf[1] = *((byte *)((uintptr_t)p + pr));
        if (cmp_args(cmp, buf[1], buf[0]))
        {
            f = true;
        }
    }
    else
    {
        if (cmp_args2(cmp, *((WORD *)((uintptr_t)p + pr)), (WORD)v))
        {
            f = true;
        }
    }
    return f;
}

void set_stat(int *p, int v, byte pr)
{
    if (stat_byte(pr))
    {
        char buf[] = "\x0";
        buf[0] = v % 256;
        PATCH_SET((char *)((uintptr_t)p + pr), buf);
    }
    else
    {
        char buf[] = "\x0\x0";
        buf[0] = v % 256;
        buf[1] = (v / 256) % 256;
        PATCH_SET((char *)((uintptr_t)p + pr), buf);
    }
}

int get_stat(int *p, byte pr)
{
    int r = 0;
    if (stat_byte(pr))
    {
        r = *((byte *)((uintptr_t)p + pr));
    }
    else
    {
        r = *((WORD *)((uintptr_t)p + pr));
    }
    return r;
}

void gamesound_play(WORD s, byte x, byte y)
{
    DWORD xy = x + 256 * 256 * y;
    ((void (*)(WORD, DWORD, byte, byte))F_GAMESOUND_XY)(s, xy, 1, 1);
}

void bullet_create(WORD x, WORD y, byte id)
{
    int *b = ((int *(*)(WORD, WORD, byte))F_BULLET_CREATE)(x, y, id);
    if (b)
    {
        if ((id == B_LIGHT_FIRE) || (id == B_HEAVY_FIRE))
        {
            char buf[] = "\x0";
            buf[0] = 5;                                  // bullet action
            PATCH_SET((char *)((uintptr_t)b + 54), buf); // 54 bullet action
            buf[0] = 1;                                  // bullet info
            PATCH_SET((char *)((uintptr_t)b + 58), buf); // 58 bullet user info
            buf[0] = 4;                                  // bullet flags
            PATCH_SET((char *)((uintptr_t)b + 53), buf); // 53 bullet flags
            buf[0] = 80;                                 // ticks
            PATCH_SET((char *)((uintptr_t)b + 56), buf); // 56 bullet life (WORD)
        }
    }
    // all moving - move to -16 -16 relative
    // 0 -  move and die (anim ok)
    // 1 -  crash when hit (no owner?)
    // 2 -  1
    // 3 -  crash (no target?)
    // 4 -  3
    // 5 -  1
    // 6 -  anim ok
    // 7 -  anim ok + sound (better just use 26)
    // 8 -  anim ok
    // 9 -  anim ok
    // 10 - anim ok
    // 11 - anim ok
    // 12 - 1
    // 13 - anim ok + sound (better just use 21)
    // 14 - 13
    // 15 - no crash but insta dies
    // 16 - 15
    // 17 - anim ok + sound (better just use 23)
    // 18 - 17
    // 19 - anim ok but prob infinitie life
    // 20 - 19
    // 21 - anim ok + sound
    // 22 - anim ok
    // 23 - anim ok no sound
    // 24 - anim ok + sound (better just use 26)
    // 25 - anim ok
    // 26 - anim ok + sound
    // 27 - anim ok
    // 28 - anim ok
}

void bullet_disp_fires()
{
    int *p = (int *)BULLETS_MASSIVE; // pointer to bullets
    p = (int *)(*p);
    int k = *(int *)BULLETS_NUMBER; // unit number
    while (k > 0)
    {
        bool f = (((*((byte *)((uintptr_t)p + 52)) == B_LIGHT_FIRE) || (*((byte *)((uintptr_t)p + 52)) == B_HEAVY_FIRE)) // both fires
                  && (*((byte *)((uintptr_t)p + 58)) == 1));                                                             // user info
        if (f)
        {
            byte life = *((byte *)((uintptr_t)p + 56));
            if (life > 0)
                life--;
            char buf[] = "\x0";
            buf[0] = life;
            PATCH_SET((char *)((uintptr_t)p + 56), buf); // 56 bullet life (WORD)
            if (life == 0)
            {
                buf[0] = 1;
                PATCH_SET((char *)((uintptr_t)p + 53), buf); // 53 bullet flags
            }
            else
            {
                buf[0] = 4;
                PATCH_SET((char *)((uintptr_t)p + 53), buf); // 53 bullet flags
            }
        }
        p = (int *)((int)p + 0x40);
        k--;
    }
}

void bullet_create_unit(int *u, byte b)
{
    WORD x = *((WORD *)((uintptr_t)u + S_DRAW_X));
    WORD y = *((WORD *)((uintptr_t)u + S_DRAW_Y));
    bullet_create(x + 16, y + 16, b);
}

void bullet_create8_around_unit(int *u, byte b)
{
    WORD ux = *((WORD *)((uintptr_t)u + S_DRAW_X));
    WORD uy = *((WORD *)((uintptr_t)u + S_DRAW_Y));
    WORD x = ux + 16;
    WORD y = uy + 16;
    if ((b == B_LIGHT_FIRE) || (b == B_HEAVY_FIRE))
        y -= 8;
    if ((b == B_LIGHTNING) || (b == B_COIL))
    {
        x += 16;
        y += 16;
    }
    WORD xx = x;
    WORD yy = y;
    bullet_create(xx + 48, yy, b);      // right
    bullet_create(xx, yy + 48, b);      // down
    bullet_create(xx + 32, yy + 32, b); // right down
    if (xx <= 32)
        xx = 0;
    else
        xx -= 32;
    bullet_create(xx, yy + 32, b); // left down
    if (yy <= 32)
        yy = 0;
    else
        yy -= 32;
    bullet_create(xx, yy, b); // left up
    xx = x;
    bullet_create(xx + 32, yy, b); // right up
    yy = y;
    if (xx <= 48)
        xx = 0;
    else
        xx -= 48;
    bullet_create(xx, yy, b); // left
    xx = x;
    if (yy <= 48)
        yy = 0;
    else
        yy -= 48;
    bullet_create(xx, yy, b); // up
}

void unit_convert(byte player, int who, int tounit, int a)
{
    // original war2 func converts units
    ((void (*)(byte, int, int, int))F_UNIT_CONVERT)(player, who, tounit, a);
}

void unit_create(int x, int y, int id, byte owner, byte n)
{
    while (n > 0)
    {
        n -= 1;
        int *p = (int *)(UNIT_SIZE_TABLE + 4 * id);                                // unit sizes table
        ((void (*)(int, int, int, byte, int *))F_UNIT_CREATE)(x, y, id, owner, p); // original war2 func creates unit
        // just call n times to create n units
    }
}

int *unit_create_place(int x, int y, int id, byte owner)
{
    int *p = (int *)(UNIT_SIZE_TABLE + 4 * id); // unit sizes table
    return ((int *(*)(int, int, int, byte, int *))F_UNIT_CREATE)(x, y, id, owner, p);
}

void unit_kill(int *u)
{
    ((void (*)(int *))F_UNIT_KILL)(u); // original war2 func kills unit
}

void unit_hide(int *u)
{
    byte f = *((byte *)((uintptr_t)u + S_FLAGS3));
    f |= SF_HIDDEN;
    set_stat(u, f, S_FLAGS3);
}

void unit_remove(int *u)
{
    byte f = *((byte *)((uintptr_t)u + S_FLAGS3));
    f |= SF_HIDDEN;
    set_stat(u, f, S_FLAGS3);
    unit_kill(u); // hide unit then kill
}

void unit_cast(int *u) // unit autocast
{
    ((void (*)(int *))F_AI_CAST)(u); // original war2 ai cast spells
}

void set_region(int x1, int y1, int x2, int y2)
{
    if (x1 < 0)
        x1 = 0;
    if (x1 > 127)
        x1 = 127;
    if (y1 < 0)
        y1 = 0;
    if (y1 > 127)
        y1 = 127;
    if (x2 < 0)
        x2 = 0;
    if (x2 > 127)
        x2 = 127;
    if (y2 < 0)
        y2 = 0;
    if (y2 > 127)
        y2 = 127;
    reg[0] = x1 % 256;
    reg[1] = y1 % 256;
    reg[2] = x2 % 256;
    reg[3] = y2 % 256;
}

WORD region_center()
{
    return (reg[0] + (byte)floor((double)(reg[2] - reg[0]) / 2)) + 256 * (reg[1] + (byte)floor((double)(reg[3] - reg[1]) / 2));
}

bool in_region(byte x, byte y, byte x1, byte y1, byte x2, byte y2)
{
    // dnt know why but without this big monstrous ussless code gam crash
    byte tmp;
    tmp = x % 256;
    x = tmp;
    tmp = y % 256;
    y = tmp;
    tmp = x1 % 256;
    x1 = tmp;
    tmp = y1 % 256;
    y1 = tmp;
    tmp = x2 % 256;
    x2 = tmp;
    tmp = y2 % 256;
    y2 = tmp;
    if (x < 0)
        x = 0;
    if (x > 127)
        x = 127;
    if (y < 0)
        y = 0;
    if (y > 127)
        y = 127;
    if (x1 < 0)
        x1 = 0;
    if (x1 > 127)
        x1 = 127;
    if (y1 < 0)
        y1 = 0;
    if (y1 > 127)
        y1 = 127;
    if (x2 < 0)
        x2 = 0;
    if (x2 > 127)
        x2 = 127;
    if (y2 < 0)
        y2 = 0;
    if (y2 > 127)
        y2 = 127;
    if (x2 < x1)
    {
        tmp = x1;
        x1 = x2;
        x2 = tmp;
    }
    if (y2 < y1)
    {
        tmp = y1;
        y1 = y2;
        y2 = tmp;
    }
    // just check if coords inside region
    return ((x >= x1) && (y >= y1) && (x <= x2) && (y <= y2));
}

bool check_unit_dead(int *p)
{
    bool dead = false;
    if (p)
    {
        if ((*((byte *)((uintptr_t)p + S_FLAGS3)) & (SF_DEAD | SF_DIEING | SF_UNIT_FREE)) != 0)
            dead = true;
    }
    else
        dead = true;
    return dead;
}

bool check_unit_complete(int *p) // for buildings
{
    bool f = false;
    if (p)
    {
        if ((*((byte *)((uintptr_t)p + S_FLAGS3)) & SF_COMPLETED) != 0) // flags3 last bit
            f = true;
    }
    else
        f = false;
    return f;
}

bool check_unit_hidden(int *p)
{
    bool f = false;
    if (p)
    {
        if ((*((byte *)((uintptr_t)p + S_FLAGS3)) & SF_HIDDEN) != 0) // flags3 4 bit
            f = true;
    }
    else
        f = true;
    return f;
}

bool check_unit_preplaced(int *p)
{
    bool f = false;
    if (p)
    {
        if ((*((byte *)((uintptr_t)p + S_FLAGS3)) & SF_PREPLACED) != 0) // flags3
            f = true;
    }
    else
        f = false;
    return f;
}

bool check_unit_near_death(int *p)
{
    bool dead = false;
    if (p)
    {
        if (((*((byte *)((uintptr_t)p + S_FLAGS3)) & SF_DIEING) != 0) && ((*((byte *)((uintptr_t)p + S_FLAGS3)) & (SF_DEAD | SF_UNIT_FREE)) == 0))
            dead = true;
    }
    else
        dead = true;
    return dead;
}

bool check_peon_loaded(int *p, byte r)
{
    bool f = false;
    if (p)
    {
        if (r == 0)
        {
            if (((*((byte *)((uintptr_t)p + S_PEON_FLAGS)) & PEON_LOADED) != 0) && ((*((byte *)((uintptr_t)p + S_PEON_FLAGS)) & PEON_HARVEST_GOLD) != 0))
                f = true;
        }
        if (r == 1)
        {
            if (((*((byte *)((uintptr_t)p + S_PEON_FLAGS)) & PEON_LOADED) != 0) && ((*((byte *)((uintptr_t)p + S_PEON_FLAGS)) & PEON_HARVEST_LUMBER) != 0))
                f = true;
        }
        if (r == 2)
        {
            if (((*((byte *)((uintptr_t)p + S_PEON_FLAGS)) & PEON_LOADED) != 0))
                f = true;
        }
    }
    return f;
}

void find_all_units(byte id)
{
    // CAREFUL with this function - ALL units get into massive
    // even if their memory was cleared already
    // all units by id will go in array
    units = 0;
    int *p = (int *)UNITS_MASSIVE; // pointer to units
    p = (int *)(*p);
    int k = *(int *)UNITS_NUMBER;
    while (k > 0)
    {
        bool f = *((byte *)((uintptr_t)p + S_ID)) == (byte)id;
        if (f)
        {
            unit[units] = p;
            units++;
        }
        p = (int *)((int)p + 0x98);
        k--;
    }
}

void find_all_alive_units(byte id)
{
    // all units by id will go in array
    units = 0;
    for (int i = 0; i < 16; i++)
    {
        int *p = (int *)(UNITS_LISTS + 4 * i); // pointer to units list for each player
        if (p)
        {
            p = (int *)(*p);
            while (p)
            {
                bool f = *((byte *)((uintptr_t)p + S_ID)) == (byte)id;
                if (id == ANY_BUILDING)
                    f = *((byte *)((uintptr_t)p + S_ID)) >= U_FARM; // buildings
                if (id == ANY_MEN)
                    f = *((byte *)((uintptr_t)p + S_ID)) < U_FARM; // all nonbuildings
                if (id == ANY_UNITS)
                    f = true;               // all ALL units
                if (id == ANY_BUILDING_2x2) // small buildings
                {
                    byte sz = *((byte *)UNIT_SIZE_TABLE + *((byte *)((uintptr_t)p + S_ID)) * 4);
                    f = sz == 2;
                }
                if (id == ANY_BUILDING_3x3) // med buildings
                {
                    byte sz = *((byte *)UNIT_SIZE_TABLE + *((byte *)((uintptr_t)p + S_ID)) * 4);
                    f = sz == 3;
                }
                if (id == ANY_BUILDING_4x4) // big buildings
                {
                    byte sz = *((byte *)UNIT_SIZE_TABLE + *((byte *)((uintptr_t)p + S_ID)) * 4);
                    f = sz == 4;
                }
                if (f)
                {
                    if (!check_unit_dead(p))
                    {
                        unit[units] = p;
                        units++;
                    }
                }
                p = (int *)(*((int *)((uintptr_t)p + S_NEXT_UNIT_POINTER)));
            }
        }
    }
}

void sort_complete()
{
    // only completed units stay in array
    int k = 0;
    for (int i = 0; i < units; i++)
    {
        if (check_unit_complete(unit[i]))
        {
            unitt[k] = unit[i];
            k++;
        }
    }
    units = k;
    for (int i = 0; i < units; i++)
    {
        unit[i] = unitt[i];
    }
}

void sort_in_region()
{
    // only units in region stay in array
    int k = 0;
    WORD x = 0, y = 0;
    for (int i = 0; i < units; i++)
    {
        x = *((WORD *)((uintptr_t)unit[i] + S_DRAW_X)) / 32;
        y = *((WORD *)((uintptr_t)unit[i] + S_DRAW_Y)) / 32;
        if (in_region((byte)x, (byte)y, reg[0], reg[1], reg[2], reg[3]))
        {
            unitt[k] = unit[i];
            k++;
        }
    }
    units = k;
    for (int i = 0; i < units; i++)
    {
        unit[i] = unitt[i];
    }
}

void sort_not_in_region()
{
    // only units not in region stay in array
    int k = 0;
    WORD x = 0, y = 0;
    for (int i = 0; i < units; i++)
    {
        x = *((WORD *)((uintptr_t)unit[i] + S_DRAW_X)) / 32;
        y = *((WORD *)((uintptr_t)unit[i] + S_DRAW_Y)) / 32;
        if (!in_region((byte)x, (byte)y, reg[0], reg[1], reg[2], reg[3]))
        {
            unitt[k] = unit[i];
            k++;
        }
    }
    units = k;
    for (int i = 0; i < units; i++)
    {
        unit[i] = unitt[i];
    }
}

void sort_target_in_region()
{
    // only units that have order coords in region stay in array
    int k = 0;
    byte x = 0, y = 0;
    for (int i = 0; i < units; i++)
    {
        x = *((byte *)((uintptr_t)unit[i] + S_ORDER_X));
        y = *((byte *)((uintptr_t)unit[i] + S_ORDER_Y));
        if (in_region(x, y, reg[0], reg[1], reg[2], reg[3]))
        {
            unitt[k] = unit[i];
            k++;
        }
    }
    units = k;
    for (int i = 0; i < units; i++)
    {
        unit[i] = unitt[i];
    }
}

void sort_stat(byte pr, int v, byte cmp)
{
    // only units stay in array if have property compared to value is true
    int k = 0;
    for (int i = 0; i < units; i++)
    {
        if (cmp_stat(unit[i], v, pr, cmp))
        {
            unitt[k] = unit[i];
            k++;
        }
    }
    units = k;
    for (int i = 0; i < units; i++)
    {
        unit[i] = unitt[i];
    }
}

void sort_ally(byte pl, bool a)
{
    // only units stay in array if allied to some player
    int k = 0;
    for (int i = 0; i < units; i++)
    {
        byte o = get_stat(unit[i], S_OWNER);
        if (check_ally(o, pl) == a)
        {
            unitt[k] = unit[i];
            k++;
        }
    }
    units = k;
    for (int i = 0; i < units; i++)
    {
        unit[i] = unitt[i];
    }
}

void sort_tp_flag()
{
    // if not teleported by portal
    int k = 0;
    for (int i = 0; i < units; i++)
    {
        if ((*((byte *)((uintptr_t)unit[i] + S_FLAGS3)) & SF_TELEPORT) == 0) // unused in actual game flag
        {
            unitt[k] = unit[i];
            k++;
        }
    }
    units = k;
    for (int i = 0; i < units; i++)
    {
        unit[i] = unitt[i];
    }
}

void sort_hidden()
{
    // only not hidden units stay in array
    int k = 0;
    for (int i = 0; i < units; i++)
    {
        if (!check_unit_hidden(unit[i]))
        {
            unitt[k] = unit[i];
            k++;
        }
    }
    units = k;
    for (int i = 0; i < units; i++)
    {
        unit[i] = unitt[i];
    }
}

void sort_self(int *u)
{
    // unit remove self from array
    int k = 0;
    for (int i = 0; i < units; i++)
    {
        if (!(unit[i] == u))
        {
            unitt[k] = unit[i];
            k++;
        }
    }
    units = k;
    for (int i = 0; i < units; i++)
    {
        unit[i] = unitt[i];
    }
}

void sort_full_hp()
{
    // if hp not full
    int k = 0;
    for (int i = 0; i < units; i++)
    {
        byte id = *((byte *)((uintptr_t)unit[i] + S_ID)); // unit id
        WORD mhp = *(WORD *)(UNIT_HP_TABLE + 2 * id);     // max hp
        WORD hp = *((WORD *)((uintptr_t)unit[i] + S_HP)); // unit hp
        if (hp < mhp)                                     // hp not full
        {
            unitt[k] = unit[i];
            k++;
        }
    }
    units = k;
    for (int i = 0; i < units; i++)
    {
        unit[i] = unitt[i];
    }
}

void sort_fleshy()
{
    // only fleshy units stay in array
    int k = 0;
    for (int i = 0; i < units; i++)
    {
        byte id = *((byte *)((uintptr_t)unit[i] + S_ID));            // unit id
        if ((*(int *)(UNIT_GLOBAL_FLAGS + id * 4) & IS_FLESHY) != 0) // fleshy global flag
        {
            unitt[k] = unit[i];
            k++;
        }
    }
    units = k;
    for (int i = 0; i < units; i++)
    {
        unit[i] = unitt[i];
    }
}

void sort_order_hp()
{
    // order array by hp from low to high
    for (int i = 0; i < units; i++)
    {
        int sm = i;
        for (int j = i + 1; j < units; j++)
        {
            WORD hpsm = *((WORD *)((uintptr_t)unit[sm] + S_HP)); // unit hp
            WORD hpj = *((WORD *)((uintptr_t)unit[j] + S_HP));   // unit hp
            if (hpj < hpsm)
            {
                sm = j;
            }
        }
        int *tmp = unit[i];
        unit[i] = unit[sm];
        unit[sm] = tmp;
    }
}

void sort_preplaced()
{
    int k = 0;
    for (int i = 0; i < units; i++)
    {
        if (!check_unit_preplaced(unit[i]))
        {
            unitt[k] = unit[i];
            k++;
        }
    }
    units = k;
    for (int i = 0; i < units; i++)
    {
        unit[i] = unitt[i];
    }
}

void sort_near_death()
{
    int k = 0;
    for (int i = 0; i < units; i++)
    {
        if (check_unit_near_death(unit[i]))
        {
            unitt[k] = unit[i];
            k++;
        }
    }
    units = k;
    for (int i = 0; i < units; i++)
    {
        unit[i] = unitt[i];
    }
}

void sort_attack_can_hit(int *p)
{
    // only units stay in array that *p can attack them
    int k = 0;
    for (int i = 0; i < units; i++)
    {
        int a = 0;
        a = ((int (*)(int *, int *))F_ATTACK_CAN_HIT)(p, unit[i]); // attack can hit original war2 function
        if (a != 0)
        {
            unitt[k] = unit[i];
            k++;
        }
    }
    units = k;
    for (int i = 0; i < units; i++)
    {
        unit[i] = unitt[i];
    }
}

void sort_attack_can_hit_range(int *p)
{
    // only units stay in array that *p can attack them and have passable terrain in attack range
    int k = 0;
    for (int i = 0; i < units; i++)
    {
        int a = 0;
        a = ((int (*)(int *, int *))F_ATTACK_CAN_HIT)(p, unit[i]); // attack can hit
        if (a != 0)
        {
            byte id = *((byte *)((uintptr_t)unit[i] + S_ID));
            byte szx = *(byte *)(UNIT_SIZE_TABLE + 4 * id);
            byte szy = *(byte *)(UNIT_SIZE_TABLE + 4 * id + 2);
            byte idd = *((byte *)((uintptr_t)p + S_ID));
            byte rng = *(byte *)(UNIT_RANGE_TABLE + idd);
            byte ms = *(byte *)MAP_SIZE;
            byte xx = *((byte *)((uintptr_t)unit[i] + S_X));
            byte yy = *((byte *)((uintptr_t)unit[i] + S_Y));
            if (xx < rng)
                xx = 0;
            else
                xx -= rng;
            if (yy < rng)
                yy = 0;
            else
                yy -= rng;
            byte cl = *((byte *)((uintptr_t)p + S_MOVEMENT_TYPE));       // movement type
            WORD mt = *(WORD *)(GLOBAL_MOVEMENT_TERRAIN_FLAGS + 2 * cl); // movement terrain flags

            bool f = false;
            for (int x = xx; (x < szx + xx + rng * 2 + 1) && (x < 127); x++)
            {
                for (int y = yy; (y < szy + yy + rng * 2 + 1) && (x < 127); y++)
                {
                    int aa = 1;
                    if ((cl == 0) || (cl == 3)) // land and docked transport
                    {
                        aa = ((int (*)(int, int, int))F_XY_PASSABLE)(x, y, (int)mt); // original war2 func if terrain passable with that movement type
                    }
                    if ((x % 2 == 0) && (y % 2 == 0)) // air and water
                    {
                        if ((cl == 1) || (cl == 2))
                        {
                            aa = ((int (*)(int, int, int))F_XY_PASSABLE)(x, y, (int)mt);
                        }
                    }
                    if (aa == 0)
                        f = true;
                }
            }
            if (f)
            {
                unitt[k] = unit[i];
                k++;
            }
        }
    }
    units = k;
    for (int i = 0; i < units; i++)
    {
        unit[i] = unitt[i];
    }
}

void sort_rune_near()
{
    int k = 0;
    for (int i = 0; i < units; i++)
    {
        byte x = *((byte *)((uintptr_t)unit[i] + S_X));
        byte y = *((byte *)((uintptr_t)unit[i] + S_Y));
        bool f = false;
        for (int r = 0; r < 50; r++) // max runes 50
        {
            WORD d = *(WORD *)(RUNEMAP_TIMERS + 2 * r);
            if (d != 0)
            {
                byte xx = *(byte *)(RUNEMAP_X + r);
                byte yy = *(byte *)(RUNEMAP_Y + r);
                if (xx == x)
                {
                    if (yy > y)
                    {
                        if ((yy - y) == 1)
                            f = true;
                    }
                    else
                    {
                        if ((y - yy) == 1)
                            f = true;
                    }
                }
                if (yy == y)
                {
                    if (xx > x)
                    {
                        if ((xx - x) == 1)
                            f = true;
                    }
                    else
                    {
                        if ((x - xx) == 1)
                            f = true;
                    }
                }
            }
        }
        if (!f)
        {
            unitt[k] = unit[i];
            k++;
        }
    }
    units = k;
    for (int i = 0; i < units; i++)
    {
        unit[i] = unitt[i];
    }
}

void sort_peon_loaded(byte r)
{
    int k = 0;
    for (int i = 0; i < units; i++)
    {
        if (check_peon_loaded(unit[i], r))
        {
            unitt[k] = unit[i];
            k++;
        }
    }
    units = k;
    for (int i = 0; i < units; i++)
    {
        unit[i] = unitt[i];
    }
}

void sort_peon_not_loaded(byte r)
{
    int k = 0;
    for (int i = 0; i < units; i++)
    {
        if (!check_peon_loaded(unit[i], r))
        {
            unitt[k] = unit[i];
            k++;
        }
    }
    units = k;
    for (int i = 0; i < units; i++)
    {
        unit[i] = unitt[i];
    }
}

void set_stat_all(byte pr, int v)
{
    for (int i = 0; i < units; i++)
    {
        set_stat(unit[i], v, pr); // set stat to all units in array
    }
}

void set_tp_flag(bool f)
{
    for (int i = 0; i < units; i++)
    {
        // set if unit can be teleported by portal (that flag unused in actual game)
        if (f)
            *((byte *)((uintptr_t)unit[i] + S_FLAGS3)) |= SF_TELEPORT;
        else
            *((byte *)((uintptr_t)unit[i] + S_FLAGS3)) &= ~SF_TELEPORT;
    }
}

void kill_all()
{
    for (int i = 0; i < units; i++)
    {
        unit_kill(unit[i]); // just kill all in array
    }
    units = 0;
}

void remove_all()
{
    for (int i = 0; i < units; i++)
    {
        unit_remove(unit[i]); // just kill all in array
    }
    units = 0;
}

void hide_all()
{
    for (int i = 0; i < units; i++)
    {
        unit_hide(unit[i]);
    }
}

void cast_all()
{
    for (int i = 0; i < units; i++)
    {
        unit_cast(unit[i]); // casting spells
    }
    units = 0;
}

void flame(int *p)
{
    // p - unit
    // original war2 func creates 1 flame with selected animation frame
    // flameshield have 5 flames
    ((void (*)(int *, int, int))F_CREATE_FLAME)(p, 0, 0);
    ((void (*)(int *, int, int))F_CREATE_FLAME)(p, 10, 4);
    ((void (*)(int *, int, int))F_CREATE_FLAME)(p, 20, 2);
    ((void (*)(int *, int, int))F_CREATE_FLAME)(p, 30, 3);
    ((void (*)(int *, int, int))F_CREATE_FLAME)(p, 40, 5);
}

void flame_all()
{
    for (int i = 0; i < units; i++)
    {
        flame(unit[i]);
    }
}

void damag(int *p, byte n1, byte n2)
{
    WORD hp = *((WORD *)((uintptr_t)p + S_HP)); // unit hp
    WORD n = n1 + 256 * n2;
    if (hp > n)
    {
        hp -= n;
        set_stat(p, hp, S_HP);
    }
    else
    {
        set_stat(p, 0, S_HP);
        unit_kill(p);
    }
}

void damag_by_player(int *p, byte n1, byte n2, byte pl)
{
    WORD hp = *((WORD *)((uintptr_t)p + S_HP)); // unit hp
    WORD n = n1 + 256 * n2;
    if (hp > n)
    {
        hp -= n;
        set_stat(p, hp, S_HP);
    }
    else
    {
        set_stat(p, 0, S_HP);
        unit_kill(p);
        byte id = *((byte *)((uintptr_t)p + S_ID));
        byte o = *((byte *)((uintptr_t)p + S_OWNER));
        if ((id <= U_OWALL) && (o <= 15) && (pl <= 15))
        {
            if (all_kills[pl * 16 * U_OWALL + o * U_OWALL + id] < 255)
                all_kills[pl * 16 * U_OWALL + o * U_OWALL + id]++;
        }
    }
}

void damag_all(byte n1, byte n2)
{
    for (int i = 0; i < units; i++)
    {
        damag(unit[i], n1, n2);
    }
}

void damag_all_by_player(byte n1, byte n2, byte pl)
{
    for (int i = 0; i < units; i++)
    {
        damag_by_player(unit[i], n1, n2, pl);
    }
}

void heal(int *p, byte n1, byte n2)
{
    byte id = *((byte *)((uintptr_t)p + S_ID));   // unit id
    WORD mhp = *(WORD *)(UNIT_HP_TABLE + 2 * id); // max hp
    WORD hp = *((WORD *)((uintptr_t)p + S_HP));   // unit hp
    WORD n = n1 + 256 * n2;
    if (hp < mhp)
    {
        hp += n;
        if (hp > mhp)
            hp = mhp; // canot heal more than max hp
        set_stat(p, hp, S_HP);
    }
}

void heal_all(byte n1, byte n2)
{
    for (int i = 0; i < units; i++)
    {
        heal(unit[i], n1, n2);
    }
}

void mana_regen(int *p, byte n)
{
    byte tid = *((byte *)((uintptr_t)p + S_ID));
    bool f = (tid == U_MAGE) || (tid == U_DK) || (tid == U_PALADIN) || (tid == U_OGREMAGE) ||
             (tid == U_HADGAR) || (tid == U_TERON) || (tid == U_GULDAN) ||
             (tid == U_UTER) || (tid == U_TYRALYON) || (tid == U_CHOGAL) || (tid == U_DENTARG);
    if (f)
    {
        byte mp = *((byte *)((uintptr_t)p + S_MANA)); // unit mana
        if (((int)mp + n) > 255)
            mp = 255;
        else
            mp += n;
        set_stat(p, mp, S_MANA);
    }
}

void mana_regen_all(byte n)
{
    for (int i = 0; i < units; i++)
    {
        mana_regen(unit[i], n);
    }
}

void give(int *p, byte owner)
{
    ((void (*)(int *, byte, byte))F_CAPTURE)(p, owner, 1); // original capture unit war2 func
    *(byte *)(RESCUED_UNITS + 2 * owner) -= 1;             // reset number of captured units
}

void give_all(byte o)
{
    for (int i = 0; i < units; i++)
    {
        give(unit[i], o);
    }
}

void peon_load(int *u, byte r)
{
    byte f = *((byte *)((uintptr_t)u + S_PEON_FLAGS));
    if (!(f & PEON_LOADED))
    {
        if (r == 0)
        {
            f |= PEON_LOADED;
            f |= PEON_HARVEST_GOLD;
            set_stat(u, f, S_PEON_FLAGS);
            ((void (*)(int *))F_GROUP_SET)(u);
        }
        else
        {
            f |= PEON_LOADED;
            f |= PEON_HARVEST_LUMBER;
            set_stat(u, f, S_PEON_FLAGS);
            ((void (*)(int *))F_GROUP_SET)(u);
        }
    }
}

void peon_load_all(byte r)
{
    for (int i = 0; i < units; i++)
    {
        peon_load(unit[i], r);
    }
}

void viz_area(byte x, byte y, byte pl, byte sz)
{
    int Vf = F_VISION2;
    switch (sz)
    {
    case 0:
        Vf = F_VISION2;
        break;
    case 1:
        Vf = F_VISION2;
        break;
    case 2:
        Vf = F_VISION2;
        break;
    case 3:
        Vf = F_VISION3;
        break;
    case 4:
        Vf = F_VISION4;
        break;
    case 5:
        Vf = F_VISION5;
        break;
    case 6:
        Vf = F_VISION6;
        break;
    case 7:
        Vf = F_VISION7;
        break;
    case 8:
        Vf = F_VISION8;
        break;
    case 9:
        Vf = F_VISION9;
        break;
    default:
        Vf = F_VISION2;
        break;
    }
    for (byte i = 0; i < 8; i++)
    {
        if (((1 << i) & pl) != 0)
        {
            ((void (*)(WORD, WORD, byte))Vf)(x, y, i);
        }
    }
}

void viz_area_add(byte x, byte y, byte pl, byte sz)
{
    if ((vizs_n >= 0) && (vizs_n <= 255))
    {
        vizs_areas[vizs_n].x = x;
        vizs_areas[vizs_n].y = y;
        vizs_areas[vizs_n].p = pl;
        vizs_areas[vizs_n].s = sz;
        vizs_n++;
    }
}

void viz_area_all(byte pl, byte sz)
{
    for (int i = 0; i < units; i++)
    {
        byte x = *((byte *)((uintptr_t)unit[i] + S_X));
        byte y = *((byte *)((uintptr_t)unit[i] + S_Y));
        viz_area_add(x, y, pl, sz);
    }
}

bool unit_move(byte x, byte y, int *unit)
{
    if (x < 0)
        return false;
    if (y < 0)
        return false;             // canot go negative
    byte mxs = *(byte *)MAP_SIZE; // map size
    if (x >= mxs)
        return false;
    if (y >= mxs)
        return false; // canot go outside map
    if (check_unit_hidden(unit))
        return false;                                            // if unit not hidden
    byte cl = *((byte *)((uintptr_t)unit + S_MOVEMENT_TYPE));    // movement type
    WORD mt = *(WORD *)(GLOBAL_MOVEMENT_TERRAIN_FLAGS + 2 * cl); // movement terrain flags

    int aa = 1;
    if ((cl == 0) || (cl == 3)) // land and docked transport
    {
        aa = ((int (*)(int, int, int))F_XY_PASSABLE)(x, y, (int)mt); // original war2 func if terrain passable with that movement type
    }
    if ((x % 2 == 0) && (y % 2 == 0)) // air and water
    {
        if ((cl == 1) || (cl == 2))
        {
            aa = ((int (*)(int, int, int))F_XY_PASSABLE)(x, y, (int)mt);
        }
    }
    if (aa == 0)
    {
        ((void (*)(int *))F_UNIT_UNPLACE)(unit); // unplace
        set_stat(unit, x, S_X);
        set_stat(unit, y, S_Y); // change real coords
        set_stat(unit, x * 32, S_DRAW_X);
        set_stat(unit, y * 32, S_DRAW_Y);      // change draw sprite coords
        ((void (*)(int *))F_UNIT_PLACE)(unit); // place
        return true;
    }
    return false;
}

void move_all(byte x, byte y)
{
    sort_stat(S_ID, U_FARM, CMP_SMALLER); // non buildings
    sort_stat(S_ANIMATION, 2, CMP_EQ);    // only if animation stop
    for (int i = 0; i < units; i++)
    {
        int xx = 0, yy = 0, k = 1;
        bool f = unit_move(x, y, unit[i]);
        xx--;
        while ((!f) & (k < 5)) // goes in spiral like original war2 (size 5)
        {
            while ((!f) & (yy < k))
            {
                f = unit_move(x + xx, y + yy, unit[i]);
                yy++;
            }
            while ((!f) & (xx < k))
            {
                f = unit_move(x + xx, y + yy, unit[i]);
                xx++;
            }
            while ((!f) & (yy > -k))
            {
                f = unit_move(x + xx, y + yy, unit[i]);
                yy--;
            }
            while ((!f) & (xx >= -k))
            {
                f = unit_move(x + xx, y + yy, unit[i]);
                xx--;
            }
            k++;
        }
    }
}

void give_order(int *u, byte x, byte y, byte o)
{
    byte id = *((byte *)((uintptr_t)u + S_ID));
    if (id < U_FARM)
    {
        char buf[] = "\x0";
        bool f = ((o >= ORDER_SPELL_VISION) && (o <= ORDER_SPELL_ROT));
        if (f)
        {
            buf[0] = o;
            PATCH_SET((char *)GW_ACTION_TYPE, buf);
        }
        int *tr = NULL;
        for (int i = 0; i < 16; i++)
        {
            int *p = (int *)(UNITS_LISTS + 4 * i); // pointer to units list for each player
            if (p)
            {
                p = (int *)(*p);
                while (p)
                {
                    if (p != u)
                    {
                        if (!check_unit_dead(p) && !check_unit_hidden(p))
                        {
                            byte xx = *(byte *)((uintptr_t)p + S_X);
                            byte yy = *(byte *)((uintptr_t)p + S_Y);
                            if ((abs(x - xx) <= 2) && (abs(y - yy) <= 2))
                            {
                                if (f)
                                {
                                    byte idd = *(byte *)((uintptr_t)p + S_ID);
                                    if (idd < U_FARM)
                                    {
                                        bool trf = true;
                                        if (o == ORDER_SPELL_ARMOR)
                                        {
                                            WORD ef = *(WORD *)((uintptr_t)p + S_SHIELD);
                                            trf = ef == 0;
                                        }
                                        if (o == ORDER_SPELL_BLOODLUST)
                                        {
                                            WORD ef = *(WORD *)((uintptr_t)p + S_BLOOD);
                                            trf = ef == 0;
                                        }
                                        if (o == ORDER_SPELL_HASTE)
                                        {
                                            WORD ef = *(WORD *)((uintptr_t)p + S_HASTE);
                                            trf = (ef == 0) || (ef > 0x7FFF);
                                        }
                                        if (o == ORDER_SPELL_SLOW)
                                        {
                                            WORD ef = *(WORD *)((uintptr_t)p + S_HASTE);
                                            trf = (ef == 0) || (ef <= 0x7FFF);
                                        }
                                        if (o == ORDER_SPELL_POLYMORPH)
                                        {
                                            trf = idd != U_CRITTER;
                                        }
                                        if (o == ORDER_SPELL_HEAL)
                                        {
                                            WORD mhp = *(WORD *)(UNIT_HP_TABLE + 2 * idd);
                                            WORD hp = *((WORD *)((uintptr_t)p + S_HP));
                                            trf = hp < mhp;
                                        }
                                        if (trf)
                                        {
                                            WORD efi = *(WORD *)((uintptr_t)p + S_INVIZ);
                                            trf = efi == 0;
                                        }
                                        if (trf)
                                            tr = p;
                                    }
                                }
                                else
                                    tr = p;
                            }
                        }
                    }
                    p = (int *)(*((int *)((uintptr_t)p + S_NEXT_UNIT_POINTER)));
                }
            }
        }
        bool aoe = (o == ORDER_SPELL_VISION) || (o == ORDER_SPELL_EXORCISM) || (o == ORDER_SPELL_FIREBALL) ||
                   (o == ORDER_SPELL_BLIZZARD) || (o == ORDER_SPELL_EYE) || (o == ORDER_SPELL_RAISEDEAD) ||
                   (o == ORDER_SPELL_DRAINLIFE) || (o == ORDER_SPELL_WHIRLWIND) || (o == ORDER_SPELL_RUNES) ||
                   (o == ORDER_SPELL_ROT) || (o == ORDER_MOVE) || (o == ORDER_PATROL) ||
                   (o == ORDER_ATTACK_AREA) || (o == ORDER_ATTACK_WALL) || (o == ORDER_STAND) ||
                   (o == ORDER_ATTACK_GROUND) || (o == ORDER_ATTACK_GROUND_MOVE) || (o == ORDER_DEMOLISH) ||
                   (o == ORDER_HARVEST) || (o == ORDER_RETURN) || (o == ORDER_UNLOAD_ALL) || (o == ORDER_STOP);

        if (o != ORDER_ATTACK_WALL)
        {
            int ord = *(int *)(ORDER_FUNCTIONS + 4 * o); // orders functions
            if (!aoe && (tr != NULL) && (tr != u))
                ((void (*)(int *, int, int, int *, int))F_GIVE_ORDER)(u, 0, 0, tr, ord); // original war2 order
            if (aoe)
                ((void (*)(int *, int, int, int *, int))F_GIVE_ORDER)(u, x, y, NULL, ord); // original war2 order
        }
        else
        {
            byte oru = *(byte *)((uintptr_t)u + S_ORDER);
            if (oru != ORDER_ATTACK_WALL)
            {
                int ord = *(int *)(ORDER_FUNCTIONS + 4 * ORDER_STOP);                      // orders functions
                ((void (*)(int *, int, int, int *, int))F_GIVE_ORDER)(u, 0, 0, NULL, ord); // original war2 order
            }
            set_stat(u, ORDER_ATTACK_WALL, S_NEXT_ORDER);
            set_stat(u, x, S_ORDER_X);
            set_stat(u, y, S_ORDER_Y);
        }

        if (f)
        {
            buf[0] = 0;
            PATCH_SET((char *)GW_ACTION_TYPE, buf);
        }
    }
}

void give_order_spell_target(int *u, int *t, byte o)
{
    if ((u != NULL) && (t != NULL))
    {
        byte id = *((byte *)((uintptr_t)u + S_ID));
        if (id < U_FARM)
        {
            char buf[] = "\x0";
            if ((o >= ORDER_SPELL_VISION) && (o <= ORDER_SPELL_ROT))
            {
                buf[0] = o;
                PATCH_SET((char *)GW_ACTION_TYPE, buf);

                int ord = *(int *)(ORDER_FUNCTIONS + 4 * o);                            // orders functions
                ((void (*)(int *, int, int, int *, int))F_GIVE_ORDER)(u, 0, 0, t, ord); // original war2 order

                buf[0] = 0;
                PATCH_SET((char *)GW_ACTION_TYPE, buf);
            }
        }
    }
}

void order_all(byte x, byte y, byte o)
{
    for (int i = 0; i < units; i++)
    {
        give_order(unit[i], x, y, o);
    }
}

void runestone()
{
    for (int i = 0; i < 16; i++)
    {
        int *p = (int *)(UNITS_LISTS + 4 * i);
        if (p)
        {
            p = (int *)(*p);
            while (p)
            {
                bool f = *((byte *)((uintptr_t)p + S_ID)) == U_RUNESTONE;
                if (f)
                {
                    if (!check_unit_dead(p) && check_unit_complete(p))
                    { // alive and completed runestone
                        byte x = *((byte *)((uintptr_t)p + S_X));
                        byte y = *((byte *)((uintptr_t)p + S_Y));
                        set_region((int)x - 4, (int)y - 4, (int)x + 5, (int)y + 5); // set region around myself
                        find_all_alive_units(ANY_MEN);
                        sort_in_region();
                        if (runes[8] == 1) // only allied units can can recieve bufs
                        {
                            byte o = *((byte *)((uintptr_t)p + S_OWNER));
                            for (int ui = 0; ui < 16; ui++)
                            {
                                if (!check_ally(o, ui)) // tp
                                    sort_stat(S_OWNER, ui, CMP_NEQ);
                            }
                        }
                        if (runes[0] == 1)
                            set_stat_all(S_INVIZ, (int)(*(WORD *)INVIZ_TIME)); // inviz
                        if (runes[1] == 1)
                            set_stat_all(S_SHIELD, (int)(*(WORD *)SHIELD_TIME)); // shield
                        if (runes[2] == 1)
                            set_stat_all(S_BLOOD, (int)(*(WORD *)BLOOD_TIME)); // blood
                        if (runes[3] == 1)
                            set_stat_all(S_HASTE, (int)(*(WORD *)HASTE_TIME1)); // haste
                        if (runes[4] == 1)
                            flame_all();
                        // 5 mana
                        // 6 heal
                        if (runes[7] == 1) // kill all not my owner units
                        {
                            byte o = *((byte *)((uintptr_t)p + S_OWNER));
                            sort_stat(S_OWNER, o, CMP_NEQ);
                            kill_all();
                        }
                        sort_stat(S_KILLS + 1, 0, CMP_EQ);
                        set_stat_all(S_KILLS + 1, 100);
                        if (runes[5] != 0)
                            mana_regen_all(runes[5]);
                        if (runes[6] != 0)
                            heal_all(runes[6], 0);
                    }
                }
                p = (int *)(*((int *)((uintptr_t)p + S_NEXT_UNIT_POINTER)));
            }
        }
    }
}

void portal()
{
    bool mp = true;
    for (int i = 0; i < 16; i++)
    {
        units = 0;
        int *p = (int *)(UNITS_LISTS + 4 * i);
        if (p)
        {
            p = (int *)(*p);
            int *fp = NULL;
            while (p)
            {
                bool f = *((byte *)((uintptr_t)p + S_ID)) == U_PORTAL;
                if (f)
                {
                    if (!check_unit_dead(p) && check_unit_complete(p))
                    { // alive and completed portal
                        if (!fp)
                            fp = p; // remember first portal
                        byte tx = *((byte *)((uintptr_t)p + S_X)) + 1;
                        byte ty = *((byte *)((uintptr_t)p + S_Y)) + 1; // exit point is in center of portal
                        move_all(tx, ty);                              // teleport from previous portal
                        set_tp_flag(true);
                        set_stat_all(S_NEXT_ORDER, ORDER_STOP);
                        set_stat_all(S_ORDER_X, 128);
                        set_stat_all(S_ORDER_Y, 128);
                        byte x = *((byte *)((uintptr_t)p + S_X));
                        byte y = *((byte *)((uintptr_t)p + S_Y));
                        set_region((int)x - 1, (int)y - 1, (int)x + 4, (int)y + 4); // set region around myself
                        find_all_alive_units(ANY_MEN);
                        sort_in_region();
                        if (aport)
                        {
                            byte o = *((byte *)((uintptr_t)p + S_OWNER));
                            for (int ui = 0; ui < 16; ui++)
                            {
                                if (!check_ally(o, ui)) // only allied units can tp
                                    sort_stat(S_OWNER, ui, CMP_NEQ);
                            }
                        }
                        sort_tp_flag(); // flag show if unit was not teleported
                        mp = true;
                        if (mport) // only teleport if some caster near
                        {
                            mp = false;
                            for (int ui = 0; ui < units; ui++)
                            {
                                byte uid = *((byte *)((uintptr_t)unit[ui] + S_ID));
                                if ((uid == U_MAGE) || (uid == U_DK) || (uid == U_TERON) || (uid == U_HADGAR) || (uid == U_GULDAN))
                                    mp = true; // can tp only if mage nearby (teron hadgar and guldan too)
                            }
                        }
                        if (!mp)
                            units = 0;
                        else
                        {
                            sort_stat(S_ORDER, ORDER_STOP, CMP_EQ);
                            sort_stat(S_ORDER_UNIT_POINTER, 0, CMP_EQ);
                            sort_stat(S_ORDER_UNIT_POINTER + 1, 0, CMP_EQ);
                            sort_stat(S_ORDER_UNIT_POINTER + 2, 0, CMP_EQ);
                            sort_stat(S_ORDER_UNIT_POINTER + 3, 0, CMP_EQ);
                            set_region((int)x, (int)y, (int)x + 3, (int)y + 3); // set region inside myself
                            sort_target_in_region();
                        }
                    }
                }
                p = (int *)(*((int *)((uintptr_t)p + S_NEXT_UNIT_POINTER)));
            }
            if (fp) // first portal teleports from last
            {
                byte tx = *((byte *)((uintptr_t)fp + S_X)) + 1;
                byte ty = *((byte *)((uintptr_t)fp + S_Y)) + 1;
                move_all(tx, ty);
                set_tp_flag(true);
                set_stat_all(S_NEXT_ORDER, ORDER_STOP);
                set_stat_all(S_ORDER_X, 128);
                set_stat_all(S_ORDER_Y, 128);
            }
        }
    }
    find_all_alive_units(ANY_MEN);
    set_tp_flag(false); // reset tp flags to all
}

void wharf()
{
    for (int i = 0; i < 16; i++)
    {
        int *p = (int *)(UNITS_LISTS + 4 * i);
        if (p)
        {
            p = (int *)(*p);
            while (p)
            {
                bool f = (*((byte *)((uintptr_t)p + S_ID)) == U_SHIPYARD) || (*((byte *)((uintptr_t)p + S_ID)) == U_WHARF);
                if (f)
                {
                    if (!check_unit_dead(p) && check_unit_complete(p))
                    {
                        byte x = *((byte *)((uintptr_t)p + S_X));
                        byte y = *((byte *)((uintptr_t)p + S_Y));
                        set_region((int)x - 2, (int)y - 2, (int)x + 4, (int)y + 4); // set region around myself
                        find_all_alive_units(ANY_MEN);
                        sort_in_region();
                        sort_hidden();
                        sort_stat(S_MOVEMENT_TYPE, MOV_WATER, CMP_BIGGER_EQ); // find ships - movement type >= water (2 or 3 actually(ships=2 transport=3))
                        byte o = *((byte *)((uintptr_t)p + S_OWNER));
                        for (int ui = 0; ui < 16; ui++)
                        {
                            if (!check_ally(o, ui)) // only allied ships
                                sort_stat(S_OWNER, ui, CMP_NEQ);
                        }
                        heal_all(4, 0);
                    }
                }
                p = (int *)(*((int *)((uintptr_t)p + S_NEXT_UNIT_POINTER)));
            }
        }
    }
}

void paladin()
{
    for (int ii = 0; ii < 16; ii++)
    {
        int *p = (int *)(UNITS_LISTS + 4 * ii);
        if (p)
        {
            p = (int *)(*p);
            while (p)
            {
                bool f = ((*((byte *)((uintptr_t)p + S_ID)) == U_PALADIN) ||
                          (*((byte *)((uintptr_t)p + S_ID)) == U_UTER) ||
                          (*((byte *)((uintptr_t)p + S_ID)) == U_TYRALYON));
                if (f)
                {
                    if (!check_unit_dead(p) && !check_unit_hidden(p))
                    {
                        byte o = *((byte *)((uintptr_t)p + S_OWNER));
                        if (((*(byte *)(SPELLS_LEARNED + 4 * o) & (1 << L_HEAL)) != 0) &&
                            ((*(byte *)(SPELLS_LEARNED + 4 * o) & (1 << L_GREATER_HEAL)) != 0))
                        // if player learned heal and autoheal
                        {
                            byte x = *((byte *)((uintptr_t)p + S_X));
                            byte y = *((byte *)((uintptr_t)p + S_Y));
                            set_region((int)x - 5, (int)y - 5, (int)x + 5, (int)y + 5); // set region around myself
                            find_all_alive_units(ANY_MEN);
                            sort_in_region();
                            sort_hidden();
                            sort_fleshy();   // fleshy units (not heal cata and ships)
                            sort_full_hp();  // if unit hp not full
                            sort_self(p);    // not heal self
                            sort_order_hp(); // heal lovest hp first
                            for (int ui = 0; ui < 16; ui++)
                            {
                                if (!check_ally(o, ui)) // only allied units
                                    sort_stat(S_OWNER, ui, 1);
                            }
                            byte cost = *(byte *)(MANACOST + 2 * GREATER_HEAL); // 2* cause manacost is WORD
                            for (int i = 0; i < units; i++)
                            {
                                byte mp = *((byte *)((uintptr_t)p + S_MANA)); // paladin mp
                                if (mp >= cost)
                                {
                                    byte id = *((byte *)((uintptr_t)unit[i] + S_ID)); // unit id
                                    WORD mhp = *(WORD *)(UNIT_HP_TABLE + 2 * id);     // max hp
                                    WORD hp = *((WORD *)((uintptr_t)unit[i] + S_HP)); // unit hp
                                    WORD shp = mhp - hp;                              // shortage of hp
                                    while (!(mp >= (shp * cost)) && (shp > 0))
                                        shp -= 1;
                                    if (shp > 0) // if can heal at least 1 hp
                                    {
                                        heal(unit[i], (byte)shp, 0);
                                        mp -= shp * cost;
                                        *((byte *)((uintptr_t)p + S_MANA)) = mp;
                                        WORD xx = *((WORD *)((uintptr_t)unit[i] + S_DRAW_X));
                                        WORD yy = *((WORD *)((uintptr_t)unit[i] + S_DRAW_Y));
                                        ((void (*)(WORD, WORD, byte))F_BULLET_CREATE)(xx + 16, yy + 16, B_HEAL);   // create heal effect
                                        ((void (*)(WORD, WORD, byte))F_SPELL_SOUND_XY)(xx + 16, yy + 16, SS_HEAL); // heal sound
                                    }
                                }
                                else
                                    i = units;
                            }
                        }
                    }
                }
                p = (int *)(*((int *)((uintptr_t)p + S_NEXT_UNIT_POINTER)));
            }
        }
    }
}

void transport()
{
    for (int i = 0; i < 16; i++)
    {
        int *p = (int *)(UNITS_LISTS + 4 * i);
        if (p)
        {
            p = (int *)(*p);
            while (p)
            {
                bool f = (*((byte *)((uintptr_t)p + S_ID)) == U_HTRANSPORT) || (*((byte *)((uintptr_t)p + S_ID)) == U_OTRANSPORT);
                if (f)
                {
                    if (!check_unit_dead(p) && (cmp_stat(p, ANIM_STOP, S_ANIMATION, CMP_EQ))) // if transport stop
                    {
                        byte x = *((byte *)((uintptr_t)p + S_X));
                        byte y = *((byte *)((uintptr_t)p + S_Y));
                        byte o = *((byte *)((uintptr_t)p + S_OWNER));
                        for (byte ui = 0; ui < 16; ui++)
                        {
                            set_region((int)x - 1, (int)y - 1, (int)x + 1, (int)y + 1); // set region around myself
                            find_all_alive_units(ANY_MEN);
                            sort_in_region();
                            sort_hidden();
                            sort_stat(S_MOVEMENT_TYPE, MOV_LAND, CMP_EQ);
                            sort_stat(S_ORDER, ORDER_STOP, CMP_EQ);
                            sort_stat(S_ANIMATION, ANIM_STOP, CMP_EQ);
                            sort_stat(S_OWNER, ui, CMP_EQ);
                            bool f = false;
                            if ((*(byte *)(CONTROLER_TYPE + o) == C_PLAYER))
                            {
                                f = true;
                                if ((*(byte *)(CONTROLER_TYPE + ui) == C_COMP))
                                {
                                    if (!check_ally(o, ui)) // only allied comps
                                        sort_stat(S_OWNER, ui, CMP_NEQ);
                                }
                            }
                            if ((*(byte *)(CONTROLER_TYPE + o) == C_COMP))
                            {
                                f = true;
                                if ((*(byte *)(CONTROLER_TYPE + ui) == C_COMP))
                                {
                                    if (!check_ally(o, ui) || (ui == o)) // only allied comps
                                        sort_stat(S_OWNER, ui, CMP_NEQ);
                                }
                                if ((*(byte *)(CONTROLER_TYPE + ui) == C_PLAYER))
                                {
                                    if (!check_ally(o, ui)) // only allied players
                                    {
                                        sort_stat(S_OWNER, ui, CMP_NEQ);
                                    }
                                    sort_stat(S_ORDER_UNIT_POINTER, 0, CMP_EQ);
                                    sort_stat(S_ORDER_UNIT_POINTER + 1, 0, CMP_EQ);
                                    sort_stat(S_ORDER_UNIT_POINTER + 2, 0, CMP_EQ);
                                    sort_stat(S_ORDER_UNIT_POINTER + 3, 0, CMP_EQ);
                                    set_region((int)x, (int)y, (int)x, (int)y); // set region inside myself
                                    sort_target_in_region();
                                }
                            }
                            if (f)
                            {
                                sort_stat(S_KILLS + 1, 0, CMP_EQ);
                                set_stat_all(S_KILLS + 1, 100);
                                set_stat_all(S_NEXT_ORDER, ORDER_ENTER_TRANSPORT);
                                set_stat_all(S_ORDER_UNIT_POINTER, ((int)p) % 256);
                                set_stat_all(S_ORDER_UNIT_POINTER + 1, (((int)p) / 256) % 256);
                                set_stat_all(S_ORDER_UNIT_POINTER + 2, ((((int)p) / 256) / 256) % 256);
                                set_stat_all(S_ORDER_UNIT_POINTER + 3, (((((int)p) / 256) / 256) / 256) % 256);
                            }
                        }
                    }
                }
                p = (int *)(*((int *)((uintptr_t)p + S_NEXT_UNIT_POINTER)));
            }
        }
    }
}

void slow_aura(byte id)
{
    for (int i = 0; i < 16; i++)
    {
        int *p = (int *)(UNITS_LISTS + 4 * i);
        if (p)
        {
            p = (int *)(*p);
            while (p)
            {
                bool f = (*((byte *)((uintptr_t)p + S_ID)) == id);
                if (f)
                {
                    if (!check_unit_dead(p))
                    {
                        byte x = *((byte *)((uintptr_t)p + S_X));
                        byte y = *((byte *)((uintptr_t)p + S_Y));
                        set_region((int)x - 5, (int)y - 5, (int)x + 5, (int)y + 5); // set region around myself
                        find_all_alive_units(ANY_MEN);
                        sort_in_region();
                        byte o = *((byte *)((uintptr_t)p + S_OWNER));
                        for (int ui = 0; ui < 16; ui++)
                        {
                            if (check_ally(o, ui)) // only enemies
                                sort_stat(S_OWNER, ui, CMP_NEQ);
                        }
                        set_stat_all(S_HASTE, 0xfcdf); //-800
                    }
                }
                p = (int *)(*((int *)((uintptr_t)p + S_NEXT_UNIT_POINTER)));
            }
        }
    }
}

void death_aura(byte id)
{
    for (int i = 0; i < 16; i++)
    {
        int *p = (int *)(UNITS_LISTS + 4 * i);
        if (p)
        {
            p = (int *)(*p);
            while (p)
            {
                bool f = (*((byte *)((uintptr_t)p + S_ID)) == id);
                if (f)
                {
                    if (!check_unit_dead(p))
                    {
                        byte mp = *((byte *)((uintptr_t)p + S_MANA));
                        byte x = *((byte *)((uintptr_t)p + S_X));
                        byte y = *((byte *)((uintptr_t)p + S_Y));
                        byte xx = *((byte *)((uintptr_t)p + S_ORDER_X));
                        byte yy = *((byte *)((uintptr_t)p + S_ORDER_Y));
                        set_stat(p, 255, S_MANA);
                        set_stat(p, x, S_ORDER_X);
                        set_stat(p, y, S_ORDER_Y);
                        char buf[] = "\x90\x90\x90\x90\x90\x90\x90\x90";
                        PATCH_SET((char *)RAISE_DEAD_DOING_SPELL1, buf);
                        char buf3[] = "\x90\x90\x90";
                        PATCH_SET((char *)RAISE_DEAD_DOING_SPELL2, buf3);
                        ((void (*)(int *))F_RAISE_DEAD)(p);
                        char buf2[] = "\x6a\x2\x53\xe8\x9f\x3\x1\x0";
                        PATCH_SET((char *)RAISE_DEAD_DOING_SPELL1, buf2);
                        char buf4[] = "\x83\xc4\x8";
                        PATCH_SET((char *)RAISE_DEAD_DOING_SPELL2, buf4);
                        set_stat(p, mp, S_MANA);
                        set_stat(p, xx, S_ORDER_X);
                        set_stat(p, yy, S_ORDER_Y);
                    }
                }
                p = (int *)(*((int *)((uintptr_t)p + S_NEXT_UNIT_POINTER)));
            }
        }
    }
}

void sneak(byte id)
{
    for (int i = 0; i < 16; i++)
    {
        int *p = (int *)(UNITS_LISTS + 4 * i);
        if (p)
        {
            p = (int *)(*p);
            while (p)
            {
                bool f = (*((byte *)((uintptr_t)p + S_ID)) == id);
                if (f)
                {
                    if (!check_unit_dead(p))
                    {
                        byte o = *((byte *)((uintptr_t)p + S_ORDER));
                        WORD n = *((WORD *)((uintptr_t)p + S_INVIZ));
                        if (o == ORDER_STAND)
                        {
                            if (n <= 10)
                                set_stat(p, 10, S_INVIZ);
                        }
                    }
                }
                p = (int *)(*((int *)((uintptr_t)p + S_NEXT_UNIT_POINTER)));
            }
        }
    }
}

bool slot_alive(byte p)
{
    return (get_val(ALL_BUILDINGS, p) + get_val(ALL_UNITS, p)) != 0; // no units and buildings
}

void ally(byte p1, byte p2, byte a)
{
    // set ally bytes in table
    *(byte *)(ALLY + p1 + 16 * p2) = a;
    *(byte *)(ALLY + p2 + 16 * p1) = a;
    ((void (*)())F_RESET_COLORS)(); // orig war2 func reset colors of sqares around units
}

void ally_one_sided(byte p1, byte p2, byte a)
{
    // set ally bytes in table
    *(byte *)(ALLY + p1 + 16 * p2) = a;
    ((void (*)())F_RESET_COLORS)(); // orig war2 func reset colors of sqares around units
}

bool check_opponents(byte player)
{
    // check if player have opponents
    bool f = false;
    byte o = C_NOBODY;
    for (byte i = 0; i < 8; i++)
    {
        if (player != i)
        {
            if (slot_alive(i) && !check_ally(player, i)) // if enemy and not dead
                f = true;
        }
    }
    return f;
}

void viz(int p1, int p2, byte a)
{
    // set vision bits
    byte v = *(byte *)(VIZ + p1);
    if (a == 0)
        v = v & (~(1 << p2));
    else
        v = v | (1 << p2);
    *(byte *)(VIZ + p1) = v;

    v = *(byte *)(VIZ + p2);
    if (a == 0)
        v = v & (~(1 << p1));
    else
        v = v | (1 << p1);
    *(byte *)(VIZ + p2) = v;
}

void viz_one_sided(int p1, int p2, byte a)
{
    // set vision bits
    byte v = *(byte *)(VIZ + p1);
    if (a == 0)
        v = v & (~(1 << p2));
    else
        v = v | (1 << p2);
    *(byte *)(VIZ + p1) = v;
}

void comps_vision(bool v)
{
    // comps can give vision too
    if (v)
    {
        char o[] = "\x0";
        PATCH_SET((char *)COMPS_VIZION, o);
        char o2[] = "\x90\x90";
        PATCH_SET((char *)COMPS_VIZION2, o2);
        char o3[] = "\x90\x90\x90\x90\x90\x90";
        PATCH_SET((char *)COMPS_VIZION3, o3);
    }
    else
    {
        char o[] = "\xAA";
        PATCH_SET((char *)COMPS_VIZION, o);
        char o2[] = "\x84\xC9";
        PATCH_SET((char *)COMPS_VIZION2, o2);
        char o3[] = "\xF\x85\x8C\x0\x0\x0";
        PATCH_SET((char *)COMPS_VIZION3, o3);
    }
}

void change_res(byte p, byte r, byte k, int m)
{
    int a = GOLD;
    int *rs = (int *)a;
    DWORD res = 0;
    bool s = false;
    if (p >= 0 && p <= 8) // player id
    {
        switch (r) // select resource and add or substract it
        {
        case 0:
            a = GOLD + 4 * p;
            s = false;
            break;
        case 1:
            a = LUMBER + 4 * p;
            s = false;
            break;
        case 2:
            a = OIL + 4 * p;
            s = false;
            break;
        case 3:
            a = GOLD + 4 * p;
            s = true;
            break;
        case 4:
            a = LUMBER + 4 * p;
            s = true;
            break;
        case 5:
            a = OIL + 4 * p;
            s = true;
            break;
        default:
            break;
        }
        if (r >= 0 && r <= 5)
        {
            rs = (int *)a; // resourse pointer
            if (s)
            {
                if (*rs > (int)(k * m))
                    res = *rs - (k * m);
                else
                    res = 0; // canot go smaller than 0
            }
            else
            {
                if (*rs <= (256 * 256 * 256 * 32))
                    res = *rs + (k * m);
            }
            patch_setdword((DWORD *)a, res);
        }
    }
}

void add_total_res(byte p, byte r, byte k, int m)
{
    int a = GOLD_TOTAL;
    int *rs = (int *)a;
    DWORD res = 0;
    if (p >= 0 && p <= 8) // player id
    {
        switch (r) // select resource and add or substract it
        {
        case 0:
            a = GOLD_TOTAL + 4 * p;
            break;
        case 1:
            a = LUMBER_TOTAL + 4 * p;
            break;
        case 2:
            a = OIL_TOTAL + 4 * p;
            break;
        default:
            break;
        }
        if (r >= 0 && r <= 2)
        {
            rs = (int *)a; // resourse pointer
            if (*rs <= (256 * 256 * 256 * 32))
                res = *rs + (k * m);
            patch_setdword((DWORD *)a, res);
        }
    }
}

void set_res(byte p, byte r, byte k1, byte k2, byte k3, byte k4)
{
    // as before but dnt add or sub res, just set given value
    char buf[4] = {0};
    int a = 0;
    if (p >= 0 && p <= 8)
    {
        switch (r)
        {
        case 0:
            a = GOLD + 4 * p;
            break;
        case 1:
            a = LUMBER + 4 * p;
            break;
        case 2:
            a = OIL + 4 * p;
            break;
        default:
            break;
        }
        if (r >= 0 && r <= 2)
        {
            buf[0] = k1;
            buf[1] = k2;
            buf[2] = k3;
            buf[3] = k4;
            PATCH_SET((char *)a, buf);
        }
    }
}

bool cmp_res(byte p, byte r, byte k1, byte k2, byte k3, byte k4, byte cmp)
{
    // compare resource to value
    int a = GOLD;
    int *rs = (int *)a;
    if (p >= 0 && p <= 8)
    {
        switch (r)
        {
        case 0:
            a = GOLD + 4 * p;
            break;
        case 1:
            a = LUMBER + 4 * p;
            break;
        case 2:
            a = OIL + 4 * p;
            break;
        default:
            break;
        }
        if (r >= 0 && r <= 2)
        {
            rs = (int *)a;
            return cmp_args4(cmp, *rs, k1 + 256 * k2 + 256 * 256 * k3 + 256 * 256 * 256 * k4);
        }
    }
    return false;
}

int empty_false(byte) { return 0; } // always return false function
int empty_true(byte) { return 1; }  // always return true function
void empty_build(int id)
{
    ((void (*)(int))F_TRAIN_UNIT)(id); // original build unit func
}
void empty_build_building(int id)
{
    ((void (*)(int))F_BUILD_BUILDING)(id); // original build func
}
void empty_build_research(int id)
{
    ((void (*)(int))F_BUILD_RESEARCH)(id);
}
void empty_build_research_spell(int id)
{
    ((void (*)(int))F_BUILD_RESEARCH_SPELL)(id);
}
void empty_build_upgrade_self(int id)
{
    ((void (*)(int))F_BUILD_UPGRADE_SELF)(id);
}
void empty_cast_spell(int id)
{
    ((void (*)(int))F_CAST_SPELL)(id);
}
int empty_research_swords(byte id) { return ((int (*)(int))F_CHECK_RESEARCH_SWORDS)(id); }       // 0 or 1
int empty_research_shield(byte id) { return ((int (*)(int))F_CHECK_RESEARCH_SHIELD)(id); }       // 0 or 1
int empty_research_cat(byte id) { return ((int (*)(int))F_CHECK_RESEARCH_CAT)(id); }             // 0 or 1
int empty_research_arrows(byte id) { return ((int (*)(int))F_CHECK_RESEARCH_ARROWS)(id); }       // 0 or 1
int empty_research_ships_at(byte id) { return ((int (*)(int))F_CHECK_RESEARCH_SHIPS_AT)(id); }   // 0 or 1
int empty_research_ships_def(byte id) { return ((int (*)(int))F_CHECK_RESEARCH_SHIPS_DEF)(id); } // 0 or 1
int empty_research_ranger(byte id) { return ((int (*)(int))F_CHECK_RESEARCH_RANGER)(id); }
int empty_research_scout(byte id) { return ((int (*)(int))F_CHECK_RESEARCH_SCOUT)(id); }
int empty_research_long(byte id) { return ((int (*)(int))F_CHECK_RESEARCH_LONG)(id); }
int empty_research_marks(byte id) { return ((int (*)(int))F_CHECK_RESEARCH_MARKS)(id); }
int empty_research_spells(byte id) { return ((int (*)(int))F_CHECK_RESEARCH_SPELL)(id); }
// 00444410
int empty_upgrade_th(byte id) { return ((int (*)(int))F_CHECK_UPGRADE_TH)(id); }       // 0 or 1
int empty_upgrade_tower(byte id) { return ((int (*)(int))F_CHECK_UPGRADE_TOWER)(id); } // 0 or 1
int empty_spell_learned(byte id) { return ((int (*)(int))F_CHECK_SPELL_LEARNED)(id); }

void build_forest(int)
{
    int *p = NULL;
    byte local = *(byte *)LOCAL_PLAYER;
    p = (int *)(UNITS_SELECTED + 9 * 4 * local);
    if (p)
    {
        p = (int *)(*p);
        if (p)
        {
            byte id = *((byte *)((uintptr_t)p + S_ID));
            byte o = *((byte *)((uintptr_t)p + S_OWNER));
            if (o == local)
            {
                if (id == U_PEASANT)
                {
                    ((void (*)(int))F_BUILD_BUILDING)(U_PIGFARM); // original build func
                }
                if (id == U_PEON)
                {
                    ((void (*)(int))F_BUILD_BUILDING)(U_FARM); // original build func
                }
            }
        }
    }
}

int _2tir()
{
    if ((get_val(TH2, *(byte *)LOCAL_PLAYER) != 0) || (get_val(TH3, *(byte *)LOCAL_PLAYER) != 0))
        return 1;
    else
        return 0;
}

int check_hero(byte id)
{
    byte local = *(byte *)LOCAL_PLAYER;
    for (int i = 0; i < 16; i++)
    {
        if (heros[i] == id)
        {
            if (herosb[i])
            {
                // return false if player already builds that unit
                return 0;
            }
        }
    }
    // substitute for original war2 func that check if player have that unit
    for (int i = 0; i < 16; i++)
    {
        int *p = (int *)(UNITS_LISTS + 4 * i);
        if (p)
        {
            p = (int *)(*p);
            while (p)
            {
                byte o = *((byte *)((uintptr_t)p + S_OWNER));
                if (o == *(byte *)LOCAL_PLAYER)
                {
                    byte idd = *((byte *)((uintptr_t)p + S_ID));
                    bool f = idd == id;
                    if (f)
                    {
                        if (!check_unit_dead(p))
                        {
                            // return false if player already have that unit
                            return 0;
                        }
                    }
                    f = (idd == U_TOWN_HALL) || (idd == U_GREAT_HALL) ||
                        (idd == U_STRONGHOLD) || (idd == U_KEEP) ||
                        (idd == U_CASTLE) || (idd == U_FORTRESS);
                    if (f)
                    {
                        if (!check_unit_dead(p) && check_unit_complete(p))
                        {
                            if (*((byte *)((uintptr_t)p + S_BUILD_ORDER)) == 0)
                            {
                                if (*((byte *)((uintptr_t)p + S_BUILD_TYPE)) == id)
                                {
                                    if (*((WORD *)((uintptr_t)p + S_BUILD_PROGRES)) != 0)
                                    {
                                        // return false if player already building that unit
                                        return 0;
                                    }
                                }
                            }
                        }
                    }
                }
                p = (int *)(*((int *)((uintptr_t)p + S_NEXT_UNIT_POINTER)));
            }
        }
    }
    return 1;
}

void build3(bool b)
{
    // third build button for peons
    if (b)
    {
        char psnt[] = "\x9";
        PATCH_SET((char *)PEASANT_BUTTONS, psnt);
        char peon[] = "\x9\x0\x0\x0\x20\xf7\x48\x0";
        patch_setdword((DWORD *)(peon + 4), (DWORD)peon_build3);
        PATCH_SET((char *)PEON_BUTTONS, peon);

        char p1[] = "\x8\x0\x6d\x0\xf0\x40\x44\x0\x70\xa6\x44\x0\x0\x6d\x38\x1\x0\x0\x0\x0";
        PATCH_SET((char *)PEASANT_RE_BUTTONS, p1);

        if (b3rune)
        {
            patch_setdword((DWORD *)(build_3 + 4), (DWORD)F_ALWAYS_TRUE);
        }
        else
        {
            int (*r)(byte) = empty_false;
            patch_setdword((DWORD *)(build_3 + 4), (DWORD)r);
        }

        if (b3port)
        {
            patch_setdword((DWORD *)(build_3 + 24), (DWORD)F_ALWAYS_TRUE);
        }
        else
        {
            int (*r)(byte) = empty_false;
            patch_setdword((DWORD *)(build_3 + 24), (DWORD)r);
        }

        if (b3cirl)
        {
            patch_setdword((DWORD *)(build_3 + 44), (DWORD)F_ALWAYS_TRUE);
        }
        else
        {
            int (*r)(byte) = empty_false;
            patch_setdword((DWORD *)(build_3 + 44), (DWORD)r);
        }

        if (b3mine)
        {
            patch_setdword((DWORD *)(build_3 + 64), (DWORD)F_ALWAYS_TRUE);
        }
        else
        {
            int (*r)(byte) = empty_false;
            patch_setdword((DWORD *)(build_3 + 64), (DWORD)r);
        }

        if (b3forest)
        {
            patch_setdword((DWORD *)(build_3 + 84), (DWORD)F_ALWAYS_TRUE);
        }
        else
        {
            int (*r)(byte) = empty_false;
            patch_setdword((DWORD *)(build_3 + 84), (DWORD)r);
        }

        void (*r)(int) = build_forest;
        patch_setdword((DWORD *)(build_3 + 88), (DWORD)r);

        char b3[] = "\x6\x0\x0\x0\x0\xf8\x48\x0";
        patch_setdword((DWORD *)(b3 + 4), (DWORD)build_3);
        PATCH_SET((char *)DEAD_BLDG_BUTTONS, b3);
    }
    else
    {
        char psnt[] = "\x8";
        PATCH_SET((char *)PEASANT_BUTTONS, psnt);
        char peon[] = "\x8\x0\x0\x0\x28\x22\x4a\x0";
        PATCH_SET((char *)PEON_BUTTONS, peon);
        char p1[] = "\x0\x0\x54\x0\xf0\x40\x44\x0\x90\x66\x43\x0\x0\x0\x88\x1\x2\x0\x0\x0";
        PATCH_SET((char *)PEASANT_RE_BUTTONS, p1);
        char b3[] = "\x0\x0\x0\x0\x0\x0\x0";
        PATCH_SET((char *)DEAD_BLDG_BUTTONS, b3);
    }
}

void sheep(bool b)
{
    // sheeps from farms
    if (b)
    {
        char farm[] = "\x1\x0\x0\x0\x0\xf7\x48\x0\x1\x0\x0\x0\x0\xf7\x48\x0";
        patch_setdword((DWORD *)(farm + 4), (DWORD)sheep_build);
        patch_setdword((DWORD *)(farm + 12), (DWORD)sheep_build);
        PATCH_SET((char *)FARM_BUTTONS, farm);

        int (*r)(byte) = empty_true;   // sheep check function
        void (*r1)(int) = empty_build; // sheep build function

        patch_setdword((DWORD *)(sheep_build + 4), (DWORD)r);
        patch_setdword((DWORD *)(sheep_build + 8), (DWORD)r1);
    }
    else
    {
        char farm[] = "\x0\x0\x0\x0\x0\x0\x0\x0\x0\x0\x0\x0\x0\x0\x0\x0";
        PATCH_SET((char *)FARM_BUTTONS, farm);
    }
}

void demon(bool b)
{
    if (b)
    {
        char d[] = "\x1\x0\x0\x0\x0\xf7\x48\x0";
        patch_setdword((DWORD *)(d + 4), (DWORD)demon_build);
        PATCH_SET((char *)CIRCLE_BUTTONS, d);

        int (*r)() = _2tir; // need 2 tier TH to build demon
        patch_setdword((DWORD *)(demon_build + 4), (DWORD)r);
    }
    else
    {
        char d[] = "\x0\x0\x0\x0\x0\x0\x0\x0";
        PATCH_SET((char *)CIRCLE_BUTTONS, d);
    }
}

byte get_icon(byte id)
{
    switch (id)
    {
    case U_ALLERIA:
        return 187;
        break; // alleria
    case U_TERON:
        return 189;
        break; // teron
    case U_KURDRAN:
        return 191;
        break; // kurdran
    case U_DENTARG:
        return 194;
        break; // dentarg
    case U_HADGAR:
        return 193;
        break; // hadgar
    case U_GROM:
        return 190;
        break; // grom
    case U_DEATHWING:
        return 192;
        break; // deathwing
    case U_TYRALYON:
        return 195;
        break; // tyralyon
    case U_DANATH:
        return 188;
        break; // danat
    case U_KARGATH:
        return 186;
        break; // kargat
    case U_CHOGAL:
        return 36;
        break; // chogal
    case U_LOTHAR:
        return 32;
        break; // lotar
    case U_GULDAN:
        return 33;
        break; // guldan
    case U_UTER:
        return 34;
        break; // uter
    case U_ZULJIN:
        return 35;
        break; // zuljin
    default:
        break;
    }
    return 113;
}

byte get_tbl(byte id)
{
    switch (id)
    {
    case U_ALLERIA:
        return '\x10';
        break; // alleria
    case U_TERON:
        return '\x17';
        break; // teron
    case U_KURDRAN:
        return '\x27';
        break; // kurdran
    case U_DENTARG:
        return '\x14';
        break; // dentarg
    case U_HADGAR:
        return '\x19';
        break; // hadgar
    case U_GROM:
        return '\x1a';
        break; // grom
    case U_DEATHWING:
        return '\x28';
        break; // deathwing
    case U_TYRALYON:
        return '\x13';
        break; // tyralyon
    case U_DANATH:
        return '\x1b';
        break; // danat
    case U_KARGATH:
        return '\x22';
        break; // kargat
    case U_CHOGAL:
        return '\x16';
        break; // chogal
    case U_LOTHAR:
        return '\x21';
        break; // lotar
    case U_GULDAN:
        return '\x1f';
        break; // guldan
    case U_UTER:
        return '\x15';
        break; // uter
    case U_ZULJIN:
        return '\xf';
        break; // zuljin
    default:
        break;
    }
    return '\xdf';
}

DWORD CALLBACK hero_thread(LPVOID lpParam)
{
    Sleep(2000);
    for (int i = 0; i < 16; i++)
    {
        herosb[i] = false;
    }
    ExitThread(0);
}

void build_hero(int id)
{
    for (int i = 0; i < 16; i++)
    {
        if ((!herosb[i]) && (heros[i] == (id % 256)))
        {
            herosb[i] = true;
            DWORD dw = 0;
            CreateThread(NULL, 0, hero_thread, &dw, 0, NULL);
        }
    }
    ((void (*)(int))F_TRAIN_UNIT)(id);      // original build func
    ((void (*)(int))F_CHANGE_STATUS)(0x6e); // change status
    ((void (*)())F_STATUS_REDRAW)();        // status redraw
}

void heroes(bool b)
{
    if (b)
    {
        for (int i = 1; i < 20; i++)
        {
            hhero[i] = build_3[100 + i];
            ohero[i] = build_3[100 + i];
        }
        byte local = *(byte *)LOCAL_PLAYER;
        int l = 0;
        l += local;
        byte mx = 2;
        if (get_val(TH2, l) != 0)
            mx = 5;
        if (get_val(TH3, l) != 0)
            mx = 8;
        byte k = 1;
        for (byte i = 1; i < 9; i++)
        {
            byte id = heros[i - 1];
            if (id == 0)
                id = U_CRITTER;
            if (id != 0)
            {
                k++;
                byte ico = get_icon(id);
                hhero[20 * i + 0] = i;     // button id
                hhero[20 * i + 1] = '\x0'; // button id?
                hhero[20 * i + 2] = ico;   // icon
                hhero[20 * i + 3] = '\x0'; // icon

                int (*r)(byte) = check_hero; // hero check function
                if ((i > mx) || (id == U_CRITTER))
                    r = empty_false;
                patch_setdword((DWORD *)(hhero + (20 * i + 4)), (DWORD)r);

                void (*r1)(int) = build_hero; // hero build function
                patch_setdword((DWORD *)(hhero + (20 * i + 8)), (DWORD)r1);

                hhero[20 * i + 12] = id;          // arg
                hhero[20 * i + 13] = id;          // unit id
                hhero[20 * i + 14] = get_tbl(id); // string from tbl
                hhero[20 * i + 15] = '\x1';       // string from tbl
                hhero[20 * i + 16] = '\x0';       // flags?
                hhero[20 * i + 17] = '\x0';       // flags?
                hhero[20 * i + 18] = '\x0';       // flags?
                hhero[20 * i + 19] = '\x0';       // flags?
            }
            else
                i = 10;
        }
        char b1[] = "\x0\x0\x0\x0\x0\xf8\x48\x0";
        b1[0] = k;
        patch_setdword((DWORD *)(b1 + 4), (DWORD)hhero);
        PATCH_SET((char *)DEAD_BLDG2_BUTTONS, b1);
        k = 1;
        for (byte i = 1; i < 9; i++)
        {
            byte id = heros[i + 8 - 1];
            if (id == 0)
                id = U_CRITTER;
            if (id != 0)
            {
                k++;
                byte ico = get_icon(id);
                ohero[20 * i + 0] = i;     // button id
                ohero[20 * i + 1] = '\x0'; // button id?
                ohero[20 * i + 2] = ico;   // icon
                ohero[20 * i + 3] = '\x0'; // icon

                int (*r)(byte) = check_hero; // hero check function
                if ((i > mx) || (id == U_CRITTER))
                    r = empty_false;
                patch_setdword((DWORD *)(ohero + (20 * i + 4)), (DWORD)r);

                void (*r1)(int) = build_hero; // hero build function
                patch_setdword((DWORD *)(ohero + (20 * i + 8)), (DWORD)r1);

                ohero[20 * i + 12] = id;          // arg
                ohero[20 * i + 13] = id;          // unit id
                ohero[20 * i + 14] = get_tbl(id); // string from tbl
                ohero[20 * i + 15] = '\x1';       // string from tbl
                ohero[20 * i + 16] = '\x0';       // flags?
                ohero[20 * i + 17] = '\x0';       // flags?
                ohero[20 * i + 18] = '\x0';       // flags?
                ohero[20 * i + 19] = '\x0';       // flags?
            }
            else
                i = 10;
        }
        char b2[] = "\x0\x0\x0\x0\x0\xf8\x48\x0";
        b2[0] = k;
        patch_setdword((DWORD *)(b2 + 4), (DWORD)ohero);
        PATCH_SET((char *)DEAD_BLDG3_BUTTONS, b2);
    }
    else
    {
        char buf[] = "\x0\x0\x0\x0\x0\x0\x0";
        PATCH_SET((char *)DEAD_BLDG2_BUTTONS, buf);
        PATCH_SET((char *)DEAD_BLDG3_BUTTONS, buf);
    }
}

void repair_all(bool b)
{
    // peon can repair all units
    if (b)
    {
        char bau[] = "\xeb"; // 0x75
        PATCH_SET((char *)REPAIR_FLAG_CHECK, bau);
    }
    else
    {
        char bau[] = "\x75";
        PATCH_SET((char *)REPAIR_FLAG_CHECK, bau);
    }
}

void repair_cat(bool b)
{
    // peon can repair unit if it have transport flag OR catapult flag
    if (b)
    {
        char r1[] = "\xeb\x75\x90\x90\x90"; // f6 c4 04 74 14
        PATCH_SET((char *)REPAIR_FLAG_CHECK2, r1);
        char r2[] = "\x66\xa9\x04\x04\x74\x9c\xeb\x86";
        PATCH_SET((char *)REPAIR_CODE_CAVE, r2);
    }
    else
    {
        char r1[] = "\xf6\xc4\x4\x74\x14";
        PATCH_SET((char *)REPAIR_FLAG_CHECK2, r1);
    }
}

void fireball_dmg(byte dmg)
{
    char fb[] = "\x28"; // 40 default
    fb[0] = dmg;
    PATCH_SET((char *)FIREBALL_DMG, fb);
}

void buff_time(byte bf, byte t1, byte t2)
{
    char tm[] = "\x0\x0";
    switch (bf)
    {
    case 0:
        tm[0] = t1;
        tm[1] = t2;
        PATCH_SET((char *)INVIZ_TIME, tm); // 2000 default
        break;
    case 1:
        tm[0] = t1;
        tm[1] = t2;
        PATCH_SET((char *)BLOOD_TIME, tm); // 1000 default
        break;
    case 2:
        tm[0] = t1;
        tm[1] = t2;
        PATCH_SET((char *)SHIELD_TIME, tm); // 500 default
        break;
    case 3:
        tm[0] = t1;
        tm[1] = t2;
        PATCH_SET((char *)HASTE_TIME1, tm); // 1000 default
        PATCH_SET((char *)HASTE_TIME2, tm);
        break;
    case 4:
        int tt = 0xffff - (t1 + 256 * t2);
        tm[0] = tt % 256;
        tm[1] = tt / 256;
        PATCH_SET((char *)SLOW_TIME1, tm); // 1000 default
        PATCH_SET((char *)SLOW_TIME2, tm);
        break;
    }
}

void fireshield_flyers(bool b)
{
    // allow cast fireshield on flyers
    if (b)
    {
        char bau[] = "\x90\x90"; // 75 51
        PATCH_SET((char *)FIRESHIELD_FLYERS, bau);
    }
    else
    {
        char bau[] = "\x75\x51";
        PATCH_SET((char *)FIRESHIELD_FLYERS, bau);
    }
}

void trigger_time(byte tm)
{
    // war2 will call victory check function every 200 game ticks
    char ttime[] = "\xc8"; // 200 default
    ttime[0] = tm;
    PATCH_SET((char *)TRIG_TIME, ttime);
}

void manacost(byte id, byte c)
{
    // spells cost of mana
    char mana[] = "\x1";
    mana[0] = c;
    PATCH_SET((char *)(MANACOST + 2 * id), mana);
}

void upgr(byte id, byte c)
{
    // upgrades power
    char up[] = "\x1";
    up[0] = c;
    PATCH_SET((char *)(UPGRD + id), up);
}

byte get_upgrade(byte id, byte pl)
{
    int a = GB_ARROWS;
    switch (id)
    {
    case ARROWS:
        a = GB_ARROWS;
        break;
    case SWORDS:
        a = GB_SWORDS;
        break;
    case ARMOR:
        a = GB_SHIELDS;
        break;
    case SHIP_DMG:
        a = GB_BOAT_ATTACK;
        break;
    case SHIP_ARMOR:
        a = GB_BOAT_ARMOR;
        break;
    case SHIP_SPEED:
        a = GB_BOAT_SPEED;
        break;
    case CATA_DMG:
        a = GB_CAT_DMG;
        break;
    default:
        a = GB_ARROWS;
        break;
    }
    return *(byte *)(a + pl);
}

void set_upgrade(byte id, byte pl, byte v)
{
    int a = GB_ARROWS;
    switch (id)
    {
    case ARROWS:
        a = GB_ARROWS;
        break;
    case SWORDS:
        a = GB_SWORDS;
        break;
    case ARMOR:
        a = GB_SHIELDS;
        break;
    case SHIP_DMG:
        a = GB_BOAT_ATTACK;
        break;
    case SHIP_ARMOR:
        a = GB_BOAT_ARMOR;
        break;
    case SHIP_SPEED:
        a = GB_BOAT_SPEED;
        break;
    case CATA_DMG:
        a = GB_CAT_DMG;
        break;
    default:
        a = GB_ARROWS;
        break;
    }
    char buf[] = "\x0";
    buf[0] = v;
    PATCH_SET((char *)(a + pl), buf);
    ((void (*)())F_STATUS_REDRAW)(); // status redraw
}

int upgr_check_swords(byte b)
{
    byte u = get_upgrade(SWORDS, *(byte *)LOCAL_PLAYER);
    if ((b == 0) && (u == 0))
        return 1;
    if ((b == 1) && (u == 1))
        return 1;
    if ((b == 2) && (u >= 2))
        return 1;
    return 0;
}

int upgr_check_shields(byte b)
{
    byte u = get_upgrade(ARMOR, *(byte *)LOCAL_PLAYER);
    if ((b == 0) && (u == 0))
        return 1;
    if ((b == 1) && (u == 1))
        return 1;
    if ((b == 2) && (u >= 2))
        return 1;
    return 0;
}

int upgr_check_boat_attack(byte b)
{
    byte u = get_upgrade(SHIP_DMG, *(byte *)LOCAL_PLAYER);
    if ((b == 0) && (u == 0))
        return 1;
    if ((b == 1) && (u == 1))
        return 1;
    if ((b == 2) && (u >= 2))
        return 1;
    return 0;
}

int upgr_check_boat_armor(byte b)
{
    byte u = get_upgrade(SHIP_ARMOR, *(byte *)LOCAL_PLAYER);
    if ((b == 0) && (u == 0))
        return 1;
    if ((b == 1) && (u == 1))
        return 1;
    if ((b == 2) && (u >= 2))
        return 1;
    return 0;
}

int upgr_check_arrows(byte b)
{
    byte u = get_upgrade(ARROWS, *(byte *)LOCAL_PLAYER);
    if ((b == 0) && (u == 0))
        return 1;
    if ((b == 1) && (u == 1))
        return 1;
    if ((b == 2) && (u >= 2))
        return 1;
    return 0;
}

void upgr_check_replace(bool f)
{
    if (f)
    {
        char buf[] = "\xC3"; // ret
        patch_ljmp((char *)UPGR_CHECK_FIX_SWORDS, (char *)upgr_check_swords);
        PATCH_SET((char *)(UPGR_CHECK_FIX_SWORDS + 5), buf);
        patch_ljmp((char *)UPGR_CHECK_FIX_SHIELDS, (char *)upgr_check_shields);
        PATCH_SET((char *)(UPGR_CHECK_FIX_SHIELDS + 5), buf);
        patch_ljmp((char *)UPGR_CHECK_FIX_SHIPS_AT, (char *)upgr_check_boat_attack);
        PATCH_SET((char *)(UPGR_CHECK_FIX_SHIPS_AT + 5), buf);
        patch_ljmp((char *)UPGR_CHECK_FIX_SHIPS_DEF, (char *)upgr_check_boat_armor);
        PATCH_SET((char *)(UPGR_CHECK_FIX_SHIPS_DEF + 5), buf);
        patch_ljmp((char *)UPGR_CHECK_FIX_ARROWS, (char *)upgr_check_arrows);
        PATCH_SET((char *)(UPGR_CHECK_FIX_ARROWS + 5), buf);
    }
    else
    {
        char buf2[] = "\x33\xC0\x33\xC9\xA0\x18"; // back
        PATCH_SET((char *)UPGR_CHECK_FIX_SWORDS, buf2);
        PATCH_SET((char *)UPGR_CHECK_FIX_SHIELDS, buf2);
        PATCH_SET((char *)UPGR_CHECK_FIX_SHIPS_AT, buf2);
        PATCH_SET((char *)UPGR_CHECK_FIX_SHIPS_DEF, buf2);
        PATCH_SET((char *)UPGR_CHECK_FIX_ARROWS, buf2);
    }
}

void no_random_dmg(bool b)
{
    // always max damage instead of random (SC1-style)
    if (b)
    {
        char b1[] = "\x1a";
        PATCH_SET((char *)DAMAGE_AREA1, b1);
        char b2[] = "\x19";
        PATCH_SET((char *)DAMAGE_AREA2, b2);
        char b3[] = "\x90\x90";
        PATCH_SET((char *)DAMAGE_AREA3, b3);
        PATCH_SET((char *)DAMAGE_AREA4, b3);
        char b4[] = "\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90";
        PATCH_SET((char *)DAMAGE_UNIT_VS_UNIT, b4);
    }
    else
    {
        char b1[] = "\x6";
        PATCH_SET((char *)DAMAGE_AREA1, b1);
        char b2[] = "\x5";
        PATCH_SET((char *)DAMAGE_AREA2, b2);
        char b3[] = "\xf7\xf1";
        PATCH_SET((char *)DAMAGE_AREA3, b3);
        PATCH_SET((char *)DAMAGE_AREA4, b3);
        char b4[] = "\x8d\x47\x1\x99\x2b\xc2\x8b\xf8\xd1\xff\xe8\x8d\x91\x6\x0\x8d\x4f\x1\x33\xd2\xf7\xf1\x3\xfa";
        PATCH_SET((char *)DAMAGE_UNIT_VS_UNIT, b4);
    }
}

void blood_fix(bool b)
{
    // remove bloodlust from game (and will make X2 in damage() function manually)
    if (b)
    {
        blood_f = true; // to make X2 in damage() function manually
        char b1[] = "\x90\x90";
        PATCH_SET((char *)BLOOD_CALC1, b1);
        char b2[] = "\x90\x90";
        PATCH_SET((char *)BLOOD_CALC2, b2);
    }
    else
    {
        blood_f = false;
        char b1[] = "\x3\xf6";
        PATCH_SET((char *)BLOOD_CALC1, b1);
        char b2[] = "\x3\xc0";
        PATCH_SET((char *)BLOOD_CALC2, b2);
    }
}

void center_view(byte x, byte y)
{
    ((void (*)(byte, byte))F_MINIMAP_CLICK)(x, y); // original war2 func that called when player click on minimap
}

void speedhack(byte speed)
{
    // change game speed koef of fastest
    char buf[] = "\xd"; // 13 is default koef on fastest
    buf[0] = speed;     // smaller = faster (1 = ultra fast)(0 = processor blow up)
    PATCH_SET((char *)FASTEST_SPEED, buf);
}

void comp_aggro(int *trg, int *atk)
{
    byte own1 = *((byte *)((uintptr_t)trg + S_OWNER));
    byte own2 = *((byte *)((uintptr_t)atk + S_OWNER));
    if (own1 != own2)
    {
        if (check_ally(own1, own2)) // if we allies
        {
            byte o1 = *(byte *)(CONTROLER_TYPE + own1);
            byte o2 = *(byte *)(CONTROLER_TYPE + own2);
            if ((o1 == C_COMP) && (o2 == C_PLAYER)) // if target unit is comp and attacker is real player
            {
                ally(own1, own2, 0);
                viz(own1, own2, 0);
                // turn off ally and viz
            }
        }
    }
}

bool capture(int *trg, int *atk)
{
    byte own1 = *((byte *)((uintptr_t)trg + S_OWNER));
    byte own2 = *((byte *)((uintptr_t)atk + S_OWNER));
    if (own1 != own2)
    {
        if (!pcpt || ((*((byte *)((uintptr_t)atk + S_ID)) == U_PEASANT) || (*((byte *)((uintptr_t)atk + S_ID)) == U_PEON)))
        {                                                   // if only peons can capture and attacker is peon(or peasant)
            if (cmp_stat(trg, U_FARM, S_ID, CMP_BIGGER_EQ)) // only buildings (id >= farm)
            {
                if (cpt)
                {
                    if (check_unit_complete(trg)) // completed buildings
                    {
                        byte tid = *((byte *)((uintptr_t)trg + S_ID));   // unit id
                        WORD mhp = *(WORD *)(UNIT_HP_TABLE + 2 * tid);   // max hp
                        WORD hp = *((WORD *)((uintptr_t)trg + S_HP));    // unit hp
                        if ((float)hp <= (((float)mhp / 100.0 * 5) + 1)) // if hp<=5%
                        {
                            WORD hl = (WORD)(((float)mhp / 100.0 * 5) + 1);
                            heal(trg, hl % 256, hl / 256); // heal 5% hp so that it will not die suddenly
                            if (thcpt)                     // if th captured, capture all
                            {
                                bool mf = (tid == U_TOWN_HALL) || (tid == U_GREAT_HALL);
                                if (get_val(TH2, (int)own1) != 0)
                                    mf = (tid == U_KEEP) || (tid == U_STRONGHOLD);
                                if (get_val(TH3, (int)own1) != 0)
                                    mf = (tid == U_CASTLE) || (tid == U_FORTRESS);
                                if (mf)
                                { // THs of 1 2 and 3 tier
                                    captk = 0;
                                    for (int i = 0; i < 16; i++)
                                    {
                                        int *p = (int *)(UNITS_LISTS + 4 * i);
                                        if (p)
                                        {
                                            p = (int *)(*p);
                                            while (p)
                                            {
                                                if (!check_unit_dead(p))
                                                {
                                                    if (cmp_stat(p, (int)own1, S_OWNER, CMP_EQ)) // capture all units of that player
                                                    {
                                                        capt[captk] = p;
                                                        captk++;
                                                    }
                                                }
                                                p = (int *)(*((int *)((uintptr_t)p + S_NEXT_UNIT_POINTER)));
                                            }
                                        }
                                    }
                                    for (int i = 0; i < captk; i++)
                                        give(capt[i], own2);
                                }
                            }
                            give(trg, own2);
                            return true;
                        }
                    }
                }
            }
            else // units capture
            {
                if (ucpt)
                {
                    if (!check_unit_dead(trg))
                    {
                        byte tid = *((byte *)((uintptr_t)trg + S_ID));   // unit id
                        WORD mhp = *(WORD *)(UNIT_HP_TABLE + 2 * tid);   // max hp
                        WORD hp = *((WORD *)((uintptr_t)trg + S_HP));    // unit hp
                        if ((float)hp <= (((float)mhp / 100.0 * 5) + 1)) // if hp<=5%
                        {
                            WORD hl = (WORD)(((float)mhp / 100.0 * 50) + 1);
                            heal(trg, hl % 256, hl / 256); // heal 50% hp so that it will not die suddenly
                            give(trg, own2);
                            return true;
                        }
                    }
                }
            }
        }
    }
    return false;
}

void steal_res(int *trg, int *atk)
{
    byte own1 = *((byte *)((uintptr_t)trg + S_OWNER));
    byte own2 = *((byte *)((uintptr_t)atk + S_OWNER));
    if ((own1 != own2) && !check_ally(own1, own2))
    {
        if (cmp_stat(trg, U_FARM, S_ID, CMP_BIGGER_EQ)) // only buildings (id >= farm)
        {
            if (check_unit_complete(trg)) // completed buildings
            {
                if (cmp_stat(atk, U_FARM, S_ID, CMP_SMALLER)) // only units (not towers)
                {
                    byte aid = *((byte *)((uintptr_t)atk + S_ID));
                    byte ar = *(byte *)(UNIT_RANGE_TABLE + aid);
                    if (ar < 2)
                    {
                        if (cmp_res(own1, 0, 2, 0, 0, 0, CMP_BIGGER_EQ)) // 2 gold
                        {
                            change_res(own1, 3, 2, 1);
                            change_res(own2, 0, 2, 1);
                        }
                        if (cmp_res(own1, 1, 1, 0, 0, 0, CMP_BIGGER_EQ)) // 1 lumber
                        {
                            change_res(own1, 4, 1, 1);
                            change_res(own2, 1, 1, 1);
                        }
                    }
                }
            }
        }
    }
}

void mana_burn(int *trg, int *atk)
{
    byte aid = *((byte *)((uintptr_t)atk + S_ID));
    if (aid == U_DEMON) // only demon have manaburn (you can put any other unit id here)
    {
        byte tid = *((byte *)((uintptr_t)trg + S_ID));
        bool f = (tid == U_MAGE) || (tid == U_DK) || (tid == U_PALADIN) || (tid == U_OGREMAGE) ||
                 (tid == U_HADGAR) || (tid == U_TERON) || (tid == U_GULDAN) ||
                 (tid == U_UTER) || (tid == U_TYRALYON) || (tid == U_CHOGAL) || (tid == U_DENTARG);
        if (f)
        {
            byte mp = *((byte *)((uintptr_t)trg + S_MANA));
            byte dmp = mp;
            if (dmp > 30)
                dmp = 30;
            mp -= dmp;
            set_stat(trg, mp, S_MANA);
            WORD hp = *((WORD *)((uintptr_t)trg + S_HP));
            if (hp != 0)
            {
                if (hp > dmp)
                    hp -= dmp;
                else
                    hp = 1;
            }
            set_stat(trg, hp, S_HP);
        }
    }
}

bool vamp_aura(int *trg, int *atk, byte id)
{
    byte tid = *((byte *)((uintptr_t)trg + S_ID));                // unit id
    if ((*(int *)(UNIT_GLOBAL_FLAGS + tid * 4) & IS_FLESHY) != 0) // fleshy global flag
    {
        byte aid = *((byte *)((uintptr_t)atk + S_ID));                // unit id
        if ((*(int *)(UNIT_GLOBAL_FLAGS + aid * 4) & IS_FLESHY) != 0) // fleshy global flag
        {
            byte x = *((byte *)((uintptr_t)atk + S_X));
            byte y = *((byte *)((uintptr_t)atk + S_Y));
            set_region((int)x - 5, (int)y - 5, (int)x + 5, (int)y + 5); // set region around myself
            find_all_alive_units(ANY_MEN);
            sort_in_region();
            sort_stat(S_ID, id, CMP_EQ);
            byte o = *((byte *)((uintptr_t)atk + S_OWNER));
            for (int ui = 0; ui < 16; ui++)
            {
                if (!check_ally(o, ui))
                    sort_stat(S_OWNER, ui, CMP_NEQ);
            }
            return units != 0;
        }
    }
    return false;
}

bool devotion_aura(int *trg, byte id)
{
    byte tid = *((byte *)((uintptr_t)trg + S_ID)); // unit id
    if (tid < U_FARM)                              // not buildings
    {
        byte x = *((byte *)((uintptr_t)trg + S_X));
        byte y = *((byte *)((uintptr_t)trg + S_Y));
        set_region((int)x - 5, (int)y - 5, (int)x + 5, (int)y + 5); // set region around myself
        find_all_alive_units(id);
        sort_in_region();
        byte o = *((byte *)((uintptr_t)trg + S_OWNER));
        for (int ui = 0; ui < 16; ui++)
        {
            if (!check_ally(o, ui))
                sort_stat(S_OWNER, ui, CMP_NEQ);
        }
        return units != 0;
    }
    else
        return false;
}

void peon_steal_attack(int *trg, int *atk)
{
    byte tid = *((byte *)((uintptr_t)trg + S_ID));
    byte aid = *((byte *)((uintptr_t)atk + S_ID));
    if ((aid == U_PEASANT) || (aid == U_PEON))
    {
        if ((tid == U_PEASANT) || (tid == U_PEON))
        {
            byte tf = *((byte *)((uintptr_t)trg + S_PEON_FLAGS));
            byte af = *((byte *)((uintptr_t)atk + S_PEON_FLAGS));
            if (!(af & PEON_LOADED))
            {
                if ((tf & PEON_LOADED))
                {
                    tf &= ~PEON_LOADED;
                    af |= PEON_LOADED;
                    if ((tf & PEON_HARVEST_GOLD))
                    {
                        tf &= ~PEON_HARVEST_GOLD;
                        af |= PEON_HARVEST_GOLD;
                    }
                    if ((tf & PEON_HARVEST_LUMBER))
                    {
                        tf &= ~PEON_HARVEST_LUMBER;
                        af |= PEON_HARVEST_LUMBER;
                    }
                    set_stat(trg, tf, S_PEON_FLAGS);
                    set_stat(atk, af, S_PEON_FLAGS);
                    ((void (*)(int *))F_GROUP_SET)(trg);
                    ((void (*)(int *))F_GROUP_SET)(atk);
                }
            }
        }
    }
}

PROC g_proc_00451054;
void count_add_to_tables_load_game(int *u)
{
    if (saveload_fixed)
    {
        byte f = *((byte *)((uintptr_t)u + S_AI_AIFLAGS));
        byte ff = f | AI_PASSIVE;
        set_stat(u, ff, S_AI_AIFLAGS);
        ((void (*)(int *))g_proc_00451054)(u); // original
        set_stat(u, f, S_AI_AIFLAGS);
    }
    else
        ((void (*)(int *))g_proc_00451054)(u); // original
}

PROC g_proc_00438A5C;
PROC g_proc_00438985;
void unset_peon_ai_flags(int *u)
{
    ((void (*)(int *))g_proc_00438A5C)(u); // original
    if (saveload_fixed)
    {
        char rep[] = "\x0\x0";
        WORD p = 0;
        for (int i = 0; i < 8; i++)
        {
            p = *((WORD *)((uintptr_t)SGW_REPAIR_PEONS + 2 * i));
            if (p > 1600)
                PATCH_SET((char *)(SGW_REPAIR_PEONS + 2 * i), rep);
            p = *((WORD *)((uintptr_t)SGW_GOLD_PEONS + 2 * i));
            if (p > 1600)
                PATCH_SET((char *)(SGW_GOLD_PEONS + 2 * i), rep);
            p = *((WORD *)((uintptr_t)SGW_TREE_PEONS + 2 * i));
            if (p > 1600)
                PATCH_SET((char *)(SGW_TREE_PEONS + 2 * i), rep);
        }
    }
}

void tech_built(int p, byte t)
{
    ((void (*)(int, byte))F_TECH_BUILT)(p, t);
}

void tech_reinit()
{
    for (int i = 0; i < 8; i++)
    {
        byte o = *(byte *)(CONTROLER_TYPE + i);
        byte a = 0;
        int s = 0;
        if (o == C_COMP)
        {
            a = *(byte *)(GB_ARROWS + i);
            if (a > 0)
                tech_built(i, UP_ARROW1);
            if (a > 1)
                tech_built(i, UP_ARROW2);
            a = *(byte *)(GB_SWORDS + i);
            if (a > 0)
                tech_built(i, UP_SWORD1);
            if (a > 1)
                tech_built(i, UP_SWORD2);
            a = *(byte *)(GB_SHIELDS + i);
            if (a > 0)
                tech_built(i, UP_SHIELD1);
            if (a > 1)
                tech_built(i, UP_SHIELD2);
            a = *(byte *)(GB_BOAT_ATTACK + i);
            if (a > 0)
                tech_built(i, UP_BOATATK1);
            if (a > 1)
                tech_built(i, UP_BOATATK2);
            a = *(byte *)(GB_BOAT_ARMOR + i);
            if (a > 0)
                tech_built(i, UP_BOATARM1);
            if (a > 1)
                tech_built(i, UP_BOATARM2);
            a = *(byte *)(GB_CAT_DMG + i);
            if (a > 0)
                tech_built(i, UP_CATDMG1);
            if (a > 1)
                tech_built(i, UP_CATDMG2);
            a = *(byte *)(GB_RANGER + i);
            if (a)
                tech_built(i, UP_RANGER);
            a = *(byte *)(GB_MARKS + i);
            if (a)
                tech_built(i, UP_SKILL1);
            a = *(byte *)(GB_LONGBOW + i);
            if (a)
                tech_built(i, UP_SKILL2);
            a = *(byte *)(GB_SCOUTING + i);
            if (a)
                tech_built(i, UP_SKILL3);

            s = *(int *)(SPELLS_LEARNED + 4 * i);
            if (s & (1 << L_ALTAR_UPGR))
                tech_built(i, UP_CLERIC);
            if (s & (1 << L_HEAL))
                tech_built(i, UP_CLERIC1);
            if (s & (1 << L_BLOOD))
                tech_built(i, UP_CLERIC1);
            if (s & (1 << L_EXORCISM))
                tech_built(i, UP_CLERIC2);
            if (s & (1 << L_RUNES))
                tech_built(i, UP_CLERIC2);
            if (s & (1 << L_FLAME_SHIELD))
                tech_built(i, UP_WIZARD1);
            if (s & (1 << L_RAISE))
                tech_built(i, UP_WIZARD1);
            if (s & (1 << L_SLOW))
                tech_built(i, UP_WIZARD2);
            if (s & (1 << L_HASTE))
                tech_built(i, UP_WIZARD2);
            if (s & (1 << L_INVIS))
                tech_built(i, UP_WIZARD3);
            if (s & (1 << L_WIND))
                tech_built(i, UP_WIZARD3);
            if (s & (1 << L_POLYMORF))
                tech_built(i, UP_WIZARD4);
            if (s & (1 << L_UNHOLY))
                tech_built(i, UP_WIZARD4);
            if (s & (1 << L_BLIZZARD))
                tech_built(i, UP_WIZARD5);
            if (s & (1 << L_DD))
                tech_built(i, UP_WIZARD5);

            find_all_alive_units(U_KEEP);
            sort_stat(S_OWNER, i, CMP_EQ);
            if (units != 0)
                tech_built(i, UP_KEEP);
            find_all_alive_units(U_STRONGHOLD);
            sort_stat(S_OWNER, i, CMP_EQ);
            if (units != 0)
                tech_built(i, UP_KEEP);
            find_all_alive_units(U_CASTLE);
            sort_stat(S_OWNER, i, CMP_EQ);
            if (units != 0)
            {
                tech_built(i, UP_KEEP);
                tech_built(i, UP_CASTLE);
            }
            find_all_alive_units(U_FORTRESS);
            sort_stat(S_OWNER, i, CMP_EQ);
            if (units != 0)
            {
                tech_built(i, UP_KEEP);
                tech_built(i, UP_CASTLE);
            }
        }
    }
}

void building_start_build(int *u, byte id, byte o)
{
    ((void (*)(int *, byte, byte))F_BLDG_START_BUILD)(u, id, o);
}

void build_inventor(int *u)
{
    if (check_unit_complete(u))
    {
        byte f = *((byte *)((uintptr_t)u + S_FLAGS1));
        if (!(f & UF_BUILD_ON))
        {
            byte id = *((byte *)((uintptr_t)u + S_ID));
            byte o = *((byte *)((uintptr_t)u + S_OWNER));
            int spr = get_val(ACTIVE_SAPPERS, o);
            byte nspr = *(byte *)(AIP_NEED_SAP + 48 * o);
            if (nspr > spr)
            {
                if (id == U_INVENTOR)
                    building_start_build(u, U_DWARWES, 0);
                if (id == U_ALCHEMIST)
                    building_start_build(u, U_GOBLINS, 0);
            }
            int flr = get_val(ACTIVE_FLYER, o);
            byte nflr = *(byte *)(AIP_NEED_FLYER + 48 * o);
            if (nflr > flr)
            {
                if (id == U_INVENTOR)
                    building_start_build(u, U_FLYER, 0);
                if (id == U_ALCHEMIST)
                    building_start_build(u, U_ZEPPELIN, 0);
            }
        }
    }
}

void build_sap_fix(bool f)
{
    if (f)
    {
        char b1[] = "\x80\xfa\x40\x0";
        void (*r1)(int *) = build_inventor;
        patch_setdword((DWORD *)b1, (DWORD)r1);
        PATCH_SET((char *)BLDG_WAIT_INVENTOR, b1);       // human inv
        PATCH_SET((char *)(BLDG_WAIT_INVENTOR + 4), b1); // orc inv
    }
    else
    {
        char b1[] = "\x80\xfa\x40\x0";
        PATCH_SET((char *)BLDG_WAIT_INVENTOR, b1);       // human inv
        PATCH_SET((char *)(BLDG_WAIT_INVENTOR + 4), b1); // orc inv
    }
}

void ai_fix_plugin(bool f)
{
    if (f)
    {
        char b1[] = "\xb2\x02";
        PATCH_SET((char *)AIFIX_PEONS_REP, b1); // 2 peon rep
        char b21[] = "\xbb\x8";
        PATCH_SET((char *)AIFIX_GOLD_LUMB1, b21); // gold lumber
        char b22[] = "\xb4\x4";
        PATCH_SET((char *)AIFIX_GOLD_LUMB2, b22); // gold lumber
        char b3[] = "\x1";
        PATCH_SET((char *)AIFIX_BUILD_SIZE, b3); // packed build
        char b4[] = "\xbe\x0\x0\x0\x0\x90\x90";
        PATCH_SET((char *)AIFIX_FIND_HOME, b4); // th corner
        char b5[] = "\x90\x90";
        PATCH_SET((char *)AIFIX_DD_BLIZ_FIX, b5); // fix dd/bliz
        char b6[] = "\x90\x90\x90\x90\x90\x90";
        PATCH_SET((char *)AIFIX_POWERBUILD, b6); // powerbuild
        char b7[] = "\x90\x90\x90\x90";
        PATCH_SET((char *)AIFIX_CATA_AFRAID, b7); // cata afraid
        char b8[] = "\x70\x79";
        PATCH_SET((char *)AIFIX_SHIPS_PATROL, b8); // ships random patrol

        char m1[] = "\xa9\x0\x0\x0\x4\x74\x16\x8b\x44\x24\xc\x66\x83\x78\x44\x0\x75\xb\x90\x90\x90\x90";
        PATCH_SET((char *)AIFIX_INVIZ_COND, m1); // inviz cond
        char m2[] = "\x90\x90\x90";
        PATCH_SET((char *)AIFIX_BLIZ_3MP1, m2); // no x3 bliz mp
        PATCH_SET((char *)AIFIX_BLIZ_3MP2, m2); // no x3 bliz mp
        char m3[] = "\x33\xd2\x8a\x50\x27\x3e\x8b\x4\x95\x24\xf5\x4c\x0\xa8\x20\x75\x19\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90";
        PATCH_SET((char *)AIFIX_FIREBALL_COND, m3); // fire cond
        char m4[] = "\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90";
        PATCH_SET((char *)AIFIX_BLIZ_COND, m4); // bliz cond
        char m5[] = "\x96";
        PATCH_SET((char *)AIFIX_INV_POLY_JMP, m5); // no inv poly > jmp exit
        char m6[] = "\xe9\x6c\x2\x0\x0";
        PATCH_SET((char *)AIFIX_INV_POLY_CAVE, m6); // jmp to exit
        char m7[] = "\x90\x90";
        PATCH_SET((char *)AIFIX_RUNES_INV, m7); // runes no inviz
        char m8[] = "\xeb";
        PATCH_SET((char *)AIFIX_STARTING_MAGE, m8); // starting mage

        build_sap_fix(true);

        ai_fixed = true;
    }
    else
    {
        char b1[] = "\x8a\xd0";
        PATCH_SET((char *)AIFIX_PEONS_REP, b1); // 2 peon rep
        char b21[] = "\xd0\x7";
        PATCH_SET((char *)AIFIX_GOLD_LUMB1, b21); // gold lumber
        char b22[] = "\xf4\x1";
        PATCH_SET((char *)AIFIX_GOLD_LUMB2, b22); // gold lumber
        char b3[] = "\x6";
        PATCH_SET((char *)AIFIX_BUILD_SIZE, b3); // packed build
        char b4[] = "\xe8\xf8\x2a\x1\x0\x8b\xf0";
        PATCH_SET((char *)AIFIX_FIND_HOME, b4); // th corner
        char b5[] = "\x75\x7";
        PATCH_SET((char *)AIFIX_DD_BLIZ_FIX, b5); // fix dd/bliz
        char b6[] = "\xf\x84\x78\x1\x0\x0";
        PATCH_SET((char *)AIFIX_POWERBUILD, b6); // powerbuild
        char b7[] = "\x89\x44\x24\x10";
        PATCH_SET((char *)AIFIX_CATA_AFRAID, b7); // cata afraid
        char b8[] = "\x10\x7A";
        PATCH_SET((char *)AIFIX_SHIPS_PATROL, b8); // ships random patrol

        char m1[] = "\x80\x7a\x5e\x2\x75\x17\xa9\x0\x0\x2\x0\x75\x9\x80\xb9\xf0\xfe\x4c\x0\x1\x74\x7";
        PATCH_SET((char *)AIFIX_INVIZ_COND, m1); // inviz cond
        char m2[] = "\x8d\x4\x40";
        PATCH_SET((char *)AIFIX_BLIZ_3MP1, m2); // no x3 bliz mp
        m2[1] = '\x14';
        PATCH_SET((char *)AIFIX_BLIZ_3MP2, m2); // no x3 bliz mp
        char m3[] = "\x6a\x3\x50\x56\xe8\x3\xff\xff\xff\x83\xc4\xc\x85\xc0\x74\x12\x56\xe8\xe6\xd8\x2\x0\x66\xd1\xe8\x83\xc4\x4\x66\x39\x46\x22\x73\x8";
        PATCH_SET((char *)AIFIX_FIREBALL_COND, m3); // fire cond
        char m4[] = "\x6a\x3\x50\x51\xe8\x96\xfe\xff\xff\x83\xc4\xc\x85\xc0\x75\x07";
        PATCH_SET((char *)AIFIX_BLIZ_COND, m4); // bliz cond
        char m5[] = "\xc";
        PATCH_SET((char *)AIFIX_INV_POLY_JMP, m5); // no inv poly > jmp exit
        char m6[] = "\x90\x90\x90\x90\x90";
        PATCH_SET((char *)AIFIX_INV_POLY_CAVE, m6); // jmp to exit
        char m7[] = "\x74\x1d";
        PATCH_SET((char *)AIFIX_RUNES_INV, m7); // runes no inviz
        char m8[] = "\x74";
        PATCH_SET((char *)AIFIX_STARTING_MAGE, m8); // starting mage

        build_sap_fix(false);

        ai_fixed = false;
    }
}

PROC g_proc_0040EEDD;
void upgrade_tower(int *u, int id, int b)
{
    if (ai_fixed)
    {
        bool c = false;
        byte o = *((byte *)((uintptr_t)u + S_OWNER));
        if ((get_val(LUMBERMILL, o) == 0) && (get_val(SMITH, o) != 0))
            c = true;
        if ((get_val(LUMBERMILL, o) != 0) && (get_val(SMITH, o) != 0) && ((get_val(TOWER, o) % 2) == 0))
            c = true;
        if (c)
            id += 2;
    }
    ((void (*)(int *, int, int))g_proc_0040EEDD)(u, id, b); // original
}

PROC g_proc_00442E25;
void create_skeleton(int x, int y, int id, int o)
{
    if (ai_fixed)
    {
        int *s = unit_create_place((x / 32) + 1, y / 32, id, o % 256);
        if (s == NULL)
            unit_create((x / 32) + 1, y / 32, id, o % 256, 1);
    }
    else
        ((void (*)(int, int, int, int))g_proc_00442E25)(x, y, id, o); // original
}

PROC g_proc_00425D1C;
int *cast_raise(int *u, int a1, int a2, int a3)
{
    if (ai_fixed)
    {
        byte o = *((byte *)((uintptr_t)u + S_OWNER));
        find_all_alive_units(U_SKELETON);
        sort_stat(S_OWNER, o, CMP_EQ);
        sort_preplaced();
        if (units < 10)
        {
            if (((*(DWORD *)(SPELLS_LEARNED + 4 * o) & (1 << L_RAISE)) == 0))
                return NULL;
            byte mp = *((byte *)((uintptr_t)u + S_MANA));
            byte cost = *(byte *)(MANACOST + 2 * RAISE_DEAD);
            if (mp < cost)
                return NULL;
            byte x = *((byte *)((uintptr_t)u + S_X));
            byte y = *((byte *)((uintptr_t)u + S_Y));
            set_region((int)x - 8, (int)y - 8, (int)x + 8, (int)y + 8); // set region around myself
            find_all_units(ANY_BUILDING);                               // dead body
            sort_in_region();
            sort_hidden();
            sort_near_death();
            if (units != 0)
            {
                byte xx = *((byte *)((uintptr_t)unit[0] + S_X));
                byte yy = *((byte *)((uintptr_t)unit[0] + S_Y));
                give_order(u, xx, yy, ORDER_SPELL_RAISEDEAD);
                return unit[0];
            }
        }
        return NULL;
    }
    else
        return ((int *(*)(int *, int, int, int))g_proc_00425D1C)(u, a1, a2, a3); // original
}

PROC g_proc_00424F94;
PROC g_proc_00424FD7;
int *cast_runes(int *u, int a1, int a2, int a3)
{
    if (ai_fixed)
    {
        byte o = *((byte *)((uintptr_t)u + S_OWNER));
        if (((*(DWORD *)(SPELLS_LEARNED + 4 * o) & (1 << L_RUNES)) == 0))
            return NULL;
        byte mp = *((byte *)((uintptr_t)u + S_MANA));
        byte cost = *(byte *)(MANACOST + 2 * RUNES);
        if (mp < cost)
            return NULL;
        byte x = *((byte *)((uintptr_t)u + S_X));
        byte y = *((byte *)((uintptr_t)u + S_Y));
        set_region((int)x - 14, (int)y - 14, (int)x + 14, (int)y + 14); // set region around myself
        find_all_alive_units(ANY_MEN);
        sort_in_region();
        sort_hidden();
        sort_stat(S_MOVEMENT_TYPE, MOV_LAND, CMP_EQ);
        for (int ui = 0; ui < 16; ui++)
        {
            if (check_ally(o, ui)) // only not allied units
                sort_stat(S_OWNER, ui, CMP_NEQ);
        }
        sort_rune_near();
        if (units != 0)
        {
            byte xx = *((byte *)((uintptr_t)unit[0] + S_X));
            byte yy = *((byte *)((uintptr_t)unit[0] + S_Y));
            give_order(u, xx, yy, ORDER_SPELL_RUNES);
            return unit[0];
        }
        return NULL;
    }
    else
        return ((int *(*)(int *, int, int, int))g_proc_00424F94)(u, a1, a2, a3); // original
}

byte get_manacost(byte s)
{
    return *(byte *)(MANACOST + 2 * s);
}

PROC g_proc_0042757E;
int ai_spell(int *u)
{
    if (ai_fixed)
    {
        byte id = *((byte *)((uintptr_t)u + S_ID));
        if ((id == U_MAGE) || (id == U_DK))
        {
            byte x = *((byte *)((uintptr_t)u + S_X));
            byte y = *((byte *)((uintptr_t)u + S_Y));
            set_region((int)x - 24, (int)y - 24, (int)x + 24, (int)y + 24); // set region around myself
            find_all_alive_units(ANY_UNITS);
            sort_in_region();
            byte o = *((byte *)((uintptr_t)u + S_OWNER));
            for (int ui = 0; ui < 16; ui++)
            {
                if (check_ally(o, ui))
                    sort_stat(S_OWNER, ui, CMP_NEQ);
            }
            if (units != 0)
            {
                byte mp = *((byte *)((uintptr_t)u + S_MANA));
                byte ord = *((byte *)((uintptr_t)u + S_ORDER));
                bool new_cast = (ord == ORDER_SPELL_ROT) || (ord == ORDER_SPELL_BLIZZARD) || (ord == ORDER_SPELL_INVIS) || (ord == ORDER_SPELL_ARMOR);
                WORD shl = *((WORD *)((uintptr_t)u + S_SHIELD));
                WORD inv = *((WORD *)((uintptr_t)u + S_INVIZ));
                if ((shl == 0) && (inv == 0))
                {
                    set_region((int)x - 12, (int)y - 12, (int)x + 12, (int)y + 12); // set region around myself
                    find_all_alive_units(ANY_MEN);
                    sort_in_region();
                    for (int ui = 0; ui < 16; ui++)
                    {
                        if (check_ally(o, ui)) // enemy
                            sort_stat(S_OWNER, ui, CMP_NEQ);
                    }
                    if (units != 0)
                    {
                        struct GPOINT
                        {
                            WORD x;
                            WORD y;
                        } l;
                        l.x = *((WORD *)((uintptr_t)u + S_X));
                        l.y = *((WORD *)((uintptr_t)u + S_Y));
                        ((int (*)(int *, int, GPOINT *))F_ICE_SET_AI_ORDER)(u, AI_ORDER_DEFEND, &l);
                        set_stat(u, l.x, S_AI_DEST_X);
                        set_stat(u, l.y, S_AI_DEST_Y);
                    }
                    if (mp < 50)
                        new_cast = false;
                    else
                    {
                        if (id == U_MAGE)
                        {
                            if ((*(DWORD *)(SPELLS_LEARNED + 4 * o) & (1 << L_INVIS)) != 0)
                            {
                                if (ord != ORDER_SPELL_POLYMORPH)
                                {
                                    if (mp >= get_manacost(INVIS))
                                    {
                                        set_region((int)x - 12, (int)y - 12, (int)x + 12, (int)y + 12); // set region around myself
                                        find_all_alive_units(ANY_MEN);
                                        sort_in_region();
                                        sort_stat(S_ID, U_DK, CMP_SMALLER_EQ);
                                        sort_stat(S_ID, U_MAGE, CMP_BIGGER_EQ);
                                        sort_self(u);
                                        sort_stat(S_INVIZ, 0, CMP_EQ);
                                        sort_stat(S_MANA, 150, CMP_BIGGER_EQ);
                                        sort_stat(S_AI_ORDER, AI_ORDER_ATTACK, CMP_EQ);
                                        for (int ui = 0; ui < 16; ui++)
                                        {
                                            if (!check_ally(o, ui))
                                                sort_stat(S_OWNER, ui, CMP_NEQ);
                                        }
                                        if (units != 0)
                                        {
                                            give_order_spell_target(u, unit[0], ORDER_SPELL_INVIS);
                                            new_cast = true;
                                        }
                                    }
                                }
                            }
                            if ((*(DWORD *)(SPELLS_LEARNED + 4 * o) & (1 << L_POLYMORF)) != 0)
                            {
                                if (ord != ORDER_SPELL_POLYMORPH)
                                {
                                    if (mp >= get_manacost(POLYMORPH))
                                    {
                                        set_region((int)x - 18, (int)y - 18, (int)x + 18, (int)y + 18); // set region around myself
                                        find_all_alive_units(ANY_MEN);
                                        sort_in_region();
                                        sort_stat(S_ID, U_DRAGON, CMP_SMALLER_EQ);
                                        sort_stat(S_ID, U_GRIFON, CMP_BIGGER_EQ);
                                        for (int ui = 0; ui < 16; ui++)
                                        {
                                            if (check_ally(o, ui)) // enemy
                                                sort_stat(S_OWNER, ui, CMP_NEQ);
                                        }
                                        if (units != 0)
                                        {
                                            give_order_spell_target(u, unit[0], ORDER_SPELL_POLYMORPH);
                                            new_cast = true;
                                        }
                                    }
                                }
                            }
                        }
                        else if (id == U_DK)
                        {
                            if ((*(DWORD *)(SPELLS_LEARNED + 4 * o) & (1 << L_UNHOLY)) != 0)
                            {
                                if (ord != ORDER_SPELL_ARMOR)
                                {
                                    if (mp >= get_manacost(UNHOLY_ARMOR))
                                    {
                                        set_region((int)x - 12, (int)y - 12, (int)x + 12, (int)y + 12); // set region around myself
                                        find_all_alive_units(ANY_MEN);
                                        sort_in_region();
                                        sort_stat(S_ID, U_DK, CMP_SMALLER_EQ);
                                        sort_stat(S_ID, U_MAGE, CMP_BIGGER_EQ);
                                        sort_self(u);
                                        sort_stat(S_SHIELD, 0, CMP_EQ);
                                        sort_stat(S_MANA, 150, CMP_BIGGER_EQ);
                                        sort_stat(S_AI_ORDER, AI_ORDER_ATTACK, CMP_EQ);
                                        for (int ui = 0; ui < 16; ui++)
                                        {
                                            if (!check_ally(o, ui))
                                                sort_stat(S_OWNER, ui, CMP_NEQ);
                                        }
                                        if (units != 0)
                                        {
                                            give_order_spell_target(u, unit[0], ORDER_SPELL_ARMOR);
                                            new_cast = true;
                                        }
                                    }
                                }
                                else
                                    new_cast = true;
                            }
                            if ((*(DWORD *)(SPELLS_LEARNED + 4 * o) & (1 << L_HASTE)) != 0)
                            {
                                if (ord != ORDER_SPELL_HASTE)
                                {
                                    if (mp >= get_manacost(HASTE))
                                    {
                                        set_region((int)x - 16, (int)y - 16, (int)x + 16, (int)y + 16); // set region around myself
                                        find_all_alive_units(ANY_MEN);
                                        sort_in_region();
                                        sort_stat(S_ID, U_DK, CMP_SMALLER_EQ);
                                        sort_stat(S_ID, U_MAGE, CMP_BIGGER_EQ);
                                        sort_self(u);
                                        sort_stat(S_SHIELD, 0, CMP_NEQ);
                                        sort_stat(S_HASTE, 0, CMP_EQ);
                                        for (int ui = 0; ui < 16; ui++)
                                        {
                                            if (!check_ally(o, ui))
                                                sort_stat(S_OWNER, ui, CMP_NEQ);
                                        }
                                        if (units != 0)
                                        {
                                            give_order_spell_target(u, unit[0], ORDER_SPELL_HASTE);
                                            new_cast = true;
                                        }
                                    }
                                }
                                else
                                    new_cast = true;
                            }
                            if ((*(DWORD *)(SPELLS_LEARNED + 4 * o) & (1 << L_COIL)) != 0)
                            {
                                if (ord != ORDER_SPELL_DRAINLIFE)
                                {
                                    if (mp >= get_manacost(COIL))
                                    {
                                        set_region((int)x - 18, (int)y - 18, (int)x + 18, (int)y + 18); // set region around myself
                                        find_all_alive_units(ANY_MEN);
                                        sort_in_region();
                                        sort_stat(S_ID, U_DRAGON, CMP_SMALLER_EQ);
                                        sort_stat(S_ID, U_GRIFON, CMP_BIGGER_EQ);
                                        sort_stat(S_ANIMATION, ANIM_MOVE, CMP_NEQ);
                                        for (int ui = 0; ui < 16; ui++)
                                        {
                                            if (check_ally(o, ui)) // enemy
                                                sort_stat(S_OWNER, ui, CMP_NEQ);
                                        }
                                        if (units != 0)
                                        {
                                            byte xx = *((byte *)((uintptr_t)unit[0] + S_X));
                                            byte yy = *((byte *)((uintptr_t)unit[0] + S_Y));
                                            give_order(u, xx, yy, ORDER_SPELL_DRAINLIFE);
                                            new_cast = true;
                                        }
                                    }
                                }
                            }
                        }
                    }
                }
                else
                {
                    set_region((int)x - 50, (int)y - 50, (int)x + 50, (int)y + 50); // set region around myself
                    find_all_alive_units(ANY_MEN);
                    sort_in_region();
                    sort_stat(S_ID, U_PEON, CMP_SMALLER_EQ);
                    sort_stat(S_ID, U_PEASANT, CMP_BIGGER_EQ);
                    sort_peon_loaded(0);
                    sort_hidden();
                    for (int ui = 0; ui < 16; ui++)
                    {
                        if (check_ally(o, ui))
                            sort_stat(S_OWNER, ui, CMP_NEQ); // enemy peons
                    }
                    if (units == 0)
                    {
                        find_all_alive_units(ANY_BUILDING);
                        sort_in_region();
                        sort_stat(S_ID, U_FORTRESS, CMP_SMALLER_EQ); // fortres castle stronghold keep
                        sort_stat(S_ID, U_KEEP, CMP_BIGGER_EQ);
                        for (int ui = 0; ui < 16; ui++)
                        {
                            if (check_ally(o, ui))
                                sort_stat(S_OWNER, ui, CMP_NEQ);
                        }
                        if (units == 0)
                        {
                            find_all_alive_units(ANY_BUILDING);
                            sort_in_region();
                            sort_stat(S_ID, U_GREAT_HALL, CMP_SMALLER_EQ);
                            sort_stat(S_ID, U_TOWN_HALL, CMP_BIGGER_EQ);
                            for (int ui = 0; ui < 16; ui++)
                            {
                                if (check_ally(o, ui))
                                    sort_stat(S_OWNER, ui, CMP_NEQ);
                            }
                        }
                    }
                    if (units != 0)
                    {
                        if (id == U_MAGE)
                        {
                            if ((*(DWORD *)(SPELLS_LEARNED + 4 * o) & (1 << L_BLIZZARD)) != 0)
                            {
                                if (ord != ORDER_SPELL_BLIZZARD)
                                {
                                    if (mp >= get_manacost(BLIZZARD))
                                    {
                                        byte tx = *((byte *)((uintptr_t)unit[0] + S_X));
                                        byte ty = *((byte *)((uintptr_t)unit[0] + S_Y));
                                        give_order(u, tx, ty, ORDER_SPELL_BLIZZARD);
                                        new_cast = true;
                                    }
                                }
                                else
                                    new_cast = true;
                            }
                        }
                        if (id == U_DK)
                        {
                            if ((*(DWORD *)(SPELLS_LEARNED + 4 * o) & (1 << L_DD)) != 0)
                            {
                                if (ord != ORDER_SPELL_ROT)
                                {
                                    if (mp >= get_manacost(DEATH_AND_DECAY))
                                    {
                                        byte tx = *((byte *)((uintptr_t)unit[0] + S_X));
                                        byte ty = *((byte *)((uintptr_t)unit[0] + S_Y));
                                        give_order(u, tx, ty, ORDER_SPELL_ROT);
                                        new_cast = true;
                                    }
                                }
                                else
                                    new_cast = true;
                            }
                            else if ((*(DWORD *)(SPELLS_LEARNED + 4 * o) & (1 << L_WIND)) != 0)
                            {
                                if (!new_cast && (ord != ORDER_SPELL_WHIRLWIND))
                                {
                                    if (mp >= get_manacost(WHIRLWIND))
                                    {
                                        byte tx = *((byte *)((uintptr_t)unit[0] + S_X));
                                        byte ty = *((byte *)((uintptr_t)unit[0] + S_Y));
                                        give_order(u, tx, ty, ORDER_SPELL_WHIRLWIND);
                                        new_cast = true;
                                    }
                                }
                                else
                                    new_cast = true;
                            }
                        }
                    }
                }
                if (!new_cast)
                    return ((int (*)(int *))g_proc_0042757E)(u); // original
            }
        }
        else
            return ((int (*)(int *))g_proc_0042757E)(u); // original
        return 0;
    }
    else
        return ((int (*)(int *))g_proc_0042757E)(u); // original
}

PROC g_proc_00427FAE;
void ai_attack(int *u, int b, int a)
{
    if (ai_fixed)
    {
        byte o = *((byte *)((uintptr_t)u + S_OWNER));
        for (int i = 0; i < 16; i++)
        {
            int *p = (int *)(UNITS_LISTS + 4 * i);
            if (p)
            {
                p = (int *)(*p);
                while (p)
                {
                    bool f = ((*((byte *)((uintptr_t)p + S_ID)) == U_MAGE) || (*((byte *)((uintptr_t)p + S_ID)) == U_DK));
                    if (f)
                    {
                        if (!check_unit_dead(p) && !check_unit_hidden(p))
                        {
                            byte ow = *((byte *)((uintptr_t)p + S_OWNER));
                            if (ow == o)
                            {
                                if ((*(byte *)(CONTROLER_TYPE + o) == C_COMP))
                                {
                                    WORD inv = *((WORD *)((uintptr_t)p + S_INVIZ));
                                    if (inv == 0)
                                    {
                                        byte aor = *((byte *)((uintptr_t)p + S_AI_ORDER));
                                        if (aor != AI_ORDER_ATTACK)
                                        {
                                            byte mp = *((byte *)((uintptr_t)p + S_MANA));
                                            if (mp >= 200)
                                            {
                                                ((void (*)(int *, int, int))F_ICE_SET_AI_ORDER)(p, AI_ORDER_ATTACK, a); // ai attack
                                            }
                                        }
                                    }
                                }
                            }
                        }
                    }
                    p = (int *)(*((int *)((uintptr_t)p + S_NEXT_UNIT_POINTER)));
                }
            }
        }

        find_all_alive_units(ANY_MEN);
        sort_stat(S_ID, U_GOBLINS, CMP_SMALLER_EQ);
        sort_stat(S_ID, U_DWARWES, CMP_BIGGER_EQ);
        sort_stat(S_OWNER, o, CMP_EQ);
        sort_stat(S_AI_ORDER, AI_ORDER_ATTACK, CMP_NEQ); // not attack already
        for (int i = 0; i < units; i++)
        {
            ((void (*)(int *, int, int))F_ICE_SET_AI_ORDER)(unit[i], AI_ORDER_ATTACK, a); // ai attack
        }

        find_all_alive_units(ANY_MEN);
        sort_stat(S_ID, U_PEON, CMP_SMALLER_EQ);
        sort_stat(S_ID, U_PEASANT, CMP_BIGGER_EQ);
        sort_stat(S_OWNER, o, CMP_EQ);
        sort_stat(S_AI_ORDER, AI_ORDER_ATTACK, CMP_EQ); // attack already
        if (units == 0)
        {
            find_all_alive_units(ANY_MEN);
            sort_stat(S_ID, U_PEON, CMP_SMALLER_EQ);
            sort_stat(S_ID, U_PEASANT, CMP_BIGGER_EQ);
            sort_stat(S_OWNER, o, CMP_EQ);
            sort_stat(S_AI_ORDER, AI_ORDER_GUARD, CMP_EQ); // guard already
            if (units == 0)
            {
                find_all_alive_units(ANY_MEN);
                sort_stat(S_ID, U_PEON, CMP_SMALLER_EQ);
                sort_stat(S_ID, U_PEASANT, CMP_BIGGER_EQ);
                sort_stat(S_OWNER, o, CMP_EQ);
                if (units > 8)
                {
                    sort_hidden();
                    if (units != 0)
                    {
                        unset_peon_ai_flags(unit[0]);
                        ((void (*)(int *, int, int))F_ICE_SET_AI_ORDER)(unit[0], AI_ORDER_ATTACK, a); // ai attack
                    }
                }
            }
        }
    }
    ((void (*)(int *, int, int))g_proc_00427FAE)(u, b, a); // original
}

PROC g_proc_00451255;
void transport_unit_place(int *u)
{
    ((void (*)(int *))g_proc_00451255)(u); // original
    if (ai_fixed)
    {
        byte o = *((byte *)((uintptr_t)u + S_OWNER));
        byte id = *((byte *)((uintptr_t)u + S_ID));
        byte aio = *((byte *)((uintptr_t)u + S_AI_ORDER));
        if ((*(byte *)(CONTROLER_TYPE + o) == C_COMP))
        {
            if ((id == U_PEON) || (id == U_PEASANT))
            {
                struct GPOINT
                {
                    WORD x;
                    WORD y;
                } l;
                l.x = *((WORD *)((uintptr_t)u + S_X));
                l.y = *((WORD *)((uintptr_t)u + S_Y));
                ((int (*)(int *, int, GPOINT *))F_ICE_SET_AI_ORDER)(u, AI_ORDER_GUARD, &l);
                set_stat(u, l.x, S_AI_DEST_X);
                set_stat(u, l.y, S_AI_DEST_Y);
            }
        }
    }
}

void sap_behaviour()
{
    for (int i = 0; i < 16; i++)
    {
        int *p = (int *)(UNITS_LISTS + 4 * i);
        if (p)
        {
            p = (int *)(*p);
            while (p)
            {
                bool f = ((*((byte *)((uintptr_t)p + S_ID)) == U_DWARWES) || (*((byte *)((uintptr_t)p + S_ID)) == U_GOBLINS));
                if (f)
                {
                    if (!check_unit_dead(p) && !check_unit_hidden(p))
                    {
                        byte o = *((byte *)((uintptr_t)p + S_OWNER));
                        if ((*(byte *)(CONTROLER_TYPE + o) == C_COMP))
                        {
                            byte ord = *((byte *)((uintptr_t)p + S_ORDER));
                            byte x = *((byte *)((uintptr_t)p + S_X));
                            byte y = *((byte *)((uintptr_t)p + S_Y));
                            if ((ord != ORDER_DEMOLISH) && (ord != ORDER_DEMOLISH_NEAR) && (ord != ORDER_DEMOLISH_AT))
                            {
                                set_region((int)x - 12, (int)y - 12, (int)x + 12, (int)y + 12); // set region around myself
                                find_all_alive_units(ANY_UNITS);
                                sort_in_region();
                                sort_stat(S_MOVEMENT_TYPE, MOV_LAND, CMP_EQ);
                                for (int ui = 0; ui < 16; ui++)
                                {
                                    if (check_ally(o, ui)) // only not allied units
                                        sort_stat(S_OWNER, ui, CMP_NEQ);
                                }
                                if (units != 0)
                                {
                                    int ord = *(int *)(ORDER_FUNCTIONS + 4 * ORDER_DEMOLISH);
                                    ((void (*)(int *, int, int, int *, int))F_GIVE_ORDER)(p, 0, 0, unit[0], ord);
                                }
                                set_region((int)x - 5, (int)y - 5, (int)x + 5, (int)y + 5); // set region around myself
                                find_all_alive_units(ANY_UNITS);
                                sort_in_region();
                                sort_stat(S_MOVEMENT_TYPE, MOV_LAND, CMP_EQ);
                                for (int ui = 0; ui < 16; ui++)
                                {
                                    if (check_ally(o, ui)) // only not allied units
                                        sort_stat(S_OWNER, ui, CMP_NEQ);
                                }
                                if (units != 0)
                                {
                                    int ord = *(int *)(ORDER_FUNCTIONS + 4 * ORDER_DEMOLISH);
                                    ((void (*)(int *, int, int, int *, int))F_GIVE_ORDER)(p, 0, 0, unit[0], ord);
                                }
                                set_region((int)x - 1, (int)y - 1, (int)x + 1, (int)y + 1); // set region around myself
                                find_all_alive_units(ANY_UNITS);
                                sort_in_region();
                                sort_stat(S_MOVEMENT_TYPE, MOV_LAND, CMP_EQ);
                                for (int ui = 0; ui < 16; ui++)
                                {
                                    if (check_ally(o, ui)) // only not allied units
                                        sort_stat(S_OWNER, ui, CMP_NEQ);
                                }
                                if (units != 0)
                                {
                                    int ord = *(int *)(ORDER_FUNCTIONS + 4 * ORDER_DEMOLISH);
                                    ((void (*)(int *, int, int, int *, int))F_GIVE_ORDER)(p, 0, 0, unit[0], ord);
                                }
                            }
                        }
                    }
                }
                p = (int *)(*((int *)((uintptr_t)p + S_NEXT_UNIT_POINTER)));
            }
        }
    }
}

void unstuk()
{
    for (int i = 0; i < 16; i++)
    {
        int *p = (int *)(UNITS_LISTS + 4 * i);
        if (p)
        {
            p = (int *)(*p);
            while (p)
            {
                byte id = *((byte *)((uintptr_t)p + S_ID));
                byte ord = *((byte *)((uintptr_t)p + S_ORDER));
                byte aord = *((byte *)((uintptr_t)p + S_AI_ORDER));
                bool f = ((((id < U_CRITTER)) && !check_unit_preplaced(p) && (ord == ORDER_STOP) && (aord == AI_ORDER_ATTACK)) ||
                          ((id == U_PEASANT) || (id == U_PEON)));
                if (*(byte *)GB_MULTIPLAYER)
                {
                    if (!((id == U_PEASANT) || (id == U_PEON)))
                        f = false;
                }
                if (f)
                {
                    if (!check_unit_dead(p) && !check_unit_hidden(p))
                    {
                        byte o = *((byte *)((uintptr_t)p + S_OWNER));
                        if ((*(byte *)(CONTROLER_TYPE + o) == C_COMP))
                        {
                            byte st = *((byte *)((uintptr_t)p + S_NEXT_FIRE));
                            byte frm = *((byte *)((uintptr_t)p + S_FRAME));
                            byte pfrm = *((byte *)((uintptr_t)p + S_NEXT_FIRE + 1));
                            if (st == 0)
                            {
                                byte map = *(byte *)MAP_SIZE - 1;
                                byte x = *((byte *)((uintptr_t)p + S_X));
                                byte y = *((byte *)((uintptr_t)p + S_Y));
                                int xx = 0, yy = 0, dir = 0;
                                xx += x;
                                yy += y;
                                dir = ((int (*)())F_NET_RANDOM)();
                                dir %= 8;
                                if (dir == 0)
                                {
                                    if (yy > 0)
                                        yy -= 1;
                                }
                                if (dir == 1)
                                {
                                    if (yy > 0)
                                        yy -= 1;
                                    if (xx < map)
                                        xx += 1;
                                }
                                if (dir == 2)
                                {
                                    if (xx < map)
                                        xx += 1;
                                }
                                if (dir == 3)
                                {
                                    if (xx < map)
                                        xx += 1;
                                    if (yy < map)
                                        yy += 1;
                                }
                                if (dir == 4)
                                {
                                    if (yy < map)
                                        yy += 1;
                                }
                                if (dir == 5)
                                {
                                    if (yy < map)
                                        yy += 1;
                                    if (xx > 0)
                                        xx -= 1;
                                }
                                if (dir == 6)
                                {
                                    if (xx > 0)
                                        xx -= 1;
                                }
                                if (dir == 7)
                                {
                                    if (xx > 0)
                                        xx -= 1;
                                    if (yy > 0)
                                        yy -= 1;
                                }
                                if ((id != U_PEON) && (id != U_PEASANT))
                                {
                                    struct GPOINT
                                    {
                                        WORD x;
                                        WORD y;
                                    };
                                    int *trg = NULL;
                                    find_all_alive_units(ANY_UNITS);
                                    sort_hidden();
                                    sort_attack_can_hit_range(p);
                                    for (int ui = 0; ui < 16; ui++)
                                    {
                                        if (check_ally(o, ui)) // only not allied units
                                            sort_stat(S_OWNER, ui, CMP_NEQ);
                                    }
                                    if (units != 0)
                                    {
                                        WORD dist = 0xFFFF;
                                        int ndu = -1;
                                        for (int j = 0; j < units; j++)
                                        {
                                            GPOINT l;
                                            l.x = *((WORD *)((uintptr_t)p + S_X));
                                            l.y = *((WORD *)((uintptr_t)p + S_Y));
                                            WORD dst = 0;
                                            dst = ((WORD(*)(GPOINT *, int *))F_MTX_DIST)(&l, unit[j]); // mtx dist
                                            if (dst < dist)
                                            {
                                                dist = dst;
                                                ndu = j;
                                            }
                                        }
                                        if (ndu != -1)
                                            trg = unit[ndu];
                                    }
                                    if (trg)
                                    {
                                        GPOINT l;
                                        l.x = *((WORD *)((uintptr_t)trg + S_X));
                                        l.y = *((WORD *)((uintptr_t)trg + S_Y));
                                        units = 1;
                                        unit[0] = trg;
                                        sort_attack_can_hit_range(p);
                                        int sa = 0;
                                        if (units != 0)
                                            sa = ((int (*)(int *, int, GPOINT *))F_ICE_SET_AI_ORDER)(p, AI_ORDER_ATTACK, &l);
                                        if (sa)
                                            give_order(p, xx % 256, yy % 256, ORDER_ATTACK_AREA);
                                    }
                                }
                                else
                                    give_order(p, xx % 256, yy % 256, ORDER_MOVE);
                                st = 10;
                            }
                            if (st > 0)
                                st -= 1;
                            if (frm != pfrm)
                                st = 255;
                            set_stat(p, st, S_NEXT_FIRE);
                            set_stat(p, frm, S_NEXT_FIRE + 1);
                        }
                    }
                }
                p = (int *)(*((int *)((uintptr_t)p + S_NEXT_UNIT_POINTER)));
            }
        }
    }
}

void goldmine_ai()
{
    for (int i = 0; i < 16; i++)
    {
        int *p = (int *)(UNITS_LISTS + 4 * i);
        if (p)
        {
            p = (int *)(*p);
            while (p)
            {
                bool f = ((*((byte *)((uintptr_t)p + S_ID)) == U_MINE));
                if (f)
                {
                    if (check_unit_complete(p))
                    {
                        byte x = *((byte *)((uintptr_t)p + S_X));
                        byte y = *((byte *)((uintptr_t)p + S_Y));
                        set_region((int)x - 9, (int)y - 9, (int)x + 8, (int)y + 8);
                        find_all_alive_units(ANY_BUILDING_4x4);
                        sort_in_region();
                        sort_stat(S_ID, U_PORTAL, CMP_NEQ);
                        bool th = units != 0;
                        byte x1, y1, x2, y2;
                        if (x > 3)
                            x1 = x - 3;
                        else
                            x1 = 0;
                        if (y > 3)
                            y1 = y - 3;
                        else
                            y1 = 0;
                        x += 3;
                        y += 3;
                        if (x >= (127 - 3))
                            x2 = 127;
                        else
                            x2 = x + 3;
                        if (y >= (127 - 3))
                            y2 = 127;
                        else
                            y2 = y + 3;
                        char *sq = (char *)*(int *)(MAP_SQ_POINTER);
                        byte mxs = *(byte *)MAP_SIZE; // map size
                        for (int xx = x1; xx < x2; xx++)
                        {
                            for (int yy = y1; yy < y2; yy++)
                            {
                                char buf[] = "\x0";
                                buf[0] = *(char *)(sq + 2 * xx + 2 * yy * mxs + 1);
                                if (th)
                                    buf[0] |= SQ_AI_BUILDING >> 8;
                                else
                                    buf[0] &= ~(SQ_AI_BUILDING >> 8);
                                PATCH_SET((char *)(sq + 2 * xx + 2 * yy * mxs + 1), buf);
                            }
                        }
                    }
                }
                p = (int *)(*((int *)((uintptr_t)p + S_NEXT_UNIT_POINTER)));
            }
        }
    }
}

void unit_timer()
{
    // timer for transport and runestone heal
    find_all_alive_units(ANY_MEN);
    sort_hidden();
    sort_stat(S_ID, U_HTRANSPORT, CMP_NEQ);
    sort_stat(S_ID, U_OTRANSPORT, CMP_NEQ);
    for (int i = 0; i < units; i++)
    {
        byte r = *(byte *)((uintptr_t)unit[i] + S_KILLS + 1);
        if (r > 0)
            r -= 1;
        set_stat(unit[i], r, S_KILLS + 1);
    }
}

PROC g_proc_0045271B;
void update_spells()
{
    ((void (*)())g_proc_0045271B)(); // original
    // this function called every game tick
    // you can place your self-writed functions here in the end if need
    bullet_disp_fires();
    unit_timer();
    if (A_portal)
        portal();
    if (A_transport)
        transport();
    if (A_autoheal)
        paladin();
    if (A_runestone)
        runestone();
    for (int i = 0; i < 255; i++)
    {
        if (m_slow_aura[i] != 255)
            slow_aura(m_slow_aura[i]);
        else
            i = 256;
    }
    for (int i = 0; i < 255; i++)
    {
        if (m_death_aura[i] != 255)
            death_aura(m_death_aura[i]);
        else
            i = 256;
    }
    for (int i = 0; i < 255; i++)
    {
        if (m_sneak[i] != 255)
            sneak(m_sneak[i]);
        else
            i = 256;
    }
    if (saveload_fixed)
        tech_reinit();
    if (ai_fixed)
    {
        sap_behaviour();
        unstuk();
        goldmine_ai();
    }
    if (vizs_n > 0)
    {
        for (int i = 0; i < vizs_n; i++)
        {
            viz_area(vizs_areas[i].x, vizs_areas[i].y, vizs_areas[i].p, vizs_areas[i].s);
        }
    }
    // PLACE your new functions here

    // PLACE your new functions here
}

void seq_change(int *u, byte tt)
{
    // change animation speeds
    // USE this function CAREFULLY
    byte t = tt;
    /*
    if (FALSE)//remove this and USE this function CAREFULLY
    {
        if (t == 1)
        {
            byte t = *((byte*)((uintptr_t)u + S_ANIMATION_TIMER));
            byte id = *((byte*)((uintptr_t)u + S_ID));
            if (id == U_ARCHER)
            {
                byte a = *((byte*)((uintptr_t)u + S_ANIMATION));
                byte f = *((byte*)((uintptr_t)u + S_FRAME));
                if (a == ANIM_MOVE)
                    if (t > 1)t -= 1;//all archer movement timers will be 1 frame faster
                //but cannot be less than 1 !!!!!!!!!!
                if (a == ANIM_ATTACK)
                {
                    if (f != 0)
                    {
                        if (t > 5)t -= 5;
                        else t = 1;//ultra fast shooting for archer
                    }
                    else
                    {
                        if (t > 25)t -= 25;
                        else t = 1;//ultra fast shooting for archer
                    }
                    set_stat(u, 1, S_ATTACK_COUNTER);//ultra fast shooting for archer
                }
                set_stat(u, t, S_ANIMATION_TIMER);
            }
        }
    }
    */
}

PROC g_proc_004522B9;
int seq_run(int *u)
{
    byte t = *((byte *)((uintptr_t)u + S_ANIMATION_TIMER));
    int original = ((int (*)(int *))g_proc_004522B9)(u); // original
    seq_change(u, t);
    return original;
}

PROC g_proc_00409F3B,                 // action
    g_proc_0040AF70, g_proc_0040AF99, // demolish
    g_proc_0041038E, g_proc_00410762, // bullet
    g_proc_004428AD;                  // spell
char fdmg = 0;                        // final damage
void damage(int *atk, int *trg, char dmg)
{
    fdmg = dmg;
    if ((trg != NULL) && (atk != NULL))
    {
        if (!check_unit_dead(trg))
        {
            byte aid = *((byte *)((uintptr_t)atk + S_ID)); // attacker id
            byte tid = *((byte *)((uintptr_t)trg + S_ID)); // target id
            byte dmg2 = dmg;                               // can deal damage by default
            int i = 0;
            while (i < 255)
            {
                if ((tid == ut[i]) && (aid != ua[i]))
                {
                    dmg2 = 0; // canot deal damage
                }
                if ((tid == ut[i]) && (aid == ua[i])) // check if only some certain units can attack that unit
                {
                    dmg2 = dmg; // can deal damage
                    i = 255;
                }
                i++;
                if (ua[i] == 255) // pairs must go in a row
                {
                    i = 255;
                }
            }
            if (*((WORD *)((uintptr_t)trg + S_SHIELD)) != 0)
                dmg2 = 0;
            if (blood_f)
            {
                int bdmg = dmg2;
                if (*((WORD *)((uintptr_t)atk + S_BLOOD)) != 0)
                    bdmg *= 2;
                if (bdmg > 255)
                    bdmg = 255;
                if (bdmg != 0)
                    dmg2 = bdmg % 256;
            }
            fdmg = dmg2;
            if (agr)
                comp_aggro(trg, atk); // check if allied comps go agro
            if (fdmg != 0)
            {
                if (cpt || ucpt)
                    if (capture(trg, atk))
                        fdmg = 0; // check if buildings or units captured on low hp
                if (steal)
                    steal_res(trg, atk);
                if (manaburn)
                    mana_burn(trg, atk);
                if (peon_steal)
                    peon_steal_attack(trg, atk);
                bool f = false;
                for (int i = 0; i < 255; i++)
                {
                    if ((m_devotion[i] != 255) && (!f))
                        f = devotion_aura(trg, m_devotion[i]);
                    else
                        i = 256;
                }
                if (f) // defence
                {
                    dmg2 = fdmg;
                    if (dmg2 > 3)
                        dmg2 -= 3;
                    else
                        dmg2 = 0;
                    fdmg = dmg2;
                }
                f = false;
                for (int i = 0; i < 255; i++)
                {
                    if ((m_prvnt[i] != 255) && (!f))
                        f = m_prvnt[i] == tid;
                    else
                        i = 256;
                }
                if (f) // prevent
                {
                    WORD hp = *((WORD *)((uintptr_t)trg + S_HP));
                    if (hp > 0)
                    {
                        if (hp <= fdmg)
                        {
                            fdmg = 0;
                            set_stat(trg, 300, S_SHIELD);
                            set_stat(trg, 0, S_HP);
                            flame(trg);
                        }
                    }
                }
                f = false;
                for (int i = 0; i < 255; i++)
                {
                    if ((m_vampire[i] != 255) && (!f))
                        f = vamp_aura(trg, atk, m_vampire[i]);
                    else
                        i = 256;
                }
                if (f) // vampire
                {
                    WORD hp = *((WORD *)((uintptr_t)trg + S_HP));
                    byte mult = 2; // 2=50%
                    if (hp > (fdmg / mult + 1))
                        heal(atk, (fdmg / mult + 1), 0);
                    else
                        heal(atk, hp % 256, 0);
                    WORD xx = *((WORD *)((uintptr_t)atk + S_DRAW_X));
                    WORD yy = *((WORD *)((uintptr_t)atk + S_DRAW_Y));
                    ((void (*)(WORD, WORD, byte))F_BULLET_CREATE)(xx + 16, yy + 16, B_SHOT_FIRE); // create effect
                }
            }
            WORD hp = *((WORD *)((uintptr_t)trg + S_HP)); // unit hp
            if ((tid < U_FARM) && (fdmg >= hp))
            {
                if ((aid != U_HTRANSPORT) || (aid != U_OTRANSPORT))
                {
                    byte k = *((byte *)((uintptr_t)atk + S_KILLS));
                    if (k < 255)
                        k += 1;
                    set_stat(atk, (int)k, S_KILLS);
                    byte to = *((byte *)((uintptr_t)trg + S_OWNER));
                    byte ao = *((byte *)((uintptr_t)atk + S_OWNER));
                    if ((tid <= U_OWALL) && (to <= 15) && (ao <= 15))
                    {
                        if (all_kills[ao * 16 * U_OWALL + to * U_OWALL + tid] < 255)
                            all_kills[ao * 16 * U_OWALL + to * U_OWALL + tid]++;
                    }
                }
            }
        }
    }
}

void damage1(int *atk, int *trg, char dmg)
{
    damage(atk, trg, dmg);
    ((void (*)(int *, int *, char))g_proc_00409F3B)(atk, trg, fdmg);
}

void damage2(int *atk, int *trg, char dmg)
{
    damage(atk, trg, dmg);
    ((void (*)(int *, int *, char))g_proc_0041038E)(atk, trg, fdmg);
}

void damage3(int *atk, int *trg, char dmg)
{
    damage(atk, trg, dmg);
    ((void (*)(int *, int *, char))g_proc_0040AF70)(atk, trg, fdmg);
}

void damage4(int *atk, int *trg, char dmg)
{
    damage(atk, trg, dmg);
    ((void (*)(int *, int *, char))g_proc_0040AF99)(atk, trg, fdmg);
}

void damage5(int *atk, int *trg, char dmg)
{
    damage(atk, trg, dmg);
    ((void (*)(int *, int *, char))g_proc_00410762)(atk, trg, fdmg);
}

void damage6(int *atk, int *trg, char dmg)
{
    damage(atk, trg, dmg);
    ((void (*)(int *, int *, char))g_proc_004428AD)(atk, trg, fdmg);
}

void tower_set_target(int *p, int x, int y)
{
    set_stat(p, 0, S_RETARGET_X1 - 2);
    set_stat(p, 0, S_RETARGET_X1 - 1);
    int *u = NULL;
    set_region(x - 3, y - 3, x, y);
    find_all_alive_units(ANY_BUILDING_4x4);
    sort_in_region();
    sort_hidden();
    sort_attack_can_hit(p);
    if (units != 0)
        u = unit[0];
    set_region(x - 2, y - 2, x, y);
    find_all_alive_units(ANY_BUILDING_3x3);
    sort_in_region();
    sort_hidden();
    sort_attack_can_hit(p);
    if (units != 0)
        u = unit[0];
    set_region(x - 1, y - 1, x, y);
    find_all_alive_units(ANY_BUILDING_2x2);
    sort_in_region();
    sort_hidden();
    sort_attack_can_hit(p);
    if (units != 0)
        u = unit[0];
    set_region(x, y, x, y);
    find_all_alive_units(ANY_UNITS);
    sort_in_region();
    sort_hidden();
    sort_attack_can_hit(p);
    if (units != 0)
        u = unit[0];
    if (u)
    {
        int fx = ((int (*)(int *, int))F_UNIT_FIXUP)(u, 1); // fixup save
        set_stat(p, (int)fx % 256, S_RETARGET_X1 - 2);
        set_stat(p, (int)fx / 256, S_RETARGET_X1 - 1);
    }
}

PROC g_proc_0043BAE1;
void rc_snd(int *p)
{
    // acknowlegement sound
    //(game crash cause buildings not have this sounds so this hooked function fix it)
    if (*((byte *)((uintptr_t)p + S_ID)) < U_FARM)
        ((void (*)(int *))g_proc_0043BAE1)(p);
}

PROC g_proc_0043B943;
void rc_build_click(int *p, int x, int y, int *t, int a)
{
    byte id = *(byte *)((uintptr_t)p + S_ID);
    if (id >= U_FARM)
    {
        if ((id == U_HARROWTOWER) || (id == U_OARROWTOWER) || (id == U_HCANONTOWER) || (id == U_OCANONTOWER) || (id == U_HTOWER) || (id == U_OTOWER))
        {
            tower_set_target(p, x, y);
        }
        else
        {
            set_stat(p, x | 128, S_RETARGET_X1 - 2);
            set_stat(p, y, S_RETARGET_X1 - 1);
            set_stat(p, 0, S_RETARGET_ORDER - 1);
            bool h = false;
            set_region(x - 3, y - 3, x, y);
            find_all_alive_units(U_MINE);
            sort_in_region();
            if (units != 0)
                h = true;
            find_all_alive_units(ANY_BUILDING);
            sort_stat(S_ID, U_HPLATFORM, CMP_BIGGER_EQ);
            sort_stat(S_ID, U_OPLATFORM, CMP_SMALLER_EQ);
            sort_in_region();
            if (units != 0)
                h = true;

            struct GPOINT
            {
                WORD x;
                WORD y;
            };
            GPOINT l;
            l.x = (WORD)x;
            l.y = (WORD)y;
            int tr1 = ((int (*)(GPOINT *))F_TILE_IS_TREE)(&l);
            int tr2 = ((int (*)(GPOINT *))F_TILE_IS_CHOPPING_TREE)(&l);
            if ((tr1 == 1) || (tr2 == 1))
                h = true;
            if (h)
                set_stat(p, 1, S_RETARGET_ORDER - 1);
        }
    }
    else
        ((void (*)(int *, int, int, int *, int))g_proc_0043B943)(p, x, y, t, a); // original
}

void rc_jmp(bool b)
{
    if (b)
    {
        char r[] = "\xf\x84\xa2\x0\x0\x0";
        PATCH_SET((char *)RIGHT_CLICK_1, r);
        patch_ljmp((char *)RIGHT_CLICK_CODE_CAVE, (char *)RIGHT_CLICK_2);
    }
    else
    {
        char r[] = "\xf\x84\x8b\x0\x0\x0";
        PATCH_SET((char *)RIGHT_CLICK_1, r);
    }
}

void brclik(bool b)
{
    if (b)
    {
        char r[] = "\x90\x90\x90\x90\x90\x90";
        PATCH_SET((char *)RIGHT_CLICK_ALLOW_BUILDINGS, r);
    }
    else
    {
        char r[] = "\xf\x84\x26\x01\x0\x0";
        PATCH_SET((char *)RIGHT_CLICK_ALLOW_BUILDINGS, r);
    }
}

PROC g_proc_0040DF71;
int *bld_unit_create(int a1, int a2, int a3, byte a4, int *a5)
{
    int *b = (int *)*(int *)UNIT_RUN_UNIT_POINTER;
    int *u = ((int *(*)(int, int, int, byte, int *))g_proc_0040DF71)(a1, a2, a3, a4, a5);
    if (b)
    {
        if (u)
        {
            byte x = *((byte *)((uintptr_t)b + S_RETARGET_X1 - 2));
            byte y = *((byte *)((uintptr_t)b + S_RETARGET_X1 - 1));
            byte ro = *((byte *)((uintptr_t)b + S_RETARGET_ORDER - 1));
            byte bp = x & 128;
            if (bp != 0)
            {
                x &= ~128;
                byte uid = *((byte *)((uintptr_t)u + S_ID));
                byte o = ORDER_ATTACK_AREA;
                if ((uid == U_PEON) || (uid == U_PEASANT) || (uid == U_HTANKER) || (uid == U_OTANKER))
                {
                    if (ro == 1)
                        o = ORDER_HARVEST;
                }
                give_order(u, x, y, o);
                set_stat(u, x, S_RETARGET_X1);
                set_stat(u, y, S_RETARGET_Y1);
                set_stat(u, o, S_RETARGET_ORDER);
            }
            if (ai_fixed)
            {
                byte o = *((byte *)((uintptr_t)u + S_OWNER));
                byte m = *((byte *)((uintptr_t)u + S_MANA));
                if ((*(byte *)(CONTROLER_TYPE + o) == C_COMP))
                {
                    if (m == 0x55) // 85 default starting mana
                    {
                        char buf[] = "\xA0"; // 160
                        PATCH_SET((char *)u + S_MANA, buf);
                    }
                }
            }
        }
    }
    return u;
}

PROC g_proc_0040AFBF;
int *tower_find_attacker(int *p)
{
    int *tr = NULL;
    byte id = *((byte *)((uintptr_t)p + S_ID));
    if ((id == U_HARROWTOWER) || (id == U_OARROWTOWER) || (id == U_HCANONTOWER) || (id == U_OCANONTOWER))
    {
        byte a1 = *((byte *)((uintptr_t)p + S_RETARGET_X1 - 2));
        byte a2 = *((byte *)((uintptr_t)p + S_RETARGET_X1 - 1));
        tr = (int *)(a1 + 256 * a2);
        tr = ((int *(*)(int *, int))F_UNIT_FIXUP)(tr, 0); // fixup load
        if (tr)
        {
            if (!check_unit_near_death(tr) && !check_unit_dead(tr) && !check_unit_hidden(tr))
            {
                int a = ((int (*)(int *, int *))F_ATTACK_CAN_HIT)(p, tr);
                if (a != 0)
                {
                    byte id = *((byte *)((uintptr_t)tr + S_ID));
                    byte szx = *(byte *)(UNIT_SIZE_TABLE + 4 * id);
                    byte szy = *(byte *)(UNIT_SIZE_TABLE + 4 * id + 2);
                    byte idd = *((byte *)((uintptr_t)p + S_ID));
                    byte rng = *(byte *)(UNIT_RANGE_TABLE + idd);
                    byte ms = *(byte *)MAP_SIZE;
                    byte xx = *((byte *)((uintptr_t)tr + S_X));
                    byte yy = *((byte *)((uintptr_t)tr + S_Y));
                    byte x1 = *((byte *)((uintptr_t)p + S_X));
                    byte y1 = *((byte *)((uintptr_t)p + S_Y));
                    byte x2 = x1;
                    byte y2 = y1;
                    if (x1 < rng)
                        x1 = 0;
                    else
                        x1 -= rng;
                    if (y1 < rng)
                        y1 = 0;
                    else
                        y1 -= rng;
                    if ((x2 + rng + 1) > ms)
                        x2 = ms;
                    else
                        x2 += rng + 1;
                    if ((y2 + rng + 1) > ms)
                        y2 = ms;
                    else
                        y2 += rng + 1;
                    if (!((xx >= x1) && (xx <= x2) && (yy >= y1) && (yy <= y2)))
                        tr = NULL;
                }
            }
            else
                tr = NULL;
        }
    }
    if (!tr)
        return ((int *(*)(int *))g_proc_0040AFBF)(p); // original
    else
    {
        return tr;
    }
}

PROC g_proc_00451728;
void unit_kill_deselect(int *u)
{
    int *ud = u;
    ((void (*)(int *))g_proc_00451728)(u); // original
    ((void (*)())F_STATUS_REDRAW)();       // status redraw
    for (int i = 0; i < 16; i++)
    {
        int *p = (int *)(UNITS_LISTS + 4 * i);
        if (p)
        {
            p = (int *)(*p);
            while (p)
            {
                byte id = *((byte *)((uintptr_t)p + S_ID));
                bool f = ((id == U_HARROWTOWER) || (id == U_OARROWTOWER) || (id == U_HCANONTOWER) || (id == U_OCANONTOWER));
                bool f2 = ((id == U_DWARWES) || (id == U_GOBLINS));
                if (f)
                {
                    if (!check_unit_dead(p) && check_unit_complete(p))
                    {
                        byte a1 = *((byte *)((uintptr_t)p + S_RETARGET_X1));
                        byte a2 = *((byte *)((uintptr_t)p + S_RETARGET_X1 + 1));
                        byte a3 = *((byte *)((uintptr_t)p + S_RETARGET_X1 + 2));
                        byte a4 = *((byte *)((uintptr_t)p + S_RETARGET_X1 + 3));
                        int *tr = (int *)(a1 + 256 * a2 + 256 * 256 * a3 + 256 * 256 * 256 * a4);
                        if (tr == ud)
                        {
                            set_stat(p, 0, S_RETARGET_X1);
                            set_stat(p, 0, S_RETARGET_X1 + 1);
                            set_stat(p, 0, S_RETARGET_X1 + 2);
                            set_stat(p, 0, S_RETARGET_X1 + 3);
                        }
                    }
                }
                if (f2)
                {
                    if (ai_fixed)
                    {
                        if (!check_unit_dead(p))
                        {
                            byte a1 = *((byte *)((uintptr_t)p + S_ORDER_UNIT_POINTER));
                            byte a2 = *((byte *)((uintptr_t)p + S_ORDER_UNIT_POINTER + 1));
                            byte a3 = *((byte *)((uintptr_t)p + S_ORDER_UNIT_POINTER + 2));
                            byte a4 = *((byte *)((uintptr_t)p + S_ORDER_UNIT_POINTER + 3));
                            int *tr = (int *)(a1 + 256 * a2 + 256 * 256 * a3 + 256 * 256 * 256 * a4);
                            if (tr == ud)
                            {
                                set_stat(p, 0, S_ORDER_UNIT_POINTER);
                                set_stat(p, 0, S_ORDER_UNIT_POINTER + 1);
                                set_stat(p, 0, S_ORDER_UNIT_POINTER + 2);
                                set_stat(p, 0, S_ORDER_UNIT_POINTER + 3);
                                give_order(p, 0, 0, ORDER_STOP);
                            }
                        }
                    }
                }
                p = (int *)(*((int *)((uintptr_t)p + S_NEXT_UNIT_POINTER)));
            }
        }
    }
}

void multicast_fix(bool f)
{
    if (f)
    {
        char rep[] = "\xeb";
        PATCH_SET((char *)MULTICAST_FIX, rep);
    }
    else
    {
        char rep[] = "\x74";
        PATCH_SET((char *)MULTICAST_FIX, rep);
    }
}

PROC g_proc_0044FF20;
PROC g_proc_0044FFE6;
int pathfind_mov(int *um, int s)
{
    if (path_fixed)
    {
        byte x = *((byte *)((uintptr_t)um + S_X - S_MOV_PATH01 + 1));
        byte y = *((byte *)((uintptr_t)um + S_Y - S_MOV_PATH01 + 1));
        byte ox = *((byte *)((uintptr_t)um + S_ORDER_X - S_MOV_PATH01 + 1));
        byte oy = *((byte *)((uintptr_t)um + S_ORDER_Y - S_MOV_PATH01 + 1));
        byte xx = abs(x - ox);
        byte yy = abs(y - oy);
        byte ss = ((xx < yy) ? xx : yy);
        if (ss <= 8)
            s = 1;
    }
    return ((int (*)(int *, int))g_proc_0044FF20)(um, s); // original
}

void pathfind_fix(bool f)
{
    if (f)
    {
        char rep[] = "\x1"; // cheap path
        PATCH_SET((char *)PATHFIND_CHEAP_PATH, rep);
        char rep2[] = "\xeb\x41\x90";
        PATCH_SET((char *)PATHFIND_FIX, rep2);
        char rep3[] = "\x66\x81\xf9\x0\x4\xeb\xb8"; // 1024 buffer
        PATCH_SET((char *)PATHFIND_FIX_BUFFER, rep3);
        path_fixed = true;
    }
    else
    {
        char rep[] = "\x7";
        PATCH_SET((char *)PATHFIND_CHEAP_PATH, rep);
        char rep2[] = "\x83\xf9\x32";
        PATCH_SET((char *)PATHFIND_FIX, rep2);
        char rep3[] = "\x90\x90\x90\x90\x90\x90\x90";
        PATCH_SET((char *)PATHFIND_FIX_BUFFER, rep3);
        path_fixed = false;
    }
}

void allow_table(byte p, int t, byte n, byte a)
{
    if (t == 0)
        t = ALLOWED_UNITS + (4 * p) + (n / 8);
    else if (t == 1)
        t = ALLOWED_UPGRADES + (4 * p) + (n / 8);
    else if (t == 2)
        t = ALLOWED_SPELLS + (4 * p) + (n / 8);
    else
        t = SPELLS_LEARNED + (4 * p) + (n / 8);
    byte b = *(byte *)t;
    if (a == 1)
        b |= (1 << (n % 8));
    else
        b &= (~(1 << (n % 8)));
    char buf[] = "\xff";
    buf[0] = b;
    PATCH_SET((char *)t, buf);
}

void draw_stats_fix(bool b)
{
    if (b)
    {
        char buf[] = "\xa0\x5b";
        PATCH_SET((char *)DEMON_STATS_DRAW, buf);   // demon
        PATCH_SET((char *)CRITTER_STATS_DRAW, buf); // critter
    }
    else
    {
        char buf[] = "\xf0\x57";
        PATCH_SET((char *)DEMON_STATS_DRAW, buf);   // demon
        PATCH_SET((char *)CRITTER_STATS_DRAW, buf); // critter
    }
}

void send_cheat(byte c)
{
    int b = *(int *)CHEATBITS;
    if ((b & (1 << c)) != 0)
        b &= ~(1 << c);
    else
        b |= 1 << c;
    ((void (*)(int))F_SEND_CHEAT_PACKET)(b);
}

void rec_autoheal()
{
    byte p = *(byte *)LOCAL_PLAYER; // player
    byte b = *(byte *)(SPELLS_LEARNED + 4 * p);
    byte sp = GREATER_HEAL;
    if ((b & (1 << sp)) != 0)
        b &= ~(1 << sp);
    else
        b |= 1 << sp;
    char buf[] = "\x0";
    buf[0] = b;
    PATCH_SET((char *)(SPELLS_LEARNED + 4 * p), buf);

    if ((*(byte *)(SPELLS_LEARNED + 4 * (p)) & (1 << L_GREATER_HEAL)) != 0)
    {
        churc[20 * 3 + 2] = '\x5b'; // icon
        char msg[] = "autoheal\x5 enabled";
        show_message(10, msg);
    }
    else
    {
        churc[20 * 3 + 2] = '\x6d'; // icon
        char msg[] = "autoheal\x3 disabled";
        show_message(10, msg);
    }
    ((void (*)())F_STATUS_REDRAW)();
}

void rec_peons()
{
    int *p = NULL;
    byte l = *(byte *)LOCAL_PLAYER; // player
    p = (int *)(UNITS_SELECTED + 9 * 4 * l);
    if (p)
    {
        p = (int *)(*p);
        if (p)
        {
            byte id = *((byte *)((uintptr_t)p + S_ID));
            bool fid = (id == U_TOWN_HALL) || (id == U_GREAT_HALL) ||
                       (id == U_KEEP) || (id == U_STRONGHOLD) ||
                       (id == U_CASTLE) || (id == U_FORTRESS);
            if (fid)
            {
                byte o = *((byte *)((uintptr_t)p + S_OWNER));
                if (o == l)
                {
                    byte x = *((byte *)((uintptr_t)p + S_X));
                    byte y = *((byte *)((uintptr_t)p + S_Y));
                    set_region((int)x - 5, (int)y - 5, (int)x + 8, (int)y + 8); // set region around myself rad 5
                    find_all_alive_units(ANY_MEN);
                    sort_in_region();
                    sort_hidden();
                    sort_stat(S_OWNER, o, CMP_EQ); // my owner
                    sort_stat(S_ID, U_ATTACK_PEASANT, CMP_BIGGER_EQ);
                    sort_stat(S_ID, U_ATTACK_PEON, CMP_SMALLER_EQ);
                    if (units != 0)
                    {
                        for (int i = 0; i < units; i++)
                        {
                            byte id = *((byte *)((uintptr_t)unit[i] + S_ID)); // unit id
                            WORD mhp = *(WORD *)(UNIT_HP_TABLE + 2 * id);     // max hp
                            WORD hp = *((WORD *)((uintptr_t)unit[i] + S_HP)); // unit hp
                            byte idd = id % 2;
                            WORD mhp2 = *(WORD *)(UNIT_HP_TABLE + 2 * (U_PEASANT + idd)); // max hp
                            int thp = hp + (mhp2 - mhp);
                            if (thp < 1)
                                thp = 1;
                            hp = thp % (256 * 256);
                            set_stat(unit[i], hp, S_HP);
                            set_stat(unit[i], (U_PEASANT + idd), S_ID);
                            set_stat(unit[i], (U_PEASANT + idd), S_COMMANDS);
                            set_stat(unit[i], (U_PEASANT + idd), S_SPRITE);
                            set_stat(unit[i], ORDER_STOP, S_NEXT_ORDER);
                        }
                        char msg[] = "workers attack mode\x3 disabled";
                        show_message(10, msg);
                    }
                    else
                    {
                        find_all_alive_units(ANY_MEN);
                        sort_in_region();
                        sort_hidden();
                        sort_stat(S_OWNER, o, CMP_EQ); // my owner
                        sort_stat(S_ID, U_PEASANT, CMP_BIGGER_EQ);
                        sort_stat(S_ID, U_PEON, CMP_SMALLER_EQ);
                        if (units != 0)
                        {
                            for (int i = 0; i < units; i++)
                            {
                                byte id = *((byte *)((uintptr_t)unit[i] + S_ID)); // unit id
                                WORD mhp = *(WORD *)(UNIT_HP_TABLE + 2 * id);     // max hp
                                WORD hp = *((WORD *)((uintptr_t)unit[i] + S_HP)); // unit hp
                                byte idd = id % 2;
                                WORD mhp2 = *(WORD *)(UNIT_HP_TABLE + 2 * (U_ATTACK_PEASANT + idd)); // max hp
                                int thp = hp + (mhp2 - mhp);
                                if (thp < 1)
                                    thp = 1;
                                hp = thp % (256 * 256);
                                set_stat(unit[i], hp, S_HP);
                                set_stat(unit[i], (U_ATTACK_PEASANT + idd), S_ID);
                                set_stat(unit[i], (U_ATTACK_PEASANT + idd), S_COMMANDS);
                                set_stat(unit[i], (U_PEASANT + idd), S_SPRITE);
                                set_stat(unit[i], ORDER_STOP, S_NEXT_ORDER);
                            }
                            char msg[] = "workers attack mode\x5 enabled";
                            show_message(10, msg);
                        }
                    }
                }
            }
        }
    }
    ((void (*)())F_STATUS_REDRAW)();
}

PROC g_proc_0045614E;
void receive_cheat(int c, int a1)
{
    // received cheat packet
    bool f = true;
    if ((c & (1 << 8)) != 0) // 8 - autoheal
    {
        rec_autoheal();
        f = false;
    }
    if ((c & (1 << 9)) != 0) // 9 - attack peons
    {
        rec_peons();
        f = false;
    }
    if (f)
    {
        ((void (*)(int, int))g_proc_0045614E)(c, a1); // orig
    }
    else
    {
        char buf[] = "\x0";
        PATCH_SET((char *)PLAYER_CHEATED, buf);
    }
}

void button_autoheal(int)
{
    send_cheat(8);
    if ((*(byte *)(SPELLS_LEARNED + 4 * (*(byte *)LOCAL_PLAYER)) & (1 << L_GREATER_HEAL)) != 0)
        churc[20 * 3 + 2] = '\x5b'; // icon
    else
        churc[20 * 3 + 2] = '\x6d'; // icon
    ((void (*)())F_STATUS_REDRAW)();
}

void autoheal(bool b)
{
    if (b)
    {
        if ((*(byte *)(SPELLS_LEARNED + 4 * (*(byte *)LOCAL_PLAYER)) & (1 << L_GREATER_HEAL)) != 0)
            churc[20 * 3 + 2] = '\x5b'; // icon
        else
            churc[20 * 3 + 2] = '\x6d'; // icon

        void (*r1)(int) = button_autoheal;
        patch_setdword((DWORD *)(churc + (20 * 3 + 8)), (DWORD)r1);

        char b1[] = "\04\x0\x0\x0\x68\x37\x4a\x0";
        char *repf = churc;
        patch_setdword((DWORD *)(b1 + 4), (DWORD)repf);
        PATCH_SET((char *)CHURCH_BUTTONS, b1);
        A_autoheal = true;
    }
    else
    {
        char b1[] = "\03\x0\x0\x0\x68\x37\x4a\x0";
        PATCH_SET((char *)CHURCH_BUTTONS, b1);
        A_autoheal = false;
    }
}

void button_peons(int)
{
    send_cheat(9);
    ((void (*)())F_STATUS_REDRAW)();
}

void th_change(bool b)
{
    // add new buttons to TH
    if (b)
    {
        heroes(true);
        char hth[] = "\x5";
        PATCH_SET((char *)HUMAN_TH1_BUTTONS, hth);
        PATCH_SET((char *)HUMAN_TH2_BUTTONS, hth);
        PATCH_SET((char *)HUMAN_TH3_BUTTONS, hth);
        char oth[] = "\x5\x0\x0\x0\x20\xf7\x48\x0";
        char *repf = oth_build;
        patch_setdword((DWORD *)(oth + 4), (DWORD)repf);
        PATCH_SET((char *)ORC_TH1_BUTTONS, oth);
        PATCH_SET((char *)ORC_TH2_BUTTONS, oth);
        PATCH_SET((char *)ORC_TH3_BUTTONS, oth);

        bool af = false;
        for (int i = 0; i < 8; i++)
        {
            if (heros[i] != 0)
                af = true;
        }
        if (af)
        {
            patch_setdword((DWORD *)(hth_build + 4), (DWORD)F_ALWAYS_TRUE);
        }
        else
        {
            int (*repf)(byte) = empty_false;
            patch_setdword((DWORD *)(hth_build + 4), (DWORD)repf);
        }
        af = false;
        for (int i = 8; i < 16; i++)
        {
            if (heros[i] != 0)
                af = true;
        }
        if (af)
        {
            patch_setdword((DWORD *)(oth_build + 64), (DWORD)F_ALWAYS_TRUE);
        }
        else
        {
            int (*repf)(byte) = empty_false;
            patch_setdword((DWORD *)(oth_build + 64), (DWORD)repf);
        }

        if (apn) // if can build attack peons
        {
            patch_setdword((DWORD *)(hth_build + 24), (DWORD)F_ALWAYS_TRUE);
            patch_setdword((DWORD *)(oth_build + 84), (DWORD)F_ALWAYS_TRUE);

            void (*repf)(int) = button_peons;
            patch_setdword((DWORD *)(hth_build + 28), (DWORD)repf);
            patch_setdword((DWORD *)(oth_build + 88), (DWORD)repf);

            char bufb[] = "\xa\x0\x0\x0\x38\x1e\x4a\x0\xa\x0\x0\x0\x0\x1f\x4a\x0";
            PATCH_SET((char *)HUMAN_TH_ONE_BUTTON, bufb);
        }
        else
        {
            int (*repf)(byte) = empty_false;
            patch_setdword((DWORD *)(hth_build + 24), (DWORD)repf);
            patch_setdword((DWORD *)(oth_build + 84), (DWORD)repf);

            char bufb[] = "\x2\x0\x0\x0\xc8\x1f\x4a\x0\x2\x0\x0\x0\xf0\x1f\x4a\x0";
            PATCH_SET((char *)HUMAN_TH_ONE_BUTTON, bufb);
        }

        PATCH_SET((char *)HUMAN_TH_COMMON, hth_build);
    }
    else
    {
        heroes(false);
        char hth[] = "\x3";
        PATCH_SET((char *)HUMAN_TH1_BUTTONS, hth);
        PATCH_SET((char *)HUMAN_TH2_BUTTONS, hth);
        PATCH_SET((char *)HUMAN_TH3_BUTTONS, hth);
        char oth[] = "\x3\x0\x0\x0\x78\x34\x4a\x0";
        PATCH_SET((char *)ORC_TH1_BUTTONS, oth);
        PATCH_SET((char *)ORC_TH2_BUTTONS, oth);
        PATCH_SET((char *)ORC_TH3_BUTTONS, oth);
        char p1[] = "\x0\x0\x0\x0\x0\x0\x1\x0\xa0\x44\x44\x0\xd0\xe6\x40\x0\x1\x3\xd\x1\x0\x0\x0\x0\x1\x0\x43\x0\xb0\x42\x44\x0\x10\xe7\x40\x0\x0\x59\x3e\x1\x0\x0\x0\x0";
        PATCH_SET((char *)HUMAN_TH_COMMON, p1);
        char bufb[] = "\x2\x0\x0\x0\xc8\x1f\x4a\x0\x2\x0\x0\x0\xf0\x1f\x4a\x0";
        PATCH_SET((char *)HUMAN_TH_ONE_BUTTON, bufb);
    }
}

void call_default_kill() // default kill all victory
{
    byte l = *(byte *)LOCAL_PLAYER;
    if (!slot_alive(l))
        lose(true);
    else
    {
        if (!check_opponents(l))
            win(true);
    }
}

void def_stat(byte u, WORD hp, byte str, byte prc, byte arm, byte rng, byte gold, byte lum, byte oil, byte time)
{
    // change some unit stats (changes for ALL units of this type)

    /*
    to change vision and multiselectable you can use this construction
    char buf[] = "\x0\x0\x0\x0";//fix vision
    patch_setdword((DWORD*)buf, (DWORD)F_VISION6);
    PATCH_SET((char*)(UNIT_VISION_FUNCTIONS_TABLE + 4 * U_DEMON), buf);
    char buf2[] = "\x1";
    PATCH_SET((char*)(UNIT_MULTISELECTABLE + U_DEMON), buf2);
    */

    char buf2[] = "\x0\x0";
    buf2[0] = hp % 256;
    buf2[1] = hp / 256;
    PATCH_SET((char *)(UNIT_HP_TABLE + 2 * u), buf2);
    char buf[] = "\x0";
    buf[0] = str;
    PATCH_SET((char *)(UNIT_STRENGTH_TABLE + u), buf);
    buf[0] = prc;
    PATCH_SET((char *)(UNIT_PIERCE_TABLE + u), buf);
    buf[0] = arm;
    PATCH_SET((char *)(UNIT_ARMOR_TABLE + u), buf);
    buf[0] = rng;
    PATCH_SET((char *)(UNIT_RANGE_TABLE + u), buf);
    buf[0] = gold;
    PATCH_SET((char *)(UNIT_GOLD_TABLE + u), buf);
    buf[0] = lum;
    PATCH_SET((char *)(UNIT_LUMBER_TABLE + u), buf);
    buf[0] = oil;
    PATCH_SET((char *)(UNIT_OIL_TABLE + u), buf);
    buf[0] = time;
    PATCH_SET((char *)(UNIT_TIME_TABLE + u), buf);
}

PROC g_proc_004526FE;
void grow_structure(int *p)
{
    ((void (*)(int *))g_proc_004526FE)(p); // original
    if (p)
    {
        byte id = (*(byte *)((uintptr_t)p + S_ID));
        if ((id == U_FARM) || (id == U_PIGFARM))
        {
            if (check_unit_complete(p))
            {
                byte cid = (*(byte *)((uintptr_t)p + S_RESOURCES));
                if (((id == U_FARM) && (cid == U_PEON)) || ((id == U_PIGFARM) && (cid == U_PEASANT)))
                {                                                // if peon created human farm OR peasant created orc farm - its not normal so its considered to grow forest
                    byte pg = *(byte *)(UNIT_GOLD_TABLE + id);   // gold
                    byte pl = *(byte *)(UNIT_LUMBER_TABLE + id); // lumber
                    byte po = *(byte *)(UNIT_OIL_TABLE + id);    // oil
                    byte o = (*(byte *)((uintptr_t)p + S_OWNER));
                    change_res(o, 0, pg, 10); // return gold
                    change_res(o, 1, pl, 10); // return lumber
                    change_res(o, 2, po, 10); // return oil

                    int price = 500;
                    if (cmp_res(o, 0, price % 256, price / 256, 0, 0, CMP_BIGGER_EQ))
                    {
                        change_res(o, 3, 1, price); // get 1000 gold
                        byte x = (*(byte *)((uintptr_t)p + S_X));
                        byte y = (*(byte *)((uintptr_t)p + S_Y));
                        byte mxs = *(byte *)MAP_SIZE;                                                      // map size
                        byte era = *(byte *)MAP_ERA;                                                       // map era
                        char *cel = (char *)*(int *)(MAP_CELLS_POINTER);                                   // map cells
                        char *sq = (char *)*(int *)(MAP_SQ_POINTER);                                       // map cells
                        char *reg = (char *)*(int *)(MAP_REG_POINTER);                                     // map reg
                        char bufte[] = "\x88\x84\x82\x81\x81\x83\x7F\x85\x81\x83\x7F\x86\x81\x83\x7F\x85"; // 4 era tiles
                        char buft[] = "\x7E\x0";
                        for (int xx = x; (xx < x + 2) && (xx < mxs); xx++)
                        {
                            for (int yy = y; (yy < y + 2) && (yy < mxs); yy++)
                            {
                                if ((xx == x) && (yy == y))
                                    buft[0] = bufte[era * 4];
                                if ((xx == x + 1) && (yy == y))
                                    buft[0] = bufte[era * 4 + 1];
                                if ((xx == x) && (yy == y + 1))
                                    buft[0] = bufte[era * 4 + 2];
                                if ((xx == x + 1) && (yy == y + 1))
                                    buft[0] = bufte[era * 4 + 3];
                                PATCH_SET((char *)(cel + 2 * xx + 2 * yy * mxs), buft);
                                char buf[] = "\x81\x0"; // unpassable land
                                PATCH_SET((char *)(sq + 2 * xx + 2 * yy * mxs), buf);
                                buf[0] = '\xFE'; // tree
                                buf[1] = '\xFF'; // tree
                                PATCH_SET((char *)(reg + 2 * xx + 2 * yy * mxs), buf);
                            }
                        }
                    }
                    byte flg = (*(byte *)((uintptr_t)p + S_FLAGS3));
                    flg |= SF_HIDDEN; // hide
                    set_stat(p, flg, S_FLAGS3);
                    ((void (*)(int *))F_DEAD_BUILDING_REDRAW)(p); // unitdraw insert dead bldg
                    unit_kill(p);
                }
            }
        }
        if (id == U_MINE)
        {
            if (check_unit_complete(p))
            {
                give(p, P_NEUTRAL);
                set_stat(p, P_NEUTRAL, S_COLOR);
                WORD mhp = *(WORD *)(UNIT_HP_TABLE + 2 * id); // max hp
                if (mhp > 300)
                {
                    set_stat(p, 300, S_HP);
                }
                int r = ((int (*)())F_NET_RANDOM)();
                r %= 50;
                r += 50;
                set_stat(p, r, S_RESOURCES);

                byte x = (*(byte *)((uintptr_t)p + S_X));
                byte y = (*(byte *)((uintptr_t)p + S_Y));
                byte mxs = *(byte *)MAP_SIZE;                    // map size
                byte era = *(byte *)MAP_ERA;                     // map era
                char *cel = (char *)*(int *)(MAP_CELLS_POINTER); // map cells
                for (int xx = x; (xx < x + 3) && (xx < mxs); xx++)
                {
                    for (int yy = y; (yy < y + 3) && (yy < mxs); yy++)
                    {
                        char buf[] = "\x4b\x1";
                        if (era == 0)
                            buf[0] = 0x4e; // forest
                        if (era == 1)
                            buf[0] = 0x4b; // winter
                        if (era == 2)
                            buf[0] = 0x4a; // wast
                        if (era == 3)
                            buf[0] = 0x50; // swamp
                        PATCH_SET((char *)(cel + 2 * xx + 2 * yy * mxs), buf);
                    }
                }
            }
        }
        // if (id==U_ ??) you can add other building if need
    }
}

PROC g_proc_00418FFE;
int *create_building(int a, int b, int id, int c)
{
    int *cr = (int *)*(int *)UNIT_RUN_UNIT_POINTER;                               // unit who created building
    int *original = ((int *(*)(int, int, int, int))g_proc_00418FFE)(a, b, id, c); // original function
    if (original)                                                                 // created building
    {
        if (cr) // unit who created building
        {
            if ((id == U_FARM) || (id == U_PIGFARM))
            {
                byte cid = *((byte *)((uintptr_t)cr + S_ID));
                set_stat(original, cid, S_RESOURCES); // set id of creator (peon or pesant)
            }
            if (id == U_CIRCLE)
            {
                WORD xx = *((WORD *)((uintptr_t)cr + S_DRAW_X));
                WORD yy = *((WORD *)((uintptr_t)cr + S_DRAW_Y));
                ((void (*)(WORD, WORD, byte))F_BULLET_CREATE)(xx + 16, yy + 16, 21);
                unit_kill(cr); // sacrifice peon
            }
        }
    }
    return original;
}

PROC g_proc_0043A974; // placing
PROC g_proc_0043ABAB; // draw
WORD placebox_query(int *p, WORD x, WORD y, byte id)
{
    // function that checking map cells when placing building
    WORD original = ((WORD(*)(int *, WORD, WORD, byte))g_proc_0043A974)(p, x, y, id); // original
    byte mxs = *(byte *)MAP_SIZE;                                                     // map size
    byte era = *(byte *)MAP_ERA;                                                      // map era
    char *cel = (char *)*(int *)(MAP_CELLS_POINTER);                                  // map cells
    byte pid = *((byte *)((uintptr_t)p + S_ID));                                      // builder
    if (((id == U_FARM) && (pid == U_PEON)) || ((id == U_PIGFARM) && (pid == U_PEASANT)))
    {                   // make new forest to grow
        WORD dr = 0x7E; // destroyed forest
        for (int xx = x; (xx < x + 2) && (xx < mxs); xx++)
        {
            for (int yy = y; (yy < y + 2) && (yy < mxs); yy++)
            {
                WORD c = *(WORD *)(cel + 2 * xx + 2 * yy * mxs);
                if (c != dr)
                {
                    char buf[] = "\x7";
                    PATCH_SET((char *)(CAN_PLACE_TBL + (xx - x) + 4 * (yy - y)), buf); // can place tbl
                    original = 7;
                }
            }
        }
    }
    if (id == U_MINE)
    {
        WORD dr = 0xA1; // destroyed rock
        if (era == 0)
            dr = 0xA6; // forest
        if (era == 1)
            dr = 0xA1; // winter
        if (era == 2)
            dr = 0xA3; // wast
        if (era == 3)
            dr = 0xA1; // swamp
        for (int xx = x; (xx < x + 3) && (xx < mxs); xx++)
        {
            for (int yy = y; (yy < y + 3) && (yy < mxs); yy++)
            {
                WORD c = *(WORD *)(cel + 2 * xx + 2 * yy * mxs);
                if (c != dr)
                {
                    char buf[] = "\x7";
                    PATCH_SET((char *)(CAN_PLACE_TBL + (xx - x) + 4 * (yy - y)), buf); // can place tbl
                    original = 7;
                }
            }
        }
    }
    return original;
}

PROC g_proc_00424745; // entering
PROC g_proc_004529C0; // grow struct
int goods_into_inventory(int *p)
{
    // use this function if you need to change amount of res peon or tanker bring back
    if (TRUE)
    {
        int tr = (*(int *)((uintptr_t)p + S_ORDER_UNIT_POINTER));
        if (tr != 0)
        {
            bool f = false;
            int *trg = (int *)tr;
            byte o = *((byte *)((uintptr_t)p + S_OWNER));
            byte id = *((byte *)((uintptr_t)p + S_ID));
            byte tid = *((byte *)((uintptr_t)trg + S_ID));
            byte pf = *((byte *)((uintptr_t)p + S_PEON_FLAGS));
            int pflag = *(int *)(UNIT_GLOBAL_FLAGS + id * 4);
            int tflag = *(int *)(UNIT_GLOBAL_FLAGS + tid * 4);
            int res = 100;
            if (pf & PEON_LOADED)
            {
                if (((pflag & IS_SHIP) != 0) && ((tflag & IS_OILRIG) == 0))
                {
                    int r = get_val(REFINERY, o);
                    if (more_res)
                    {
                        res = 100 + 25 * r;
                    }
                    else
                    {
                        if (r != 0)
                        {
                            res = 125;
                        }
                        else
                        {
                            res = 100;
                        }
                    }
                    change_res(o, 2, 1, res);
                    add_total_res(o, 2, 1, res);
                    f = true;
                }
                else
                {
                    if (((tflag & IS_TOWNHALL) != 0) || ((tflag & IS_LUMBER) != 0))
                    {
                        if (((tflag & IS_TOWNHALL) != 0))
                        {
                            pf |= PEON_IN_CASTLE;
                            set_stat(p, pf, S_PEON_FLAGS);
                        }
                        if (((pf & PEON_HARVEST_GOLD) != 0) && ((tflag & IS_TOWNHALL) != 0))
                        {
                            int r2 = get_val(TH2, o);
                            int r3 = get_val(TH3, o);
                            if (more_res)
                            {
                                res = 100 + 10 * r2 + 20 * r3;
                            }
                            else
                            {
                                if (r3 != 0)
                                {
                                    res = 120;
                                }
                                else
                                {
                                    if (r2 != 0)
                                    {
                                        res = 110;
                                    }
                                    else
                                    {
                                        res = 100;
                                    }
                                }
                            }
                            pf &= ~PEON_HARVEST_GOLD;
                            change_res(o, 0, 1, res);
                            add_total_res(o, 0, 1, res);
                            f = true;
                        }
                        else
                        {
                            if (((pf & PEON_HARVEST_LUMBER) != 0))
                            {
                                int r = get_val(LUMBERMILL, o);
                                if (more_res)
                                {
                                    res = 100 + 25 * r;
                                }
                                else
                                {
                                    if (r != 0)
                                    {
                                        res = 125;
                                    }
                                    else
                                    {
                                        res = 100;
                                    }
                                }
                                pf &= ~PEON_HARVEST_LUMBER;
                                change_res(o, 1, 1, res);
                                add_total_res(o, 1, 1, res);
                                f = true;
                            }
                        }
                    }
                }
            }
            if (f)
            {
                pf &= ~PEON_LOADED;
                set_stat(p, pf, S_PEON_FLAGS);
                ((void (*)(int *))F_GROUP_SET)(p);
                return 1;
            }
        }
        return 0;
    }
    else
        return ((int (*)(int *))g_proc_00424745)(p); // original
}

void sounds_ready_table_set(byte id, WORD snd)
{
    char buf[] = "\x0\x0";
    buf[0] = snd % 256;
    buf[1] = snd / 256;
    PATCH_SET((char *)(UNIT_SOUNDS_READY_TABLE + 2 * id), buf);
}

void sounds_tables()
{
    sounds_ready_table_set(U_CRITTER, 247);
    sounds_ready_table_set(U_DEMON, 301);

    sounds_ready_table_set(U_ALLERIA, 254);
    sounds_ready_table_set(U_DANATH, 263);
    sounds_ready_table_set(U_HADGAR, 272);
    sounds_ready_table_set(U_KURDRAN, 281);
    sounds_ready_table_set(U_TYRALYON, 290);
    sounds_ready_table_set(U_UTER, 154);
    sounds_ready_table_set(U_LOTHAR, 155);

    sounds_ready_table_set(U_DEATHWING, 300);
    sounds_ready_table_set(U_DENTARG, 310);
    sounds_ready_table_set(U_GROM, 318);
    sounds_ready_table_set(U_KARGATH, 327);
    sounds_ready_table_set(U_TERON, 336);
    sounds_ready_table_set(U_ZULJIN, 204);
    sounds_ready_table_set(U_CHOGAL, 179);
    sounds_ready_table_set(U_GULDAN, 88);
}

PROC g_proc_0044A65C;
int status_get_tbl(void *tbl, WORD str_id)

{
    int *u = (int *)*(int *)UNIT_STATUS_TBL;
    void *new_tbl = NULL;
    //-------------------------------------------------
    if (u != NULL)
    {
        if (str_id < U_EMPTY_BUTTONS)
        {
            byte id = *((byte *)((uintptr_t)u + S_ID));
            if (id == U_PEASANT)
            {
                if ((*(byte *)LEVEL_OBJ == LVL_HUMAN1) || (*(byte *)LEVEL_OBJ == LVL_HUMAN2))
                {
                    new_tbl = tbl_name1;
                    str_id = 1;
                }
            }
            else if (id == U_DANATH)
            {
                new_tbl = tbl_name1;
                str_id = 1;
            }
            else if (id == U_TYRALYON)
            {
                new_tbl = tbl_name1;
                str_id = 1;
            }
            else if (id == U_UTER)
            {
                new_tbl = tbl_name2;
                str_id = 1;
            }
            else if (id == U_LOTHAR)
            {
                new_tbl = tbl_name3;
                str_id = 1;
            }
            else if (id == U_GRUNT)
            {
                new_tbl = tbl_name4;
                str_id = 1;
            }
            else if (id == U_RANGER)
            {
                new_tbl = tbl_name5;
                str_id = 1;
            }
            else if (id == U_OGRE)
            {
                new_tbl = tbl_name6;
                str_id = 1;
            }
            else if (id == U_DENTARG)
            {
                new_tbl = tbl_name7;
                str_id = 1;
            }
        }
    }
    //-------------------------------------------------

    if (new_tbl)
        return ((int (*)(void *, int))g_proc_0044A65C)(new_tbl, str_id);
    else
        return ((int (*)(void *, int))g_proc_0044A65C)(tbl, str_id); // original
}

int *hover_unit;

PROC g_proc_0044AC83;
void unit_hover_get_id(int a, int *b)
{
    if (b != NULL)
    {
        byte id = *((byte *)((uintptr_t)b + 0x20));
        hover_unit = (int *)*(int *)(LOCAL_UNIT_SELECTED_PANEL + 4 * id);
    }
    else
        hover_unit = NULL;
    ((void (*)(int, int *))g_proc_0044AC83)(a, b); // original
}

PROC g_proc_0044AE27;
int unit_hover_get_tbl(void *tbl, WORD str_id)
{
    void *new_tbl = NULL;
    //-------------------------------------------------
    int *u = hover_unit;
    if (u != NULL)
    {
        byte id = *((byte *)((uintptr_t)u + S_ID));
        if (id == U_PEON)
        {
            new_tbl = tbl_name1;
            str_id = 1;
        }
        else if (id == U_UTER)
        {
            new_tbl = tbl_name2;
            str_id = 1;
        }
        else if (id == U_LOTHAR)
        {
            new_tbl = tbl_name3;
            str_id = 1;
        }
        else if (id == U_GRUNT)
        {
            new_tbl = tbl_name4;
            str_id = 1;
        }
    }
    //-------------------------------------------------
    if (new_tbl)
        return ((int (*)(void *, int))g_proc_0044AE27)(new_tbl, str_id);
    else
        return ((int (*)(void *, int))g_proc_0044AE27)(tbl, str_id); // original
}

char tbl_kill[] = "Kill all";

PROC g_proc_004354C8;
int objct_get_tbl_custom(void *tbl, WORD str_id)
{
    void *new_tbl = NULL;
    //-------------------------------------------------
    return (int)tbl_kill;
    //-------------------------------------------------
    // new_tbl = tbl_obj;

    if (new_tbl)
        return ((int (*)(void *, int))g_proc_004354C8)(new_tbl, str_id);
    else
        return ((int (*)(void *, int))g_proc_004354C8)(tbl, str_id); // original
}

const char FILES_PATH[] = ".\\Drogn\\";

void *pud_map1;
DWORD pud_map1_size;
void *pud_map2;
DWORD pud_map2_size;
void *pud_map3;
DWORD pud_map3_size;
void *pud_map4;
DWORD pud_map4_size;
void *pud_map5;
DWORD pud_map5_size;
void *pud_map6;
DWORD pud_map6_size;
void *pud_map7;
DWORD pud_map7_size;
void *pud_map_secret;
DWORD pud_map_secret_size;
void *pud_emap1;
DWORD pud_emap1_size;
void *pud_emap2;
DWORD pud_emap2_size;
void *pud_emap3;
DWORD pud_emap3_size;
void *pud_emap4;
DWORD pud_emap4_size;
void *pud_emap5;
DWORD pud_emap5_size;
void *pud_emap6;
DWORD pud_emap6_size;
void *pud_emap7;
DWORD pud_emap7_size;

PROC g_proc_0041F97D;
int map_file_load(int a, int b, void **map, DWORD *size)
{
    byte lvl = *(byte *)LEVEL_OBJ;
    byte d = *(byte *)GB_DEMO; // demo
    bool f = false;
    if (d == 1)
    {
        //*map = pud_map_atr;
        //*size = pud_map_atr_size;
        // f = true;
    }
    else
    {
        if (lvl == LVL_HUMAN1)
        {
            *map = pud_map1;
            *size = pud_map1_size;
            f = true;
        }
        else if (lvl == LVL_HUMAN2)
        {
            *map = pud_map2;
            *size = pud_map2_size;
            f = true;
        }
        else if (lvl == LVL_HUMAN3)
        {
            *map = pud_map3;
            *size = pud_map3_size;
            f = true;
        }
        else if (lvl == LVL_HUMAN4)
        {
            *map = pud_map4;
            *size = pud_map4_size;
            f = true;
        }
        else if (lvl == LVL_HUMAN5)
        {
            *map = pud_map5;
            *size = pud_map5_size;
            f = true;
        }
        else if (lvl == LVL_HUMAN6)
        {
            *map = pud_map6;
            *size = pud_map6_size;
            f = true;
        }
        else if (lvl == LVL_HUMAN7)
        {
            *map = pud_map7;
            *size = pud_map7_size;
            f = true;
        }
    }
    if (f)
        return 1;
    else
        return ((int (*)(int, int, void *, DWORD *))g_proc_0041F97D)(a, b, map, size); // original
}

void *file_load(const char name[])
{
    void *file = NULL;
    FILE *fp;
    char path[MAX_PATH] = {0};
    _snprintf(path, sizeof(path), "%s%s", FILES_PATH, name);
    if ((fp = fopen(path, "rb")) != NULL) // file opened
    {
        fseek(fp, 0, SEEK_END); // seek to end of file
        DWORD size = ftell(fp); // get current file pointer
        fseek(fp, 0, SEEK_SET); // seek back to beginning of file
        file = malloc(size);
        fread(file, sizeof(unsigned char), size, fp); // read
        fclose(fp);
    }
    return file;
}

void file_load_size(const char name[], void **m, DWORD *s)
{
    void *file = NULL;
    FILE *fp;
    char path[MAX_PATH] = {0};
    _snprintf(path, sizeof(path), "%s%s", FILES_PATH, name);
    if ((fp = fopen(path, "rb")) != NULL) // file opened
    {
        fseek(fp, 0, SEEK_END); // seek to end of file
        DWORD size = ftell(fp); // get current file pointer
        *s = size;
        fseek(fp, 0, SEEK_SET); // seek back to beginning of file
        file = malloc(size);
        fread(file, sizeof(unsigned char), size, fp); // read
        fclose(fp);
    }
    *m = file;
}

PROC g_proc_00454BCA;
int grp_draw_unit(int a, int *u, void *grp, int frame)
{
    void *new_grp = NULL;
    //-------------------------------------------------
    byte id = *((byte *)((uintptr_t)u + S_ID));
    if (id == U_PEASANT)
    {
        if ((*(byte *)LEVEL_OBJ == LVL_HUMAN1) || (*(byte *)LEVEL_OBJ == LVL_HUMAN2))
            new_grp = grg_hugh;
    }
    else if (id == U_FOOTMAN)
    {
        // if ((*(byte *)LEVEL_OBJ == LVL_HUMAN1) || (*(byte *)LEVEL_OBJ == LVL_XHUMAN1))
        new_grp = grg_foot_shield;
    }
    else if (id == U_GRUNT)
    {
        // if ((*(byte *)LEVEL_OBJ == LVL_HUMAN1) || (*(byte *)LEVEL_OBJ == LVL_XHUMAN1))
        new_grp = grg_pirat;
    }
    else if (id == U_LOTHAR)
    {
        // if ((*(byte *)LEVEL_OBJ == LVL_HUMAN1) || (*(byte *)LEVEL_OBJ == LVL_XHUMAN1))
        new_grp = grg_old_lothar;
    }
    else if (id == U_UTER)
    {
        // if ((*(byte *)LEVEL_OBJ == LVL_HUMAN1) || (*(byte *)LEVEL_OBJ == LVL_XHUMAN1))
        new_grp = grg_castellan;
    }

    if (new_grp)
        return ((int (*)(int, int *, void *, int))g_proc_00454BCA)(a, u, new_grp, frame);
    else
        return ((int (*)(int, int *, void *, int))g_proc_00454BCA)(a, u, grp, frame); // original
}

PROC g_proc_0041C51C;
int netstat_get_tbl_nation(void *tbl, WORD str_id)
{
    void *new_tbl = NULL;
    // 46 blue hum
    // 47 blue orc
    // 48 white hum
    // 49 white orc
    // 50 red hum
    // 51 red orc
    // 52 green hum
    // 53 green orc
    // 54 black hum
    // 55 black orc
    // 56 violet hum
    // 57 violet orc
    // 58 orange hum
    // 59 orange orc
    // 60 yellow hum
    // 61 yellow orc
    // 92 blue xorc
    // 93 white xorc
    // 94 red xorc
    // 95 green xorc
    // 96 black xorc
    // 97 violet xorc
    // 98 orange xorc
    // 99 yellow xorc
    //-------------------------------------------------
    if (str_id == 48 || str_id == 49)
    {
        new_tbl = tbl_nations;
        str_id = 1;
    }
    else if (str_id == 58 || str_id == 59)
    {
        new_tbl = tbl_nations;
        str_id = 2;
    }
    else if (str_id == 46 || str_id == 47)
    {
        new_tbl = tbl_nations;
        str_id = 3;
    }
    else if (str_id == 50 || str_id == 51 || str_id == 54 || str_id == 55 || str_id == 56 || str_id == 57)
    {
        new_tbl = tbl_nations;
        str_id = 4;
    }
    // else if (str_id == 46 || str_id == 47) настроить нежить
    // {
    //     new_tbl = tbl_nations;
    //     str_id = 5;
    // }

    //-------------------------------------------------

    if (new_tbl)
        return ((int (*)(void *, int))g_proc_0041C51C)(new_tbl, str_id);
    else
        return ((int (*)(void *, int))g_proc_0041C51C)(tbl, str_id); // original
}

void *pcx_act1;
void *pcx_act1_pal;
void *pcx_act2;
void *pcx_act2_pal;
void *pcx_act3;
void *pcx_act3_pal;
void *pcx_act4;
void *pcx_act4_pal;
void *pcx_act5;
void *pcx_act5_pal;
void *tbl_act;
void *pcx_menu;
void *pcx_menu_pal;

void files_init()
{
    file_load_size("maps\\dr01.pud", &pud_map1, &pud_map1_size);
    file_load_size("maps\\dr02.pud", &pud_map2, &pud_map2_size);
    file_load_size("maps\\dr03.pud", &pud_map3, &pud_map3_size);
    file_load_size("maps\\dr04.pud", &pud_map4, &pud_map4_size);
    file_load_size("maps\\dr05.pud", &pud_map5, &pud_map5_size);
    file_load_size("maps\\dr06.pud", &pud_map6, &pud_map6_size);
    file_load_size("maps\\dr07.pud", &pud_map7, &pud_map7_size);
    grg_hugh = file_load("units\\hugh.grp");
    grg_hugh_icon = file_load("icons\\hugh_icon.grp");
    grg_pirat = file_load("units\\pirat.grp");
    grg_old_lothar = file_load("units\\old_lothar.grp");
    grg_castellan = file_load("units\\castellan.grp");
    grg_foot_shield = file_load("units\\foot_shield.grp");
    // иконки
    grp_drogn_icons = file_load("icons\\drogn_icons.grp");
    // Благодарности
    tbl_credits = file_load("textes\\credits.tbl");
    // Брифинги

    tbl_brif1 = file_load("textes\\brif1.tbl");
    tbl_brif2 = file_load("textes\\brif2.tbl");
    tbl_brif3 = file_load("textes\\brif3.tbl");
    tbl_brif4 = file_load("textes\\brif4.tbl");
    tbl_brif5 = file_load("textes\\brif5.tbl");
    tbl_brif6 = file_load("textes\\brif6.tbl");
    tbl_brif7 = file_load("textes\\brif7.tbl");
    tbl_end = file_load("textes\\end.tbl");
    // цели миссии
    tbl_task1 = file_load("textes\\task1.tbl");
    tbl_task2 = file_load("textes\\task2.tbl");
    tbl_task3 = file_load("textes\\task3.tbl");
    tbl_task4 = file_load("textes\\task4.tbl");
    tbl_task5 = file_load("textes\\task5.tbl");
    tbl_task6 = file_load("textes\\task6.tbl");
    tbl_task7 = file_load("textes\\task7.tbl");
    tbl_task8 = file_load("textes\\task8.tbl");

    // заголовки Брифинга
    tbl_title1 = file_load("textes\\title1.tbl");
    tbl_title2 = file_load("textes\\title2.tbl");
    tbl_title3 = file_load("textes\\title3.tbl");
    tbl_title4 = file_load("textes\\title4.tbl");
    tbl_title5 = file_load("textes\\title5.tbl");
    tbl_title6 = file_load("textes\\title6.tbl");
    tbl_title7 = file_load("textes\\title7.tbl");
    tbl_title8 = file_load("textes\\title8.tbl");
    tbl_title9 = file_load("textes\\title9.tbl");

    // Имена юнитов
    tbl_name1 = file_load("textes\\name1.tbl");
    tbl_name2 = file_load("textes\\name2.tbl");
    tbl_name3 = file_load("textes\\name3.tbl");
    tbl_name4 = file_load("textes\\name4.tbl");
    tbl_name5 = file_load("textes\\name5.tbl");
    tbl_name6 = file_load("textes\\name6.tbl");
    tbl_name7 = file_load("textes\\name7.tbl");
    tbl_name8 = file_load("textes\\name8.tbl");
    tbl_name9 = file_load("textes\\name9.tbl");
    tbl_name10 = file_load("textes\\name10.tbl");
    tbl_name11 = file_load("textes\\name11.tbl");

    // нации при победе или поражении
    tbl_nations = file_load("textes\\nations.tbl");

    // Текст в Актах (пока хз разбираюсь)
    tbl_act = file_load("textes\\act.tbl");

    // Картики Актов
    pcx_act1 = file_load("images\\act1.raw");
    pcx_act1_pal = file_load("images\\act1.pal");
    pcx_act2 = file_load("images\\act2.raw");
    pcx_act2_pal = file_load("images\\act2.pal");
    pcx_act3 = file_load("images\\act3.raw");
    pcx_act3_pal = file_load("images\\act3.pal");
    pcx_act4 = file_load("images\\act4.raw");
    pcx_act4_pal = file_load("images\\act4.pal");
    pcx_act5 = file_load("images\\act5.raw");
    pcx_act5_pal = file_load("images\\act5.pal");

    // главное меню
    pcx_menu = file_load("images\\menu.raw");
    pcx_menu_pal = file_load("images\\menu.pal");
}

int cred_num = 0;
void pal_load(byte *palette_adr, void *pal)
{
    if (palette_adr != NULL)
    {
        if (pal != NULL)
        {
            DWORD i = 0;
            while (i < (256 * 4))
            {
                *(byte *)(palette_adr + i) = *(byte *)((DWORD)pal + i);
                i++;
            }
        }
    }
}

PROC g_proc_004372EE;
void pcx_load_menu(char *name, void *pcx_info, byte *palette_adr)
{
    ((void (*)(char *, void *, byte *))g_proc_004372EE)(name, pcx_info, palette_adr); // original
    void *new_pcx_pixels = NULL;
    if (CompareString(LOCALE_SYSTEM_DEFAULT, NORM_IGNORECASE, name, -1, "art\\titlemenu_bne.pcx", -1) == CSTR_EQUAL)
    {
        cred_num = 0;
        new_pcx_pixels = pcx_menu;
        pal_load(palette_adr, pcx_menu_pal);
    }

    if (new_pcx_pixels)
        patch_setdword((DWORD *)((DWORD)pcx_info + 4), (DWORD)new_pcx_pixels);
}

PROC g_proc_0042968A;
void act_get_tbl_small(void *tbl, WORD str_id)
{
    void *new_tbl = NULL;
    byte lvl = *(byte *)LEVEL_OBJ;
    //-------------------------------------------------
    if (lvl == LVL_HUMAN1)
    {
        new_tbl = tbl_act;
        str_id = 1;
    }
    if ((lvl == LVL_HUMAN2) || (lvl == LVL_HUMAN3))
    {
        new_tbl = tbl_act;
        str_id = 3;
    }
    if ((lvl == LVL_HUMAN4) || (lvl == LVL_HUMAN5) || (lvl == LVL_HUMAN6) || (lvl == LVL_HUMAN7) || (lvl == LVL_HUMAN8))
    {
        new_tbl = tbl_act;
        str_id = 5;
    }
    if ((lvl == LVL_HUMAN9) || (lvl == LVL_HUMAN10) || (lvl == LVL_HUMAN11))
    {
        new_tbl = tbl_act;
        str_id = 7;
    }
    if ((lvl == LVL_HUMAN12) || (lvl == LVL_HUMAN13) || (lvl == LVL_HUMAN14))
    {
        new_tbl = tbl_act;
        str_id = 9;
    }
    //-------------------------------------------------

    if (new_tbl)
        return ((void (*)(void *, int))g_proc_0042968A)(new_tbl, str_id);
    else
        return ((void (*)(void *, int))g_proc_0042968A)(tbl, str_id); // original
}

PROC g_proc_004296A9;
void act_get_tbl_big(void *tbl, WORD str_id)
{
    void *new_tbl = NULL;
    byte lvl = *(byte *)LEVEL_OBJ;
    //-------------------------------------------------
    if (lvl == LVL_HUMAN1)
    {
        new_tbl = tbl_act;
        str_id = 2;
    }
    if ((lvl == LVL_HUMAN2) || (lvl == LVL_HUMAN3))
    {
        new_tbl = tbl_act;
        str_id = 4;
    }
    if ((lvl == LVL_HUMAN4) || (lvl == LVL_HUMAN5) || (lvl == LVL_HUMAN6) || (lvl == LVL_HUMAN7) || (lvl == LVL_HUMAN8))
    {
        new_tbl = tbl_act;
        str_id = 6;
    }
    if ((lvl == LVL_HUMAN9) || (lvl == LVL_HUMAN10) || (lvl == LVL_HUMAN11))
    {
        new_tbl = tbl_act;
        str_id = 8;
    }
    if ((lvl == LVL_HUMAN12) || (lvl == LVL_HUMAN13) || (lvl == LVL_HUMAN14))
    {
        new_tbl = tbl_act;
        str_id = 10;
    }
    //-------------------------------------------------

    if (new_tbl)
        return ((void (*)(void *, int))g_proc_004296A9)(new_tbl, str_id);
    else
        return ((void (*)(void *, int))g_proc_004296A9)(tbl, str_id); // original
}

PROC g_proc_0042A443;
void act_init()
{
    WORD m = *(WORD *)LEVEL_ID;
    if (*(byte *)LEVEL_OBJ == LVL_HUMAN1)
        *(WORD *)LEVEL_ID = 0x52D0;
    else
        *(WORD *)LEVEL_ID = 0x52C8;  // mission file number
    *(WORD *)PREVIOUS_ACT = 0;       // prev act
    ((void (*)())g_proc_0042A443)(); // original
    *(WORD *)LEVEL_ID = m;           // mission file number restore
}

PROC g_proc_004354FA;
int objct_get_tbl_campanign(void *tbl, WORD str_id)
{
    void *new_tbl = NULL;
    byte lvl = *(byte *)LEVEL_OBJ;
    //-------------------------------------------------
    if ((lvl == LVL_HUMAN1) || (lvl == LVL_XHUMAN1))
        new_tbl = tbl_task1;
    else if ((lvl == LVL_HUMAN2) || (lvl == LVL_XHUMAN2))
        new_tbl = tbl_task2;
    else if ((lvl == LVL_HUMAN3) || (lvl == LVL_XHUMAN3))
        new_tbl = tbl_task3;
    else if ((lvl == LVL_HUMAN4) || (lvl == LVL_XHUMAN4))
        new_tbl = tbl_task4;
    else if ((lvl == LVL_HUMAN5) || (lvl == LVL_XHUMAN5))
        new_tbl = tbl_task5;
    else if ((lvl == LVL_HUMAN6) || (lvl == LVL_XHUMAN6))
        new_tbl = tbl_task6;
    else if ((lvl == LVL_HUMAN7) || (lvl == LVL_XHUMAN7))
        new_tbl = tbl_task7;
    else if (lvl == LVL_HUMAN10)
        return (int)tbl_task_secret;
    else if (lvl == LVL_ORC1)
        new_tbl = tbl_task8;
    str_id = 1;
    //-------------------------------------------------
    // new_tbl = tbl_obj;

    if (new_tbl)
        return ((int (*)(void *, int))g_proc_004354FA)(new_tbl, str_id);
    else
        return ((int (*)(void *, int))g_proc_004354FA)(tbl, str_id); // original
}

PROC g_proc_00429625; // load palette
PROC g_proc_00429654; // load image
void pcx_load_act(char *name, void *pcx_info, byte *palette_adr)
{
    ((void (*)(char *, void *, byte *))g_proc_00429625)(name, pcx_info, palette_adr); // original
    byte lvl = *(byte *)LEVEL_OBJ;
    void *new_pcx_pixels = NULL;
    if (lvl == LVL_HUMAN1)
    {
        new_pcx_pixels = pcx_act1;
        pal_load(palette_adr, pcx_act1_pal);
    }
    if ((lvl == LVL_HUMAN2) || (lvl == LVL_HUMAN3))
    {
        new_pcx_pixels = pcx_act2;
        pal_load(palette_adr, pcx_act2_pal);
    }
    if ((lvl == LVL_HUMAN4) || (lvl == LVL_HUMAN5) || (lvl == LVL_HUMAN6) || (lvl == LVL_HUMAN7) || (lvl == LVL_HUMAN8))
    {
        new_pcx_pixels = pcx_act3;
        pal_load(palette_adr, pcx_act3_pal);
    }
    if ((lvl == LVL_HUMAN9) || (lvl == LVL_HUMAN10) || (lvl == LVL_HUMAN11))
    {
        new_pcx_pixels = pcx_act4;
        pal_load(palette_adr, pcx_act4_pal);
    }
    if ((lvl == LVL_HUMAN12) || (lvl == LVL_HUMAN13) || (lvl == LVL_HUMAN14))
    {
        new_pcx_pixels = pcx_act5;
        pal_load(palette_adr, pcx_act5_pal);
    }
    if (new_pcx_pixels)
        patch_setdword((DWORD *)((DWORD)pcx_info + 4), (DWORD)new_pcx_pixels);
}

PROC g_proc_004300A5;
int objct_get_tbl_briefing_task(void *tbl, WORD str_id)
{
    void *new_tbl = NULL;
    byte lvl = *(byte *)LEVEL_OBJ;
    //-------------------------------------------------
    if ((lvl == LVL_HUMAN1) || (lvl == LVL_XHUMAN1))
        new_tbl = tbl_task1;
    else if ((lvl == LVL_HUMAN2) || (lvl == LVL_XHUMAN2))
        new_tbl = tbl_task2;
    else if ((lvl == LVL_HUMAN3) || (lvl == LVL_XHUMAN3))
        new_tbl = tbl_task3;
    else if ((lvl == LVL_HUMAN4) || (lvl == LVL_XHUMAN4))
        new_tbl = tbl_task4;
    else if ((lvl == LVL_HUMAN5) || (lvl == LVL_XHUMAN5))
        new_tbl = tbl_task5;
    else if ((lvl == LVL_HUMAN6) || (lvl == LVL_XHUMAN6))
        new_tbl = tbl_task6;
    else if ((lvl == LVL_HUMAN7) || (lvl == LVL_XHUMAN7))
        new_tbl = tbl_task7;
    else if (lvl == LVL_HUMAN10)
        return (int)tbl_task_secret;
    else if (lvl == LVL_ORC1)
        new_tbl = tbl_task8;
    str_id = 1;
    //-------------------------------------------------
    // new_tbl = tbl_obj;

    if (new_tbl)
        return ((int (*)(void *, int))g_proc_004300A5)(new_tbl, str_id);
    else
        return ((int (*)(void *, int))g_proc_004300A5)(tbl, str_id); // original
}

PROC g_proc_004300CA;
int objct_get_tbl_briefing_title(void *tbl, WORD str_id)
{
    void *new_tbl = NULL;
    byte lvl = *(byte *)LEVEL_OBJ;
    //-------------------------------------------------
    if ((lvl == LVL_HUMAN1) || (lvl == LVL_XHUMAN1))
        new_tbl = tbl_title1;
    else if ((lvl == LVL_HUMAN2) || (lvl == LVL_XHUMAN2))
        new_tbl = tbl_title2;
    else if ((lvl == LVL_HUMAN3) || (lvl == LVL_XHUMAN3))
        new_tbl = tbl_title3;
    else if ((lvl == LVL_HUMAN4) || (lvl == LVL_XHUMAN4))
        new_tbl = tbl_title4;
    else if ((lvl == LVL_HUMAN5) || (lvl == LVL_XHUMAN5))
        new_tbl = tbl_title5;
    else if ((lvl == LVL_HUMAN6) || (lvl == LVL_XHUMAN6))
        new_tbl = tbl_title6;
    else if ((lvl == LVL_HUMAN7) || (lvl == LVL_XHUMAN7))
        new_tbl = tbl_title7;
    else if (lvl == LVL_HUMAN10)
        new_tbl = tbl_title8;
    else if (lvl == LVL_ORC1)
        new_tbl = tbl_title9;
    str_id = 1;
    //-------------------------------------------------

    if (new_tbl)
        return ((int (*)(void *, int))g_proc_004300CA)(new_tbl, str_id);
    else
        return ((int (*)(void *, int))g_proc_004300CA)(tbl, str_id); // original
}

PROC g_proc_0041F1E8;
int finale_credits_get_tbl(void *tbl, WORD str_id)
{
    void *new_tbl = NULL;
    byte lvl = *(byte *)LEVEL_OBJ;
    //-------------------------------------------------
    if (lvl == (LVL_XHUMAN12 + 2))
        return (int)tbl_brif8;
    else
    {
        new_tbl = tbl_credits;
        str_id = 1;
    }
    //-------------------------------------------------

    if (new_tbl)
        return ((int (*)(void *, int))g_proc_0041F1E8)(new_tbl, str_id);
    else
        return ((int (*)(void *, int))g_proc_0041F1E8)(tbl, str_id); // original
}

char story1[] = "RedMist\\storyteller\\1.wav";
char story2[] = "RedMist\\storyteller\\2.wav";
char story3[] = "RedMist\\storyteller\\3.wav";
char story4[] = "RedMist\\storyteller\\4.wav";
char story5[] = "RedMist\\storyteller\\5.wav";
char story6[] = "RedMist\\storyteller\\6.wav";
char story7[] = "RedMist\\storyteller\\7.wav";
char story_secret[] = "RedMist\\storyteller\\secret.wav";
char story_end[] = "RedMist\\storyteller\\end.wav";
char story8[] = "RedMist\\storyteller\\tutorial.wav";

void set_speech(char *speech, char *adr)
{
    patch_setdword((DWORD *)(speech + 4), (DWORD)adr);
    patch_setdword((DWORD *)(speech + 12), 0);
}

DWORD remember_music = 101;
DWORD remember_sound = 101;

PROC g_proc_0041F027;
int finale_get_speech(char *speech)
{
    remember_music = *(DWORD *)VOLUME_MUSIC;
    remember_sound = *(DWORD *)VOLUME_SOUND;
    if (remember_music != 0)
        *(DWORD *)VOLUME_MUSIC = 22;
    *(DWORD *)VOLUME_SOUND = 100;
    ((void (*)(DWORD))F_SET_VOLUME)(SET_VOLUME_PARAM); // set volume

    DWORD remember1 = *(DWORD *)(speech + 4);
    DWORD remember2 = *(DWORD *)(speech + 12);

    byte lvl = *(byte *)LEVEL_OBJ;
    //-------------------------------------------------
    if (lvl == (LVL_XHUMAN12 + 2))
        set_speech(speech, story8);
    else
        set_speech(speech, story_end);
    //-------------------------------------------------
    int original = ((int (*)(char *))g_proc_0041F027)(speech); // original
    patch_setdword((DWORD *)(speech + 4), remember1);
    patch_setdword((DWORD *)(speech + 12), remember2);
    return original;
}

PROC g_proc_004301CA;
int objct_get_tbl_briefing_text(void *tbl, WORD str_id)
{
    void *new_tbl = NULL;
    byte lvl = *(byte *)LEVEL_OBJ;
    //-------------------------------------------------
    if ((lvl == LVL_HUMAN1) || (lvl == LVL_XHUMAN1))
        new_tbl = tbl_brif1;
    else if ((lvl == LVL_HUMAN2) || (lvl == LVL_XHUMAN2))
        new_tbl = tbl_brif2;
    else if ((lvl == LVL_HUMAN3) || (lvl == LVL_XHUMAN3))
        new_tbl = tbl_brif3;
    else if ((lvl == LVL_HUMAN4) || (lvl == LVL_XHUMAN4))
        new_tbl = tbl_brif4;
    else if ((lvl == LVL_HUMAN5) || (lvl == LVL_XHUMAN5))
        new_tbl = tbl_brif5;
    else if ((lvl == LVL_HUMAN6) || (lvl == LVL_XHUMAN6))
        new_tbl = tbl_brif6;
    else if ((lvl == LVL_HUMAN7) || (lvl == LVL_XHUMAN7))
        new_tbl = tbl_brif7;
    else if (lvl == LVL_HUMAN10)
        return (int)tbl_brif_secret;
    else if (lvl == LVL_ORC1)
        return (int)tbl_brif8;
    str_id = 1;
    //-------------------------------------------------

    if (new_tbl)
        return ((int (*)(void *, int))g_proc_004301CA)(new_tbl, str_id);
    else
        return ((int (*)(void *, int))g_proc_004301CA)(tbl, str_id); // original
}

PROC g_proc_00430113;
int objct_get_briefing_speech(char *speech)
{
    remember_music = *(DWORD *)VOLUME_MUSIC;
    remember_sound = *(DWORD *)VOLUME_SOUND;
    if (remember_music != 0)
        *(DWORD *)VOLUME_MUSIC = 20;
    *(DWORD *)VOLUME_SOUND = 100;
    ((void (*)(DWORD))F_SET_VOLUME)(SET_VOLUME_PARAM); // set volume

    DWORD remember1 = *(DWORD *)(speech + 4);
    DWORD remember2 = *(DWORD *)(speech + 12);
    byte lvl = *(byte *)LEVEL_OBJ;
    //-------------------------------------------------
    if ((lvl == LVL_HUMAN1) || (lvl == LVL_XHUMAN1))
        set_speech(speech, story1);
    else if ((lvl == LVL_HUMAN2) || (lvl == LVL_XHUMAN2))
        set_speech(speech, story2);
    else if ((lvl == LVL_HUMAN3) || (lvl == LVL_XHUMAN3))
        set_speech(speech, story3);
    else if ((lvl == LVL_HUMAN4) || (lvl == LVL_XHUMAN4))
        set_speech(speech, story4);
    else if ((lvl == LVL_HUMAN5) || (lvl == LVL_XHUMAN5))
        set_speech(speech, story5);
    else if ((lvl == LVL_HUMAN6) || (lvl == LVL_XHUMAN6))
        set_speech(speech, story6);
    else if ((lvl == LVL_HUMAN7) || (lvl == LVL_XHUMAN7))
        set_speech(speech, story7);
    else if (lvl == LVL_HUMAN10)
        set_speech(speech, story_secret);
    else if (lvl == LVL_ORC1)
        set_speech(speech, story8);
    //-------------------------------------------------

    int original = ((int (*)(char *))g_proc_00430113)(speech); // original
    patch_setdword((DWORD *)(speech + 4), remember1);
    patch_setdword((DWORD *)(speech + 12), remember2);
    return original;
}

bool finale_dlg = false;
PROC g_proc_0041F0F5;
int finale_get_tbl(void *tbl, WORD str_id)
{
    finale_dlg = false;
    void *new_tbl = NULL;
    byte lvl = *(byte *)LEVEL_OBJ;
    //-------------------------------------------------
    if (lvl == (LVL_XHUMAN12 + 2))
        return (int)tbl_brif8;
    else
    {
        new_tbl = tbl_end;
        str_id = 1;
    }
    //-------------------------------------------------

    if (new_tbl)
        return ((int (*)(void *, int))g_proc_0041F0F5)(new_tbl, str_id);
    else
        return ((int (*)(void *, int))g_proc_0041F0F5)(tbl, str_id); // original
}

PROC g_proc_00422D76;
void sound_play_unit_speech_replace(WORD sid, int a, int *u, int b, void *snd, char *name)
{
    def_name = (void *)*(int *)(SOUNDS_FILES_LIST + 8 + 24 * sid);
    def_sound = (void *)*(int *)(SOUNDS_FILES_LIST + 16 + 24 * sid); // save default
    patch_setdword((DWORD *)(SOUNDS_FILES_LIST + 8 + 24 * sid), (DWORD)name);
    patch_setdword((DWORD *)(SOUNDS_FILES_LIST + 16 + 24 * sid), (DWORD)snd);
    ((void (*)(WORD, int, int *, int))g_proc_00422D76)(sid, a, u, b); // original
    snd = (void *)*(int *)(SOUNDS_FILES_LIST + 16 + 24 * sid);
    patch_setdword((DWORD *)(SOUNDS_FILES_LIST + 16 + 24 * sid), (DWORD)def_sound);
    patch_setdword((DWORD *)(SOUNDS_FILES_LIST + 8 + 24 * sid), (DWORD)def_name); // restore default
}

// PROC g_proc_00422D76;
void sound_play_unit_speech(WORD sid, int a, int *u, int b)
{
    bool f = true;
    if (u != NULL)
    {
        WORD sn = 0;
        byte id = *((byte *)((uintptr_t)u + S_ID));
        if ((id == U_PEASANT) || (id == U_DANATH) || (id == U_TYRALYON) || (id == U_KARGATH))
        {
            if (w_sounds_e)
            {
                if ((sid >= 223) && (sid <= 238)) // peasant
                {
                    sn = sid - 223;
                    sound_play_unit_speech_replace(sid, a, u, b, hugh_sounds[sn], hugh_names[sn]);
                    f = false;
                }
            }
            if ((sid >= 262) && (sid <= 270)) // danath
            {
                sn = sid - 262;
                sound_play_unit_speech_replace(sid, a, u, b, hugh_sounds[sn], hugh_names[sn]);
                f = false;
            }

            // добавить ещё тириона со звуком даната
        }
    }

    if (f)
        ((void (*)(WORD, int, int *, int))g_proc_00422D76)(sid, a, u, b); // original
}

PROC g_proc_00422D5F;
void sound_play_unit_speech_soft_replace(WORD sid, int a, int *u, int b, void *snd, char *name)
{
    def_name = (void *)*(int *)(SOUNDS_FILES_LIST + 8 + 24 * sid);
    def_sound = (void *)*(int *)(SOUNDS_FILES_LIST + 16 + 24 * sid); // save default
    patch_setdword((DWORD *)(SOUNDS_FILES_LIST + 8 + 24 * sid), (DWORD)name);
    patch_setdword((DWORD *)(SOUNDS_FILES_LIST + 16 + 24 * sid), (DWORD)snd);
    ((void (*)(WORD, int, int *, int))g_proc_00422D5F)(sid, a, u, b); // original
    snd = (void *)*(int *)(SOUNDS_FILES_LIST + 16 + 24 * sid);
    patch_setdword((DWORD *)(SOUNDS_FILES_LIST + 16 + 24 * sid), (DWORD)def_sound);
    patch_setdword((DWORD *)(SOUNDS_FILES_LIST + 8 + 24 * sid), (DWORD)def_name); // restore default
}

// PROC g_proc_00422D5F;
void sound_play_unit_speech_soft(WORD sid, int a, int *u, int b)
{
    bool f = true;
    if (u != NULL)
    {
    }
    if (f)
        ((void (*)(WORD, int, int *, int))g_proc_00422D5F)(sid, a, u, b); // original
}

// write your custom victory functions here
//-------------------------------------------------------------------------------
void v_human1(bool rep_init) // Первая миссия
{
    if (rep_init)
    {
        saveload_fixed = true;
        // your initialize
    }
    else
    {

        // Имя для Пеона - Хью, Лотар - Барон, Утер - Кастелян
        // Узнать как менять цель миссии в течении игры и задать стартовую цель миссии.

        // Враги
        ally(P_BLACK, P_BLUE, 0);
        ally(P_BLACK, P_ORANGE, 0);

        // Союзники
        ally(P_RED, P_ORANGE, 1);
        ally(P_WHITE, P_ORANGE, 1);
        ally(P_WHITE, P_BLUE, 1);
        ally(P_WHITE, P_RED, 1);

        // обзор
        comps_vision(true); // компы могут давать виз
        viz(P_WHITE, P_BLUE, 1);

        // пират стоит на берегу чтоб герой не просочился по берегу
        set_region(37, 66, 43, 73);
        find_all_alive_units(U_GRUNT);
        sort_in_region();
        order_all(38, 68, ORDER_STAND);

        // превращаем орков в людей
        unit_convert(P_BLACK, U_ODESTROYER, U_HDESTROYER, 1);
        unit_convert(P_BLACK, U_OTRANSPORT, U_HTRANSPORT, 1);
        unit_convert(P_BLACK, U_OBARRACK, U_HBARRACK, 1);

        // создаём ГГ
        // find_all_alive_units(U_PEASANT);
        // sort_stat(S_OWNER, P_WHITE, CMP_EQ);
        // unit_convert(P_WHITE, U_PEASANT, U_PEON, 1);

        // синие рабочие идут в форт
        find_all_alive_units(U_ATTACK_PEASANT);
        sort_stat(S_OWNER, P_BLUE, CMP_EQ);
        order_all(26, 56, ORDER_MOVE);

        // орковский рабочий превращается в ГГ - Это уже не работает
        // find_all_alive_units(U_PEON);
        // sort_stat(S_COLOR, P_RED, CMP_EQ);
        // set_stat_all(S_COLOR, P_WHITE); // перекрашиваем юнитов в списке в белых - эта ебала чтоб не переделывать всех орков и постройки в людские
        // give_all(P_WHITE);              // передаём всех юнитов в списке белому

        // проверка что есть барак в форте
        find_all_alive_units(U_HBARRACK);
        sort_stat(S_OWNER, P_WHITE, CMP_EQ);
        if (units > 0 && *(byte *)(GB_HORSES + 0) == 0)
        {

            if (*(byte *)(GB_HORSES + 14) == 0) // таймер 1 раз
            {
                if (*(byte *)(GB_HORSES + 15) < 6)
                    *(byte *)(GB_HORSES + 15) = *(byte *)(GB_HORSES + 15) + 1;
                else
                {
                    viz_area_add(59, 39, 1 << P_WHITE, 5);                                         // открыть карту белого размером в 5 клеток
                    char message[] = "Castellan: Run to the outpost, rang the bell on the tower!"; // надо переделать, русские сиволы не работают "Кастелян: Беги на заставу, позвони в колокол на башне!"
                    show_message(10, message);
                    // изменить цель миссии

                    // создаём пиратов и пиратские корабли
                    unit_create(11, 65, U_HDESTROYER, P_BLACK, 1);
                    unit_create(19, 77, U_HDESTROYER, P_BLACK, 1);
                    unit_create(15, 70, U_HTRANSPORT, P_BLACK, 1);
                    unit_create(9, 59, U_HTRANSPORT, P_BLACK, 1);
                    unit_create(15, 60, U_GRUNT, P_BLACK, 4);
                    unit_create(21, 64, U_GRUNT, P_BLACK, 4);
                    unit_create(20, 48, U_GRUNT, P_BLACK, 4);
                    *(byte *)(GB_HORSES + 14) = 1; // таймер 1 раз
                    *(byte *)(GB_HORSES + 0) = 1;  // чекпоинт 1
                }
            }
        }

        // герой подошел к башне
        set_region(61, 39, 63, 40); // установить регион
        find_all_alive_units(U_PEASANT);
        sort_in_region();
        if (units > 0 && *(byte *)(GB_HORSES + 0) == 1)
        {
            // передача башни игроку
            find_all_alive_units(U_HTOWER);
            give_all(P_WHITE);
            *(byte *)(GB_HORSES + 0) = 2; // чекпоинт 2
        }

        if (*(byte *)(GB_HORSES + 0) == 2)
        {
            if (*(byte *)(GB_HORSES + 6) == 0) //  1 раз
            {
                char mess[] = "Run to the Baron's castle under his protection.";
                show_message(10, mess);
                viz_area_add(82, 14, 1 << P_WHITE, 9); // открыть карту для белого размером в 7 клеток
                *(byte *)(GB_HORSES + 6) = 1;
            }

            if (*(byte *)(GB_HORSES + 13) < 2)
                *(byte *)(GB_HORSES + 13) = *(byte *)(GB_HORSES + 13) + 1;
            else
            {

                if (*(byte *)(GB_HORSES + 11) < 3)
                    *(byte *)(GB_HORSES + 11) = *(byte *)(GB_HORSES + 13) + 1; // тут нужен таймер чтоб крестьяне не сразу вооружались
                {
                    unit_convert(P_ORANGE, U_ATTACK_PEASANT, U_FOOTMAN, 1); // превращаем злых крестьян в футов, всегда 1 ставим хз нахуй
                }
                // изменить цель миссии

                *(byte *)(GB_HORSES + 12) = 1; // 1 раз
                *(byte *)(GB_HORSES + 0) = 3;  // чекпоинт 2
            }
        }

        find_all_alive_units(U_GRUNT); // нападение черных
        sort_stat(S_OWNER, P_BLACK, CMP_EQ);
        order_all(88, 7, ORDER_PATROL);

        find_all_alive_units(U_HDESTROYER); // нападение черных кораблей
        sort_stat(S_OWNER, P_BLACK, CMP_EQ);
        order_all(30, 92, ORDER_MOVE_PATROL);

        // герой прибыл к барону
        set_region(82, 14, 83, 15); // установить регион
        find_all_alive_units(U_PEASANT);
        sort_in_region();

        if (units > 0 && *(byte *)(GB_HORSES + 0) == 3)
        {
            if (*(byte *)(GB_HORSES + 10) == 0) // таймер 1 раз
            {

                char mess[] = "Baron: Many of my people have died, swim to the King for help!"; // "Барон: Многие мои люди погибли, плыви к Королю за подмогой!"
                show_message(10, mess);
                viz_area_add(59, 75, 1 << P_WHITE, 5); // открыть карту для белого размером в 7 клеток
                *(byte *)(GB_HORSES + 10) = 1;         // таймер 1 раз
                *(byte *)(GB_HORSES + 0) = 4;          // чекпоинт 3

                // изменить цель миссии - сесть на корабль и отплыть за помощью
            }
        }

        // дорога в порт - красим красных грунтов в черных
        find_all_alive_units(U_GRUNT);
        set_stat_all(S_COLOR, P_BLACK);

        set_region(70, 55, 95, 70); // установить регион
        find_all_alive_units(U_PEASANT);
        sort_in_region();

        if (units > 0)
        {
            find_all_alive_units(U_GRUNT);
            give_all(P_BLACK);
        }

        // герой прибыл в порт
        set_region(59, 75, 60, 76); // установить регион
        find_all_alive_units(U_PEASANT);
        sort_stat(S_OWNER, P_WHITE, CMP_EQ);
        sort_in_region();

        if (units > 0 && *(byte *)(GB_HORSES + 0) == 4)
        {
            // передать корабль игроку
            find_all_alive_units(U_BATTLESHIP);
            give_all(P_WHITE);

            // убрать героя с карты
            find_all_alive_units(U_PEASANT);
            remove_all();

            // дать обзор куда плыть
            viz_area_add(5, 5, 1 << P_WHITE, 5);
            // Изменить цель миссии на "Уплыть на северо-запад"
        }

        // когда корабль получает урон он переходит черному
        find_all_alive_units(U_BATTLESHIP);
        sort_stat(S_HP, 100, CMP_SMALLER_EQ); // сравниваем, хп меньше или равно 100

        if (units > 0)
        {
            give_all(P_BLACK);
            char mess[] = "Mission complete."; // "Миссия завершена."
            show_message(5, mess);
            win(true);
        }

        find_all_alive_units(U_BATTLESHIP);
        sort_stat(S_OWNER, P_WHITE, CMP_EQ);
        if (units == 0)
        {
            find_all_alive_units(U_PEASANT);
            sort_stat(S_OWNER, P_WHITE, CMP_EQ);
            if (units == 0)
            {
                if (*(byte *)(GB_HORSES + 8) < 2)
                    *(byte *)(GB_HORSES + 8) = *(byte *)(GB_HORSES + 8) + 1;
                else
                    lose(true);
            }
        }

        find_all_alive_units(U_UTER);
        sort_stat(S_HP, 60, CMP_SMALLER_EQ);
        remove_all(); // удалить утера если у него меньше 60 хп

        // Союзы:
        // - черный и белый враги
        // - оранжевый и белый союзники
        // - синий и белый союзники
        // - черный и оранжевый враги
        // - Компы могут давать вид - Да
        // - Синий и белый могут видеть друг друга

        byte local = *(byte *)LOCAL_PLAYER;

        if (!slot_alive(local))
            lose(true);
        if (get_val(FARM, local) >= 2)
            win(true);
        // char mess[] = "pidec";
        // show_message(5, mess);
    }
}

void v_human2(bool rep_init)
{
    if (rep_init)
    {
        saveload_fixed = true;
        // your initialize
    }
    else
    {

        // Враги
        ally(P_BLACK, P_BLUE, 0);
        ally(P_VIOLET, P_BLUE, 0);
        ally(P_GREEN, P_BLUE, 0);
        ally(P_RED, P_BLUE, 0);

        // Союзники
        ally(P_WHITE, P_BLUE, 1);
        ally(P_WHITE, P_YELLOW, 1);

        // обзор
        viz(P_WHITE, P_YELLOW, 1);
        viz_area_add(55, 18, 1 << P_WHITE, 5);
        // viz(P_WHITE, P_VIOLET, 1); // пробуем

        // Делаем всех компов черными
        find_all_alive_units(ANY_UNITS);
        sort_stat(S_OWNER, P_GREEN, CMP_EQ);
        set_stat_all(S_COLOR, P_BLACK);

        // Делаем Барт белый
        find_all_alive_units(U_RANGER);
        set_stat_all(S_COLOR, P_WHITE);

        find_all_alive_units(ANY_UNITS);
        sort_stat(S_OWNER, P_RED, CMP_EQ);
        set_stat_all(S_COLOR, P_BLACK);

        find_all_alive_units(U_HSMITH);
        set_stat_all(S_COLOR, P_BLACK);

        comps_vision(true); // чтоб компы могли давать виз

        // превращаем орковские юниты в людские
        unit_convert(P_GREEN, U_ODESTROYER, U_HDESTROYER, 1);

        // Только утер может победить непобедимого огра
        ua[0] = U_UTER;
        ut[0] = U_DENTARG;

        viz_area_add(60, 5, 1 << P_WHITE, 1);

        // Гномы взрывают горы одну ячейку в самом начале
        find_all_alive_units(U_DWARWES);
        sort_stat(S_OWNER, P_WHITE, CMP_EQ);
        if (units > 0)
        {
            tile_remove_rocks(8, 1);
        }

        // Гномы взрывают горы
        find_all_alive_units(U_DWARWES);
        set_region(8, 1, 8, 1);
        sort_in_region();
        if (units > 0)
        {

            tile_remove_rocks(9, 1);
            tile_remove_rocks(10, 1);
            tile_remove_rocks(11, 1);
            tile_remove_rocks(12, 1);
            tile_remove_rocks(13, 1);
            tile_remove_rocks(14, 1);
            tile_remove_rocks(15, 1);
            tile_remove_rocks(16, 1);
            tile_remove_rocks(17, 1);
            tile_remove_rocks(18, 1);
            tile_remove_rocks(19, 1);
            tile_remove_rocks(20, 1);
            tile_remove_rocks(21, 1);
            tile_remove_rocks(22, 1);
        }

        find_all_alive_units(U_DWARWES);
        set_region(26, 1, 26, 1);
        sort_in_region();
        if (units > 0)
        {
            bullet_create(27, 1, B_BOOM_FIRE);
            tile_remove_rocks(27, 1);
            tile_remove_rocks(28, 1);
        }

        find_all_alive_units(U_DWARWES);
        set_region(33, 1, 33, 1);
        sort_in_region();
        if (units > 0)
        {
            tile_remove_rocks(34, 1);
            tile_remove_rocks(35, 1);
        }

        find_all_alive_units(U_DWARWES);
        set_region(41, 2, 41, 2);
        sort_in_region();
        if (units > 0)
        {
            tile_remove_rocks(42, 2);
            tile_remove_rocks(43, 2);
            tile_remove_rocks(44, 2);
            tile_remove_rocks(45, 2);
            tile_remove_rocks(46, 2);
            tile_remove_rocks(47, 2);
            tile_remove_rocks(48, 2);
            tile_remove_rocks(49, 2);
            tile_remove_rocks(50, 2);
            tile_remove_rocks(51, 2);
            tile_remove_rocks(52, 2);
            tile_remove_rocks(53, 2);
            tile_remove_rocks(54, 2);
            tile_remove_rocks(55, 2);
            tile_remove_rocks(56, 2);
        }

        find_all_alive_units(U_DWARWES);
        set_region(64, 2, 64, 2);
        sort_in_region();
        if (units > 0)
        {
            tile_remove_rocks(65, 2);
            tile_remove_rocks(66, 2);
        }

        // Вооружаем крестьян в кузне
        find_all_alive_units(U_PEASANT);
        set_region(58, 3, 62, 7);
        sort_in_region();
        give_all(P_YELLOW);
        if (units > 0)
        {
            find_all_alive_units(U_PEASANT);
            set_region(58, 3, 62, 7);
            sort_in_region();
            sort_stat(S_OWNER, P_YELLOW, CMP_EQ);
            unit_convert(P_YELLOW, U_PEASANT, U_DANATH, 1);
            give_all(P_WHITE);
        }

        set_region(58, 3, 62, 7);
        find_all_alive_units(U_ATTACK_PEASANT);
        sort_in_region();
        give_all(P_YELLOW);
        if (units > 0)
        {
            find_all_alive_units(U_ATTACK_PEASANT);
            set_region(58, 3, 62, 7);
            sort_in_region();
            sort_stat(S_OWNER, P_YELLOW, CMP_EQ);
            unit_convert(P_YELLOW, U_ATTACK_PEASANT, U_FOOTMAN, 1);
            give_all(P_WHITE);
        }

        // Огр начинает бить лучников
        if (*(byte *)(GB_HORSES + 15) < 20)
            *(byte *)(GB_HORSES + 15) = *(byte *)(GB_HORSES + 15) + 1;
        else
        {
            if (*(byte *)(GB_HORSES + 1) == 0)
            {
                char message[] = "We need to save the archers"; // Нужно спасти лучников!"
                show_message(10, message);
                ally_one_sided(P_YELLOW, P_VIOLET, 0);

                *(byte *)(GB_HORSES + 1) = 1;
            }
        }

        // если огр умирает лучники переходят игроку
        find_all_alive_units(U_OGRE);
        if (units == 0)
        {

            if (*(byte *)(GB_HORSES + 2) == 0)
            {
                find_all_alive_units(U_ARCHER);
                sort_stat(S_OWNER, P_YELLOW, CMP_EQ);
                give_all(P_WHITE);
                find_all_alive_units(U_RANGER);
                sort_stat(S_OWNER, P_YELLOW, CMP_EQ);
                give_all(P_WHITE);

                char message[] = "Bart Vigilant: We need to light a lighthouse, it will distract the pirates!"; // Барт Зоркий: Нужно зажечь маяк, это отвлечёт пиратов!
                show_message(10, message);
                viz_area_add(80, 33, 1 << P_WHITE, 3);

                *(byte *)(GB_HORSES + 2) = 1;
            }
        }

        // проверка поджига маяка
        set_region(80, 32, 81, 33);
        find_all_alive_units(U_HTOWER);
        sort_in_region();
        sort_stat(S_HP, 95, CMP_SMALLER_EQ);
        if (units > 0)
        {
            if (*(byte *)(GB_HORSES + 3) == 0)
            {
                set_region(80, 32, 81, 33);
                find_all_alive_units(U_HTOWER);
                sort_in_region();
                set_stat_all(S_HP, 30);

                char message[] = "Pirate: We are under the attack!"; // Пират: На нас напали, тревога!
                show_message(10, message);
                viz(P_WHITE, P_GREEN, 1);

                find_all_alive_units(U_GRUNT);
                sort_stat(S_OWNER, P_GREEN, CMP_EQ);
                order_all(60, 30, ORDER_ATTACK_AREA);

                find_all_alive_units(U_HDESTROYER);
                sort_stat(S_OWNER, P_GREEN, CMP_EQ);
                order_all(90, 35, ORDER_ATTACK_AREA);

                *(byte *)(GB_HORSES + 3) = 1;
                *(byte *)(GB_HORSES + 9) = 1;
            }
        }

        if (*(byte *)(GB_HORSES + 9) == 1)
        {
            find_all_alive_units(U_HDESTROYER);
            sort_stat(S_OWNER, P_GREEN, CMP_EQ);
            order_all(90, 35, ORDER_ATTACK_AREA);
        }

        find_all_alive_units(U_DANATH);
        set_region(16, 87, 18, 90);
        sort_in_region();
        if (units > 0)
        {
            if (*(byte *)(GB_HORSES + 4) == 0)
            {
                char message[] = "Sir Uter: I can kill, a thick-skinned man!"; // Кастелян: Я могу убить толстокожего!
                show_message(10, message);
                viz_area_add(24, 73, 1 << P_WHITE, 2);

                viz(P_WHITE, P_BLUE, 1); // дать вид синего - показать корабль

                find_all_alive_units(U_UTER);
                give_all(P_WHITE);

                *(byte *)(GB_HORSES + 4) = 1;
            }
        }

        // Непробиваемый страж удерживает позицию
        find_all_alive_units(U_DENTARG);
        order_all(12, 61, ORDER_STAND);

        // если страж умер показать корабль
        find_all_alive_units(U_DENTARG);
        if (units == 0)
        {
            if (*(byte *)(GB_HORSES + 5) == 0)
            {
                find_all_alive_units(U_UTER);
                kill_all();

                *(byte *)(GB_HORSES + 5) = 1;
                *(byte *)(GB_HORSES + 8) = 1;
            }
        }

        if (*(byte *)(GB_HORSES + 8) == 1)
        {
            // отправить пиратов для нападения
            find_all_alive_units(U_GRUNT);
            sort_stat(S_OWNER, P_GREEN, CMP_EQ);
            order_all(24, 73, ORDER_ATTACK_AREA);
        }

        // герои прыгают на корабль
        find_all_alive_units(U_DANATH);
        set_region(36, 85, 37, 86);
        sort_in_region();
        if (units > 0)
            *(byte *)(GB_HORSES + 6) = 1;

        find_all_alive_units(U_RANGER);
        set_region(36, 85, 37, 86);
        sort_in_region();
        if (units > 0)
            *(byte *)(GB_HORSES + 7) = 1;

        if (*(byte *)(GB_HORSES + 6) == 1)
        {
            if (*(byte *)(GB_HORSES + 7) == 1)
            {

                // передать корабль игроку
                find_all_alive_units(U_BATTLESHIP);
                sort_stat(S_OWNER, P_BLUE, CMP_EQ);
                give_all(P_WHITE);

                // сели на корабль
                find_all_alive_units(ANY_UNITS);
                set_region(33, 79, 38, 90);
                sort_in_region();
                remove_all();
                viz_area_add(85, 85, 1 << P_WHITE, 5);
            }
        }

        // условие поражения

        find_all_alive_units(U_PEASANT);
        int sum = units;
        find_all_alive_units(U_DANATH);
        sum = sum + units;
        find_all_alive_units(U_RANGER);
        sum = sum + units;

        if (sum < 2)
        {
            find_all_alive_units(U_BATTLESHIP);
            sort_stat(S_OWNER, P_WHITE, CMP_EQ);
            if (units == 0)
            {
                if (*(byte *)(GB_HORSES + 14) < 2)
                    *(byte *)(GB_HORSES + 14) = *(byte *)(GB_HORSES + 14) + 1;
                else
                    lose(true);
            }
        }

        // условие победы
        set_region(80, 80, 95, 95);
        find_all_alive_units(U_BATTLESHIP);
        sort_in_region();
        if (units > 0)
        {

            if (*(byte *)(GB_HORSES + 15) < 1)
                *(byte *)(GB_HORSES + 15) = *(byte *)(GB_HORSES + 15) + 1;
            else
                win(true);
        }
    }
}

void v_human3(bool rep_init)
{
    if (rep_init)
    {
        saveload_fixed = true;
        // your initialize
    }
    else
    {
        // Враги
        ally(P_BLUE, P_VIOLET, 0);
        ally(P_BLUE, P_BLACK, 0);
        ally(P_BLACK, P_WHITE, 0);
        ally(P_BLACK, P_ORANGE, 0);
        ally(P_VIOLET, P_WHITE, 0);
        ally(P_VIOLET, P_ORANGE, 0);
        ally(P_RED, P_WHITE, 0);
        ally(P_RED, P_ORANGE, 0);

        // Союзники
        ally(P_ORANGE, P_BLUE, 1);
        ally(P_ORANGE, P_YELLOW, 1);
        ally(P_ORANGE, P_WHITE, 1);
        ally(P_ORANGE, P_GREEN, 1);
        ally(P_WHITE, P_BLUE, 1);
        ally(P_GREEN, P_BLUE, 1);
        ally(P_GREEN, P_YELLOW, 1);
        ally(P_GREEN, P_WHITE, 1);

        ally(P_BLACK, P_VIOLET, 1);
        ally(P_BLACK, P_RED, 1);
        ally(P_RED, P_VIOLET, 1);

        // обзор
        comps_vision(true); // компы могут давать виз
        viz(P_WHITE, P_BLUE, 1);
        viz(P_WHITE, P_ORANGE, 1);

        // Делаем всех фиолетовых компов черными
        find_all_alive_units(ANY_UNITS);
        sort_stat(S_OWNER, P_VIOLET, CMP_EQ);
        set_stat_all(S_COLOR, P_BLACK);

        // Делаем всех красных компов черными
        find_all_alive_units(ANY_UNITS);
        sort_stat(S_OWNER, P_RED, CMP_EQ);
        set_stat_all(S_COLOR, P_BLACK);

        // Делаем всех красных компов черными
        find_all_alive_units(ANY_BUILDING);
        sort_stat(S_OWNER, P_RED, CMP_EQ);
        set_stat_all(S_COLOR, P_BLACK);

        // Создание волн пиратов 9 волн

        if (*(byte *)(GB_HORSES + 14) < 9)
        {
            if (*(byte *)(GB_HORSES + 15) < 20) // таймер на 20
                *(byte *)(GB_HORSES + 15) = *(byte *)(GB_HORSES + 15) + 1;
            else
            {
                // создание пиратов
                unit_create(26, 46, U_GRUNT, P_BLACK, 3);
                unit_create(26, 59, U_GRUNT, P_BLACK, 8);
                unit_create(26, 72, U_GRUNT, P_BLACK, 5);

                *(byte *)(GB_HORSES + 14) += 1; // таймер 1 раз

                // если есть лесопилка создаю крестьян
                find_all_alive_units(U_HLUMBER);
                if (units != 0)
                {
                    unit_create(30, 2, U_ATTACK_PEASANT, P_ORANGE, 3);
                }

                *(byte *)(GB_HORSES + 15) = 0; // обнуляем таймер
            }

            if (*(byte *)(GB_HORSES + 14) == 6) // на 7 волне считая с нуля прибывают всадники
            {
                if (*(byte *)(GB_HORSES + 11) == 0)
                {
                    unit_create(19, 8, U_KNIGHT, P_BLUE, 12);
                    unit_create(63, 88, U_KNIGHT, P_BLUE, 12);
                    *(byte *)(GB_HORSES + 11) = 1;
                }
            }
        }

        // крестьяне бегут с лесопилки в город к бараку
        find_all_alive_units(U_ATTACK_PEASANT);
        sort_stat(S_OWNER, P_ORANGE, CMP_EQ);
        order_all(86, 2, ORDER_MOVE);

        // нападение пиратов
        find_all_alive_units(U_GRUNT);
        set_region(20, 43, 32, 50);
        sort_in_region();
        sort_stat(S_OWNER, P_BLACK, CMP_EQ);
        order_all(30, 4, ORDER_PATROL);

        find_all_alive_units(U_GRUNT);
        set_region(21, 0, 40, 10);
        sort_in_region();
        sort_stat(S_OWNER, P_BLACK, CMP_EQ);
        order_all(88, 7, ORDER_PATROL);

        find_all_alive_units(U_GRUNT);
        set_region(19, 56, 33, 62);
        sort_in_region();
        sort_stat(S_OWNER, P_BLACK, CMP_EQ);
        order_all(88, 7, ORDER_PATROL);

        find_all_alive_units(U_GRUNT);
        set_region(17, 66, 33, 77);
        sort_in_region();
        sort_stat(S_OWNER, P_BLACK, CMP_EQ);
        order_all(75, 75, ORDER_PATROL);

        find_all_alive_units(U_GRUNT);
        set_region(56, 69, 87, 84);
        sort_in_region();
        sort_stat(S_OWNER, P_BLACK, CMP_EQ);
        order_all(88, 7, ORDER_PATROL);

        find_all_alive_units(U_GRUNT);
        set_region(19, 56, 33, 62);
        sort_in_region();
        sort_stat(S_OWNER, P_BLACK, CMP_EQ);
        order_all(88, 7, ORDER_PATROL);

        // нападение кораблей пиратов
        find_all_alive_units(U_HDESTROYER);
        order_all(56, 73, ORDER_PATROL);

        // Конница сначала бежит в замок барона, а потом атачит форт пиратов
        find_all_alive_units(U_KNIGHT);
        sort_stat(S_OWNER, P_BLUE, CMP_EQ);
        give_all(P_ORANGE); // Передаём конницу оранжевому

        find_all_alive_units(U_KNIGHT);
        set_region(11, 0, 31, 15); // установить регион
        sort_in_region();
        order_all(85, 7, ORDER_PATROL);

        find_all_alive_units(U_KNIGHT);
        set_region(50, 80, 80, 93); // установить регион
        sort_in_region();
        order_all(85, 7, ORDER_PATROL);

        find_all_alive_units(U_KNIGHT);
        set_region(69, 0, 95, 23); // установить регион
        sort_in_region();
        order_all(20, 60, ORDER_PATROL);

        // Оранжевые футы патрулируют окресности
        find_all_alive_units(U_FOOTMAN);
        sort_stat(S_OWNER, P_ORANGE, CMP_EQ);
        set_region(72, 21, 95, 0); // установить регион
        sort_in_region();
        order_all(73, 19, ORDER_PATROL);

        // Лотар держит позицию
        find_all_alive_units(U_LOTHAR);
        order_all(84, 14, ORDER_STAND);

        // Превращение в футов оранжевого
        find_all_alive_units(U_ATTACK_PEASANT);
        set_region(78, 0, 86, 6);
        sort_in_region();
        give_all(P_BLUE);

        find_all_alive_units(U_ATTACK_PEASANT);
        sort_stat(S_OWNER, P_BLUE, CMP_EQ);
        unit_convert(P_BLUE, U_ATTACK_PEASANT, U_FOOTMAN, 1);

        find_all_alive_units(U_FOOTMAN);
        sort_stat(S_OWNER, P_BLUE, CMP_EQ);
        give_all(P_ORANGE);

        // Превращение в футов белого
        find_all_alive_units(U_PEASANT);
        set_region(78, 0, 86, 6);
        sort_in_region();
        sort_stat(S_OWNER, P_WHITE, CMP_EQ);
        give_all(P_GREEN);

        find_all_alive_units(U_PEASANT);
        sort_stat(S_OWNER, P_GREEN, CMP_EQ);
        unit_convert(P_GREEN, U_PEASANT, U_FOOTMAN, 1);
        give_all(P_WHITE);

        // условие поражения
        find_all_alive_units(U_LOTHAR);
        int sum = units;
        find_all_alive_units(U_DANATH);
        sum = sum + units;

        if (sum < 2)
        {
            // задержка перед поражением, чтоб успеть загрузиться
            if (*(byte *)(GB_HORSES + 4) < 2)
                *(byte *)(GB_HORSES + 4) = *(byte *)(GB_HORSES + 4) + 1;
            else
                lose(true);
        }

        // условие победы
        find_all_alive_units(U_HBARRACK);
        sort_stat(S_OWNER, P_RED, CMP_EQ);

        if (units == 0)
        {
            // задержка перед победой
            if (*(byte *)(GB_HORSES + 8) < 1)
                *(byte *)(GB_HORSES + 8) = *(byte *)(GB_HORSES + 8) + 1;
            else
                win(true);
        }
    }
}

void v_human4(bool rep_init)
{
    if (rep_init)
    {
        // your initialize
    }
    else
    {
        // Враги
        ally(P_VIOLET, P_ORANGE, 0);
        ally(P_BLACK, P_ORANGE, 0);

        // Союзники
        ally(P_WHITE, P_ORANGE, 1);
        ally(P_WHITE, P_BLUE, 1);

        // обзор
        comps_vision(true); // компы могут давать виз
        viz(P_WHITE, P_BLUE, 1);
        viz(P_WHITE, P_ORANGE, 1);

        // Только Герой и Паладины могуд наносить урон Демону
        ua[0] = U_TYRALYON;
        ut[0] = U_TROLL;
        ua[1] = U_PALADIN;
        ut[1] = U_TROLL;

        // Ритуал у Камня
        set_region(77, 32, 77, 32);
        find_all_alive_units(U_TYRALYON);
        sort_in_region();
        *(byte *)(GB_HORSES + 0) = *(byte *)(GB_HORSES + 0) + units;
        find_all_alive_units(U_PALADIN);
        sort_in_region();
        *(byte *)(GB_HORSES + 0) = *(byte *)(GB_HORSES + 0) + units;

        set_region(77, 26, 77, 26);
        find_all_alive_units(U_TYRALYON);
        sort_in_region();
        *(byte *)(GB_HORSES + 1) = *(byte *)(GB_HORSES + 1) + units;
        find_all_alive_units(U_PALADIN);
        sort_in_region();
        *(byte *)(GB_HORSES + 1) = *(byte *)(GB_HORSES + 1) + units;

        set_region(79, 29, 79, 29);
        find_all_alive_units(U_TYRALYON);
        sort_in_region();
        *(byte *)(GB_HORSES + 2) = *(byte *)(GB_HORSES + 2) + units;
        find_all_alive_units(U_PALADIN);
        sort_in_region();
        *(byte *)(GB_HORSES + 2) = *(byte *)(GB_HORSES + 2) + units;

        set_region(73, 27, 73, 27);
        find_all_alive_units(U_TYRALYON);
        sort_in_region();
        *(byte *)(GB_HORSES + 3) = *(byte *)(GB_HORSES + 3) + units;
        find_all_alive_units(U_PALADIN);
        sort_in_region();
        *(byte *)(GB_HORSES + 3) = *(byte *)(GB_HORSES + 3) + units;

        set_region(73, 31, 73, 31);
        find_all_alive_units(U_TYRALYON);
        sort_in_region();
        *(byte *)(GB_HORSES + 4) = *(byte *)(GB_HORSES + 4) + units;
        find_all_alive_units(U_PALADIN);
        sort_in_region();
        *(byte *)(GB_HORSES + 4) = *(byte *)(GB_HORSES + 4) + units;

        allow_table(P_WHITE, 3, L_EXORCISM, 1);

        if (*(byte *)(GB_HORSES + 0) != 0)
        {
            if (*(byte *)(GB_HORSES + 1) != 0)
            {
                if (*(byte *)(GB_HORSES + 2) != 0)
                {
                    if (*(byte *)(GB_HORSES + 3) != 0)
                    {
                        if (*(byte *)(GB_HORSES + 4) != 0)
                        {
                            char message[] = "RITUAL WORKING! We studied exorcism!";
                            show_message(10, message);
                            allow_table(P_WHITE, 3, L_EXORCISM, 1);
                        }
                    }
                }
            }
        }

        // приплытие паладинов
        if (*(byte *)(GB_HORSES + 5) < 2) // таймер
            *(byte *)(GB_HORSES + 5) = *(byte *)(GB_HORSES + 5) + 1;
        else
        {
            // Плывёт корабль
            find_all_alive_units(U_HTRANSPORT);
            order_all(21, 62, ORDER_MOVE);
        }

        // Создаём паладинов
        set_region(15, 56, 21, 62);
        find_all_alive_units(U_HTRANSPORT);
        sort_in_region();
        if (units > 0)
        {
            if (*(byte *)(GB_HORSES + 6) == 0)
            {
                unit_create(20, 58, U_PALADIN, P_BLUE, 4);
                char message[] = "PALADIN: We need to perform a ritual at the Runestone! To study Exorcism!";
                show_message(10, message);
                viz_area_add(76, 29, 1 << P_WHITE, 2);
                *(byte *)(GB_HORSES + 6) = 1;
                find_all_alive_units(U_PALADIN);
                give_all(P_WHITE);
            }
        }

        // Пока жив Демон создавать скелетов и слать в атаку
        find_all_alive_units(U_TROLL);
        if (units > 0)
        {
            if (*(byte *)(GB_HORSES + 7) < 10) // таймер на 20
                *(byte *)(GB_HORSES + 7) = *(byte *)(GB_HORSES + 7) + 1;
            else
            {

                unit_create(60, 35, U_SKELETON, P_BLACK, 6);
                unit_create(49, 45, U_SKELETON, P_BLACK, 5);
                unit_create(49, 45, U_DK, P_BLACK, 1);
                unit_create(93, 76, U_FOOTMAN, P_ORANGE, 8);

                // // Отправляем черного в бой с помощью убийства последнего здания
                // unit_create(94, 1, U_FARM, P_BLACK, 1);
                // find_all_alive_units(U_FARM);
                // sort_stat(S_OWNER, P_BLACK, CMP_EQ);
                // kill_all();

                *(byte *)(GB_HORSES + 7) = 0; // таймер 1 раз
            }
        }

        // нежить атакует барона
        find_all_alive_units(U_DK);
        sort_stat(S_OWNER, P_BLACK, CMP_EQ);
        order_all(80, 90, ORDER_PATROL);

        find_all_alive_units(U_SKELETON);
        sort_stat(S_OWNER, P_BLACK, CMP_EQ);
        order_all(80, 90, ORDER_PATROL);

        // Футы Барона атакуют
        find_all_alive_units(U_FOOTMAN);
        sort_stat(S_OWNER, P_ORANGE, CMP_EQ);
        order_all(40, 34, ORDER_PATROL);

        // условие поражения
        find_all_alive_units(U_TYRALYON);
        if (units == 0)
        {
            if (*(byte *)(GB_HORSES + 13) < 2)
                *(byte *)(GB_HORSES + 13) = *(byte *)(GB_HORSES + 13) + 1;
            else
                lose(true);
        }

        find_all_alive_units(U_PALADIN);
        if (units > 0 && units < 4)
        {
            // задержка перед поражением, чтоб успеть загрузиться
            if (*(byte *)(GB_HORSES + 14) < 2)
                *(byte *)(GB_HORSES + 14) = *(byte *)(GB_HORSES + 14) + 1;
            else
                lose(true);
        }

        // условие победы
        find_all_alive_units(U_ALTAR);

        if (units == 0)
        {
            // задержка перед победой
            if (*(byte *)(GB_HORSES + 15) < 1)
                *(byte *)(GB_HORSES + 15) = *(byte *)(GB_HORSES + 15) + 1;
            else
                win(true);
        }
    }
}

void v_human5(bool rep_init)
{
    if (rep_init)
    {
        // your initialize
    }
    else
    {
        // your custom victory conditions
    }
}

void v_human6(bool rep_init)
{
    if (rep_init)
    {
        // your initialize
    }
    else
    {
        // your custom victory conditions
    }
}

void v_human7(bool rep_init)
{
    if (rep_init)
    {
        // your initialize
    }
    else
    {
        // your custom victory conditions
    }
}

void v_human8(bool rep_init)
{
    if (rep_init)
    {
        // your initialize
    }
    else
    {
        // your custom victory conditions
    }
}

void v_human9(bool rep_init)
{
    if (rep_init)
    {
        // your initialize
    }
    else
    {
        // your custom victory conditions
    }
}

void v_human10(bool rep_init)
{
    if (rep_init)
    {
        // your initialize
    }
    else
    {
        // your custom victory conditions
    }
}

void v_human11(bool rep_init)
{
    if (rep_init)
    {
        // your initialize
    }
    else
    {
        // your custom victory conditions
    }
}

void v_human12(bool rep_init)
{
    if (rep_init)
    {
        // your initialize
    }
    else
    {
        // your custom victory conditions
    }
}

void v_human13(bool rep_init)
{
    if (rep_init)
    {
        // your initialize
    }
    else
    {
        // your custom victory conditions
    }
}

void v_human14(bool rep_init)
{
    if (rep_init)
    {
        // your initialize
    }
    else
    {
        // your custom victory conditions
    }
}

void v_xhuman1(bool rep_init)
{
    if (rep_init)
    {
        // your initialize
    }
    else
    {
        // your custom victory conditions
    }
}

void v_xhuman2(bool rep_init)
{
    if (rep_init)
    {
        // your initialize
    }
    else
    {
        // your custom victory conditions
    }
}

void v_xhuman3(bool rep_init)
{
    if (rep_init)
    {
        // your initialize
    }
    else
    {
        // your custom victory conditions
    }
}

void v_xhuman4(bool rep_init)
{
    if (rep_init)
    {
        // your initialize
    }
    else
    {
        // your custom victory conditions
    }
}

void v_xhuman5(bool rep_init)
{
    if (rep_init)
    {
        // your initialize
    }
    else
    {
        // your custom victory conditions
    }
}

void v_xhuman6(bool rep_init)
{
    if (rep_init)
    {
        // your initialize
    }
    else
    {
        // your custom victory conditions
    }
}

void v_xhuman7(bool rep_init)
{
    if (rep_init)
    {
        // your initialize
    }
    else
    {
        // your custom victory conditions
    }
}

void v_xhuman8(bool rep_init)
{
    if (rep_init)
    {
        // your initialize
    }
    else
    {
        // your custom victory conditions
    }
}

void v_xhuman9(bool rep_init)
{
    if (rep_init)
    {
        // your initialize
    }
    else
    {
        // your custom victory conditions
    }
}

void v_xhuman10(bool rep_init)
{
    if (rep_init)
    {
        // your initialize
    }
    else
    {
        // your custom victory conditions
    }
}

void v_xhuman11(bool rep_init)
{
    if (rep_init)
    {
        // your initialize
    }
    else
    {
        // your custom victory conditions
    }
}

void v_xhuman12(bool rep_init)
{
    if (rep_init)
    {
        // your initialize
    }
    else
    {
        // your custom victory conditions
    }
}

void v_orc1(bool rep_init)
{
    if (rep_init)
    {
        // your initialize
    }
    else
    {
        // your custom victory conditions
    }
}

void v_orc2(bool rep_init)
{
    if (rep_init)
    {
        // your initialize
    }
    else
    {
        // your custom victory conditions
    }
}

void v_orc3(bool rep_init)
{
    if (rep_init)
    {
        // your initialize
    }
    else
    {
        // your custom victory conditions
    }
}

void v_orc4(bool rep_init)
{
    if (rep_init)
    {
        // your initialize
    }
    else
    {
        // your custom victory conditions
    }
}

void v_orc5(bool rep_init)
{
    if (rep_init)
    {
        // your initialize
    }
    else
    {
        // your custom victory conditions
    }
}

void v_orc6(bool rep_init)
{
    if (rep_init)
    {
        // your initialize
    }
    else
    {
        // your custom victory conditions
    }
}

void v_orc7(bool rep_init)
{
    if (rep_init)
    {
        // your initialize
    }
    else
    {
        // your custom victory conditions
    }
}

void v_orc8(bool rep_init)
{
    if (rep_init)
    {
        // your initialize
    }
    else
    {
        // your custom victory conditions
    }
}

void v_orc9(bool rep_init)
{
    if (rep_init)
    {
        // your initialize
    }
    else
    {
        // your custom victory conditions
    }
}

void v_orc10(bool rep_init)
{
    if (rep_init)
    {
        // your initialize
    }
    else
    {
        // your custom victory conditions
    }
}

void v_orc11(bool rep_init)
{
    if (rep_init)
    {
        // your initialize
    }
    else
    {
        // your custom victory conditions
    }
}

void v_orc12(bool rep_init)
{
    if (rep_init)
    {
        // your initialize
    }
    else
    {
        // your custom victory conditions
    }
}

void v_orc13(bool rep_init)
{
    if (rep_init)
    {
        // your initialize
    }
    else
    {
        // your custom victory conditions
    }
}

void v_orc14(bool rep_init)
{
    if (rep_init)
    {
        // your initialize
    }
    else
    {
        // your custom victory conditions
    }
}

void v_xorc1(bool rep_init)
{
    if (rep_init)
    {
        // your initialize
    }
    else
    {
        // your custom victory conditions
    }
}

void v_xorc2(bool rep_init)
{
    if (rep_init)
    {
        // your initialize
    }
    else
    {
        // your custom victory conditions
    }
}

void v_xorc3(bool rep_init)
{
    if (rep_init)
    {
        // your initialize
    }
    else
    {
        // your custom victory conditions
    }
}

void v_xorc4(bool rep_init)
{
    if (rep_init)
    {
        // your initialize
    }
    else
    {
        // your custom victory conditions
    }
}

void v_xorc5(bool rep_init)
{
    if (rep_init)
    {
        // your initialize
    }
    else
    {
        // your custom victory conditions
    }
}

void v_xorc6(bool rep_init)
{
    if (rep_init)
    {
        // your initialize
    }
    else
    {
        // your custom victory conditions
    }
}

void v_xorc7(bool rep_init)
{
    if (rep_init)
    {
        // your initialize
    }
    else
    {
        // your custom victory conditions
    }
}

void v_xorc8(bool rep_init)
{
    if (rep_init)
    {
        // your initialize
    }
    else
    {
        // your custom victory conditions
    }
}

void v_xorc9(bool rep_init)
{
    if (rep_init)
    {
        // your initialize
    }
    else
    {
        // your custom victory conditions
    }
}

void v_xorc10(bool rep_init)
{
    if (rep_init)
    {
        // your initialize
    }
    else
    {
        // your custom victory conditions
    }
}

void v_xorc11(bool rep_init)
{
    if (rep_init)
    {
        // your initialize
    }
    else
    {
        // your custom victory conditions
    }
}

void v_xorc12(bool rep_init)
{
    if (rep_init)
    {
        // your initialize
    }
    else
    {
        // your custom victory conditions
    }
}

void v_custom(bool rep_init)
{
    if (rep_init)
    {
        // pathfind_fix(true);
        // ai_fix_plugin(true);
        // your initialize
    }
    else
    {
        // your custom victory conditions
    }
}

void (*triggers[])(bool) = {v_human1, v_orc1, v_human2, v_orc2, v_human3, v_orc3, v_human4, v_orc4, v_human5, v_orc5, v_human6, v_orc6, v_human7, v_orc7, v_human8, v_orc8, v_human9, v_orc9, v_human10, v_orc10, v_human11, v_orc11, v_human12, v_orc12, v_human13, v_orc13, v_human14, v_orc14, v_xhuman1, v_xorc1, v_xhuman2, v_xorc2, v_xhuman3, v_xorc3, v_xhuman4, v_xorc4, v_xhuman5, v_xorc5, v_xhuman6, v_xorc6, v_xhuman7, v_xorc7, v_xhuman8, v_xorc8, v_xhuman9, v_xorc9, v_xhuman10, v_xorc10, v_xhuman11, v_xorc11, v_xhuman12, v_xorc12};

void trig()
{
    byte lvl = *(byte *)LEVEL_OBJ;
    if (a_custom)
    {
        v_custom(false);
    }
    else
    {
        if ((lvl >= 0) && (lvl < 52))
            ((void (*)(bool))triggers[lvl])(false);
        else
            v_custom(false);
    }
    first_step = false;
    memset(all_kills, 0, sizeof(all_kills));
}

void trig_init()
{
    first_step = true;
    byte lvl = *(byte *)LEVEL_OBJ;
    if (a_custom)
    {
        v_custom(true);
    }
    else
    {
        if ((lvl >= 0) && (lvl < 52))
            ((void (*)(bool))triggers[lvl])(true);
        else
            v_custom(true);
    }
}

void replace_def()
{
    // set all vars to default
    memset(ua, 255, sizeof(ua));
    memset(ut, 255, sizeof(ut));
    memset(runes, 0, sizeof(runes));
    memset(hhero, 0, sizeof(hhero));
    memset(ohero, 0, sizeof(ohero));
    memset(heros, 0, sizeof(heros));
    memset(herosb, false, sizeof(herosb));
    memset(m_slow_aura, 255, sizeof(m_slow_aura));
    memset(m_death_aura, 255, sizeof(m_death_aura));
    memset(m_sneak, 255, sizeof(m_sneak));
    memset(m_devotion, 255, sizeof(m_devotion));
    memset(m_vampire, 255, sizeof(m_vampire));
    memset(m_prvnt, 255, sizeof(m_prvnt));
    memset(vizs_areas, 0, sizeof(vizs_areas));
    vizs_n = 0;
    table = false;
    agr = false;
    cpt = false;
    pcpt = false;
    thcpt = false;
    ucpt = false;
    steal = false;
    aport = false;
    mport = false;
    b3rune = false;
    b3port = false;
    b3cirl = false;
    b3mine = false;
    b3forest = false;
    apn = false;
    manaburn = false;
    A_runestone = false;
    A_portal = false;
    A_transport = false;
    A_autoheal = false;
    blood_f = false;
    more_res = false;
    path_fixed = false;
    ai_fixed = false;
    saveload_fixed = false;
    peon_steal = false;
}

void replace_common()
{
    // peon can build any buildings
    char ballbuildings[] = "\x0\x0"; // d1 05
    PATCH_SET((char *)BUILD_ALL_BUILDINGS1, ballbuildings);
    char ballbuildings2[] = "\x0"; // 0a
    PATCH_SET((char *)BUILD_ALL_BUILDINGS2, ballbuildings2);
    char ballbuildings3[] = "\x0"; // 68
    PATCH_SET((char *)BUILD_ALL_BUILDINGS3, ballbuildings3);

    // any building can train any unit
    char ballunits[] = "\xeb"; // 0x74
    PATCH_SET((char *)BUILD_ALL_UNITS1, ballunits);
    char ballunits2[] = "\xA1\xBC\x47\x49\x0\x90\x90"; // 8b 04 85 bc 47 49 00
    PATCH_SET((char *)BUILD_ALL_UNITS2, ballunits2);

    // show kills
    byte d = S_KILLS;
    char sdmg[] = "\x8a\x90\x82\x0\x0\x0\x8b\xfa"; // units
    sdmg[2] = d;
    PATCH_SET((char *)SPEED_STAT_UNITS, sdmg);
    char sdmg2[] = "\x8a\x82\x82\x0\x0\x0\x90\x90\x90"; // catas
    sdmg2[2] = d;
    PATCH_SET((char *)SPEED_STAT_CATS, sdmg2);
    char sdmg3[] = "\x8a\x88\x82\x0\x0\x0\x90\x90\x90"; // archers
    sdmg3[2] = d;
    PATCH_SET((char *)SPEED_STAT_ARCHERS, sdmg3);
    char sdmg4[] = "\x8a\x82\x82\x0\x0\x0\x90\x90\x90"; // berserkers
    sdmg4[2] = d;
    PATCH_SET((char *)SPEED_STAT_BERSERKERS, sdmg4);
    char sdmg5[] = "\x8a\x88\x82\x0\x0\x0\x90\x90\x90"; // ships
    sdmg5[2] = d;
    PATCH_SET((char *)SPEED_STAT_SHIPS, sdmg5);

    char dmg_fix[] = "\xeb";
    PATCH_SET((char *)DMG_FIX, dmg_fix);

    th_change(true);
    draw_stats_fix(true);
}

void replace_back()
{
    // replace all to default
    comps_vision(false);
    sheep(false);
    build3(false);
    demon(false);
    th_change(false);
    repair_all(false);
    repair_cat(false);
    trigger_time('\xc8');
    fireball_dmg(40);
    buff_time(0, 0xd0, 0x7);
    buff_time(1, 0xe8, 0x3);
    buff_time(2, 0xf4, 0x1);
    buff_time(3, 0xe8, 0x3);
    buff_time(4, 0xe8, 0x3);
    upgr(SWORDS, 2);
    upgr(ARMOR, 2);
    upgr(ARROWS, 1);
    upgr(SHIP_DMG, 5);
    upgr(SHIP_ARMOR, 5);
    upgr(CATA_DMG, 15);
    manacost(VISION, 70);
    manacost(HEAL, 6);
    manacost(GREATER_HEAL, 5);
    manacost(EXORCISM, 1);
    manacost(FIREBALL, 100);
    manacost(FLAME_SHIELD, 80);
    manacost(SLOW, 50);
    manacost(INVIS, 200);
    manacost(POLYMORPH, 200);
    manacost(BLIZZARD, 25);
    manacost(EYE_OF_KILROG, 70);
    manacost(BLOOD, 50);
    manacost(RAISE_DEAD, 50);
    manacost(COIL, 100);
    manacost(WHIRLWIND, 100);
    manacost(HASTE, 50);
    manacost(UNHOLY_ARMOR, 100);
    manacost(RUNES, 200);
    manacost(DEATH_AND_DECAY, 25);
    no_random_dmg(false);
    blood_fix(false);
    draw_stats_fix(false);
    brclik(false);
    rc_jmp(false);
    autoheal(false);
    multicast_fix(false);
    pathfind_fix(false);
    ai_fix_plugin(false);
}

void replace_trigger()
{
    replace_back();
    replace_def();
    replace_common();

    // replace original victory trigger
    char trig_jmp[] = "\x74\x1A"; // 74 0F
    PATCH_SET((char *)SINGLEPLAYER_JMP, trig_jmp);
    char rep[] = "\xc7\x05\x38\x0d\x4c\x0\x30\x8C\x45\x0";
    void (*repf)() = trig;
    patch_setdword((DWORD *)(rep + 6), (DWORD)repf);
    PATCH_SET((char *)SINGLEPLAYER_TRIGGER, rep);
    trig_init();
}

PROC g_proc_0042A4A1;
void new_game(int a, int b, long c)
{
    a_custom = b % 256; // custom game or campaign
    if (a_custom)
        *(byte *)LEVEL_OBJ = 53; // remember custom obj
    else
    {
        if (*(byte *)LEVEL_OBJ == 53)
            a_custom = 1; // fix for when saved game loads custom get broken
    }
    replace_trigger();
    memset((void *)GB_HORSES, 0, 16 * sizeof(byte));
    ((void (*)(int, int, long))g_proc_0042A4A1)(a, b, c); // original
}

PROC g_proc_0041F7E4;
int load_game(int a)
{
    int original = ((int (*)(int))g_proc_0041F7E4)(a); // original
    replace_trigger();
    return original;
}

void hook(int adr, PROC *p, char *func)
{
    *p = patch_call((char *)adr, func);
}

int *portrait_unit;

PROC g_proc_0044538D;
void grp_portrait_init(int *a)
{
    ((void (*)(int *))g_proc_0044538D)(a); // original
    portrait_unit = (int *)*((int *)((uintptr_t)a + 0x26));
}

PROC g_proc_004453A7; // draw unit portrait
void grp_draw_portrait(void *grp, byte frame, int b, int c)
{
    bool f = true;
    void *new_grp = NULL;
    //-------------------------------------------------
    int *u = portrait_unit;
    if (u != NULL)
    {
        byte id = *((byte *)((uintptr_t)u + S_ID));
        if (id == U_PEASANT)
        {
            if ((*(byte *)LEVEL_OBJ == LVL_HUMAN1) || (*(byte *)LEVEL_OBJ == LVL_HUMAN2))
            {

                byte o = *((byte *)((uintptr_t)u + S_OWNER));
                // if (o != *(byte *)LOCAL_PLAYER)
                // {
                new_grp = grg_hugh_icon;
                frame = 0;
                f = false;
            }
        }
    }

    if (f)
    {
        byte era = *(byte *)MAP_ERA;
        if (era == 0)
            new_grp = grp_drogn_icons;
        else if (era == 1)
            new_grp = grp_drogn_icons;
        else if (era == 2)
            new_grp = grp_drogn_icons;
        else if (era == 3)
            new_grp = grp_drogn_icons;
    }

    if (new_grp)
        return ((void (*)(void *, byte, int, int))g_proc_004453A7)(new_grp, frame, b, c);
    else
        return ((void (*)(void *, byte, int, int))g_proc_004453A7)(grp, frame, b, c); // original
}

void files_hooks()
{
    files_init();
    hook(0x0044A65C, &g_proc_0044A65C, (char *)status_get_tbl);               // имя в статусе около иконки
    hook(0x0044AC83, &g_proc_0044AC83, (char *)unit_hover_get_id);            // id
    hook(0x0044AE27, &g_proc_0044AE27, (char *)unit_hover_get_tbl);           // имя при наведении внизу
    hook(0x004354C8, &g_proc_004354C8, (char *)objct_get_tbl_custom);         // цель миссии в меню для одиночной миссии
    hook(0x004354FA, &g_proc_004354FA, (char *)objct_get_tbl_campanign);      // цель миссии для компании
    hook(0x004300A5, &g_proc_004300A5, (char *)objct_get_tbl_briefing_task);  // цель миссии в брифинге
    hook(0x004300CA, &g_proc_004300CA, (char *)objct_get_tbl_briefing_title); // название миссии в брифинге
    hook(0x004301CA, &g_proc_004301CA, (char *)objct_get_tbl_briefing_text);  // текст брифинга
    hook(0x00430113, &g_proc_00430113, (char *)objct_get_briefing_speech);    // звук брифинга
    hook(0x0041F97D, &g_proc_0041F97D, (char *)map_file_load);
    hook(0x00454BCA, &g_proc_00454BCA, (char *)grp_draw_unit);          // замена моделек юнитов
    hook(0x0041F1E8, &g_proc_0041F1E8, (char *)finale_credits_get_tbl); // Вводная
    hook(0x0041F027, &g_proc_0041F027, (char *)finale_get_speech);      // Финалочка вроде как
    hook(0x0041F0F5, &g_proc_0041F0F5, (char *)finale_get_tbl);
    hook(0x0041C51C, &g_proc_0041C51C, (char *)netstat_get_tbl_nation); // нации при победе или поражении
    hook(0x004453A7, &g_proc_004453A7, (char *)grp_draw_portrait);      // icons
    hook(0x0044538D, &g_proc_0044538D, (char *)grp_portrait_init);
}

void common_hooks()
{
    hook(0x0045271B, &g_proc_0045271B, (char *)update_spells);

    hook(0x004522B9, &g_proc_004522B9, (char *)seq_run);

    hook(0x0041038E, &g_proc_0041038E, (char *)damage1);
    hook(0x00409F3B, &g_proc_00409F3B, (char *)damage2);
    hook(0x0040AF70, &g_proc_0040AF70, (char *)damage3);
    hook(0x0040AF99, &g_proc_0040AF99, (char *)damage4);
    hook(0x00410762, &g_proc_00410762, (char *)damage5);
    hook(0x004428AD, &g_proc_004428AD, (char *)damage6);

    hook(0x0043BAE1, &g_proc_0043BAE1, (char *)rc_snd);
    hook(0x0043B943, &g_proc_0043B943, (char *)rc_build_click);
    hook(0x0040DF71, &g_proc_0040DF71, (char *)bld_unit_create);
    hook(0x0040AFBF, &g_proc_0040AFBF, (char *)tower_find_attacker);
    hook(0x00451728, &g_proc_00451728, (char *)unit_kill_deselect);

    hook(0x0045614E, &g_proc_0045614E, (char *)receive_cheat);

    hook(0x004526FE, &g_proc_004526FE, (char *)grow_structure);
    hook(0x00418FFE, &g_proc_00418FFE, (char *)create_building);
    hook(0x0043A974, &g_proc_0043A974, (char *)placebox_query);
    hook(0x0043ABAB, &g_proc_0043ABAB, (char *)placebox_query);
    hook(0x00424745, &g_proc_00424745, (char *)goods_into_inventory);
    hook(0x004529C0, &g_proc_004529C0, (char *)goods_into_inventory);

    hook(0x00451054, &g_proc_00451054, (char *)count_add_to_tables_load_game);
    hook(0x00438A5C, &g_proc_00438A5C, (char *)unset_peon_ai_flags);
    hook(0x00438985, &g_proc_00438985, (char *)unset_peon_ai_flags);

    hook(0x0040EEDD, &g_proc_0040EEDD, (char *)upgrade_tower);
    hook(0x00442E25, &g_proc_00442E25, (char *)create_skeleton);
    hook(0x00425D1C, &g_proc_00425D1C, (char *)cast_raise);
    hook(0x00424F94, &g_proc_00424F94, (char *)cast_runes);
    hook(0x00424FD7, &g_proc_00424FD7, (char *)cast_runes);
    hook(0x0042757E, &g_proc_0042757E, (char *)ai_spell);
    hook(0x00427FAE, &g_proc_00427FAE, (char *)ai_attack);
    hook(0x00451255, &g_proc_00451255, (char *)transport_unit_place);

    hook(0x0042A4A1, &g_proc_0042A4A1, (char *)new_game);
    hook(0x0041F7E4, &g_proc_0041F7E4, (char *)load_game);
    // Act image
    hook(0x00429625, &g_proc_00429625, (char *)pcx_load_act);
    hook(0x00429654, &g_proc_00429654, (char *)pcx_load_act);
    // Act text
    hook(0x0042968A, &g_proc_0042968A, (char *)act_get_tbl_small);
    hook(0x004296A9, &g_proc_004296A9, (char *)act_get_tbl_big);
    // menu
    hook(0x004372EE, &g_proc_004372EE, (char *)pcx_load_menu);
}

void capture_fix()
{
    char buf[] = "\xB0\x01\xF6\xC1\x02\x74\x02\xB0\x02\x50\x66\x8B\x7B\x18\x66\x8B\x6B\x1A\x8B\xD7\x8B\xF5\x29\xC2\x29\xC6\x8D\x43\x27\x31\xC9\x89\x44\x24\x24\x8A\x08\xC1\xE1\x02\x66\x8B\x81\x1C\xEE\x4C\x00\x66\x8B\x89\x1E\xEE\x4C\x00\x66\x01\xF8\x66\x01\xE9\x5D\x01\xE8\x01\xE9\x90\x90";
    PATCH_SET((char *)CAPTURE_BUG, buf);
}

void common_init()
{
    sounds_tables();          // fix hero sounds
    capture_fix();            // fix capture big units from right side
    upgr_check_replace(true); // fix upgrades big lvl draw buttons
}

extern "C" __declspec(dllexport) void w2p_init()
{
    files_hooks();
    common_hooks(); // hook functions
    common_init();
}