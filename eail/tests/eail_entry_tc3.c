/*
 * Tested interface: AtkEditableText
 *
 * Tested AtkObject: EailEntry
 *
 * Description: Test AtkEditableText interface
 *
 * Test input: accessible object representing EailEntry
 *
 * Expected test result: test should return 0 (success)
 */

#include <Elementary.h>
#include <atk/atk.h>

#include "eail_test_utils.h"

#define ENTRY_TEXT "Trololo lololololo olololo"
#define ENTRY_FIRST "first"
#define ENTRY_SECOND "second"

INIT_TEST("EailEntry")

void
_test_state(AtkObject *obj, AtkStateType state_type)
{
   AtkStateSet *state_set;

   state_set = atk_object_ref_state_set(obj);
   g_assert(atk_state_set_contains_state(state_set, state_type));
   g_object_unref(state_set);
}

static void
_do_test(AtkObject *obj)
{
   gchar *text;
   int start_offset, end_offset;

   g_assert(ATK_IS_EDITABLE_TEXT(obj));

   text = atk_text_get_text(ATK_TEXT(obj), 0, -1);
   if (!g_strcmp0(text, ENTRY_FIRST))
     {
        g_free(text);

        text = g_strdup(ENTRY_TEXT);
        atk_editable_text_set_text_contents(ATK_EDITABLE_TEXT(obj), text);
        g_free(text);

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

        atk_editable_text_cut_text(ATK_EDITABLE_TEXT(obj), 3, 6);
        text = atk_text_get_text(ATK_TEXT(obj), 0, -1);
        g_assert_cmpstr(text, ==, "lololololo");
        g_free(text);
     }
   else if (!g_strcmp0(text, ENTRY_SECOND))
     {
        g_free(text);
        atk_editable_text_paste_text(ATK_EDITABLE_TEXT(obj), 0);
        text = atk_text_get_text(ATK_TEXT(obj), 0, -1);
        g_assert_cmpstr(text, ==, "123second");
        g_free(text);

        eailu_test_code_called = 1;
     }
}

static void
_init_entry(Evas_Object *win)
{
   Evas_Object *box, *tb, *bg, *en, *en2;

   elm_policy_set(ELM_POLICY_QUIT, ELM_POLICY_QUIT_LAST_WINDOW_CLOSED);
   elm_win_autodel_set(win, EINA_TRUE);

   bg = elm_bg_add(win);
   elm_win_resize_object_add(win, bg);
   evas_object_size_hint_weight_set(bg, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
   evas_object_show(bg);

   box = elm_box_add(win);
   evas_object_size_hint_weight_set(box, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
   elm_win_resize_object_add(win, box);
   evas_object_show(box);

   tb = elm_box_add(win);
   elm_box_horizontal_set(tb, EINA_TRUE);
   evas_object_size_hint_weight_set(tb, EVAS_HINT_EXPAND, 0.0);
   evas_object_size_hint_weight_set(tb, EVAS_HINT_FILL, EVAS_HINT_FILL);
   elm_box_pack_end(box, tb);
   evas_object_show(tb);

   en = elm_entry_add(win);
   elm_entry_autosave_set(en, EINA_FALSE);
   elm_entry_line_wrap_set(en, ELM_WRAP_WORD);
   elm_entry_entry_set(en, ENTRY_FIRST);
   evas_object_size_hint_weight_set(en, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
   evas_object_size_hint_align_set(en, EVAS_HINT_FILL, EVAS_HINT_FILL);
   elm_box_pack_end(box, en);
   evas_object_show(en);

   en2 = elm_entry_add(win);
   elm_entry_autosave_set(en2, EINA_FALSE);
   elm_entry_line_wrap_set(en2, ELM_WRAP_WORD);
   elm_entry_entry_set(en2, ENTRY_SECOND);
   evas_object_size_hint_weight_set(en2, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
   evas_object_size_hint_align_set(en2, EVAS_HINT_FILL, EVAS_HINT_FILL);
   elm_box_pack_end(box, en2);
   evas_object_show(en2);

   evas_object_resize(win, 200, 300);
}

EAPI_MAIN int
elm_main(int argc, char *argv[])
{
   Evas_Object *win;

   win = eailu_create_test_window_with_glib_init(_on_done, _on_focus_in);
   _init_entry(win);
   evas_object_show(win);
   elm_run();
   elm_shutdown();

   return 0;
}
ELM_MAIN()
