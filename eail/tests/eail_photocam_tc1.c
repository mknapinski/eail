/*
 * Tested interface: AtkObject
 *
 * Tested AtkObject: EailPhotocam
 *
 * Description: Test AtkObject interface
 *
 * Test input: accessible object representing EailPhotocam
 *
 * Expected test result: test should return 0 (success)
 */

#include <Elementary.h>
#include <atk/atk.h>

#define DEBUG 1

#include "eail_test_utils.h"

INIT_TEST("EailPhotocam");

static void
_do_test(AtkObject *obj)
{
   g_assert(ATK_IS_OBJECT(obj));
   atk_object_set_description(obj, "test");
   g_assert_cmpstr(atk_object_get_description(obj), ==, "test");

   atk_object_set_name(obj, "test name");
   g_assert_cmpstr(atk_object_get_name(obj), ==, "test name");

   AtkStateSet *state_set = atk_object_ref_state_set(obj);

   g_assert(!atk_state_set_is_empty(state_set));

   g_object_unref(state_set);

   eailu_test_code_called = 1;
}

static void
_init_photocam(Evas_Object *win)
{
   Evas_Object *bg, *photocam;
   const char * const filename = "./data/whiterabbit01.jpg";

   bg = elm_bg_add(win);
   elm_bg_color_set(bg, 255, 255, 255);
   evas_object_size_hint_weight_set(bg, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
   elm_win_resize_object_add(win, bg);
   evas_object_show(bg);

   photocam = elm_photocam_add(win);
   g_assert(EVAS_LOAD_ERROR_NONE == elm_photocam_file_set(photocam, filename));
   elm_scroller_bounce_set(photocam, EINA_FALSE, EINA_TRUE);
   evas_object_resize(photocam, 300, 300);
   evas_object_show(photocam);
}

EAPI_MAIN int
elm_main(int argc, char **argv)
{
   Evas_Object *win;

   win = eailu_create_test_window_with_glib_init(_on_done, _on_focus_in);
   g_assert(win);
   _init_photocam(win);
   evas_object_show(win);
   elm_run();
   elm_shutdown();

   return 0;
}
ELM_MAIN()
