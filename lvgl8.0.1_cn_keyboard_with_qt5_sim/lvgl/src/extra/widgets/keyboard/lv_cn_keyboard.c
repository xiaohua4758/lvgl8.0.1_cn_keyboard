
/**
 * @file lv_cn_keyboard.c
 */

/*
 * modify from lv_keyboard.c
 * qq:1223419620
*/

/*********************
 *      INCLUDES
 *********************/
#include "lv_cn_keyboard.h"
#include "../../../lvgl.h"
#include <stdio.h>

#if LV_USE_CN_KEYBOARD

/*********************
 *      DEFINES
 *********************/

/**********************
 *      TYPEDEFS
 **********************/

/**********************
 *  STATIC PROTOTYPES
 **********************/
static void lv_cn_keyboard_constructor(const lv_obj_class_t * class_p, lv_obj_t * obj);
static void lv_cn_keyboard_destructor(const lv_obj_class_t * class_p, lv_obj_t * obj);
static void lv_cn_keyboard_update_map(lv_obj_t * obj, uint8_t en_or_cn);
static void creat_candidate_box(lv_obj_t *parent);
static void switch_lang_type(uint8_t l_mode);

/**********************
 *  STATIC VARIABLES
 **********************/
const lv_obj_class_t lv_cn_keyboard_class = {
    .constructor_cb = lv_cn_keyboard_constructor,
    .destructor_cb = lv_cn_keyboard_destructor,
    .width_def = LV_PCT(100),
    .height_def = LV_PCT(50),
    .instance_size = sizeof(lv_cn_keyboard_t),
    .editable = 1,
    .base_class = &lv_btnmatrix_class
};

static const char * const default_kb_map_lc[] = {" ", "\n",
                                                 " ", "\n",
                                                 "1#", "q", "w", "e", "r", "t", "y", "u", "i", "o", "p", LV_SYMBOL_BACKSPACE, "\n",
                                                 "ABC", "a", "s", "d", "f", "g", "h", "j", "k", "l", LV_SYMBOL_NEW_LINE, "\n",
                                                 "_", "-", "z", "x", "c", "v", "b", "n", "m", ".", ",", ":", "\n",
                                                 LV_SYMBOL_KEYBOARD, "En/Cn", LV_SYMBOL_LEFT, " ", LV_SYMBOL_RIGHT, LV_SYMBOL_OK, ""
                                                };

static const lv_btnmatrix_ctrl_t default_kb_ctrl_lc_map[] = {
    LV_BTNMATRIX_CTRL_HIDDEN, LV_BTNMATRIX_CTRL_HIDDEN,
    LV_CN_KEYBOARD_CTRL_BTN_FLAGS | 5, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, LV_BTNMATRIX_CTRL_CHECKED | 7,
    LV_CN_KEYBOARD_CTRL_BTN_FLAGS | 6, 3, 3, 3, 3, 3, 3, 3, 3, 3, LV_BTNMATRIX_CTRL_CHECKED | 7,
    LV_BTNMATRIX_CTRL_CHECKED | 1, LV_BTNMATRIX_CTRL_CHECKED | 1,  1, 1, 1, 1, 1, 1, 1, LV_BTNMATRIX_CTRL_CHECKED | 1, LV_BTNMATRIX_CTRL_CHECKED | 1, LV_BTNMATRIX_CTRL_CHECKED | 1,
    LV_CN_KEYBOARD_CTRL_BTN_FLAGS | 2, LV_CN_KEYBOARD_CTRL_BTN_FLAGS | 3, LV_BTNMATRIX_CTRL_CHECKED | 2, 6, LV_BTNMATRIX_CTRL_CHECKED | 2, LV_CN_KEYBOARD_CTRL_BTN_FLAGS | 2
};

static const char * const default_kb_map_uc[] = {" ", "\n",
                                                 " ", "\n",
                                                 "1#", "Q", "W", "E", "R", "T", "Y", "U", "I", "O", "P", LV_SYMBOL_BACKSPACE, "\n",
                                                 "abc", "A", "S", "D", "F", "G", "H", "J", "K", "L", LV_SYMBOL_NEW_LINE, "\n",
                                                 "_", "-", "Z", "X", "C", "V", "B", "N", "M", ".", ",", ":", "\n",
                                                 LV_SYMBOL_KEYBOARD, "En/Cn", LV_SYMBOL_LEFT, " ", LV_SYMBOL_RIGHT, LV_SYMBOL_OK, ""
                                                };

