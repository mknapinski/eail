/*
 * Tested interface: AtkAction
 *
 * Tested AtkObject: EailIcon
 *
 * Description: Test AtkAction interface
 *
 * Test input: accessible object representing EailIcon
 *
 * Expected test result: test should return 0 (success)
 */

#include <Elementary.h>
#include <atk/atk.h>
#include "eail_test_utils.h"

#define ACTIONS_NUMBER 1

#define ICON_NAME "home"
#define ICON_SIZE 32

INIT_TEST("EailIcon")

static void
_init_icon(Evas_Object *win)
{
   Evas_Object *bg, *icon;

   bg = elm_bg_add(win);
   elm_win_resize_object_add(win, bg);
   evas_object_size_hint_weight_set(bg, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
   evas_object_show(bg);

   icon = elm_icon_add(win);
   elm_icon_order_lookup_set(icon, ELM_ICON_LOOKUP_THEME_FDO);
   elm_icon_standard_set(icon, ICON_NAME);
   evas_object_resize(icon, ICON_SIZE, ICON_SIZE);
   evas_object_show(icon);
}

static void
_do_test(AtkObject *obj)
{
   g_assert(ATK_IS_ACTION(obj));

   g_assert(ACTIONS_NUMBER == atk_action_get_n_actions(ATK_ACTION(obj)));

   eailu_test_action_activate(ATK_ACTION(obj), "click");

   g_assert(-1 == (eailu_get_action_number(ATK_ACTION(obj), "typo")));

   eailu_test_action_description_all(ATK_ACTION(obj));

   eailu_test_code_called = 1;
}

EAPI_MAIN int
elm_main(int argc, char **argv)
{
   Evas_Object *win;

   win = eailu_create_test_window_with_glib_init(_on_done, _on_focus_in);
   g_assert(win);
   _init_icon(win);
   evas_object_show(win);
   elm_run();
   elm_shutdown();

   return 0;
}
ELM_MAIN()
