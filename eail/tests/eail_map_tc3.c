/*
 * Tested interface: AtkAction
 *
 * Tested AtkObject: EailMap
 *
 * Description: Test AtkAction interface
 *
 * Test input: accessible object representing EailMap
 *
 * Expected test result: test should return 0 (success)
 */

#include <Elementary.h>
#include <atk/atk.h>

#include "eail_test_utils.h"

INIT_TEST("EailMap")

static void
_do_test(AtkObject *object)
{
   int n;
   int i;

   g_assert(ATK_IS_ACTION(object));

   eailu_test_action_description_all(ATK_ACTION(object));

   n = atk_action_get_n_actions(ATK_ACTION(object));
   g_assert(n == 8);

   for (i = 0; i < n; ++i)
     {
        const char *name = atk_action_get_name(ATK_ACTION(object), i);
        eailu_test_action_activate(ATK_ACTION(object), name);
     }

   eailu_test_code_called = 1;
}

static void
_init_map(Evas_Object *win)
{
   Evas_Object *bg, *map;

   bg = elm_bg_add(win);
   elm_win_resize_object_add(win, bg);
   evas_object_size_hint_weight_set(bg, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
   evas_object_show(bg);

   map = elm_map_add(win);
   elm_win_resize_object_add(win, map);
   evas_object_size_hint_weight_set(map, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
   evas_object_show(map);

   elm_map_zoom_set(map, 6);
   elm_map_region_show(map, 19.45, 51.75);
   evas_object_resize(win, 512, 512);
}

EAPI_MAIN int
elm_main(int argc, char **argv)
{
   Evas_Object *win;

   win = eailu_create_test_window_with_glib_init(_on_done, _on_focus_in);
   _init_map(win);
   evas_object_show(win);

   elm_run();
   elm_shutdown();

   return 0;
}
ELM_MAIN()
