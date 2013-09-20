/*
 * Tested interface: AtkAction
 *
 * Tested AtkObject: EailBubble
 *
 * Description: Test AtkAction interface
 *
 * Test input: accessible object representing EailBubble
 *
 * Expected test result: test should return 0 (success)
 */

#include <Elementary.h>
#include <atk/atk.h>

#define DEBUG 1

#include "eail_test_utils.h"

#define ACTIONS_NUMBER 1

INIT_TEST("EailBubble")

static void
_bubble_clicked(void *data, Evas_Object *obj, void *event_info)
{
   static unsigned char counter = 0;

   if (3 < counter)
     counter = 0;

   elm_bubble_pos_set(obj, counter);

   counter++;
}

static void
_init_bubble(Evas_Object *win)
{
   Evas_Object *bg, *label, *icon, *bubble;

   bg = elm_bg_add(win);
   evas_object_size_hint_weight_set(bg, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
   elm_win_resize_object_add(win, bg);
   evas_object_show(bg);

   label = elm_label_add(win);
   elm_object_text_set(label, "This is the CONTENT of our bubble");
   evas_object_show(label);

   icon = elm_icon_add(win);
   elm_icon_order_lookup_set(icon, ELM_ICON_LOOKUP_THEME_FDO);
   elm_icon_standard_set(icon, "home");
   evas_object_show(icon);

   bubble = elm_bubble_add(win);
   elm_object_part_content_set(bubble, "icon", icon);
   elm_object_part_text_set(bubble, "info", "INFO");
   elm_object_text_set(bubble, "LABEL");
   elm_object_content_set(bubble, label);
   evas_object_smart_callback_add(bubble, "clicked", _bubble_clicked, NULL);
   evas_object_resize(bubble, 300, 100);
   evas_object_show(bubble);
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
   _init_bubble(win);
   evas_object_show(win);
   elm_run();
   elm_shutdown();

   return 0;
}
ELM_MAIN()
