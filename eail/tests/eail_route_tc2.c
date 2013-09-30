/*
 * Tested interface: AtkValue
 *
 * Tested AtkObject: EailRoute
 *
 * Description: Test AtkValue interface
 *
 * Test input: accessible object representing EailRoute
 *
 * Expected test result: test should return 0 (success)
 */

#include <Elementary.h>
#include <atk/atk.h>

#include "eail_test_utils.h"

INIT_TEST("EailRoute")

static void
_init_route(Evas_Object *win)
{
   Evas_Object *bg;

   bg = elm_bg_add(win);
   elm_win_resize_object_add(win, bg);
   evas_object_size_hint_weight_set(bg, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
   evas_object_show(bg);

   elm_route_add(win);
}

static void
_do_test(AtkObject *obj)
{
   g_assert(ATK_IS_VALUE(obj));

   GValue value = G_VALUE_INIT;
   atk_value_get_current_value(ATK_VALUE(obj), &value);
   _printf("atk_value_get_current_value:\n%s\n", g_value_get_string(&value));

   g_assert_cmpstr(g_value_get_string(&value), ==,
                   "<longitude><min>0.000000</min><max>0.000000</max></longitude>\n"
                   "<latitude><min>0.000000</min><max>0.000000</max></latitude>");

   atk_value_get_maximum_value(ATK_VALUE(obj), &value);
   _printf("atk_value_get_maximum_value:\n%s\n", g_value_get_string(&value));

   atk_value_get_minimum_value(ATK_VALUE(obj), &value);
   _printf("atk_value_get_minimum_value:\n%s\n", g_value_get_string(&value));

   atk_value_get_minimum_increment(ATK_VALUE(obj), &value);
   _printf("atk_value_get_minimum_increment:\n%s\n", g_value_get_string(&value));

   eailu_test_code_called = 1;
}

EAPI_MAIN int
elm_main(int argc, char **argv)
{
   Evas_Object *win;

   win = eailu_create_test_window_with_glib_init(_on_done, _on_focus_in);
   g_assert(win);
   _init_route(win);
   evas_object_show(win);
   elm_run();
   elm_shutdown();

   return 0;
}
ELM_MAIN()