static const lv_btnmatrix_ctrl_t default_kb_ctrl_uc_map[] = {
    LV_BTNMATRIX_CTRL_HIDDEN, LV_BTNMATRIX_CTRL_HIDDEN,
    LV_CN_KEYBOARD_CTRL_BTN_FLAGS | 5, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, LV_BTNMATRIX_CTRL_CHECKED | 7,
    LV_CN_KEYBOARD_CTRL_BTN_FLAGS | 6, 3, 3, 3, 3, 3, 3, 3, 3, 3, LV_BTNMATRIX_CTRL_CHECKED | 7,
    LV_BTNMATRIX_CTRL_CHECKED | 1, LV_BTNMATRIX_CTRL_CHECKED | 1, 1, 1, 1, 1, 1, 1,  1, LV_BTNMATRIX_CTRL_CHECKED | 1, LV_BTNMATRIX_CTRL_CHECKED | 1, LV_BTNMATRIX_CTRL_CHECKED | 1,
    LV_CN_KEYBOARD_CTRL_BTN_FLAGS | 2, LV_CN_KEYBOARD_CTRL_BTN_FLAGS | 3, LV_BTNMATRIX_CTRL_CHECKED | 2, 6, LV_BTNMATRIX_CTRL_CHECKED | 2, LV_CN_KEYBOARD_CTRL_BTN_FLAGS | 2
};

static const char * const default_kb_map_spec[] = {" ", "\n",
                                                   " ", "\n",
                                                   "1", "2", "3", "4", "5", "6", "7", "8", "9", "0", LV_SYMBOL_BACKSPACE, "\n",
                                                   "abc", "+", "-", "/", "*", "=", "%", "!", "?", "#", "<", ">", "\n",
                                                   "\\",  "@", "$", "(", ")", "{", "}", "[", "]", ";", "\"", "'", "\n",
                                                   LV_SYMBOL_KEYBOARD, "En/Cn", LV_SYMBOL_LEFT, " ", LV_SYMBOL_RIGHT, LV_SYMBOL_OK, ""
                                                  };

static const lv_btnmatrix_ctrl_t default_kb_ctrl_spec_map[] = {
    LV_BTNMATRIX_CTRL_HIDDEN, LV_BTNMATRIX_CTRL_HIDDEN,
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, LV_BTNMATRIX_CTRL_CHECKED | 2,
    LV_CN_KEYBOARD_CTRL_BTN_FLAGS | 2, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
    LV_CN_KEYBOARD_CTRL_BTN_FLAGS | 2, LV_CN_KEYBOARD_CTRL_BTN_FLAGS | 3, LV_BTNMATRIX_CTRL_CHECKED | 2, 6, LV_BTNMATRIX_CTRL_CHECKED | 2, LV_CN_KEYBOARD_CTRL_BTN_FLAGS | 2
};

static const char * const default_kb_map_num[] = {"1", "2", "3", LV_SYMBOL_KEYBOARD, "\n",
                                                  "4", "5", "6", LV_SYMBOL_OK, "\n",
                                                  "7", "8", "9", LV_SYMBOL_BACKSPACE, "\n",
                                                  "+/-", "0", ".", LV_SYMBOL_LEFT, LV_SYMBOL_RIGHT, ""
                                                 };

static const lv_btnmatrix_ctrl_t default_kb_ctrl_num_map[] = {
    1, 1, 1, LV_CN_KEYBOARD_CTRL_BTN_FLAGS | 2,
    1, 1, 1, LV_CN_KEYBOARD_CTRL_BTN_FLAGS | 2,
    1, 1, 1, 2,
    1, 1, 1, 1, 1
};

static const char * * en_kb_map[5] = {
    (const char * *)&default_kb_map_lc[4],
    (const char * *)&default_kb_map_uc[4],
    (const char * *)&default_kb_map_spec[4],
    (const char * *)default_kb_map_num,
    (const char * *)NULL,
};

static const lv_btnmatrix_ctrl_t * en_kb_ctrl[5] = {
    &default_kb_ctrl_lc_map[2],
    &default_kb_ctrl_uc_map[2],
    &default_kb_ctrl_spec_map[2],
    default_kb_ctrl_num_map,
    NULL,
};

