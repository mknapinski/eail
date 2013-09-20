/*
 * Tested interface: AtkObject
 *
 * Tested AtkObject: EailNotify
 *
 * Description: Test AtkObject interface
 *
 * Test input: accessible object representing EailNotify
 *
 * Expected test result: test should return 0 (success)
 */

#include <Elementary.h>
#include <atk/atk.h>

//#define DEBUG 1

#include "eail_test_utils.h"

#define EAIL_TYPE_FOR_CHECK "EailNotify"
#define EAIL_TEST_LABEL_TITTLE "test label"

INIT_TEST("EailNotify")

static void
_do_test(AtkObject *obj)
{
   int child_amount = 0;
   AtkObject *nested_obj;

   _printf("Testing notify instance....\n");
   atk_object_set_description(obj, "test");
   g_assert_cmpstr(atk_object_get_description(obj), ==, "test");

   atk_object_set_name(obj, "test name");
   g_assert_cmpstr(atk_object_get_name(obj), ==, "test name");

   child_amount = atk_object_get_n_accessible_children(obj);
   /* we have to check if there is nested label in Notify */
   g_assert(1 == child_amount);

   /* test if notify child  is label-type*/
   nested_obj = atk_object_ref_accessible_child(obj, 0);
   g_assert(eailu_is_object_with_type(nested_obj, "EailLabel"));
   g_assert(eailu_is_object_with_name(nested_obj, EAIL_TEST_LABEL_TITTLE));

   /* For notify widget focusing doesn't work */
   eailu_test_atk_focus(obj, FALSE);

   _printf("Tests passed successfully\n");

   eailu_test_code_called = 1;
}

static void
_init_notify(Evas_Object *win)
{
   Evas_Object *notify, *box, *label;

   box = elm_box_add(win);
   evas_object_size_hint_weight_set(box, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
   elm_win_resize_object_add(win, box);
   evas_object_show(box);

   label = elm_label_add(win);
   elm_object_text_set(label, EAIL_TEST_LABEL_TITTLE);
   evas_object_show(label);

   notify = elm_notify_add(win);
   elm_notify_allow_events_set(notify, EINA_FALSE);
   //elm_notify_timeout_set(notify, 5);
   elm_object_content_set(notify, label);
   evas_object_size_hint_weight_set(notify, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
   evas_object_show(notify);
   elm_box_pack_end(box, notify);
}

EAPI_MAIN int
elm_main(int argc, char **argv)
{
   Evas_Object *win;

   win = eailu_create_test_window_with_glib_init(_on_done, _on_focus_in);
   _init_notify(win);

   evas_object_show(win);

   elm_run();
   elm_shutdown();

   /* exit code */
   return 0;
}
ELM_MAIN()
