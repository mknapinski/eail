/*
 * Tested interface: AtkAction
 *
 * Tested AtkObject: EailPanel
 *
 * Description: Test AtkAction interface
 *
 * Test input: accessible object representing EailPanel
 *
 * Expected test result: test should return 0 (success)
 */

#include <Elementary.h>
#include <atk/atk.h>
#include "eail_test_utils.h"

INIT_TEST("EailPanel")

static void
_do_test(AtkObject *object)
{
   gboolean result;

   g_assert(ATK_IS_ACTION(object));

   AtkAction *action = ATK_ACTION(object);
   int actions_num = atk_action_get_n_actions(action);
   g_assert(actions_num == 1);
   const gchar *action_name = atk_action_get_name(action, 0);
   g_assert(!strcmp("toggle", action_name));
   result = atk_action_do_action(action, 0);
   eailu_test_action_description_all(ATK_ACTION(object));
   g_assert(result);

   AtkStateSet *state_set = atk_object_ref_state_set(object);
   gboolean contains = atk_state_set_contains_state(state_set,
                                                    ATK_STATE_VISIBLE);
   g_assert(contains);
   g_object_unref(state_set);

   eailu_test_code_called = 1;
}

static void
_init_panel(Evas_Object *win)
{
   Evas_Object *bg, *bx, *panel, *content;


   bg = elm_bg_add(win);
   elm_win_resize_object_add(win, bg);
   evas_object_show(bg);

   bx = elm_box_add(win);
   elm_box_horizontal_set(bx, EINA_FALSE);
   evas_object_size_hint_weight_set(bx, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
   evas_object_size_hint_align_set(bx, EVAS_HINT_FILL, EVAS_HINT_FILL);
   elm_win_resize_object_add(win, bx);
   evas_object_show(bx);

   panel = elm_panel_add(win);
   elm_panel_orient_set(panel, ELM_PANEL_ORIENT_TOP);
   elm_panel_toggle(panel);
   evas_object_size_hint_weight_set(panel, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
   evas_object_size_hint_align_set(panel, EVAS_HINT_FILL, EVAS_HINT_FILL);
   evas_object_show(panel);
   elm_box_pack_end(bx, panel);

   content = elm_label_add(win);
   elm_object_text_set(content, "content");
   evas_object_show(content);
   elm_object_content_set(panel, content);

   panel = elm_panel_add(win);
   elm_panel_orient_set(panel, ELM_PANEL_ORIENT_RIGHT);
   elm_panel_hidden_set(panel, EINA_TRUE);
   evas_object_size_hint_weight_set(panel, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
   evas_object_size_hint_align_set(panel, EVAS_HINT_FILL, EVAS_HINT_FILL);
   evas_object_show(panel);
   elm_box_pack_end(bx, panel);

   content = elm_label_add(win);
   elm_object_text_set(content, "content2");
   evas_object_show(content);
   elm_object_content_set(panel, content);

   panel = elm_panel_add(win);
   evas_object_size_hint_weight_set(panel, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
   evas_object_size_hint_align_set(panel, EVAS_HINT_FILL, EVAS_HINT_FILL);
   evas_object_show(panel);
   elm_box_pack_end(bx, panel);

   content = elm_label_add(win);
   elm_object_text_set(content, "content3");
   evas_object_show(content);
   elm_object_content_set(panel, content);
}

EAPI_MAIN int
elm_main(int argc, char **argv)
{
   Evas_Object *win;
   win = eailu_create_test_window_with_glib_init(_on_done, _on_focus_in);
   _init_panel(win);

   evas_object_show(win);
   elm_run();
   elm_shutdown();

   return 0;
}
ELM_MAIN()