#if (MONITOR_HOR_RES <= 320)
static const char * * cn_kb_map[5] = {
    (const char * *)default_kb_map_lc,
    (const char * *)default_kb_map_uc,
    (const char * *)default_kb_map_spec,
    (const char * *)default_kb_map_num,
    (const char * *)NULL,
};
static const lv_btnmatrix_ctrl_t * cn_kb_ctrl[5] = {
    default_kb_ctrl_lc_map,
    default_kb_ctrl_uc_map,
    default_kb_ctrl_spec_map,
    default_kb_ctrl_num_map,
    NULL,
};
#else
static const char * * cn_kb_map[5] = {
    (const char * *)&default_kb_map_lc[2],
    (const char * *)&default_kb_map_uc[2],
    (const char * *)&default_kb_map_spec[2],
    (const char * *)default_kb_map_num,
    (const char * *)NULL,
};
static const lv_btnmatrix_ctrl_t * cn_kb_ctrl[5] = {
    &default_kb_ctrl_lc_map[1],
    &default_kb_ctrl_uc_map[1],
    &default_kb_ctrl_spec_map[1],
    default_kb_ctrl_num_map,
    NULL,
};
#endif

typedef struct cn_input_method_struct{
    uint8_t lang_type;              // 0-英文输入法, 1-中文输入法
    lv_font_t *font;                // 字体(转换出来的中文字体)
    lv_obj_t *obj_key_board;        // 键盘句柄
    lv_obj_t *obj_candidate_box;    // 候选框
    lv_obj_t *cand_textarea;        // 候选框的textarea
    lv_obj_t *pinyin_tip;           // 拼音字母textarea
    uint8_t ext_cand_flag;          // 0-英文(无候选框) 1-单行候选 2-多行候选
    lv_obj_t *cand_btn_label;       // 0-向下箭头(单行候选), 1-向上箭头(多行候选)
    char *src_txt;
}cn_ime_struct;

static cn_ime_struct *cn_ime = NULL;

/**********************
 *      MACROS
 **********************/

/**********************
 *   GLOBAL FUNCTIONS
 **********************/
/**
 * Create a keyboard objects
 * @param par pointer to an object, it will be the parent of the new keyboard
 * @return pointer to the created keyboard
 */
lv_obj_t * lv_cn_keyboard_create(lv_obj_t * parent, const lv_font_t *cn_font)
{
    LV_LOG_INFO("begin")
    if(cn_ime)
        return NULL;         //仅允许一个实例
    else{
        cn_ime = lv_mem_alloc(sizeof(cn_ime_struct));
        if(NULL == cn_ime)
            return NULL;     //error
        lv_memset(cn_ime, 0, sizeof(cn_ime_struct));
        cn_ime->font = (lv_font_t *)cn_font;
    }

    lv_obj_t * obj = lv_obj_class_create_obj(&lv_cn_keyboard_class, parent);
    lv_obj_class_init_obj(obj);
    lv_group_remove_obj(obj);
    return obj;
}

/*=====================
 * Setter functions
 *====================*/

/**
 * Assign a Text Area to the Keyboard. The pressed characters will be put there.
 * @param kb pointer to a Keyboard object
 * @param ta pointer to a Text Area object to write there
 */
void lv_cn_keyboard_set_textarea(lv_obj_t * obj, lv_obj_t * ta)
{
    if(ta) {
        LV_ASSERT_OBJ(ta, &lv_textarea_class);
    }

    lv_cn_keyboard_t * keyboard = (lv_cn_keyboard_t *)obj;

    /*Hide the cursor of the old Text area if cursor management is enabled*/
    if(keyboard->ta) {
        lv_obj_clear_state(obj, LV_STATE_FOCUSED);
    }

    keyboard->ta = ta;

    /*Show the cursor of the new Text area if cursor management is enabled*/
    if(keyboard->ta) {
        lv_obj_add_flag(obj, LV_STATE_FOCUSED);
    }

    if(keyboard->ta){
        if(cn_ime->lang_type){
            lv_obj_clear_flag(cn_ime->obj_candidate_box, LV_OBJ_FLAG_HIDDEN);   //cn
            lv_obj_scroll_to_y(cn_ime->obj_candidate_box, 0, LV_ANIM_OFF);
        }
        else                        //eng
            lv_obj_add_flag(cn_ime->obj_candidate_box, LV_OBJ_FLAG_HIDDEN);
    }
    else
        lv_obj_add_flag(cn_ime->obj_candidate_box, LV_OBJ_FLAG_HIDDEN);
}

/* set keyboard type
 * kb_type-0: en keyboard, kb_type-1: cn keyboard,
*/
void lv_cn_keyboard_set_kb_type(uint8_t kb_type)
{
    if(NULL != cn_ime){
        cn_ime->lang_type = (0 == kb_type) ? 0 : 1;
        if(NULL != cn_ime->obj_key_board){
            lv_cn_keyboard_update_map(cn_ime->obj_key_board, cn_ime->lang_type);
            switch_lang_type(cn_ime->lang_type);
        }
    }
}

