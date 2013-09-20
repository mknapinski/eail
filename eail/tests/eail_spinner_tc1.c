/*
 * Tested interface: AtkObject
 *
 * Tested AtkObject: EailSpinner
 *
 * Description: Test AtkObject interface
 *
 * Test input: accessible object representing EailSpinner
 *
 * Expected test result: test should return 0 (success)
 */

#include <Elementary.h>
#include <atk/atk.h>

#define DEBUG 1

#include "eail_test_utils.h"

INIT_TEST("EailSpinner")

struct
{
   double min, max, set, step;
}
static sp[4] = { { 0,   100, 80,  1   },
                 { 0,   100, 2.2, 1   },
                 { -50, 250, 100, 1.5 },
                 { 0,   100, 13,  1   } };

static void
_init_spinner(Evas_Object *win)
{
   Evas_Object *bg, *bx, *sp1, *sp2, *sp3, *sp4;

   bg = elm_bg_add(win);
   elm_win_resize_object_add(win, bg);
   evas_object_size_hint_weight_set(bg, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
   evas_object_show(bg);

   bx = elm_box_add(win);
   evas_object_size_hint_weight_set(bx, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
   elm_win_resize_object_add(win, bx);
   evas_object_show(bx);

   // default
   sp1 = elm_spinner_add(win);
   elm_spinner_value_set(sp1, sp[0].set);
   evas_object_size_hint_align_set(sp1, EVAS_HINT_FILL, 0.5);
   evas_object_size_hint_weight_set(sp1, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
   elm_box_pack_end(bx, sp1);
   evas_object_show(sp1);

   // format
   sp2 = elm_spinner_add(win);
   elm_spinner_label_format_set(sp2, "Percentage %%%1.2f something");
   elm_spinner_value_set(sp2, sp[1].set);
   evas_object_size_hint_align_set(sp2, EVAS_HINT_FILL, 0.5);
   evas_object_size_hint_weight_set(sp2, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
   elm_box_pack_end(bx, sp2);
   evas_object_show(sp2);

   // min max, step and wrap
   sp3 = elm_spinner_add(win);
   elm_spinner_label_format_set(sp3, "%1.1f units");
   elm_spinner_step_set(sp3, sp[2].step);
   elm_spinner_wrap_set(sp3, EINA_TRUE);
   elm_spinner_min_max_set(sp3, sp[2].min, sp[2].max);
   elm_spinner_value_set(sp3, sp[2].set);
   evas_object_size_hint_align_set(sp3, EVAS_HINT_FILL, 0.5);
   evas_object_size_hint_weight_set(sp3, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
   elm_box_pack_end(bx, sp3);
   evas_object_show(sp3);

   sp4 = elm_spinner_add(win);
   elm_spinner_value_set(sp4, sp[3].set);
   evas_object_size_hint_align_set(sp4, EVAS_HINT_FILL, 0.5);
   evas_object_size_hint_weight_set(sp4, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
   elm_box_pack_end(bx, sp4);
   evas_object_show(sp4);
}

static void
_do_test(AtkObject *obj)
{
   const char *type_name = g_type_name(G_TYPE_FROM_INSTANCE(obj));

   g_assert(ATK_IS_OBJECT(obj));
   atk_object_set_description(obj, "test");
   g_assert_cmpstr(atk_object_get_description(obj), ==, "test");

   atk_object_set_name(obj, "test name");
   g_assert_cmpstr(atk_object_get_name(obj), ==, "test name");

   g_assert_cmpstr(type_name, ==, "EailSpinner");

   g_assert(atk_object_get_role(obj) == ATK_ROLE_SPIN_BUTTON);

   eailu_test_code_called = 1;
}

EAPI_MAIN int
elm_main(int argc, char **argv)
{
   Evas_Object *win;

   win = eailu_create_test_window_with_glib_init(_on_done, _on_focus_in);
   g_assert(win);
   _init_spinner(win);
   evas_object_show(win);
   elm_run();
   elm_shutdown();

   return 0;
}
ELM_MAIN()
