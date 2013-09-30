/*
 * Tested interface: AtkObject
 *
 * Tested AtkObject: EailBubble
 *
 * Description: Test AtkObject interface
 *
 * Test input: accessible object representing EailBubble
 *
 * Expected test result: test should return 0 (success)
 */

#include <Elementary.h>
#include <atk/atk.h>

#include "eail_test_utils.h"

INIT_TEST("EailBubble")

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
   evas_object_resize(bubble, 300, 100);
   evas_object_show(bubble);
}

static void
_do_test(AtkObject *obj)
{
   const char *type_name = g_type_name(G_TYPE_FROM_INSTANCE(obj));
   const char *type_name_child;
   AtkObject *xobj;

   g_assert(ATK_IS_OBJECT(obj));

   atk_object_set_description(obj, "test");
   g_assert_cmpstr(atk_object_get_description(obj), ==, "test");
   atk_object_set_name(obj, "test name");
   g_assert_cmpstr(atk_object_get_name(obj), ==, "test name");

   g_assert(atk_object_get_role(obj) == ATK_ROLE_FILLER);

   g_assert_cmpstr(type_name, ==, "EailBubble");

   // test children
   g_assert(2 == atk_object_get_n_accessible_children(obj));

   for (int count = 0; 2 > count; count++)
     {
        xobj = atk_object_ref_accessible_child(obj, count);
        g_assert(xobj);
        type_name_child = g_type_name(G_TYPE_FROM_INSTANCE(xobj));
        _printf(" child num: %d, type_name: %s\n", count, type_name_child);
        g_assert(!strcmp(type_name_child, "EailIcon") || !strcmp(type_name_child, "EailLabel"));
     }

   /* bubble object does not support focus */
   eailu_test_atk_focus(obj, FALSE);

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
