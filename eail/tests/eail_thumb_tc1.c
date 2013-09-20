/*
 * Tested interface: AtkObject
 *
 * Tested AtkObject: EailThumb
 *
 * Description: Test AtkObject interface
 *
 * Test input: accessible object representing EailThumb
 *
 * Expected test result: test should return 0 (success)
 */

#include <Elementary.h>
#include <atk/atk.h>
#include "eail_test_utils.h"

INIT_TEST("EailThumb")

static void
_init_thumb(Evas_Object *win)
{
   const char *filename = "./data/06.jpg";
   Evas_Object *bg, *thumb;

   elm_need_ethumb();

   bg = elm_bg_add(win);
   elm_win_resize_object_add(win, bg);
   evas_object_size_hint_weight_set(bg, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
   evas_object_show(bg);

   thumb = elm_thumb_add(win);
   elm_thumb_editable_set(thumb, EINA_FALSE);
   elm_thumb_file_set(thumb, filename, NULL);
   elm_thumb_reload(thumb);
   evas_object_size_hint_weight_set(thumb, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
   elm_win_resize_object_add(win, thumb);
   evas_object_show(thumb);
}

static void
_do_test(AtkObject *obj)
{
   g_assert(atk_object_get_role(obj) == ATK_ROLE_IMAGE);
   atk_object_set_description(obj, "test");
   g_assert_cmpstr(atk_object_get_description(obj), ==, "test");

   atk_object_set_name(obj, "test name");
   g_assert_cmpstr(atk_object_get_name(obj), ==, "test name");

   g_assert(NULL != atk_object_ref_state_set(obj));

   eailu_test_code_called = 1;
}

EAPI_MAIN int
elm_main(int argc, char **argv)
{
   Evas_Object *win;

   win = eailu_create_test_window_with_glib_init(_on_done, _on_focus_in);
   g_assert(win);
   _init_thumb(win);
   evas_object_show(win);
   elm_run();
   elm_shutdown();

   return 0;
}
ELM_MAIN()
