/*
 * Tested interface: AtkAction
 *
 * Tested AtkObject: EailHover
 *
 * Description: Test AtkAction interface
 *
 * Test input: accessible object representing EailHover
 *
 * Expected test result: test should return 0 (success)
 */

#include <Elementary.h>
#include <atk/atk.h>

#include "eail_test_utils.h"

INIT_TEST("EailHover")

#define ACTIONS_COUNT 1

static int action_test_passed;

static void
_hide_hover(void *data, Evas_Object *obj, void *event_info)
{
   evas_object_hide(data);
   ++action_test_passed;
}

void
_init_hover(Evas_Object *win)
{
   Evas_Object *bg, *bt, *bt2, *rect, *rect2, *rect3, *hover;

   bg = elm_bg_add(win);
   evas_object_size_hint_weight_set(bg, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
   elm_win_resize_object_add(win, bg);
   evas_object_show(bg);

   rect = evas_object_rectangle_add(evas_object_evas_get(win));
   evas_object_size_hint_min_set(rect, 25, 25);
   evas_object_color_set(rect, 255, 0, 0, 255);
   evas_object_show(rect);

   rect2 = evas_object_rectangle_add(evas_object_evas_get(win));
   evas_object_size_hint_min_set(rect2, 25, 25);
   evas_object_color_set(rect2, 0, 255, 0, 255);
   evas_object_show(rect2);

   rect3 = evas_object_rectangle_add(evas_object_evas_get(win));
   evas_object_size_hint_min_set(rect3, 25, 25);
   evas_object_color_set(rect3, 0, 0, 255, 255);
   evas_object_show(rect3);

   bt = elm_button_add(win);
   elm_object_text_set(bt, "Show hover");
   evas_object_move(bt, 60, 90);
   evas_object_resize(bt, 80, 20);
   evas_object_show(bt);

   bt2 = elm_button_add(win);
   elm_object_text_set(bt2, "Hide hover");
   evas_object_show(bt2);

   hover = elm_hover_add(win);
   elm_hover_parent_set(hover, win);
   elm_hover_target_set(hover, bt);
   evas_object_smart_callback_add(hover, "clicked", _hide_hover, hover);
   evas_object_show(hover);

   elm_object_style_set(hover, "popout");

   elm_object_part_content_set(hover, "left", rect);
   elm_object_part_content_set(hover, "top", rect2);
   elm_object_part_content_set(hover, "right", rect3);
   elm_object_part_content_set(hover, "middle", bt2);

   evas_object_resize(win, 200, 200);
   evas_object_show(win);
}

void
_do_test(AtkObject *obj)
{
   g_assert(ATK_IS_ACTION(obj));

   g_assert(atk_action_get_n_actions(ATK_ACTION(obj)) == ACTIONS_COUNT);

   eailu_test_action_activate(ATK_ACTION(obj), "click");

   g_assert((eailu_get_action_number(ATK_ACTION(obj), "typo")) == -1);

   eailu_test_action_description_all(ATK_ACTION(obj));

   g_assert(action_test_passed == 1);

   eailu_test_code_called = 1;
}

EAPI_MAIN int
elm_main(int argc, char **argv)
{
   Evas_Object *win = NULL;

   win = eailu_create_test_window_with_glib_init(_on_done, _on_focus_in);
   if (!win) return 1;

   _init_hover(win);
   evas_object_show(win);
   elm_run();
   elm_shutdown();

   return 0;
}
ELM_MAIN()

