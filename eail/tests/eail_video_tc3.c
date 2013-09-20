/*
 * Tested interface: AtkStreamableContent
 *
 * Tested AtkObject: EailVideo
 *
 * Description: Test AtkStreamableContent interface
 *
 * Test input: accessible object representing EailVideo
 *
 * Expected test result: test should return 0 (success)
 */

#include <Elementary.h>
#include <atk/atk.h>

//#define DEBUG 1

#include "eail_test_utils.h"

#define VIDEO_FILE "./data/softboy.avi"

INIT_TEST("EailVideo")

static void
_do_test(AtkObject *object)
{
   AtkStreamableContent *streamable;
   const gchar *mime, *uri;
   int n_mimes;
   GIOChannel *gio;

   /*AtkStreamableContent test*/
   g_assert(ATK_IS_STREAMABLE_CONTENT(object));
   streamable = ATK_STREAMABLE_CONTENT(object);
   n_mimes = atk_streamable_content_get_n_mime_types(streamable);
   g_assert(n_mimes == 1);
   mime = atk_streamable_content_get_mime_type(streamable, 0);
   g_assert(!strcmp(mime, "video/x-msvideo"));
   uri = atk_streamable_content_get_uri(streamable, "video/x-msvideo");
   _printf("file uri %s\n", uri);
   char *res = g_strrstr(uri, "softboy.avi");
   g_assert(res);
   gio = NULL;
   gio = atk_streamable_content_get_stream(streamable, "video/x-msvideo");
   g_assert(gio);
   if (gio)
     {
        g_io_channel_shutdown(gio, TRUE, NULL);
        g_io_channel_unref(gio);
     }

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

   win = eailu_create_test_window_with_glib_init(0,0);
   _init_video(win);

   elm_run();
   elm_shutdown();

   return 0;
}
ELM_MAIN()