/**
 * Set a new a mode (text or number map)
 * @param kb pointer to a Keyboard object
 * @param mode the mode from 'lv_keyboard_mode_t'
 */
void lv_cn_keyboard_set_mode(lv_obj_t * obj, lv_cn_keyboard_mode_t mode)
{
    lv_cn_keyboard_t * keyboard = (lv_cn_keyboard_t *)obj;
    if(keyboard->mode == mode) return;

    keyboard->mode = mode;
    if(mode == LV_CN_KEYBOARD_MODE_NUMBER){
        lv_obj_set_style_text_font(obj, LV_FONT_DEFAULT, 0);
        cn_ime->lang_type = 0;
    }
    lv_cn_keyboard_update_map(obj, cn_ime->lang_type);
    switch_lang_type(cn_ime->lang_type);
}

/**
 * Set a new map for the keyboard
 * @param kb pointer to a Keyboard object
 * @param mode keyboard map to alter 'lv_keyboard_mode_t'
 * @param map pointer to a string array to describe the map.
 *            See 'lv_btnmatrix_set_map()' for more info.
 */
void lv_cn_keyboard_set_map(lv_obj_t * obj, lv_cn_keyboard_mode_t mode, const char * map[], const lv_btnmatrix_ctrl_t ctrl_map[])
{
    en_kb_map[mode] = map;
    en_kb_ctrl[mode] = ctrl_map;
    lv_cn_keyboard_update_map(obj, 0);
}

/*=====================
 * Getter functions
 *====================*/

/**
 * Assign a Text Area to the Keyboard. The pressed characters will be put there.
 * @param kb pointer to a Keyboard object
 * @return pointer to the assigned Text Area object
 */
lv_obj_t * lv_cn_keyboard_get_textarea(const lv_obj_t * obj)
{
    lv_cn_keyboard_t * keyboard = (lv_cn_keyboard_t *)obj;
    return keyboard->ta;
}

/**
 * Set a new a mode (text or number map)
 * @param kb pointer to a Keyboard object
 * @return the current mode from 'lv_keyboard_mode_t'
 */
lv_cn_keyboard_mode_t lv_cn_keyboard_get_mode(const lv_obj_t * obj)
{
    lv_cn_keyboard_t * keyboard = (lv_cn_keyboard_t *)obj;
    return keyboard->mode;
}

/*=====================
 * Other functions
 *====================*/
#include "../../../../../user/tiny_pinyin.h"
static char *py_search(uint8_t *pinyin)
{
    uint8_t input[32]={0};
    char *ptr;
    int inlen=0,outlen=0,i,j;
    inlen = strlen((char *)pinyin);
    for(i=0,j=0; i<inlen; i++){
        if((pinyin[i] >= 'A') && (pinyin[i]<='Z'))
            input[j++] = pinyin[i]+32;      //转小写
        else if((pinyin[i] >= 'a') && (pinyin[i]<='z'))
            input[j++] = pinyin[i];
        else if(0 == pinyin[i])
            break;
    }
    inlen = j;

    ptr = PYSearch(input, &outlen);

    if(outlen)
        return ptr;
    else
        return NULL;
}

static void set_candidate_box(uint8_t cand_show)
{
    switch (cand_show) {
        case 0:
            if(NULL != cn_ime->obj_candidate_box)
                lv_obj_add_flag(cn_ime->obj_candidate_box, LV_OBJ_FLAG_HIDDEN);
            cn_ime->ext_cand_flag = 0;
            break;

        case 1:
            cn_ime->ext_cand_flag = 1;
        case 2:
            if(2==cand_show){
                cn_ime->ext_cand_flag = 2;
                lv_label_set_text(cn_ime->cand_btn_label,LV_SYMBOL_UP);
            }
            else
                lv_label_set_text(cn_ime->cand_btn_label,LV_SYMBOL_DOWN);

            if(NULL != cn_ime->obj_candidate_box){
                lv_obj_clear_flag(cn_ime->obj_candidate_box, LV_OBJ_FLAG_HIDDEN);
                if(1 == cn_ime->ext_cand_flag){
                    lv_textarea_set_one_line(cn_ime->cand_textarea, true);
                    lv_obj_set_scroll_dir(cn_ime->cand_textarea, LV_DIR_HOR);
                    lv_obj_set_height(cn_ime->cand_textarea, 35);
                    lv_obj_set_height(cn_ime->obj_candidate_box, 35);
                }
                else{
                    lv_textarea_set_one_line(cn_ime->cand_textarea, false);
                    lv_obj_set_scroll_dir(cn_ime->cand_textarea, LV_DIR_VER);
                    lv_obj_set_height(cn_ime->cand_textarea, lv_obj_get_height(cn_ime->obj_key_board));
                    lv_obj_set_height(cn_ime->obj_candidate_box, lv_obj_get_height(cn_ime->obj_key_board));
                }

            }
            break;
    }
    lv_obj_scroll_to(cn_ime->cand_textarea, 0, 0, LV_ANIM_OFF);
}

