/*
 * Tested interface: AtkAction
 *
 * Tested AtkObject: EailVideo
 *
 * Description: Test AtkAction interface
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
   AtkAction *action;
   gboolean success;
   const gchar *name, *action_name;
   int action_num;

   g_assert(ATK_IS_ACTION(object));
   action = ATK_ACTION(object);
   action_num =  atk_action_get_n_actions(action);
   g_assert(action_num == 7);
   action_name = atk_action_get_name(action, 2);
   g_assert(!strcmp(action_name, "pause"));
   success =  atk_action_set_description(action, 2, "pause action desc");
   g_assert(success);
   name = atk_action_get_description(action, 2);
   g_assert(!strcmp(name, "pause action desc"));
   success = atk_action_do_action(action, 2);

   /* test set/get action description */
   eailu_test_action_description_all(ATK_ACTION(object));

   eailu_test_code_called = 1;
}

static void
_playback_started(void *data, Evas_Object *obj, void *ev)
{
   _on_focus_in(NULL, data, NULL);
}

static void
_init_video(Evas_Object *win)
{
   Evas_Object *bg, *video, *emotion;

   bg = elm_bg_add(win);
   evas_object_size_hint_weight_set(bg, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
   elm_win_resize_object_add(win, bg);
   evas_object_show(bg);

   video = elm_video_add(win);
   evas_object_size_hint_weight_set(video, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
   elm_win_resize_object_add(win, video);
   evas_object_show(video);


   elm_video_file_set(video, VIDEO_FILE);
   elm_video_play(video);
   emotion = elm_video_emotion_get(video);
   evas_object_smart_callback_add(emotion,
                                  "playback_started",
                                  _playback_started,
                                  win);

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
