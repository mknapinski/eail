/*
 * Tested interface: AtkText
 *
 * Tested AtkObject: EailMultibuttonentry
 *
 * Description: Test AtkText interface
 *
 * Test input: accessible object representing EailMultibuttonentry
 *
 * Expected test result: test should return 0 (success)
 */

#include <Elementary.h>
#include <atk/atk.h>

#include "eail_test_utils.h"

#define EAIL_TYPE_FOR_MBE "EailMultibuttonentry"
#define EAIL_TEST_INPUT_TEXT "Input text"
#define EAIL_TEST_CHANGED_TEXT "custom text"

INIT_TEST("EailMultibuttonentry")

static void
_do_test(AtkObject *obj)
{
   g_assert(ATK_IS_TEXT(obj));

   atk_editable_text_set_text_contents(ATK_EDITABLE_TEXT(obj),
                                       EAIL_TEST_INPUT_TEXT);
   g_assert(atk_text_get_character_count(ATK_TEXT(obj)) == 10);
   g_assert(atk_text_get_character_at_offset(ATK_TEXT(obj), 3) == 'u');

   eailu_test_code_called = 1;
}

static void
_init_multibutton_entry(Evas_Object *win)
{
   Evas_Object *scroller, *bg, *box, *mb_entry;

   elm_policy_set(ELM_POLICY_QUIT, ELM_POLICY_QUIT_LAST_WINDOW_CLOSED);
   elm_win_autodel_set(win, EINA_TRUE);

   bg = elm_bg_add(win);
   evas_object_size_hint_weight_set(bg, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
   elm_win_resize_object_add(win, bg);
   evas_object_show(bg);

   box = elm_box_add(win);
   evas_object_size_hint_weight_set(box, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
   elm_win_resize_object_add(win, box);
   evas_object_show(box);

   scroller = elm_scroller_add(win);
   elm_scroller_bounce_set(scroller, EINA_FALSE, EINA_TRUE);
   elm_scroller_policy_set
       (scroller, ELM_SCROLLER_POLICY_OFF,ELM_SCROLLER_POLICY_AUTO);
   evas_object_show(scroller);

   mb_entry = elm_multibuttonentry_add(win);
   elm_object_part_text_set(mb_entry, "guide", "Tap to add recipient");
   elm_object_text_set(mb_entry, EAIL_TEST_INPUT_TEXT);
   evas_object_size_hint_weight_set(mb_entry, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
   evas_object_size_hint_align_set(mb_entry, EVAS_HINT_FILL, EVAS_HINT_FILL);
   elm_object_content_set(scroller, mb_entry);
   evas_object_show(mb_entry);

   elm_multibuttonentry_item_append(mb_entry, "first", NULL, NULL);
   elm_multibuttonentry_item_append(mb_entry, "second", NULL, NULL);
   elm_multibuttonentry_item_append(mb_entry, "third", NULL, NULL);
   elm_multibuttonentry_item_append(mb_entry, "fourth", NULL, NULL);

   elm_object_focus_set(mb_entry, EINA_TRUE);
   elm_box_pack_end(box, mb_entry);

   evas_object_resize(win, 130, 180);
}

EAPI_MAIN int
elm_main(int argc, char **argv)
{
   Evas_Object *win;

   win = eailu_create_test_window_with_glib_init(_on_done, _on_focus_in);
   _init_multibutton_entry(win);

   evas_object_show(win);
   elm_run();
   elm_shutdown();

   /* exit code */
   return 0;
}
ELM_MAIN()