static void show_cand_txt(char *text_uft8)
{
    int16_t i=0, j=0;
    char uft8_char[4*100+10]={0};       //最多100个候选字，utf-8编码

    lv_textarea_set_text(cn_ime->cand_textarea, "");
    if(NULL == text_uft8)
        return;

    i=j=0;
    while((uint16_t)j<sizeof(uft8_char)){
        if(0==text_uft8[i])
            break;

        uft8_char[j++]=text_uft8[i];
        if(0==((i+1)%3))
            uft8_char[j++]=' ';     //space
        i++;
    }
    uft8_char[j++]='\n';
    uft8_char[j++]='\n';

    lv_textarea_set_text(cn_ime->cand_textarea, uft8_char);
    lv_obj_scroll_to(cn_ime->cand_textarea, 0, 0, LV_ANIM_OFF);
    return;
}

static void reset_cand_textare(void)
{
    cn_ime->src_txt = "拼音";
    show_cand_txt(cn_ime->src_txt);
    lv_textarea_set_text(cn_ime->pinyin_tip, "");
    lv_obj_scroll_to(cn_ime->cand_textarea, 0, 0, LV_ANIM_OFF);
}

static void switch_lang_type(uint8_t l_mode)
{
    cn_ime->src_txt = NULL;
    switch (l_mode) {
        case 0:
            set_candidate_box(0);
        break;

        case 1:
            set_candidate_box(1);
        break;
    }
    reset_cand_textare();
}

/**
 * Default keyboard event to add characters to the Text area and change the map.
 * If a custom `event_cb` is added to the keyboard this function be called from it to handle the
 * button clicks
 * @param kb pointer to a  keyboard
 * @param event the triggering event
 */
