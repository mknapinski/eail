/*
 * Tested interface: AtkObject
 *
 * Tested AtkObject: EailLabel
 *
 * Description: Test AtkObject interface
 *
 * Test input: accessible object representing EailLabel
 *
 * Expected test result: test should return 0 (success)
 */

#include <Elementary.h>

#include "eail_test_utils.h"

INIT_TEST("EailLabel")

static void
_init_label(Evas_Object *win)
{
   Evas_Object *bg, *label;

   bg = elm_bg_add(win);
   evas_object_size_hint_weight_set(bg, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
   elm_win_resize_object_add(win, bg);
   evas_object_show(bg);

   label = elm_label_add(win);
   elm_object_text_set(label, "Lorem ipsum dolor sit amet, consectetur adipisicing elit,"
                       "sed do eiusmod tempor incididunt ut labore et dolore magna aliqua."
                       "Ut enim ad minim veniam, quis nostrud exercitation ullamco laboris"
                       "nisi ut aliquip ex ea commodo consequat. Duis aute irure dolor in"
                       "reprehenderit in voluptate velit esse cillum dolore eu fugiat nulla"
                       "pariatur. Excepteur sint occaecat cupidatat non proident, sunt"
                       "in culpa qui officia deserunt mollit anim id est laborum.");
   elm_label_line_wrap_set(label, ELM_WRAP_WORD);
   evas_object_resize(label, 200, 170);
   evas_object_move(label, 0, 0);
   evas_object_show(label);

   evas_object_resize(win, 200, 320);
}

static void
_do_test(AtkObject *obj)
{
   AtkStateSet *state_set = atk_object_ref_state_set(obj);

   g_assert(ATK_IS_OBJECT(obj));
   atk_object_set_description(obj, "test");
   g_assert_cmpstr(atk_object_get_description(obj), ==, "test");

   atk_object_set_name(obj, "test name");
   g_assert_cmpstr(atk_object_get_name(obj), ==, "test name");

   g_assert(atk_object_get_role(obj) == ATK_ROLE_LABEL);

   g_assert(atk_state_set_contains_state(state_set, ATK_STATE_MULTI_LINE));
   g_object_unref(state_set);

   eailu_test_code_called = 1;
}

EAPI_MAIN int
elm_main(int argc, char **argv)
{
   Evas_Object *win;

   win = eailu_create_test_window_with_glib_init(_on_done, _on_focus_in);
   g_assert(win);
   _init_label(win);

   evas_object_show(win);
   elm_run();
   elm_shutdown();

   return 0;
}
ELM_MAIN()


