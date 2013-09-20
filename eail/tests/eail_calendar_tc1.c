/*
 * Tested interface: AtkObject
 *
 * Tested AtkObject: EailCalendar
 *
 * Description: Test AtkObject interface
 *
 * Test input: accessible object representing EailCalendar
 *
 * Expected test result: test should return 0 (success)
 */

#include <Elementary.h>
#include <atk/atk.h>

#define DEBUG 1
#include "eail_test_utils.h"

INIT_TEST("EailCalendar")

static int year_limit_min = 2000;
static int year_limit_max = 2030;

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
   g_assert(atk_object_get_role(obj) == ATK_ROLE_CALENDAR);

   atk_object_set_name(obj, "cal");
   g_assert_cmpstr(atk_object_get_name(obj), ==, "cal");
   atk_object_set_description(obj, "test");
   g_assert_cmpstr(atk_object_get_description(obj), ==, "test");

   atk_object_set_name(obj, "test name");
   g_assert_cmpstr(atk_object_get_name(obj), ==, "test name");

   /* calendar object to support focus */
   eailu_test_atk_focus(obj, TRUE);

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
