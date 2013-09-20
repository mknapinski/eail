/*
 * Tested interface: AtkObject
 *
 * Tested AtkObject: EailRoute
 *
 * Description: Test AtkObject interface
 *
 * Test input: accessible object representing EailRoute
 *
 * Expected test result: test should return 0 (success)
 */


#include <Elementary.h>
#include <atk/atk.h>

//#define DEBUG 1

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
   const char *type_name = g_type_name(G_TYPE_FROM_INSTANCE(obj));
   const char *role_name = atk_role_get_name(atk_object_get_role(obj));

   _printf("atk_object_get_role: %s\n", role_name ? role_name : "NULL");
   _printf("atk_object_get_type_name: %s\n", type_name ? type_name : "NULL");

   g_assert(ATK_IS_OBJECT(obj));
   atk_object_set_description(obj, "test");
   g_assert_cmpstr(atk_object_get_description(obj), ==, "test");

   atk_object_set_name(obj, "test name");
   g_assert_cmpstr(atk_object_get_name(obj), ==, "test name");

   g_assert(atk_object_get_role(obj) == ATK_ROLE_IMAGE_MAP);

   /* NOTE: route widget does not support focus */
   eailu_test_atk_focus(obj, FALSE);

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
