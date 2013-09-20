/*
 * Tested interface: AtkObject
 *
 * Tested AtkObject: EailIcon
 *
 * Description: Test AtkObject interface
 *
 * Test input: accessible object representing EailIcon
 *
 * Expected test result: test should return 0 (success)
 */

#include <Elementary.h>
#include <atk/atk.h>
#include "eail_test_utils.h"

#define ICON_NAME "home"
#define ICON_SIZE 32

INIT_TEST("EailIcon")

static void
_init_icon(Evas_Object *win)
{
   Evas_Object *bg, *icon;

   bg = elm_bg_add(win);
   elm_win_resize_object_add(win, bg);
   evas_object_size_hint_weight_set(bg, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
   evas_object_show(bg);

   icon = elm_icon_add(win);
   elm_icon_order_lookup_set(icon, ELM_ICON_LOOKUP_THEME_FDO);
   elm_icon_standard_set(icon, ICON_NAME);
   evas_object_resize(icon, ICON_SIZE, ICON_SIZE);
   evas_object_show(icon);
}

static void
_do_test(AtkObject *obj)
{
   const char *type_name = g_type_name(G_TYPE_FROM_INSTANCE(obj));
   const char *name = atk_object_get_name(obj);

   g_assert(ATK_IS_OBJECT(obj));
   atk_object_set_description(obj, "test");
   g_assert_cmpstr(atk_object_get_description(obj), ==, "test");

   atk_object_set_name(obj, "test name");
   g_assert_cmpstr(atk_object_get_name(obj), ==, "test name");

   g_assert_cmpstr(type_name, ==, "EailIcon");

   g_assert_cmpstr(name, ==, ICON_NAME);

   g_assert(atk_object_get_role(obj) == ATK_ROLE_ICON);

   eailu_test_code_called = 1;
}

EAPI_MAIN int
elm_main(int argc, char **argv)
{
   Evas_Object *win;

   win = eailu_create_test_window_with_glib_init(_on_done, _on_focus_in);
   g_assert(win);
   _init_icon(win);
   evas_object_show(win);
   elm_run();
   elm_shutdown();

   return 0;
}
ELM_MAIN()
