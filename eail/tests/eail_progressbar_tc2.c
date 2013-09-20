/*
 * Tested interface: AtkValue
 *
 * Tested AtkObject: EailProgressbar
 *
 * Description: Test AtkValue interface
 *
 * Test input: accessible object representing EailProgressbar
 *
 * Expected test result: test should return 0 (success)
 */

#include <Elementary.h>
#include <atk/atk.h>

//#define DEBUG 1

#include "eail_test_utils.h"

INIT_TEST("EailProgressbar")

const double progressbar_min = 0.0;
const double progressbar_max = 1.0;
const double progressbar_set = 0.2;

static void
_do_test(AtkObject *obj)
{
   double minimum, maximum, current, minimum_increment;
   const double val_test_set = 0.47;

   GValue value = G_VALUE_INIT;
   g_value_init(&value, G_TYPE_DOUBLE);

   atk_value_get_minimum_value(ATK_VALUE(obj), &value);
   minimum = g_value_get_double(&value);
   _printf("atk_value_get_minimum_value: %0.2f\n", minimum);
   g_assert((float)progressbar_min == (float)minimum);

   atk_value_get_maximum_value(ATK_VALUE(obj), &value);
   maximum = g_value_get_double(&value);
   _printf("atk_value_get_maximum_value: %0.2f\n", maximum);
   g_assert((float)progressbar_max == (float)maximum);

   atk_value_get_current_value(ATK_VALUE(obj), &value);
   current = g_value_get_double(&value);
   _printf("atk_value_get_current_value: %0.2f\n", current);
   g_assert((float)progressbar_set == (float)current);

   atk_value_get_minimum_increment(ATK_VALUE(obj), &value);
   minimum_increment = g_value_get_double(&value);
   _printf("atk_value_get_minimum_increment: %0.2f\n", minimum_increment);
   g_assert((float)G_MINDOUBLE == (float)minimum_increment);

   g_value_set_double(&value, val_test_set);
   g_assert(atk_value_set_current_value(ATK_VALUE(obj), &value));
   _printf("atk_value_set_current_value: %0.2f\n", val_test_set);

   atk_value_get_current_value(ATK_VALUE(obj), &value);
   current = g_value_get_double(&value);
   _printf("atk_value_get_current_value: %0.2f\n", current);

   g_assert((float)val_test_set == (float)current);

   g_value_set_double(&value, minimum - 0.01);
   g_assert(!atk_value_set_current_value(ATK_VALUE(obj), &value));

   g_value_set_double(&value, maximum + 0.01);
   g_assert(!atk_value_set_current_value(ATK_VALUE(obj), &value));

   eailu_test_code_called = 1;
}

static void
_init_progressbar(Evas_Object *win)
{
   Evas_Object *bg, *bx, *pb;

   bg = elm_bg_add(win);
   elm_win_resize_object_add(win, bg);
   evas_object_size_hint_weight_set(bg, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
   evas_object_show(bg);

   bx = elm_box_add(win);
   elm_win_resize_object_add(win, bx);
   evas_object_size_hint_weight_set(bx, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
   evas_object_show(bx);

   pb = elm_progressbar_add(win);
   elm_progressbar_span_size_set(pb, 260);
   elm_progressbar_value_set(pb, progressbar_set);
   evas_object_size_hint_weight_set(pb, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
   evas_object_size_hint_align_set(pb, EVAS_HINT_FILL, 0.5);
   elm_box_pack_end(bx, pb);
   evas_object_show(pb);
}

EAPI_MAIN int
elm_main(int argc, char **argv)
{
   Evas_Object *win;

   win = eailu_create_test_window_with_glib_init(_on_done, _on_focus_in);
   g_assert(win);
   _init_progressbar(win);
   evas_object_show(win);
   elm_run();
   elm_shutdown();

   return 0;
}
ELM_MAIN()
