/*
 * Tested interface: AtkAction
 *
 * Tested AtkObject: EailButton
 *
 * Description: Test AtkAction interface
 *
 * Test input: accessible object representing EailButton
 *
 * Expected test result: test should return 0 (success)
 */

#include <Elementary.h>
#include <atk/atk.h>

#include "eail_test_utils.h"

#define DEBUG

#define ACTIONS_NUMBER 3
#define BUTTON_TEXT1   "Button1"
#define BUTTON_TEXT2   "button2"

static int action_test_passed;

INIT_TEST("EailButton")

static void
_on_click(void *data, Evas_Object *obj, void *event_info)
{
   _printf("button click\n");
   ++action_test_passed;
}

static void
_on_press(void *data, Evas_Object *obj, void *event_info)
{
   _printf("button press\n");
   ++action_test_passed;
}

static void
_on_release(void *data, Evas_Object *obj, void *event_info)
{
   _printf("button release\n");
   ++action_test_passed;
}

static void
_do_test(AtkObject *obj)
{
   g_assert(ATK_IS_ACTION(obj));

   g_assert(ACTIONS_NUMBER == atk_action_get_n_actions(ATK_ACTION(obj)));
   eailu_test_action_activate(ATK_ACTION(obj), "click");
   eailu_test_action_activate(ATK_ACTION(obj), "press");
   eailu_test_action_activate(ATK_ACTION(obj), "release");
   g_assert((eailu_get_action_number(ATK_ACTION(obj), "typo")) == -1);
   eailu_test_action_description_all(ATK_ACTION(obj));

   g_assert(action_test_passed == 3);

   eailu_test_code_called = 1;
}

static void
_init_button(Evas_Object *win)
{
   Evas_Object *bg, *icon, *button1;

   bg = elm_bg_add(win);
   evas_object_size_hint_weight_set(bg, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
   elm_win_resize_object_add(win, bg);
   evas_object_show(bg);

   icon = elm_icon_add(win);
   elm_icon_order_lookup_set(icon, ELM_ICON_LOOKUP_THEME_FDO);
   elm_icon_standard_set(icon, "home");
   evas_object_show(icon);

   button1 = elm_button_add(win);
   elm_object_text_set(button1, BUTTON_TEXT1);
   evas_object_smart_callback_add(button1, "clicked", _on_click, "button1");
   evas_object_smart_callback_add(button1, "pressed", _on_press, "button1");
   evas_object_smart_callback_add(button1, "unpressed", _on_release, "button1");

   evas_object_resize(button1, 140, 60);
   evas_object_move(button1, 50, 20);
   evas_object_show(button1);
}

EAPI_MAIN int
elm_main(int argc, char **argv)
{
   Evas_Object *win;

   win = eailu_create_test_window_with_glib_init(_on_done, _on_focus_in);
   g_assert(win);
   _init_button(win);
   evas_object_show(win);
   elm_run();
   elm_shutdown();

   return 0;
}
ELM_MAIN()
