/*
 * Tested interface: AtkEditableText
 *
 * Tested AtkObject: EailMultibuttonentry
 *
 * Description: Test AtkEditableText interface
 *
 * Test input: accessible object representing EailMultibuttonentry
 *
 * Expected test result: test should return 0 (success)
 */

#include <Elementary.h>
#include <atk/atk.h>

#include "eail_test_utils.h"

#define ENTRY_TEXT "Trololo lololololo olololo"

INIT_TEST("EailMultibuttonentry")

static void
_do_test(AtkObject *obj)
{
   gchar *text = NULL;
   gint start_offset;
   gint end_offset;

   g_assert(ATK_IS_EDITABLE_TEXT(obj));

   atk_editable_text_set_text_contents(ATK_EDITABLE_TEXT(obj), ENTRY_TEXT);

   start_offset = 0;
   end_offset = -1;
   text = atk_text_get_text(ATK_TEXT(obj), start_offset, end_offset);
   g_assert_cmpstr(text, ==, ENTRY_TEXT);
   g_free(text);

   start_offset = 0;
   end_offset = 7;
   atk_editable_text_delete_text(ATK_EDITABLE_TEXT(obj), start_offset, end_offset);
   text = atk_text_get_text(ATK_TEXT(obj), 0, -1);
   g_assert_cmpstr(text, ==, " lololololo olololo");
   g_free(text);

   atk_editable_text_insert_text(ATK_EDITABLE_TEXT(obj), "Trololo", 7,
                                 &start_offset);
   text = atk_text_get_text(ATK_TEXT(obj), 0, -1);
   g_assert(start_offset == 7);
   g_assert_cmpstr(text, ==, ENTRY_TEXT);
   g_free(text);

   start_offset = 8;
   end_offset = 18;
   atk_editable_text_copy_text(ATK_EDITABLE_TEXT(obj), start_offset, end_offset);
   atk_editable_text_set_text_contents(ATK_EDITABLE_TEXT(obj), "123");
   atk_editable_text_paste_text(ATK_EDITABLE_TEXT(obj), 3);

   start_offset = 0;
   end_offset = -1;
   text = atk_text_get_text(ATK_TEXT(obj), start_offset, end_offset);
   g_assert_cmpstr(text, ==, "123lololololo");
   g_free(text);

   start_offset = 0;
   end_offset = 3;
   atk_editable_text_cut_text(ATK_EDITABLE_TEXT(obj), start_offset, end_offset);
   text = atk_text_get_text(ATK_TEXT(obj), start_offset, end_offset);
   g_assert_cmpstr(text, ==, "lol");
   g_free(text);

   start_offset = 0;
   end_offset = -1;
   atk_editable_text_paste_text(ATK_EDITABLE_TEXT(obj), 3);
   text = atk_text_get_text(ATK_TEXT(obj), start_offset, end_offset);
   g_assert_cmpstr(text, ==, "lol123olololo");
   g_free(text);

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
   evas_object_size_hint_weight_set(mb_entry, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
   evas_object_size_hint_align_set(mb_entry, EVAS_HINT_FILL, EVAS_HINT_FILL);
   elm_object_content_set(scroller, mb_entry);
   elm_multibuttonentry_editable_set(mb_entry, EINA_TRUE);
   evas_object_show(mb_entry);

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
