/*
 * Tested interface: AtkObject
 *
 * Tested AtkObject: EailLayout
 *
 * Description: Test AtkObject interface
 *
 * Test input: accessible object representing EailLayout
 *
 * Expected test result: test should return 0 (success)
 */

#include <Elementary.h>
#include <atk/atk.h>

#include "eail_test_utils.h"

INIT_TEST("EailLayout")

static void
_do_test(AtkObject *object)
{
   int child_count;

   atk_object_set_description(object, "test");
   g_assert_cmpstr(atk_object_get_description(object), ==, "test");

   atk_object_set_name(object, "test name");
   g_assert_cmpstr(atk_object_get_name(object), ==, "test name");

   eailu_test_code_called++;
   child_count = atk_object_get_n_accessible_children(object);
   if (eailu_test_code_called == 1)
     g_assert(2 == child_count);
   else
     g_assert(3 == child_count);

   for(int i =0; i < child_count; i++)
     {
        AtkObject *child = atk_object_ref_accessible_child(object, i);
        gboolean success = FALSE;
        if (eailu_test_code_called == 1)
          success = eailu_is_object_with_role(child, ATK_ROLE_ICON);
        else
          success = eailu_is_object_with_role(child, ATK_ROLE_PUSH_BUTTON);
        g_object_unref(child);
        g_assert(success);
     }

   g_assert(eailu_is_object_with_role(object, ATK_ROLE_FILLER));
   g_assert(2 >= eailu_test_code_called);
}


static void
_init_layout(Evas_Object *win)
{
   Evas_Object *box, *ly, *bt;
   char buf[PATH_MAX];

   box = elm_box_add(win);
   evas_object_size_hint_weight_set(box, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
   elm_win_resize_object_add(win, box);
   evas_object_show(box);

   ly = elm_layout_add(win);

   if (!elm_layout_theme_set(
           ly, "layout", "application", "titlebar"))
     fprintf(stderr, "Failed to set layout");

   elm_object_part_text_set(ly, "elm.text", "Some title");
   evas_object_size_hint_weight_set(ly, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
   evas_object_size_hint_align_set(ly, EVAS_HINT_FILL, EVAS_HINT_FILL);
   elm_box_pack_end(box, ly);
   evas_object_show(ly);

   bt = elm_icon_add(win);
   elm_icon_standard_set(bt, "chat");
   evas_object_size_hint_min_set(bt, 20, 20);
   elm_layout_icon_set(ly, bt);

   bt = elm_icon_add(win);
   elm_icon_standard_set(bt, "close");
   evas_object_size_hint_min_set(bt, 20, 20);
   elm_layout_end_set(ly, bt);

   ly = elm_layout_add(win);
   g_snprintf(buf, sizeof(buf), "%s/test.edj","./data/");
   elm_layout_file_set(ly, buf, "layout");
   evas_object_size_hint_weight_set(ly, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
   elm_box_pack_end(box, ly);
   evas_object_show(ly);

   bt = elm_button_add(win);
   elm_object_text_set(bt, "Button 1");
   elm_object_part_content_set(ly, "element1", bt);
   evas_object_show(bt);

   bt = elm_button_add(win);
   elm_object_text_set(bt, "Button 2");
   elm_object_part_content_set(ly, "element2", bt);
   evas_object_show(bt);

   bt = elm_button_add(win);
   elm_object_text_set(bt, "Button 3");
   elm_object_part_content_set(ly, "element3", bt);
   evas_object_show(bt);

   evas_object_show(win);
}

EAPI_MAIN int
elm_main(int argc, char **argv)
{
   Evas_Object *win;

   win = eailu_create_test_window_with_glib_init(_on_done, _on_focus_in);
   _init_layout(win);
   elm_run();
   elm_shutdown();

   return 0;
}
ELM_MAIN()


