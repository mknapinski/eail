/*
 * Tested interface: AtkText
 *
 * Tested AtkObject: EailEntry
 *
 * Description: Test AtkText interface
 *
 * Test input: accessible object representing EailEntry
 *
 * Expected test result: test should return 0 (success)
 */

#include <Elementary.h>
#include <atk/atk.h>

#include "eail_test_utils.h"

#define TEXT_LEN 475

INIT_TEST("EailEntry")

static void
_do_test(AtkObject *obj)
{
   gchar *text;
   int start_offset, end_offset;
   AtkAttributeSet *elem = NULL, *at_set = NULL;
   AtkAttribute *attr = NULL;
   gboolean wrap_exist = FALSE, editable_exist = FALSE;

   g_assert(ATK_IS_TEXT(obj));

   text = atk_text_get_text(ATK_TEXT(obj), 6, 11);
   g_assert_cmpstr(text, ==, "ipsum");
   g_free(text);

   g_assert(atk_text_get_character_at_offset(ATK_TEXT(obj), 12) == 'd');
   g_assert(atk_text_get_character_count(ATK_TEXT(obj)) == TEXT_LEN);
   g_assert(atk_text_set_caret_offset(ATK_TEXT(obj), 13));
   g_assert(atk_text_get_caret_offset(ATK_TEXT(obj)) == 13);

   start_offset = 0;
   end_offset = 5;
   g_assert(atk_text_get_n_selections(ATK_TEXT(obj)) == 0);
   g_assert(atk_text_set_selection(ATK_TEXT(obj), 0, start_offset, end_offset));
   text = atk_text_get_selection(ATK_TEXT(obj), 0, &start_offset,  &end_offset);
   g_assert_cmpstr(text, ==, "Lorem");
   g_assert(start_offset == 0);
   g_assert(end_offset == 5);

   g_assert(atk_text_get_n_selections(ATK_TEXT(obj)) == 1);
   g_assert(atk_text_remove_selection(ATK_TEXT(obj), 0) == TRUE);
   g_assert(atk_text_get_n_selections(ATK_TEXT(obj)) == 0);
   g_assert(atk_text_add_selection(ATK_TEXT(obj), start_offset, end_offset));
   g_assert(atk_text_get_n_selections(ATK_TEXT(obj)) == 1);
   g_free(text);

   at_set = atk_text_get_run_attributes
       (ATK_TEXT(obj), 0, &start_offset, &end_offset);
   g_assert(at_set);

   for (elem = at_set; elem; elem = elem->next)
     {
        attr = (AtkAttribute *)elem->data;
        if (0 == g_strcmp0(attr->name, atk_text_attribute_get_name
                           (ATK_TEXT_ATTR_WRAP_MODE)))
          wrap_exist = TRUE;
        else if (0 == g_strcmp0(attr->name, atk_text_attribute_get_name
                                (ATK_TEXT_ATTR_EDITABLE)))
          editable_exist = TRUE;
     }

   g_assert(0 == start_offset);
   g_assert(atk_text_get_character_count(ATK_TEXT(obj)) == end_offset);

   g_assert(wrap_exist);
   g_assert(editable_exist);

   /* out of bounds offset in run attributes test*/
   at_set = atk_text_get_run_attributes
       (ATK_TEXT(obj), atk_text_get_character_count
        (ATK_TEXT(obj)), &start_offset, &end_offset);
   g_assert(NULL == at_set);
   g_assert(-1 == start_offset);
   g_assert(-1 == end_offset);

   text = atk_text_get_text_at_offset(ATK_TEXT(obj), 6,
                                      ATK_TEXT_BOUNDARY_CHAR,
                                      &start_offset,
                                      &end_offset);
   g_assert_cmpstr(text, ==, "i");
   g_free(text);
   g_assert(start_offset == 6);
   g_assert(end_offset == 7);

   text = atk_text_get_text_at_offset(ATK_TEXT(obj), 2,
                                      ATK_TEXT_BOUNDARY_WORD_START,
                                      &start_offset,
                                      &end_offset);
   g_assert_cmpstr(text, ==, "Lorem ");
   g_free(text);

   text = atk_text_get_text_at_offset(ATK_TEXT(obj), 11,
                                      ATK_TEXT_BOUNDARY_WORD_START,
                                      &start_offset,
                                      &end_offset);
   g_assert_cmpstr(text, ==, "ipsum ");
   g_free(text);

   text = atk_text_get_text_at_offset(ATK_TEXT(obj), 11,
                                      ATK_TEXT_BOUNDARY_WORD_END,
                                      &start_offset,
                                      &end_offset);
   g_assert_cmpstr(text, ==, " dolor");
   g_free(text);

   text = atk_text_get_text_at_offset(ATK_TEXT(obj), 10,
                                      ATK_TEXT_BOUNDARY_WORD_END,
                                      &start_offset,
                                      &end_offset);
   g_assert_cmpstr(text, ==, " ipsum");
   g_free(text);

   text = atk_text_get_text_after_offset(ATK_TEXT(obj), 6,
                                         ATK_TEXT_BOUNDARY_CHAR,
                                         &start_offset,
                                         &end_offset);
   g_assert_cmpstr(text, ==, "p");
   g_free(text);
   g_assert(start_offset == 7);
   g_assert(end_offset == 8);

   text = atk_text_get_text_after_offset(ATK_TEXT(obj), 12,
                                         ATK_TEXT_BOUNDARY_WORD_START,
                                         &start_offset,
                                         &end_offset);
   g_assert_cmpstr(text, ==, "sit ");
   g_free(text);

   text = atk_text_get_text_after_offset(ATK_TEXT(obj), 10,
                                         ATK_TEXT_BOUNDARY_WORD_END,
                                         &start_offset,
                                         &end_offset);
   g_assert_cmpstr(text, ==, " dolor");
   g_free(text);

   text = atk_text_get_text_before_offset(ATK_TEXT(obj), 5,
                                         ATK_TEXT_BOUNDARY_CHAR,
                                         &start_offset,
                                         &end_offset);
   g_assert_cmpstr(text, ==, "m");
   g_free(text);
   g_assert(start_offset == 4);
   g_assert(end_offset == 5);

   text = atk_text_get_text_at_offset(ATK_TEXT(obj), 12,
                                         ATK_TEXT_BOUNDARY_LINE_START,
                                         &start_offset,
                                         &end_offset);
   g_assert_cmpstr(text, ==,
                   "Lorem ipsum dolor sit amet, consectetur adipisicing elit,\n");
   g_free(text);

   text = atk_text_get_text_at_offset(ATK_TEXT(obj), 129,
                                         ATK_TEXT_BOUNDARY_LINE_END,
                                         &start_offset,
                                         &end_offset);
   g_assert_cmpstr(
       text, ==, "Ut enim ad minim veniam, quis nostrud exercitation ullamc");
   g_free(text);

   text = atk_text_get_text_after_offset(ATK_TEXT(obj), 2,
                                      ATK_TEXT_BOUNDARY_LINE_START,
                                      &start_offset,
                                      &end_offset);
   g_assert_cmpstr(
       text, ==,
       "sed do eiusmod tempor incididunt ut labore et dolore magna aliqua.\n");
   g_free(text);

   text = atk_text_get_text_after_offset(ATK_TEXT(obj), 70,
                                         ATK_TEXT_BOUNDARY_LINE_END,
                                         &start_offset,
                                         &end_offset);
   g_assert_cmpstr(
       text, ==,
       "Ut enim ad minim veniam, quis nostrud exercitation ullamco laboris\n");
   g_free(text);

   eailu_test_code_called = 1;
}

static void
_init_entry(Evas_Object *win)
{
   Evas_Object *box, *tb, *bg, *en;
   const char *text1 = "Lorem ipsum dolor sit amet, consectetur adipisicing elit,<br>"
       "sed do eiusmod tempor incididunt ut labore et dolore magna aliqua.<br>"
       "Ut enim ad minim veniam, quis nostrud exercitation ullamco laboris<br>"
       "nisi ut aliquip ex ea commodo consequat. Duis aute irure dolor in<br>"
       "reprehenderit in voluptate velit esse cillum dolore eu fugiat nulla<br>"
       "pariatur. Excepteur sint occaecat cupidatat non proident, sunt<br>"
       "in culpa qui officia deserunt mollit anim id est laborum.<br>";

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
   elm_entry_line_wrap_set(en, ELM_WRAP_NONE);
   elm_entry_entry_set(en, text1);
   evas_object_size_hint_weight_set(en, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
   evas_object_size_hint_align_set(en, EVAS_HINT_FILL, EVAS_HINT_FILL);
   elm_box_pack_end(box, en);
   evas_object_show(en);

   evas_object_resize(win, 200, 200);
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
