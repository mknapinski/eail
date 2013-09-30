/*
 * Tested interface: AtkText
 *
 * Tested AtkObject: EailButton
 *
 * Description: Test AtkText interface
 *
 * Test input: accessible object representing EailButton
 *
 * Expected test result: test should return 0 (success)
 */

#include <Elementary.h>
#include <atk/atk.h>

#include "eail_test_utils.h"

#define BUTTON_TEXT2   "button2"

#define ICON_SIZE 56

INIT_TEST("EailButton")

static void
_do_test(AtkObject *obj)
{
   g_assert(ATK_IS_TEXT(obj));
   g_assert(atk_text_get_character_at_offset(ATK_TEXT(obj), 1) == 'u');
   g_assert(atk_text_get_character_count(ATK_TEXT(obj)) == 7);
   g_assert_cmpstr(atk_text_get_text(ATK_TEXT(obj), 1, 4), ==, "utt");

   eailu_test_code_called = 1;
}

static void
_init_button(Evas_Object *win)
{
   Evas_Object *bg, *button2;

   bg = elm_bg_add(win);
   evas_object_size_hint_weight_set(bg, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
   elm_win_resize_object_add(win, bg);
   evas_object_show(bg);

   button2 = elm_button_add(win);
   elm_object_text_set(button2, BUTTON_TEXT2);
   evas_object_resize(button2, 140, 60);
   evas_object_show(button2);
}

EAPI_MAIN int
elm_main(int argc, char **argv)
{
   Evas_Object *win;

   win = eailu_create_test_window_with_glib_init(_on_done, _on_focus_in);
   _init_button(win);
   evas_object_show(win);
   elm_run();
   elm_shutdown();

   return 0;
}
ELM_MAIN()
