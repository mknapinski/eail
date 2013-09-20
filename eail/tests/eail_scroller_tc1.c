/*
 * Tested interface: AtkObject
 *
 * Tested AtkObject: EailScroller
 *
 * Description: Test whether accessible object is successfully registered
 *    in GObject type system and is visible in AtkObject tree. Test
 *    accessible children availability.
 *
 * Test input: accessible object representing EailScroller
 *
 * Expected test result: test should return 0 (success)
 */


#include <Elementary.h>
#include <atk/atk.h>
#include "eail_test_utils.h"

INIT_TEST("EailScroller")

static void
_do_test(AtkObject *obj)
{
   int child_count = 0;

   g_assert(ATK_IS_OBJECT(obj));
   atk_object_set_description(obj, "test");
   g_assert_cmpstr(atk_object_get_description(obj), ==, "test");

   atk_object_set_name(obj, "test name");
   g_assert_cmpstr(atk_object_get_name(obj), ==, "test name");

   g_assert(atk_object_get_role(obj) == ATK_ROLE_SCROLL_PANE);

   child_count = atk_object_get_n_accessible_children(obj);
   g_assert(1 == child_count);

   eailu_test_atk_focus(obj, TRUE);

   eailu_test_code_called = TRUE;
}

static void
_init_scroller(Evas_Object *win)
{
   Evas_Object  *label, *scroller;

   /* default */
   label = elm_label_add(win);
   elm_object_text_set(label, "");
   evas_object_show(label);

   scroller = elm_scroller_add(win);
   evas_object_size_hint_weight_set(scroller, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
   elm_win_resize_object_add(win, scroller);
   evas_object_show(scroller);
   elm_object_content_set(scroller, label);
   elm_scroller_bounce_set(scroller, EINA_TRUE, EINA_FALSE);
   elm_scroller_policy_set(scroller, ELM_SCROLLER_POLICY_ON, ELM_SCROLLER_POLICY_ON);
   elm_scroller_propagate_events_set(scroller, EINA_TRUE);
   elm_scroller_page_relative_set(scroller, 0, 1);
   elm_scroller_region_show(scroller, 50, 50, 200, 200);
}

EAPI_MAIN int
elm_main(int argc, char **argv)
{
   Evas_Object *win;

   win = eailu_create_test_window_with_glib_init(_on_done, _on_focus_in);
   _init_scroller(win);

   evas_object_show(win);

   elm_run();
   elm_shutdown();

   /* exit code */
   return 0;
}
ELM_MAIN()
