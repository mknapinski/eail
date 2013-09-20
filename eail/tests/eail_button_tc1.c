/*
 * Tested interface: AtkObject
 *
 * Tested AtkObject: EailButton
 *
 * Description: Test whether accessible object is successfully registered
 *    in GObject type system and is visible in AtkObject tree.
 *
 * Test input: accessible object representing EailButton
 *
 * Expected test result: test should return 0 (success)
 */

#include <Elementary.h>
#include <atk/atk.h>
#include "eail_test_utils.h"

#define BUTTON_TEXT1 "Button1"

INIT_TEST("EailButton")

void button_focus_cb(AtkObject *obj, gboolean b)
{
   printf("aaa");
   g_assert_cmpstr(atk_object_get_name(obj), ==, BUTTON_TEXT1"1");
}

static void
_do_test(AtkObject *obj)
{
   int handler_id;

   g_assert(ATK_IS_OBJECT(obj));
   g_assert(atk_object_get_role(obj) == ATK_ROLE_PUSH_BUTTON);
   g_assert_cmpstr(atk_object_get_name(obj), ==, BUTTON_TEXT1);
   atk_object_set_name(obj, BUTTON_TEXT1"test");
   g_assert_cmpstr(atk_object_get_name(obj), ==, BUTTON_TEXT1"test");

   handler_id = atk_component_add_focus_handler(ATK_COMPONENT(obj),
                                                button_focus_cb);
   g_assert(handler_id != 0);

   handler_id = atk_component_add_focus_handler(ATK_COMPONENT(obj),
                                                button_focus_cb);
   g_assert(handler_id == 0);

   g_assert(atk_component_grab_focus(ATK_COMPONENT(obj)));
   atk_component_remove_focus_handler(ATK_COMPONENT(obj), handler_id);

   eailu_test_code_called = 1;
}

static void
_init_button(Evas_Object *win)
{
   Evas_Object *bg, *button1;

   bg = elm_bg_add(win);
   evas_object_size_hint_weight_set(bg, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
   elm_win_resize_object_add(win, bg);
   evas_object_show(bg);

   button1 = elm_button_add(win);
   elm_object_text_set(button1, BUTTON_TEXT1);

   evas_object_resize(button1, 140, 60);
   evas_object_move(button1, 50, 20);
   evas_object_show(button1);
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
