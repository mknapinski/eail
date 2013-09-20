/*
 * Tested interface: AtkAobject
 *
 * Tested AtkObject: EailMultibuttonEntry
 *
 * Description: Test AtkObject interface
 *
 * Test input: accessible object representing EailMultibuttonEntry
 *
 * Expected test result: test should return 0 (success)
 */

#include <Elementary.h>
#include <atk/atk.h>

#include "eail_test_utils.h"

#define EAIL_TYPE_FOR_MBE "EailMultibuttonentry"
#define EAIL_TEST_INPUT_TEXT "Input text"

INIT_TEST("EailMultibuttonentry")

static void
_do_test(AtkObject *obj)
{
   int child_amount = 0, i =0;
   AtkObject *nested_obj;
   atk_object_set_description(obj, "test");
   g_assert_cmpstr(atk_object_get_description(obj), ==, "test");

   atk_object_set_name(obj, "test name");
   g_assert_cmpstr(atk_object_get_name(obj), ==, "test name");
   /* standard tests, types, child num etc.*/
   child_amount = atk_object_get_n_accessible_children(obj);
   g_assert(4 == child_amount);
   for (i = 0; i < child_amount; ++i)
     {
        nested_obj = atk_object_ref_accessible_child(obj, i);
        g_assert(nested_obj);
        eailu_is_object_with_role(nested_obj, ATK_ROLE_LABEL);
        eailu_is_object_with_type(nested_obj, "EailItem");

        g_object_unref(nested_obj);
     }

   eailu_test_code_called = 1;
}

static void
_init_multibuttonentry(Evas_Object *win)
{
   Evas_Object *scroller, *bg, *box, *mb_entry;

   bg = elm_bg_add(win);
   evas_object_size_hint_weight_set(bg, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
   elm_win_resize_object_add(win, bg);
   evas_object_show(bg);

   box = elm_box_add(win);
   evas_object_size_hint_weight_set(box, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
   elm_win_resize_object_add(win, box);
   evas_object_show(box);

   scroller = elm_scroller_add(win);
   elm_scroller_bounce_set(scroller, EINA_FALSE, EINA_TRUE);
   elm_scroller_policy_set
       (scroller, ELM_SCROLLER_POLICY_OFF,ELM_SCROLLER_POLICY_AUTO);
   evas_object_show(scroller);

   mb_entry = elm_multibuttonentry_add(win);
   elm_object_part_text_set(mb_entry, "guide", "Tap to add recipient");
   elm_object_text_set(mb_entry, EAIL_TEST_INPUT_TEXT);
   evas_object_size_hint_weight_set(mb_entry, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
   evas_object_size_hint_align_set(mb_entry, EVAS_HINT_FILL, EVAS_HINT_FILL);
   elm_object_content_set(scroller, mb_entry);
   evas_object_show(mb_entry);

   elm_multibuttonentry_item_append(mb_entry, "first", NULL, NULL);
   elm_multibuttonentry_item_append(mb_entry, "second", NULL, NULL);
   elm_multibuttonentry_item_append(mb_entry, "third", NULL, NULL);
   elm_multibuttonentry_item_append(mb_entry, "fourth", NULL, NULL);

   elm_object_focus_set(mb_entry, EINA_TRUE);
   elm_box_pack_end(box, mb_entry);

   evas_object_resize(win, 130, 180);
}

EAPI_MAIN int
elm_main(int argc, char **argv)
{
   Evas_Object *win;

   win = eailu_create_test_window_with_glib_init(_on_done, _on_focus_in);
   _init_multibuttonentry(win);

   evas_object_show(win);
   elm_run();
   elm_shutdown();

   /* exit code */
   return 0;
}
ELM_MAIN()
