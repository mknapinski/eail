/*
 * Tested interface: AtkText
 *
 * Tested AtkObject: EailLabel
 *
 * Description: Test AtkObject interface
 *
 * Test input: accessible object representing EailLabel
 *
 * Expected test result: test should return 0 (success)
 */

#include <Elementary.h>

#include "eail_test_utils.h"

INIT_TEST("EailLabel")

static void
_init_label(Evas_Object *win)
{
   Evas_Object *bg, *label;

   bg = elm_bg_add(win);
   evas_object_size_hint_weight_set(bg, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
   elm_win_resize_object_add(win, bg);
   evas_object_show(bg);

   label = elm_label_add(win);
   elm_object_text_set(label, "Lorem ipsum dolor sit amet, consectetur adipisicing elit,"
                       "sed do eiusmod tempor incididunt ut labore et dolore magna aliqua."
                       "Ut enim ad minim veniam, quis nostrud exercitation ullamco laboris"
                       "nisi ut aliquip ex ea commodo consequat. Duis aute irure dolor in"
                       "reprehenderit in voluptate velit esse cillum dolore eu fugiat nulla"
                       "pariatur. Excepteur sint occaecat cupidatat non proident, sunt"
                       "in culpa qui officia deserunt mollit anim id est laborum.");
   elm_label_line_wrap_set(label, ELM_WRAP_WORD);
   evas_object_resize(label, 200, 170);
   evas_object_move(label, 0, 0);
   evas_object_show(label);

   evas_object_resize(win, 200, 320);
}

static void
_do_test(AtkObject *obj)
{
   int start_offset, end_offset;
   AtkAttributeSet *elem = NULL, *at_set = NULL;
   AtkAttribute *attr = NULL;
   gboolean wrap_exist = FALSE, editable_exist = FALSE;

   g_assert(ATK_IS_TEXT(obj));

   gchar *text = atk_text_get_text(ATK_TEXT(obj), 6, 11);
   g_assert_cmpstr(text, ==, "ipsum");
   g_free(text);

   g_assert(atk_text_get_character_at_offset(ATK_TEXT(obj), 12) == 'd');

   g_assert(atk_text_get_character_count(ATK_TEXT(obj)) == 440);

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

   eailu_test_code_called = 1;
}

EAPI_MAIN int
elm_main(int argc, char **argv)
{
   Evas_Object *win;

   win = eailu_create_test_window_with_glib_init(_on_done, _on_focus_in);
   g_assert(win);
   _init_label(win);

   evas_object_show(win);
   elm_run();
   elm_shutdown();

   return 0;
}
ELM_MAIN()


