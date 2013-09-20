/*
 * Tested interface: AtkAction
 *
 * Tested AtkObject: EailPhotocam
 *
 * Description: Test AtkAction interface
 *
 * Test input: accessible object representing EailPhotocam
 *
 * Expected test result: test should return 0 (success)
 */

#include <Elementary.h>
#include <atk/atk.h>
#include "eail_test_utils.h"

#define ACTIONS_NUMBER 7

static int action_test_passed;

INIT_TEST("EailPhotocam")

static void
_on_photocam_clicked(void *data, Evas_Object *obj, void *ev)
{
   action_test_passed = 1;
}

static void
_do_test(AtkObject *obj)
{
   g_assert(ATK_IS_ACTION(obj));

   g_assert(ACTIONS_NUMBER == atk_action_get_n_actions(ATK_ACTION(obj)));

   eailu_test_action_activate(ATK_ACTION(obj), "click");

   g_assert((eailu_get_action_number(ATK_ACTION(obj), "typo")) == -1);

   eailu_test_action_description_all(ATK_ACTION(obj));

   for (int count = 0; ACTIONS_NUMBER > count; count++)
     g_assert(atk_action_do_action(ATK_ACTION(obj), count));

   g_assert(action_test_passed);

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
   evas_object_smart_callback_add(photocam, "clicked", _on_photocam_clicked, NULL);
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
