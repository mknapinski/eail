/*
 * Tested interface: AtkObject
 *
 * Tested AtkObject: EailTooltip
 *
 * Description: Test whether accessible object is successfully registered
 *    in GObject type system and is visible in AtkObject tree.
 *
 * Test input: accessible object representing EailTooltip
 *
 * Expected test result: test should return 0 (success)
 */

#include <Elementary.h>
#include <atk/atk.h>
#include "eail_test_utils.h"

INIT_TEST("EailWindow")

static void
_do_test(AtkObject *obj)
{
   g_assert(ATK_IS_OBJECT(obj));

   g_assert(atk_object_get_role(obj) == ATK_ROLE_WINDOW);

   atk_object_set_description(obj, "test");
   g_assert_cmpstr(atk_object_get_description(obj), ==, "test");

   atk_object_set_name(obj, "test name");
   g_assert_cmpstr(atk_object_get_name(obj), ==, "test name");

   eailu_test_code_called = 1;
}

static void
_init_tooltip(Evas_Object *win)
{
   const char *text = "test text";
   elm_object_tooltip_text_set(win, text);
   elm_object_tooltip_show(win);

   evas_object_resize(win, 320, 320);
}

EAPI_MAIN int
elm_main(int argc, char **argv)
{
   Evas_Object *win;

   win = eailu_create_test_window_with_glib_init(_on_done, _on_focus_in);
   _init_tooltip(win);

   evas_object_show(win);
   elm_run();
   elm_shutdown();

   return 0;
}
ELM_MAIN()