void lv_cn_keyboard_def_event_cb(lv_event_t * e)
{
    lv_obj_t * obj = lv_event_get_target(e);
    lv_cn_keyboard_t * keyboard = (lv_cn_keyboard_t *)obj;
    uint16_t btn_id   = lv_btnmatrix_get_selected_btn(obj);
    if(btn_id == LV_BTNMATRIX_BTN_NONE) return;

    const char * txt = lv_btnmatrix_get_btn_text(obj, lv_btnmatrix_get_selected_btn(obj));
    if(txt == NULL) return;

    /*输入法切换*/
    if(strcmp(txt, "En/Cn") == 0){
        cn_ime->lang_type = (0 == cn_ime->lang_type) ? 1 : 0;
        lv_cn_keyboard_update_map(obj, cn_ime->lang_type);
        switch_lang_type(cn_ime->lang_type);
        return;
    }

    if(strcmp(txt, "abc") == 0) {
        keyboard->mode = LV_CN_KEYBOARD_MODE_TEXT_LOWER;
        lv_cn_keyboard_update_map(obj, cn_ime->lang_type);
        return;
    }
    else if(strcmp(txt, "ABC") == 0) {
        keyboard->mode = LV_CN_KEYBOARD_MODE_TEXT_UPPER;
        lv_cn_keyboard_update_map(obj, cn_ime->lang_type);
        return;
    }
    else if(strcmp(txt, "1#") == 0) {
        keyboard->mode = LV_CN_KEYBOARD_MODE_SPECIAL;
        lv_cn_keyboard_update_map(obj, cn_ime->lang_type);
        return;
    }
    else if(strcmp(txt, LV_SYMBOL_CLOSE) == 0 || strcmp(txt, LV_SYMBOL_KEYBOARD) == 0) {
        lv_obj_add_flag(cn_ime->obj_candidate_box, LV_OBJ_FLAG_HIDDEN);
        lv_res_t res = lv_event_send(obj, LV_EVENT_CANCEL, NULL);
        if(res != LV_RES_OK) return;

        if(keyboard->ta) {
            res = lv_event_send(keyboard->ta, LV_EVENT_CANCEL, NULL);
            if(res != LV_RES_OK) return;
        }
        return;
    }
    else if(strcmp(txt, LV_SYMBOL_OK) == 0) {
        lv_res_t res = lv_event_send(obj, LV_EVENT_READY, NULL);
        if(res != LV_RES_OK) return;

        if(keyboard->ta) {
            res = lv_event_send(keyboard->ta, LV_EVENT_READY, NULL);
            if(res != LV_RES_OK) return;
        }
        return;
    }

    /*Add the characters to the text area if set*/
    if(keyboard->ta == NULL) return;

    if(strcmp(txt, "Enter") == 0 || strcmp(txt, LV_SYMBOL_NEW_LINE) == 0) {
        lv_textarea_add_char(keyboard->ta, '\n');
        if(lv_textarea_get_one_line(keyboard->ta)) {
            lv_res_t res = lv_event_send(keyboard->ta, LV_EVENT_READY, NULL);
            if(res != LV_RES_OK) return;
        }
    } else if(strcmp(txt, LV_SYMBOL_LEFT) == 0) {
        lv_textarea_cursor_left(keyboard->ta);
    } else if(strcmp(txt, LV_SYMBOL_RIGHT) == 0) {
        lv_textarea_cursor_right(keyboard->ta);
    } else if(strcmp(txt, LV_SYMBOL_BACKSPACE) == 0) {
        const char *tmp;
        tmp = lv_textarea_get_text(cn_ime->pinyin_tip);
        if(tmp[0] != 0){
            lv_textarea_del_char(cn_ime->pinyin_tip);
        }
        tmp = lv_textarea_get_text(cn_ime->pinyin_tip);
        if(tmp[0] == 0){
            reset_cand_textare();
            lv_textarea_del_char(keyboard->ta);
        }
        else{
            const char *kb_input=lv_textarea_get_text(cn_ime->pinyin_tip);
            if(kb_input[0] != 0)
                cn_ime->src_txt = py_search((uint8_t *)kb_input);
            show_cand_txt(cn_ime->src_txt);
        }
    } else if(strcmp(txt, "+/-") == 0) {
        uint16_t cur        = lv_textarea_get_cursor_pos(keyboard->ta);
        const char * ta_txt = lv_textarea_get_text(keyboard->ta);
        if(ta_txt[0] == '-') {
            lv_textarea_set_cursor_pos(keyboard->ta, 1);
            lv_textarea_del_char(keyboard->ta);
            lv_textarea_add_char(keyboard->ta, '+');
            lv_textarea_set_cursor_pos(keyboard->ta, cur);
        }
        else if(ta_txt[0] == '+') {
            lv_textarea_set_cursor_pos(keyboard->ta, 1);
            lv_textarea_del_char(keyboard->ta);
            lv_textarea_add_char(keyboard->ta, '-');
            lv_textarea_set_cursor_pos(keyboard->ta, cur);
        }
        else {
            lv_textarea_set_cursor_pos(keyboard->ta, 0);
            lv_textarea_add_char(keyboard->ta, '-');
            lv_textarea_set_cursor_pos(keyboard->ta, cur + 1);
        }
    }
    else {
        if(NULL != keyboard->ta){
            if((0 == cn_ime->lang_type)||(keyboard->mode == LV_CN_KEYBOARD_MODE_SPECIAL))
                lv_textarea_add_text(keyboard->ta, txt);
            else if(1 == cn_ime->lang_type){
                lv_textarea_add_text(cn_ime->pinyin_tip, txt);
                const char *kb_input=lv_textarea_get_text(cn_ime->pinyin_tip);
                if(kb_input[0] != 0){
                    cn_ime->src_txt = py_search((uint8_t *)kb_input);
                    show_cand_txt(cn_ime->src_txt);
                }
            }
        }
    }
}

static void ext_candidate_click_cb(lv_event_t * e)
{
    LV_UNUSED(e);

    if(1==cn_ime->ext_cand_flag)
        cn_ime->ext_cand_flag=2;
    else
        cn_ime->ext_cand_flag=1;

    set_candidate_box(cn_ime->ext_cand_flag);
}

static void cand_pick_event_cb(lv_event_t * e)
{
    lv_event_code_t code = lv_event_get_code(e);
    lv_obj_t * ta = lv_event_get_target(e);

    static uint16_t index=0;
    if(code == LV_EVENT_CLICKED) {
        index = lv_textarea_get_cursor_pos(ta);
//        printf("index:%d\n", index);
        if(0==(index&0x01)){
            index = (index+1)/2;
            if(NULL != cn_ime->src_txt){
                if(index < strlen(cn_ime->src_txt)/3){
                    char uft8_char[4]={0};
                    lv_cn_keyboard_t * keyboard = (lv_cn_keyboard_t *) cn_ime->obj_key_board;
                    lv_memcpy(uft8_char, &cn_ime->src_txt[index*3], 3);
                    if(NULL != keyboard->ta){
                        lv_textarea_add_text(keyboard->ta, uft8_char);
                    }
                }
            }
        }

        lv_textarea_set_text(cn_ime->pinyin_tip,"");    //clear
    }
}

