/*
 * Tested interface: AtkValue
 *
 * Tested AtkObject: EailDatetime
 *
 * Description: Test AtkValue interface
 *
 * Test input: accessible object representing EailDatetime
 *
 * Expected test result: test should return 0 (success)
 */

#include <Elementary.h>
#include <atk/atk.h>
#include "eail_test_utils.h"

static time_t time_min, time_max, time_current;

INIT_TEST("EailDatetime")

static void
_init_datetime(Evas_Object *win)
{
   Evas_Object *bg, *bx, *dt;
   struct tm tmp;

   bg = elm_bg_add(win);
   elm_win_resize_object_add(win, bg);
   evas_object_size_hint_weight_set(bg, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
   evas_object_show(bg);

   bx = elm_box_add(win);
   evas_object_size_hint_weight_set(bx, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
   elm_win_resize_object_add(win, bx);
   elm_box_horizontal_set(bx, EINA_FALSE);
   evas_object_show(bx);

   dt = elm_datetime_add(bx);
   evas_object_size_hint_weight_set(dt, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
   evas_object_size_hint_align_set(dt, EVAS_HINT_FILL, 0.5);

   time_min = time(NULL) - 3600;
   localtime_r(&time_min, &tmp);
   elm_datetime_value_min_set(dt, &tmp);

   time_max = time(NULL) + 3600;
   localtime_r(&time_max, &tmp);
   elm_datetime_value_max_set(dt, &tmp);

   time_current = time(NULL);
   localtime_r(&time_current, &tmp);
   elm_datetime_value_set(dt, &tmp);

   elm_box_pack_end(bx, dt);
   evas_object_show(dt);
}

static void
_do_test(AtkObject *obj)
{
   time_t time_set;

   g_assert(ATK_IS_VALUE(obj));

   GValue value = G_VALUE_INIT;

   atk_value_get_minimum_value(ATK_VALUE(obj), &value);
   g_assert(g_value_get_int(&value) == time_min);

   atk_value_get_maximum_value(ATK_VALUE(obj), &value);
   g_assert(g_value_get_int(&value) == time_max);

   atk_value_get_current_value(ATK_VALUE(obj), &value);
   g_assert(g_value_get_int(&value) == time_current);

   atk_value_get_minimum_increment(ATK_VALUE(obj), &value);
   g_assert(g_value_get_int(&value) == 1);

   time_set = time_current + 60;

   g_value_set_int(&value, time_set);

   atk_value_get_current_value(ATK_VALUE(obj), &value);
   g_assert(g_value_get_int(&value) != time_set);

   eailu_test_code_called = 1;
}

EAPI_MAIN int
elm_main(int argc, char **argv)
{
   Evas_Object *win;

   win = eailu_create_test_window_with_glib_init(_on_done, _on_focus_in);
   g_assert(win);
   _init_datetime(win);
   evas_object_show(win);
   elm_run();
   elm_shutdown();

   return 0;
}
ELM_MAIN()
