/*
 * Tested interface: AtkObject
 *
 * Tested AtkObject: EailFlip
 *
 * Description: Test AtkObject interface
 *
 * Test input: accessible object representing EailFlip
 *
 * Expected test result: test should return 0 (success)
 */

#include <Elementary.h>
#include <atk/atk.h>

#define DEBUG 1

#include "eail_test_utils.h"

#define ACTIONS_NUMBER 1

INIT_TEST("EailFlip")

static void
_init_flip(Evas_Object *win)
{
   Evas_Object *bg, *image, *icon, *flip;

   bg = elm_bg_add(win);
   elm_win_resize_object_add(win, bg);
   evas_object_size_hint_weight_set(bg, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
   evas_object_show(bg);

   image = elm_image_add(win);
   g_assert(elm_image_file_set(image, "./data/07.jpg", NULL));
   evas_object_size_hint_weight_set(image, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
   elm_win_resize_object_add(win, image);
   evas_object_show(image);

   icon = elm_icon_add(win);
   elm_icon_order_lookup_set(icon, ELM_ICON_LOOKUP_THEME_FDO);
   elm_icon_standard_set(icon, "home");
   elm_win_resize_object_add(win, icon);
   evas_object_show(icon);

   flip = elm_flip_add(win);
   elm_object_part_content_set(flip, "front", image);
   elm_object_part_content_set(flip, "back", icon);
   evas_object_show(flip);

   elm_flip_interaction_set(flip, ELM_FLIP_INTERACTION_PAGE);
   elm_flip_interaction_direction_enabled_set(flip, ELM_FLIP_DIRECTION_UP, EINA_TRUE);
   elm_flip_interaction_direction_enabled_set(flip, ELM_FLIP_DIRECTION_DOWN, EINA_TRUE);
   elm_flip_interaction_direction_enabled_set(flip, ELM_FLIP_DIRECTION_LEFT, EINA_TRUE);
   elm_flip_interaction_direction_enabled_set(flip, ELM_FLIP_DIRECTION_RIGHT, EINA_TRUE);
   elm_flip_interaction_direction_hitsize_set(flip, ELM_FLIP_DIRECTION_UP, 1);
   elm_flip_interaction_direction_hitsize_set(flip, ELM_FLIP_DIRECTION_DOWN, 1);
   elm_flip_interaction_direction_hitsize_set(flip, ELM_FLIP_DIRECTION_LEFT, 1);
   elm_flip_interaction_direction_hitsize_set(flip, ELM_FLIP_DIRECTION_RIGHT, 1);
}

static void
_do_test(AtkObject *obj)
{
   const char *type_name_child;
   AtkObject *xobj;
   static int count = 0;

   g_assert(ATK_IS_OBJECT(obj));

   atk_object_set_description(obj, "test");
   g_assert_cmpstr(atk_object_get_description(obj), ==, "test");

   atk_object_set_name(obj, "test name");
   g_assert_cmpstr(atk_object_get_name(obj), ==, "test name");

   g_assert(atk_object_get_role(obj) == ATK_ROLE_PAGE_TAB_LIST);

   g_assert(1 == atk_object_get_n_accessible_children(obj));

   xobj = atk_object_ref_accessible_child(obj, 0);
   g_assert(NULL != xobj);
   type_name_child = g_type_name(G_TYPE_FROM_INSTANCE(xobj));

   if (count % 2)
     g_assert_cmpstr(type_name_child, ==, "EailIcon");
   else
     g_assert_cmpstr(type_name_child, ==, "EailImage");

   count++;

   /* flip object does not support focus */
   eailu_test_atk_focus(obj, FALSE);

   eailu_test_code_called = 1;
}

EAPI_MAIN int
elm_main(int argc, char **argv)
{
   Evas_Object *win;

   win = eailu_create_test_window_with_glib_init(_on_done, _on_focus_in);
   g_assert(win);
   _init_flip(win);
   evas_object_show(win);
   elm_run();
   elm_shutdown();

   return 0;
}
ELM_MAIN()
