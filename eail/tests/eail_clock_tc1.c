/*
 * Tested interface: AtkValue
 *
 * Tested AtkObject: EailClock
 *
 * Description: Test AtkValue interface
 *
 * Test input: accessible object representing EailClock
 *
 * Expected test result: test should return 0 (success)
 */

#include <Elementary.h>
#include <atk/atk.h>
#include "eail_test_utils.h"

INIT_TEST("EailClock")

static void
_printf_clock_int(const char *str, int current_second_day)
{
   int hour, minute, second;

   hour = current_second_day / 3600;
   minute = (current_second_day % 3600) / 60;
   second = current_second_day % 60;

   _printf("[%02d:%02d:%02d] (%s)\n", hour, minute, second, str);
}

static void
_do_test(AtkObject *obj)
{
   int minimum, maximum, current, minimum_increment;
   const int lambda = 5; /*tolerance for get time in case that test will take
   longer than expected on very slow machine*/
   const int val_test_init = 4 * 3600 + 44 * 60 + 22; // 04:44:22
   const int val_test_set = 12 * 3600 + 4 * 60 + 7;   // 12:04:07

   g_assert(atk_object_get_role(obj) == ATK_ROLE_TEXT);

   GValue value = G_VALUE_INIT;

   atk_value_get_minimum_value(ATK_VALUE(obj), &value);
   minimum = g_value_get_int(&value);
   _printf_clock_int("atk_value_get_minimum_value", minimum);
   g_assert(0 == minimum);

   atk_value_get_maximum_value(ATK_VALUE(obj), &value);
   maximum = g_value_get_int(&value);
   _printf_clock_int("atk_value_get_maximum_value", maximum);
   g_assert((24 * 3600 - 1) == maximum);

   atk_value_get_current_value(ATK_VALUE(obj), &value);
   current = g_value_get_int(&value);
   _printf_clock_int("atk_value_get_current_value", current);
   g_assert(abs(current - val_test_init) < lambda);

   atk_value_get_minimum_increment(ATK_VALUE(obj), &value);
   minimum_increment = g_value_get_int(&value);
   _printf_clock_int("atk_value_get_minimum_increment", minimum_increment);
   g_assert(1 == minimum_increment);

   g_value_set_int(&value, val_test_set);
   g_assert(atk_value_set_current_value(ATK_VALUE(obj), &value));
   _printf_clock_int("atk_value_set_current_value", val_test_set);

   atk_value_get_current_value(ATK_VALUE(obj), &value);
   current = g_value_get_int(&value);
   _printf_clock_int("atk_value_get_current_value", current);
   g_assert(abs(current - val_test_set) < lambda);

   g_value_set_int(&value, 24 * 3600);
   g_assert(!atk_value_set_current_value(ATK_VALUE(obj), &value));

   eailu_test_code_called = 1;
}

void
_init_clock(Evas_Object *win)
{
   Evas_Object *bg, *bx, *cl;

   bg = elm_bg_add(win);
   elm_win_resize_object_add(win, bg);
   evas_object_size_hint_weight_set(bg, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
   evas_object_show(bg);

   bx = elm_box_add(win);
   evas_object_size_hint_weight_set(bx, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
   elm_win_resize_object_add(win, bx);
   evas_object_show(bx);

   cl = elm_clock_add(win);
   elm_clock_show_seconds_set(cl, EINA_TRUE);
   elm_box_pack_end(bx, cl);
   elm_clock_time_set(cl, 4, 44, 22);
   evas_object_show(cl);
}

EAPI_MAIN int
elm_main(int argc, char **argv)
{
   Evas_Object *win;

   win = eailu_create_test_window_with_glib_init(_on_done, _on_focus_in);
   g_assert(win);
   _init_clock(win);
   evas_object_show(win);
   elm_run();
   elm_shutdown();

   return 0;
}
ELM_MAIN()
