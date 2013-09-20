/*
 * Tested interface: AtkValue
 *
 * Tested AtkObject: EailCalendar
 *
 * Description: Test AtkValue interface
 *
 * Test input: accessible object representing EailCalendar
 *
 * Expected test result: test should return 0 (success)
 */

#include <Elementary.h>
#include <atk/atk.h>

//#define DEBUG 1

#include "eail_test_utils.h"

INIT_TEST("EailCalendar")

static int year_limit_min = 2000;
static int year_limit_max = 2030;

static void
_printf_calendar_int(const char *str, time_t atime)
{
   struct tm tmp;

   localtime_r(&atime, &tmp);

   _printf("[%04d-%02d-%02d] (%s)\n",
           tmp.tm_year + 1900, tmp.tm_mon + 1, tmp.tm_mday, str);
}

static time_t
_val_min(int year)
{
   struct tm minimum;

   memset(&minimum, 0, sizeof(minimum));

   minimum.tm_year = year - 1900;
   minimum.tm_mon = 0;
   minimum.tm_mday = 1;
   minimum.tm_hour = 0;
   minimum.tm_min = 0;
   minimum.tm_sec = 0;

   return mktime(&minimum);
}

static int
_val_date_cmp(time_t time1, time_t time2)
{
   struct tm tmp1, tmp2;

   localtime_r(&time1, &tmp1);
   localtime_r(&time2, &tmp2);

   if (tmp1.tm_year == tmp2.tm_year &&
       tmp1.tm_mon == tmp2.tm_mon &&
       tmp1.tm_mday == tmp2.tm_mday)
     {
        return TRUE;
     }
     else
     {
        return FALSE;
     }
}

static time_t
_val_max(int year)
{
   struct tm maximum;

   memset(&maximum, 0, sizeof(maximum));

   maximum.tm_year = year - 1900;
   maximum.tm_mon = 11;
   maximum.tm_mday = 31;
   maximum.tm_hour = 23;
   maximum.tm_min = 59;
   maximum.tm_sec = 59;

   return mktime(&maximum);
}

static void
_init_calendar(Evas_Object *win)
{
   Evas_Object *bg, *cal;

   bg = elm_bg_add(win);
   elm_win_resize_object_add(win, bg);
   evas_object_size_hint_weight_set(bg, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
   evas_object_show(bg);

   cal = elm_calendar_add(win);
   elm_calendar_min_max_year_set(cal, year_limit_min, year_limit_max);
   elm_win_resize_object_add(win, cal);
   evas_object_size_hint_weight_set(cal, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
   evas_object_show(cal);
}

static void
_do_test(AtkObject *obj)
{
   int minimum, maximum, current, minimum_increment;
   struct tm val_test_set;

   g_assert(ATK_IS_VALUE(obj));

   GValue value = G_VALUE_INIT;

   atk_value_get_minimum_value(ATK_VALUE(obj), &value);
   minimum = g_value_get_int(&value);
   _printf_calendar_int("atk_value_get_minimum_value", minimum);
   g_assert(_val_min(year_limit_min) == minimum);

   atk_value_get_maximum_value(ATK_VALUE(obj), &value);
   maximum = g_value_get_int(&value);
   _printf_calendar_int("atk_value_get_maximum_value", maximum);
   g_assert(_val_max(year_limit_max) == maximum);

   atk_value_get_current_value(ATK_VALUE(obj), &value);
   current = g_value_get_int(&value);
   _printf_calendar_int("atk_value_get_current_value", current);
   g_assert(_val_date_cmp(time(NULL), current));

   atk_value_get_minimum_increment(ATK_VALUE(obj), &value);
   minimum_increment = g_value_get_int(&value);
   _printf_calendar_int("atk_value_get_minimum_increment", minimum_increment);
   g_assert(1 == minimum_increment);

   memset(&val_test_set, 0, sizeof(val_test_set));
   val_test_set.tm_year = 120;  // 120 + 1900 = 2020 years
   val_test_set.tm_mon = 2;
   val_test_set.tm_mday = 15;
   val_test_set.tm_hour = 7;
   val_test_set.tm_min = 21;
   val_test_set.tm_sec = 33;

   g_value_set_int(&value, mktime(&val_test_set));
   g_assert(atk_value_set_current_value(ATK_VALUE(obj), &value));
   _printf_calendar_int("atk_value_set_current_value", mktime(&val_test_set));

   atk_value_get_current_value(ATK_VALUE(obj), &value);
   current = g_value_get_int(&value);
   _printf_calendar_int("atk_value_get_current_value", current);
   g_assert(_val_date_cmp(mktime(&val_test_set), current));

   val_test_set.tm_year = 99;  // 99 + 1900 = 1999 years
   g_value_set_int(&value, mktime(&val_test_set));
   g_assert(!atk_value_set_current_value(ATK_VALUE(obj), &value));

   val_test_set.tm_year = 131;  // 131 + 1900 = 2031 years
   g_value_set_int(&value, mktime(&val_test_set));
   g_assert(!atk_value_set_current_value(ATK_VALUE(obj), &value));

   eailu_test_code_called = 1;
}

EAPI_MAIN int
elm_main(int argc, char **argv)
{
   Evas_Object *win;

   win = eailu_create_test_window_with_glib_init(_on_done, _on_focus_in);
   g_assert(win);
   _init_calendar(win);
   evas_object_show(win);
   elm_run();
   elm_shutdown();

   return 0;
}
ELM_MAIN()