static void creat_candidate_box(lv_obj_t *parent)
{
    lv_obj_update_layout(parent);

    /*候选框*/
    cn_ime->obj_candidate_box = lv_obj_create(parent);
    int16_t x, y;
    x = 0 - lv_obj_get_style_pad_left(parent, 0);
    y = 0 - lv_obj_get_style_pad_top(parent, 0);
    lv_obj_set_pos(cn_ime->obj_candidate_box, x, y);
    x = lv_obj_get_width(parent);
    lv_obj_set_size(cn_ime->obj_candidate_box, x, 35);
    lv_obj_set_style_bg_color(cn_ime->obj_candidate_box, lv_color_hex(0x262626), 0);
    lv_obj_set_style_radius(cn_ime->obj_candidate_box,2,0);
    lv_obj_set_style_bg_opa(cn_ime->obj_candidate_box, LV_OPA_80, 0);
    lv_obj_set_style_pad_all(cn_ime->obj_candidate_box, 5, 0);
    lv_obj_set_style_pad_top(cn_ime->obj_candidate_box, 12, 0);
    lv_obj_set_style_border_opa(cn_ime->obj_candidate_box, LV_OPA_TRANSP, 0);
    lv_obj_set_style_outline_width(cn_ime->obj_candidate_box, 0, 0);
    lv_obj_clear_flag(cn_ime->obj_candidate_box, LV_OBJ_FLAG_SCROLLABLE);

    /*拼音提示*/
    cn_ime->pinyin_tip = lv_textarea_create(cn_ime->obj_candidate_box);
    lv_obj_remove_style_all(cn_ime->pinyin_tip);
    lv_obj_set_y(cn_ime->pinyin_tip, -12);
    lv_obj_clear_flag(cn_ime->pinyin_tip, LV_OBJ_FLAG_CLICKABLE);
    lv_obj_set_size(cn_ime->pinyin_tip, lv_pct(90), 15);
    lv_obj_set_style_text_font(cn_ime->pinyin_tip, &lv_font_montserrat_12, 0);
    lv_obj_set_style_text_color(cn_ime->pinyin_tip, lv_color_hex(0xffffff), 0);
    lv_obj_set_style_bg_opa(cn_ime->pinyin_tip, LV_OPA_TRANSP, 0);

    /*候选展开按钮*/
    lv_obj_t *ext_btn = lv_btn_create(cn_ime->obj_candidate_box);
    lv_obj_set_align(ext_btn, LV_ALIGN_TOP_RIGHT);
    lv_obj_set_height(ext_btn, 30);
    lv_obj_set_pos(ext_btn, 5, -8);
    lv_obj_set_style_radius(ext_btn,0,0);
    lv_obj_set_style_bg_opa(ext_btn, LV_OPA_0, 0);
    lv_obj_set_style_shadow_width(ext_btn, 0, 0);
    lv_obj_add_event_cb(ext_btn, ext_candidate_click_cb, LV_EVENT_CLICKED, NULL);
    lv_obj_clear_flag(ext_btn, LV_OBJ_FLAG_CLICK_FOCUSABLE|LV_OBJ_FLAG_FLOATING);        //取消focus属性-以免抢了textarea的focus
    cn_ime->cand_btn_label = lv_label_create(ext_btn);
    lv_obj_set_y(cn_ime->cand_btn_label, -5);
    lv_obj_set_style_text_font(cn_ime->cand_btn_label, LV_FONT_DEFAULT, 0);
    lv_label_set_text(cn_ime->cand_btn_label,LV_SYMBOL_DOWN);
    lv_obj_update_layout(cn_ime->obj_candidate_box);

    /* 候选字的textarea */
    cn_ime->cand_textarea = lv_textarea_create(cn_ime->obj_candidate_box);
    lv_obj_set_y(cn_ime->cand_textarea, 15);

    lv_obj_set_style_bg_opa(cn_ime->cand_textarea, LV_OPA_TRANSP, 0);
    lv_obj_set_style_pad_all(cn_ime->cand_textarea, 0, 0);
    lv_obj_set_style_shadow_width(cn_ime->cand_textarea, 0, 0);
    lv_obj_set_style_border_opa(cn_ime->cand_textarea, LV_OPA_TRANSP, 0);
    lv_obj_add_event_cb(cn_ime->cand_textarea, cand_pick_event_cb, LV_EVENT_CLICKED, 0);
    uint16_t w;
    w = lv_obj_get_width(cn_ime->obj_candidate_box)-lv_obj_get_width(ext_btn)-5;
    lv_obj_set_size(cn_ime->cand_textarea, w, lv_pct(100));
    lv_obj_align(cn_ime->cand_textarea, LV_ALIGN_TOP_LEFT, 0, 0);
    lv_obj_clear_state(cn_ime->cand_textarea, LV_STATE_FOCUSED);
    lv_obj_clear_flag(cn_ime->cand_textarea, LV_OBJ_FLAG_CLICK_FOCUSABLE);
    lv_obj_set_style_text_font(cn_ime->cand_textarea, cn_ime->font, 0);
    lv_obj_set_style_text_color(cn_ime->cand_textarea, lv_color_white(), 0);
    lv_obj_set_scrollbar_mode(cn_ime->cand_textarea, LV_SCROLLBAR_MODE_OFF);
    lv_obj_scroll_to(cn_ime->cand_textarea, 0, 0, LV_ANIM_OFF);

    lv_group_remove_obj(cn_ime->obj_candidate_box);     /*不参与任何group以免被lv_group_focus_next/prev选中*/
    lv_group_remove_obj(cn_ime->pinyin_tip);            /*不参与任何group*/
    lv_group_remove_obj(ext_btn);                       /*不参与任何group*/
    lv_group_remove_obj(cn_ime->cand_textarea);         /*不参与任何group*/

    lv_obj_update_layout(parent);
    switch_lang_type(cn_ime->lang_type);
}

