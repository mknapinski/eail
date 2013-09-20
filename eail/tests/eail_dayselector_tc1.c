/*
 * Tested interface: AtkObject
 *
 * Tested AtkObject: EailDayselector
 *
 * Description: Test AtkObject interface
 *
 * Test input: accessible object representing EailDayselector
 *
 * Expected test result: test should return 0 (success)
 */

#include <Elementary.h>
#include <atk/atk.h>

#include "eail_test_utils.h"

#define EAIL_TYPE_FOR_DAY_SELECTOR "EailDayselector"
#define EAIL_TYPE_FOR_CHECK "EailCheck"

INIT_TEST("EailDayselector")

static void
_do_test(AtkObject *obj)
{
   int child_count = 0;
   int i;

   atk_object_set_description(obj, "test");
   g_assert_cmpstr(atk_object_get_description(obj), ==, "test");

   atk_object_set_name(obj, "test name");
   g_assert_cmpstr(atk_object_get_name(obj), ==, "test name");

   child_count =  atk_object_get_n_accessible_children(obj);
   g_assert(child_count == 7);

   /* all fields have to be CHECK type*/
   for (i = 0; i < child_count; i++)
    {
       AtkObject *child = atk_object_ref_accessible_child(obj, i);
       g_assert(ATK_IS_OBJECT(child));
       g_assert(eailu_is_object_with_type(child, EAIL_TYPE_FOR_CHECK));

       g_object_unref(child);
     }

   eailu_test_atk_focus(obj, TRUE);

   eailu_test_code_called = 1;
}

static void
_init_dayselector(Evas_Object *win)
{
   Evas_Object *dayselector, *box;

   box = elm_box_add(win);
   evas_object_size_hint_weight_set(box, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
   elm_win_resize_object_add(win, box);
   evas_object_show(box);

   dayselector = elm_dayselector_add(win);
   evas_object_size_hint_weight_set(dayselector, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
   evas_object_size_hint_align_set(dayselector, EVAS_HINT_FILL, 0.5);

   elm_box_pack_end(box, dayselector);
   evas_object_show(dayselector);
}

EAPI_MAIN int
elm_main(int argc, char **argv)
{
   Evas_Object *win;

   win = eailu_create_test_window_with_glib_init(_on_done, _on_focus_in);
   _init_dayselector(win);

   evas_object_show(win);
   elm_run();
   elm_shutdown();

   /* exit code */
   return 0;
}
ELM_MAIN()
