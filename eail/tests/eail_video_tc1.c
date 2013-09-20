/*
 * Tested interface: AtkObject
 *
 * Tested AtkObject: EailVideo
 *
 * Description: Test AtkObject interface
 *
 * Test input: accessible object representing EailVideo
 *
 * Expected test result: test should return 0 (success)
 */

#include <Elementary.h>
#include <atk/atk.h>
#include "eail_test_utils.h"

#define VIDEO_FILE "./data/softboy.avi"

INIT_TEST("EailVideo")

static void
_do_test(AtkObject *object)
{
   AtkStateSet *state_set;
   gboolean contains;

   g_assert(ATK_IS_OBJECT(object));
   atk_object_set_description(object, "test");
   g_assert_cmpstr(atk_object_get_description(object), ==, "test");

   g_assert(atk_object_get_role(object) == ATK_ROLE_ANIMATION);
   atk_object_set_name(object, VIDEO_FILE);
   g_assert_cmpstr(atk_object_get_name(object), ==, VIDEO_FILE);

   state_set = atk_object_ref_state_set(object);
   contains = atk_state_set_contains_state(state_set, ATK_STATE_ANIMATED);
   g_object_unref(state_set);
   g_assert(contains);

   eailu_test_code_called = 1;
}

static void
_init_video(Evas_Object *win)
{
   Evas_Object *bg, *video;

   bg = elm_bg_add(win);
   evas_object_size_hint_weight_set(bg, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
   elm_win_resize_object_add(win, bg);
   evas_object_show(bg);

   video = elm_video_add(win);
   evas_object_size_hint_weight_set(video, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
   elm_win_resize_object_add(win, video);
   evas_object_show(video);

   evas_object_resize(win, 800, 800);
   evas_object_show(win);
}

EAPI_MAIN int
elm_main(int argc, char **argv)
{
   Evas_Object *win;

   win = eailu_create_test_window_with_glib_init(_on_done, _on_focus_in);
   _init_video(win);

   elm_run();
   elm_shutdown();

   return 0;
}
ELM_MAIN()