/**********************
 *   STATIC FUNCTIONS
 **********************/
static void lv_cn_keyboard_constructor(const lv_obj_class_t * class_p, lv_obj_t * obj)
{
    LV_UNUSED(class_p);
    lv_obj_clear_flag(obj, LV_OBJ_FLAG_CLICK_FOCUSABLE);

    lv_cn_keyboard_t * keyboard = (lv_cn_keyboard_t *)obj;
    keyboard->ta         = NULL;
    keyboard->mode       = LV_CN_KEYBOARD_MODE_TEXT_LOWER;

    lv_disp_t * disp = lv_obj_get_disp(obj);
    obj->class_p = &lv_keyboard_class;
    disp->theme->apply_cb(NULL, obj);                       //获取系统键盘的style否则键盘显示不正常
    obj->class_p = &lv_cn_keyboard_class;
    lv_obj_clear_flag(obj, LV_OBJ_FLAG_SCROLLABLE);

    cn_ime->obj_key_board = obj;                            //保存键盘句柄
    cn_ime->lang_type = 1;                                  //default: cn
    lv_obj_align(obj, LV_ALIGN_BOTTOM_MID, 0, 0);
    lv_obj_add_event_cb(obj, lv_cn_keyboard_def_event_cb, LV_EVENT_VALUE_CHANGED, NULL);
    lv_obj_set_style_base_dir(obj, LV_BASE_DIR_LTR, 0);

    lv_obj_set_style_text_font(obj, LV_FONT_DEFAULT, 0);    //使用默认字体以显示图标
    lv_cn_keyboard_update_map(obj, cn_ime->lang_type);

    lv_obj_update_layout(obj);
    creat_candidate_box(obj);
    set_candidate_box(cn_ime->ext_cand_flag);
}
static void lv_cn_keyboard_destructor(const lv_obj_class_t * class_p, lv_obj_t * obj)
{
    LV_UNUSED(class_p);
    LV_UNUSED(obj);

    if(cn_ime)
        lv_mem_free(cn_ime);
    cn_ime = NULL;
}
/**
 * Update the key map for the current mode
 * @param kb pointer to a keyboard object
 * @param en_or_cn 0-en, 1-cn
 */
static void lv_cn_keyboard_update_map(lv_obj_t * obj, uint8_t en_or_cn)
{
    lv_cn_keyboard_t * keyboard = (lv_cn_keyboard_t *)obj;
    if(0 == en_or_cn){
        lv_btnmatrix_set_map(obj, en_kb_map[keyboard->mode]);
        lv_btnmatrix_set_ctrl_map(obj, en_kb_ctrl[keyboard->mode]);
    }
    else{
        lv_btnmatrix_set_map(obj, cn_kb_map[keyboard->mode]);
        lv_btnmatrix_set_ctrl_map(obj, cn_kb_ctrl[keyboard->mode]);
    }
}

#endif  /*LV_USE_KEYBOARD*/
