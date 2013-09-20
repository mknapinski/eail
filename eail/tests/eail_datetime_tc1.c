/*
 * Tested interface: AtkObject
 *
 * Tested AtkObject: EailDatetime
 *
 * Description: Test AtkObject interface
 *
 * Test input: accessible object representing EailDatetime
 *
 * Expected test result: test should return 0 (success)
 */

#include <Elementary.h>
#include <atk/atk.h>

//#define DEBUG 1

#include "eail_test_utils.h"

static time_t time_min, time_max, time_current;

INIT_TEST("EailDatetime")

static void
_changed_cb(void *data, Evas_Object *obj, void *event_info)
{
   _printf("action: changed\n");
}

static void
_printf_datetime_int(time_t atime, const char *str)
{
   struct tm tmp;

   localtime_r(&atime, &tmp);

   _printf("[%04d-%02d-%02d] [%02d:%02d:%02d] (%s)\n",
           tmp.tm_year + 1900, tmp.tm_mon + 1, tmp.tm_mday,
           tmp.tm_hour, tmp.tm_min, tmp.tm_sec, str);
}

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
   evas_object_smart_callback_add(dt, "changed", _changed_cb, NULL);

   time_min = time(NULL) - 3600;
   localtime_r(&time_min, &tmp);
   elm_datetime_value_min_set(dt, &tmp);
   _printf_datetime_int(time_min, "time_min");

   time_max = time(NULL) + 3600;
   localtime_r(&time_max, &tmp);
   elm_datetime_value_max_set(dt, &tmp);
   _printf_datetime_int(time_max, "time_max");

   time_current = time(NULL);
   localtime_r(&time_current, &tmp);
   elm_datetime_value_set(dt, &tmp);
   _printf_datetime_int(time_current, "time_current");

   elm_box_pack_end(bx, dt);
   evas_object_show(dt);
}

static void
_do_test(AtkObject *obj)
{
   g_assert(ATK_IS_OBJECT(obj));
   g_assert(atk_object_get_role(obj) == ATK_ROLE_DATE_EDITOR);

   atk_object_set_description(obj, "test");
   g_assert_cmpstr(atk_object_get_description(obj), ==, "test");

   atk_object_set_name(obj, "test name");
   g_assert_cmpstr(atk_object_get_name(obj), ==, "test name");

   eailu_test_code_called = 1;
}

EAPI_MAIN int
elm_main(int argc, char **argv)
{
   Evas_Object *win;

   win = eailu_create_test_window_with_glib_init(NULL, _on_focus_in);
   g_assert(win);
   _init_datetime(win);

   evas_object_show(win);
   elm_run();
   elm_shutdown();

   return 0;
}
ELM_MAIN()
