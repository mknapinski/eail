/*
 * Tested interface: AtkObject
 *
 * Tested AtkObject: EailConformant
 *
 * Description: Test AtkObject interface
 *
 * Test input: accessible object representing EailConformant
 *
 * Expected test result: test should return 0 (success)
 */

#include <Elementary.h>
#include <atk/atk.h>

#include "eail_test_utils.h"

INIT_TEST("EailConformant")

static void
_do_test(AtkObject *obj)
{
   int child_count = 0;
   AtkRole role = atk_object_get_role(obj);
   g_assert(role == ATK_ROLE_FILLER);

   atk_object_set_description(obj, "test");
   g_assert_cmpstr(atk_object_get_description(obj), ==, "test");

   atk_object_set_name(obj, "test name");
   g_assert_cmpstr(atk_object_get_name(obj), ==, "test name");

   child_count = atk_object_get_n_accessible_children(obj);
   g_assert(child_count == 1);

   for (int i = 0; i < child_count; i++)
     {
        AtkObject *child = atk_object_ref_accessible_child(obj, i);
        g_assert(ATK_IS_OBJECT(child));
        role = atk_object_get_role(child);
        g_assert(role == ATK_ROLE_FILLER);
        g_object_unref(child);
     }

   eailu_test_atk_focus(obj, TRUE);

   eailu_test_code_called = 1;
}

EAPI_MAIN int
elm_main(int argc, char **argv)
{
   Evas_Object *win, *bg, *conform, *btn, *bx, *en;

   win = eailu_create_test_window_with_glib_init(_on_done, _on_focus_in);
   elm_win_conformant_set(win, EINA_TRUE);

   bg = elm_bg_add(win);
   elm_win_resize_object_add(win, bg);
   evas_object_size_hint_weight_set(bg, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
   evas_object_show(bg);

   conform = elm_conformant_add(win);
   elm_win_resize_object_add(win, conform);
   evas_object_size_hint_weight_set(conform, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
   evas_object_show(conform);

   bx = elm_box_add(win);
   evas_object_size_hint_weight_set(bx, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
   evas_object_size_hint_align_set(bx, EVAS_HINT_FILL, EVAS_HINT_FILL);

   btn = elm_button_add(win);
   elm_object_text_set(btn, "Test Conformant");
   evas_object_size_hint_weight_set(btn, EVAS_HINT_EXPAND, 0);
   evas_object_size_hint_align_set(btn, EVAS_HINT_FILL, 0);
   elm_box_pack_end(bx, btn);
   evas_object_show(btn);

   en = elm_entry_add(win);
   elm_entry_scrollable_set(en, EINA_TRUE);
   elm_object_text_set(en,
                       "This is a multi-line entry at the bottom<br>"
                       "This can contain more than 1 line of text and be "
                       "scrolled around to allow for entering of lots of "
                       "content. It is also to test to see that autoscroll "
                       "moves to the right part of a larger multi-line "
                       "text entry that is inside of a scroller than can be "
                       "scrolled around, thus changing the expected position "
                       "as well as cursor changes updating auto-scroll when "
                       "it is enabled.");

   evas_object_size_hint_weight_set(en, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
   evas_object_size_hint_align_set(en, EVAS_HINT_FILL, EVAS_HINT_FILL);
   evas_object_show(en);
   elm_box_pack_end(bx, en);

   btn = elm_button_add(win);
   elm_object_text_set(btn, "Test Conformant");
   evas_object_size_hint_weight_set(btn, EVAS_HINT_EXPAND, 0);
   evas_object_size_hint_align_set(btn, EVAS_HINT_FILL, 0);
   elm_box_pack_end(bx, btn);
   evas_object_show(btn);

   elm_object_content_set(conform, bx);
   evas_object_show(bx);

   evas_object_resize(win, 240, 480);
   evas_object_show(win);

   elm_run();
   elm_shutdown();

   return 0;
}
ELM_MAIN()
