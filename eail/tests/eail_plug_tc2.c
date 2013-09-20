/*
 * Tested interface: AtkAction
 *
 * Tested AtkObject: EailPlug
 *
 * Description: Test AtkAction interface
 *
 * Test input: accessible object representing EailPlug
 *
 * Expected test result: test should return 0 (success)
 */

#include <Elementary.h>
#include <atk/atk.h>

#define DEBUG 1

#include "eail_test_utils.h"

#define ACTIONS_NUMBER 1

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
   g_assert(ATK_IS_ACTION(obj));

   g_assert(ACTIONS_NUMBER == atk_action_get_n_actions(ATK_ACTION(obj)));

   eailu_test_action_activate(ATK_ACTION(obj), "click");

   g_assert((eailu_get_action_number(ATK_ACTION(obj), "typo")) == -1);

   eailu_test_action_description_all(ATK_ACTION(obj));

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
