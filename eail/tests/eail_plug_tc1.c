/*
 * Tested interface: AtkObject
 *
 * Tested AtkObject: EailPlug
 *
 * Description: Test AtkObject interface
 *
 * Test input: accessible object representing EailPlug
 *
 * Expected test result: test should return 0 (success)
 */

#include <Elementary.h>
#include <atk/atk.h>

#define DEBUG 1

#include "eail_test_utils.h"

INIT_TEST("EailPlug")

static void
_init_plug(Evas_Object *win)
{
   Evas_Object *bg, *plug;
   const char *filename = "data/plant_01.jpg";

   bg = elm_bg_add(win);
   elm_bg_file_set(bg, filename, NULL);
   elm_win_resize_object_add(win, bg);
   evas_object_size_hint_weight_set(bg, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
   evas_object_show(bg);

   plug = elm_plug_add(win);
   evas_object_resize(plug, 380, 500);
   evas_object_move(plug, 10, 10);
   evas_object_show(plug);
}

static void
_do_test(AtkObject *obj)
{
   const char *type_name = g_type_name(G_TYPE_FROM_INSTANCE(obj));

   g_assert(ATK_IS_OBJECT(obj));
   atk_object_set_description(obj, "test");
   g_assert_cmpstr(atk_object_get_description(obj), ==, "test");
   g_assert_cmpstr(type_name, ==, "EailPlug");

   atk_object_set_name(obj, "test name");
   g_assert_cmpstr(atk_object_get_name(obj), ==, "test name");

   g_assert(atk_object_get_role(obj) == ATK_ROLE_IMAGE);

   eailu_test_code_called = 1;
}

EAPI_MAIN int
elm_main(int argc, char **argv)
{
   Evas_Object *win;

   win = eailu_create_test_window_with_glib_init(_on_done, _on_focus_in);
   g_assert(win);
   _init_plug(win);
   evas_object_show(win);
   elm_run();
   elm_shutdown();

   return 0;
}
ELM_MAIN()
