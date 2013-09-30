/*
 * Tested interface: AtkValue
 *
 * Tested AtkObject: EailSlider
 *
 * Description: Test AtkValue interface
 *
 * Test input: accessible object representing EailSlider
 *
 * Expected test result: test should return 0 (success)
 */

#include <Elementary.h>
#include <atk/atk.h>

#include "eail_test_utils.h"

INIT_TEST("EailSlider")

const char * const slider_name = "slider-name";
const double slider_min = -50;
const double slider_max = 50;
const double slider_set = 20;

static void
_init_slider(Evas_Object *win)
{
   Evas_Object *bg, *bx, *sl;

   bg = elm_bg_add(win);
   elm_win_resize_object_add(win, bg);
   evas_object_size_hint_weight_set(bg, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
   evas_object_show(bg);

   bx = elm_box_add(win);
   evas_object_size_hint_weight_set(bx, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
   elm_win_resize_object_add(win, bx);
   evas_object_show(bx);

   sl = elm_slider_add(win);
   elm_object_text_set(sl, slider_name);
   elm_slider_min_max_set(sl, slider_min, slider_max);
   elm_slider_value_set(sl, slider_set);
   elm_slider_unit_format_set(sl, "%2.1f");
   elm_slider_span_size_set(sl, 120);
   evas_object_size_hint_align_set(sl, EVAS_HINT_FILL, 0.5);
   evas_object_size_hint_weight_set(sl, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
   elm_box_pack_end(bx, sl);
   evas_object_show(sl);
}

static void
_do_test(AtkObject *obj)
{
   double minimum, maximum, current, minimum_increment;
   const double val_test_set = 12.3;

   g_assert(ATK_IS_VALUE(obj));

   GValue value = G_VALUE_INIT;
   g_value_init(&value, G_TYPE_DOUBLE);

   atk_value_get_minimum_value(ATK_VALUE(obj), &value);
   minimum = g_value_get_double(&value);
   g_assert(float_equal(slider_min,minimum));

   atk_value_get_maximum_value(ATK_VALUE(obj), &value);
   maximum = g_value_get_double(&value);
   g_assert(float_equal(slider_max,maximum));

   atk_value_get_current_value(ATK_VALUE(obj), &value);
   current = g_value_get_double(&value);
   g_assert(float_equal(slider_set,current));

   atk_value_get_minimum_increment(ATK_VALUE(obj), &value);
   minimum_increment = g_value_get_double(&value);
   g_assert(float_equal(G_MINDOUBLE,minimum_increment));

   g_value_set_double(&value, val_test_set);
   g_assert(atk_value_set_current_value(ATK_VALUE(obj), &value));

   atk_value_get_current_value(ATK_VALUE(obj), &value);
   current = g_value_get_double(&value);

   g_assert(float_equal(val_test_set,current));

   g_value_set_double(&value, minimum - 1);
   g_assert(!atk_value_set_current_value(ATK_VALUE(obj), &value));

   g_value_set_double(&value, maximum + 1);
   g_assert(!atk_value_set_current_value(ATK_VALUE(obj), &value));

   atk_value_get_minimum_increment(ATK_VALUE(obj), &value);
   g_assert(float_equal(g_value_get_double(&value),0));

   eailu_test_code_called = 1;
}

EAPI_MAIN int
elm_main(int argc, char **argv)
{
   Evas_Object *win;

   win = eailu_create_test_window_with_glib_init(_on_done, _on_focus_in);
   g_assert(win);
   _init_slider(win);
   evas_object_show(win);
   elm_run();
   elm_shutdown();

   return 0;
}
ELM_MAIN()
