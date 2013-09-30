/*
 * Tested interface: AtkObject
 *
 * Tested AtkObject: EailSegmentControl
 *
 * Description: Test AtkObject interface
 *
 * Test input: accessible object representing EailSegmentControl
 *
 * Expected test result: test should return 0 (success)
 */

#include <Elementary.h>
#include <atk/atk.h>

#include "eail_test_utils.h"

#define EAIL_TEST_SELECTED_INDEX 3

INIT_TEST("EailSegmentControl")

static void
_do_test(AtkObject *obj)
{
   int child_count = 0, i = 0;

   _printf("Testing SegmentControl instance....\n");
   child_count = atk_object_get_n_accessible_children(obj);
   g_assert(7 == child_count);
   atk_object_set_description(obj, "test");
   g_assert_cmpstr(atk_object_get_description(obj), ==, "test");

   atk_object_set_name(obj, "test name");
   g_assert_cmpstr(atk_object_get_name(obj), ==, "test name");


   /* testing child refs*/
   for (i = 0; i < 7; ++i)
     {
        AtkObject *child = atk_object_ref_accessible_child(obj, i);
        g_assert(child);

        g_object_unref(child);
     }

   /* NOTE: segment control widget does not support focus */
   eailu_test_atk_focus(obj, FALSE);

   _printf("DONE. All SegmentControl tests passed successfully \n");

   eailu_test_code_called = 1;
}

static void
_init_diskselector(Evas_Object *win)
{
   Evas_Object *sgm_ctrl = NULL, *vbox;
   Elm_Object_Item *obj_item = NULL;
   unsigned int i;
   static const char *lbl[] =
     {
        "Sunday",
        "Monday",
        "Tuesday",
        "Wednesday",
        "Thursday",
        "Friday",
        "Saturday"
     };

   vbox = elm_box_add(win);
   elm_win_resize_object_add(win, vbox);
   evas_object_size_hint_weight_set(vbox, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
   evas_object_show(vbox);

   /* default */
   sgm_ctrl = elm_segment_control_add(win);
   evas_object_size_hint_weight_set
       (sgm_ctrl, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
   evas_object_size_hint_align_set
       (sgm_ctrl, EVAS_HINT_FILL, EVAS_HINT_FILL);

   elm_box_pack_end(vbox, sgm_ctrl);

   for (i = 0; i < sizeof(lbl) / sizeof(lbl[0]); i++)
     {
        obj_item = elm_segment_control_item_add(sgm_ctrl, NULL , lbl[i]);

        /* marking element as selected */
        if (EAIL_TEST_SELECTED_INDEX == i)
          elm_segment_control_item_selected_set(obj_item, EINA_TRUE);
     }


   evas_object_show(sgm_ctrl);
   evas_object_resize(win, 800, 200);
}

EAPI_MAIN int
elm_main(int argc, char **argv)
{
   Evas_Object *win;

   win = eailu_create_test_window_with_glib_init(_on_done, _on_focus_in);
   _init_diskselector(win);

   evas_object_show(win);

   elm_run();
   elm_shutdown();

   /* exit code */
   return 0;
}
ELM_MAIN()
