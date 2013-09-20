/*
 * Tested interface: AtkObject
 *
 * Tested AtkObject: EailTable
 *
 * Description: Test AtkObject interface
 *
 * Test input: accessible object representing EailTable
 *
 * Expected test result: test should return 0 (success)
 */

#include <Elementary.h>
#include <atk/atk.h>

#include "eail_test_utils.h"

INIT_TEST("EailTable")

static void
_init_table(Evas_Object *win)
{
   Evas_Object *bg, *label, *table, *hover, *rect, *rect2, *rect3, *bt,
               *spinner, *slider, *check, *icon, *en, *image, *video,
               *global_gui_list;
   const char * const filename = "./data/whiterabbit01.jpg";

   bg = elm_bg_add(win);
   evas_object_size_hint_weight_set(bg, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
   elm_win_resize_object_add(win, bg);
   evas_object_show(bg);

   table = elm_table_add(win);
   elm_win_resize_object_add(win, table);
   elm_table_padding_set(table, 0,0 );
   elm_table_homogeneous_set(table, EVAS_OBJECT_TABLE_HOMOGENEOUS_NONE);
   evas_object_size_hint_weight_set(table, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
   evas_object_show(table);

   label = elm_label_add(win);
   elm_object_text_set(label, "label 0");
   evas_object_show(label);
   elm_table_pack(table, label, 0, 0, 1, 1);

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
   elm_table_pack(table, bt, 1, 0, 1, 1);

   hover = elm_hover_add(win);
   elm_hover_parent_set(hover, win);
   elm_hover_target_set(hover, bt);
   elm_object_style_set(hover, "popout");
   elm_object_part_content_set(hover, "left", rect);
   elm_object_part_content_set(hover, "top", rect2);
   elm_object_part_content_set(hover, "right", rect3);

   elm_table_pack(table, hover, 2, 0, 1, 1);
   evas_object_move(win, 500, 300);

   spinner = elm_spinner_add(win);
   elm_spinner_label_format_set(spinner, "%1.1f units");
   evas_object_show(spinner);
   elm_table_pack(table, spinner, 3, 0, 1, 1);

   slider = elm_slider_add(win);
   elm_slider_min_max_set(slider, 0, 20);
   elm_slider_unit_format_set(slider, "%2.1f");
   evas_object_show(slider);
   elm_table_pack(table, slider, 4, 0, 1, 1);

   check = elm_check_add(win);
   evas_object_show(check);
   elm_table_pack(table, check, 5, 0, 1, 1);

   en = elm_entry_add(win);
   elm_entry_autosave_set(en, EINA_FALSE);
   elm_entry_line_wrap_set(en, ELM_WRAP_WORD);
   evas_object_show(en);
   elm_table_pack(table, en, 6, 0, 1, 1);

   icon = elm_icon_add(win);
   elm_icon_order_lookup_set(icon, ELM_ICON_LOOKUP_THEME_FDO);
   elm_icon_standard_set(icon, "home");
   evas_object_resize(icon, 32, 32);
   evas_object_show(icon);
   elm_table_pack(table, icon, 7, 0, 1, 1);

   image = elm_image_add(win);
   g_assert(elm_image_file_set(image, filename, NULL));
   elm_image_no_scale_set(image, EINA_TRUE);
   elm_image_resizable_set(image, EINA_FALSE, EINA_TRUE);
   elm_image_smooth_set(image, EINA_FALSE);
   elm_image_orient_set(image, ELM_IMAGE_FLIP_HORIZONTAL);
   elm_image_aspect_fixed_set(image, EINA_TRUE);
   elm_image_fill_outside_set(image, EINA_TRUE);
   elm_image_editable_set(image, EINA_TRUE);
   evas_object_size_hint_weight_set(image, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
   evas_object_show(image);
   elm_table_pack(table, image, 8, 0, 1, 1);

   video = elm_video_add(win);
   evas_object_size_hint_align_set(video, EVAS_HINT_FILL, EVAS_HINT_FILL);
   elm_table_pack(table, video, 9, 0, 1, 1);
   evas_object_show(video);
   evas_object_resize(video, 200, 200);
   elm_video_file_set(video, "./data/softboy.avi");
   elm_video_play(video);

   global_gui_list = elm_list_add(win);
   elm_list_multi_select_set(global_gui_list, EINA_TRUE);
   evas_object_size_hint_weight_set(global_gui_list, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
   evas_object_size_hint_align_set(global_gui_list, EVAS_HINT_FILL, EVAS_HINT_FILL);
   evas_object_show(global_gui_list);

   elm_list_item_append(global_gui_list, "List Label1", NULL, NULL, NULL, NULL);
   elm_list_item_append(global_gui_list, "List Label2", NULL, NULL, NULL, NULL);
   elm_list_item_append(global_gui_list, "List Label3", NULL, NULL, NULL, NULL);
   elm_list_item_append(global_gui_list, "List Label4", NULL, NULL, NULL, NULL);

   elm_table_pack(table, global_gui_list, 0, 1, 6, 1);
}

static void
_do_test(AtkObject *obj)
{
   AtkObject *child;
   int index = 0;
   int i;

   atk_object_set_description(obj, "test");
   g_assert_cmpstr(atk_object_get_description(obj), ==, "test");

   atk_object_set_name(obj, "test name");
   g_assert_cmpstr(atk_object_get_name(obj), ==, "test name");

   for (i = 0; i < atk_object_get_n_accessible_children(obj); ++i)
     {
        child = atk_object_ref_accessible_child(obj, i);
        g_assert(child);

        index = atk_object_get_index_in_parent(child);
        g_assert(index == i);

        g_object_unref(child);
     }

   g_assert(ATK_IS_COMPONENT(obj));
   g_assert(atk_component_set_position(ATK_COMPONENT(obj), -10, -10,
                                       ATK_XY_SCREEN) == FALSE);
   g_assert(atk_component_set_size(ATK_COMPONENT(obj), -5, -5) == FALSE);

   eailu_test_code_called = 1;
}

EAPI_MAIN int
elm_main(int argc, char **argv)
{
   Evas_Object *win;

   win = eailu_create_test_window_with_glib_init(_on_done, _on_focus_in);
   g_assert(win);
   _init_table(win);
   evas_object_show(win);
   elm_run();
   elm_shutdown();

   return 0;
}
ELM_MAIN()
