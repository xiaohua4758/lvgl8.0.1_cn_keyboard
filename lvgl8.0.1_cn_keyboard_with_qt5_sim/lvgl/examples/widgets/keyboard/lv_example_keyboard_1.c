#include "../../lv_examples.h"
#if LV_USE_KEYBOARD && LV_BUILD_EXAMPLES

#if 0
static void ta_event_cb(lv_event_t * e)
{
    lv_event_code_t code = lv_event_get_code(e);
    lv_obj_t * ta = lv_event_get_target(e);
    lv_obj_t * kb = lv_event_get_user_data(e);
    if(code == LV_EVENT_FOCUSED) {
        lv_keyboard_set_textarea(kb, ta);
        lv_obj_clear_flag(kb, LV_OBJ_FLAG_HIDDEN);
    }

    if(code == LV_EVENT_DEFOCUSED) {
        lv_keyboard_set_textarea(kb, NULL);
        lv_obj_add_flag(kb, LV_OBJ_FLAG_HIDDEN);
    }
}

void lv_example_keyboard_1(void)
{
    /*Create a keyboard to use it with an of the text areas*/
    lv_obj_t *kb = lv_keyboard_create(lv_scr_act());

    /*Create a text area. The keyboard will write here*/
    lv_obj_t * ta;
    ta = lv_textarea_create(lv_scr_act());
    lv_obj_align(ta, LV_ALIGN_TOP_LEFT, 10, 10);
    lv_obj_add_event_cb(ta, ta_event_cb, LV_EVENT_ALL, kb);
    lv_textarea_set_placeholder_text(ta, "Hello");
    lv_obj_set_size(ta, 140, 80);

    ta = lv_textarea_create(lv_scr_act());
    lv_obj_align(ta, LV_ALIGN_TOP_RIGHT, -10, 10);
    lv_obj_add_event_cb(ta, ta_event_cb, LV_EVENT_ALL, kb);
    lv_obj_set_size(ta, 140, 80);

    lv_keyboard_set_textarea(kb, ta);
}
#endif

/*modify by xiaohua, qq:1223419620*/
LV_FONT_DECLARE(my_cn_font1);
LV_FONT_DECLARE(my_cn_font2);

lv_obj_t *input_result;

/*textarea的回调函数*/
static void ta_event_cb(lv_event_t * e)
{
    static lv_obj_t *kb = NULL;
    lv_event_code_t code = lv_event_get_code(e);
    lv_obj_t * ta = lv_event_get_target(e);
    lv_obj_t * cont = lv_event_get_user_data(e);

    /*收到点击或聚焦事件，如果键盘未被创建则创建一个，并将textarea与键盘的textarea绑定*/
    if((code == LV_EVENT_FOCUSED)||(code == LV_EVENT_CLICKED)){
        if(NULL == kb){
            kb = lv_cn_keyboard_create(cont, &my_cn_font1);     //如果键盘未被创建,则创建一个
        }
        if(NULL != kb)
            lv_cn_keyboard_set_textarea(kb, ta);
    }

    /* 如收到失聚焦(EVENT_DEFOCUSED)、取消(EVENT_CANCEL)事件，则删除键盘释放资源
     * 如收到确认(EVENT_READY)事件，则处理输入的数据 */
    if((code == LV_EVENT_DEFOCUSED)||(code == LV_EVENT_CANCEL)||(code == LV_EVENT_READY)) {
        if(NULL != kb){
            /* 这里获取输入的字符串,input_dat就是字符串指针 */
            if(code == LV_EVENT_READY){         //打勾按钮
                const char *input_dat = lv_textarea_get_text(lv_cn_keyboard_get_textarea(kb));
                lv_label_set_text(input_result, input_dat);
            }
            else if(code == LV_EVENT_CANCEL){   //关闭键盘按钮
                ;   //do what you want...
            }

            if(code != LV_EVENT_READY){
                lv_obj_del(kb);                 //删除键盘,以便释放键盘资源
                kb = NULL;
            }
        }
    }
}

void lv_example_keyboard_1(void)
{
    int16_t w, h;

    /*Create a keyboard to use it with an of the text areas*/
    lv_obj_t *cont = lv_obj_create(lv_scr_act());
    lv_obj_set_size(cont, lv_pct(100), lv_pct(100));
    lv_obj_set_style_bg_color(cont, lv_color_hex(0xa0a0a0), 0); //color gray
    lv_obj_set_style_text_font(cont, &my_cn_font1, 0);
    lv_obj_update_layout(cont);

    w = lv_obj_get_width(cont);
    h = lv_obj_get_height(cont);

    /*创建一个textarea,接收键盘输入,明文模式显示*/
    lv_obj_t * ta;
    ta = lv_textarea_create(cont);
    lv_obj_align(ta, LV_ALIGN_TOP_LEFT, 0, 15);
    lv_obj_add_event_cb(ta, ta_event_cb, LV_EVENT_ALL, cont);   //ta_event_cb是回调函数
    lv_textarea_set_placeholder_text(ta, "请输入");
    lv_obj_set_size(ta, w/4, h/4);

    /*创建一个textarea,接收键盘输入,密码模式显示*/
    ta = lv_textarea_create(cont);
    lv_textarea_set_password_mode(ta, true);    /*password mode*/
    lv_obj_align(ta, LV_ALIGN_TOP_RIGHT, 0, 15);
    lv_obj_add_event_cb(ta, ta_event_cb, LV_EVENT_ALL, cont);   //ta_event_cb是回调函数
    lv_textarea_set_placeholder_text(ta, "密码模式");
    lv_obj_set_size(ta, w/4, h/4);

    /*创建一个textarea,无输入,用于切换textarea产生LV_EVENT_DEFOCUSED事件来关闭键盘*/
    lv_obj_t *new_ta = lv_textarea_create(cont);
    lv_obj_set_pos(new_ta, 0, h/4+17);
    lv_obj_set_size(new_ta, lv_pct(100), h/8);
    lv_textarea_set_placeholder_text(new_ta, "切换此框时自动关闭键盘并释放资源");

    /* 创建一个label来显示已确认的输入 */
    lv_obj_t *label = lv_label_create(cont);
    lv_obj_align(label, LV_ALIGN_TOP_MID, 0, -5);
    lv_label_set_text(label, "Your input");
    lv_obj_set_style_text_color(label, lv_color_hex(0xffff00), 0);
    input_result = lv_label_create(cont);
    lv_obj_align(input_result, LV_ALIGN_TOP_MID, 0, 15);
    lv_obj_set_style_bg_opa(input_result, LV_OPA_COVER, 0);
    lv_obj_set_size(input_result, w/3, h/4);
    lv_label_set_text(input_result, "");
}
#endif
