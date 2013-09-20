/*
 * Tested interface: AtkAction
 *
 * Tested AtkObject: EailCheck
 *
 * Description: Test AtkAction interface
 *
 * Test input: accessible object representing EailCheck
 *
 * Expected test result: test should return 0 (success)
 */

#include <Elementary.h>
#include <atk/atk.h>

//#define DEBUG 1

#include "eail_test_utils.h"

#define ACTIONS_NUMBER 1

#define CHECK_TEXT1 "checkbox1"
#define CHECK_TEXT2 "checkbox2"

static int action_test_passed;

INIT_TEST("EailCheck")

static void
_print(void *data, Evas_Object *obj, void *event_info)
{
   _printf("\tcheck %s - %smarked\n", elm_object_part_text_get(obj, "default"),
           *((Eina_Bool*)data) ? "" : "un");

   action_test_passed = 1;
}

static void
_init_check(Evas_Object *win)
{
   Evas_Object *bg, *cb1;
   Eina_Bool value1;

   bg = elm_bg_add(win);
   evas_object_size_hint_weight_set(bg, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
   elm_win_resize_object_add(win, bg);
   evas_object_show(bg);

   cb1 = elm_check_add(win);
   elm_object_text_set(cb1, CHECK_TEXT1);
   elm_check_state_pointer_set(cb1, &value1);
   elm_check_state_set(cb1, EINA_TRUE);
   evas_object_smart_callback_add(cb1, "changed", _print, &value1);
   evas_object_move(cb1, 10, 10);
   evas_object_resize(cb1, 200, 30);
   evas_object_show(cb1);
}

static void
_do_test(AtkObject *obj)
{
   g_assert(ATK_IS_ACTION(obj));
   g_assert(ACTIONS_NUMBER == atk_action_get_n_actions(ATK_ACTION(obj)));
   eailu_test_action_activate(ATK_ACTION(obj), "click");
   g_assert((eailu_get_action_number(ATK_ACTION(obj), "typo")) == -1);
   eailu_test_action_description_all(ATK_ACTION(obj));

   atk_object_set_name(obj, "test name");
   g_assert_cmpstr(atk_object_get_name(obj), ==, "test name");

   g_assert(action_test_passed);
   eailu_test_code_called = 1;
}

EAPI_MAIN int
elm_main(int argc, char **argv)
{
   Evas_Object *win;

   win = eailu_create_test_window_with_glib_init(_on_done, _on_focus_in);
   g_assert(win);
   _init_check(win);
   evas_object_show(win);
   elm_run();
   elm_shutdown();

   return 0;
}
ELM_MAIN()
