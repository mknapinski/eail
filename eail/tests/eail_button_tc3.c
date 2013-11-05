/*
 * Tested interface: AtkObject
 *
 * Tested AtkObject: EailButton
 *
 * Description: Test whether EailIcon is only child of EailButton
 *
 * Test input: accessible object representing EailButton
 *
 * Expected test result: test should return 0 (success)
 */

#include <Elementary.h>
#include <atk/atk.h>

#include "eail_test_utils.h"

#define BUTTON_TEXT2   "button2"

#define ICON_SIZE 56

INIT_TEST("EailButton")

static void
_do_test(AtkObject *obj)
{
   const char *type_name_child;

   AtkObject *xobj;

   _printf("Button test: %s\n", BUTTON_TEXT2);
   // test children
   g_assert(1 == atk_object_get_n_accessible_children(obj));

   xobj = atk_object_ref_accessible_child(obj, 0);
   g_assert(NULL != xobj);
   type_name_child = g_type_name(G_TYPE_FROM_INSTANCE(xobj));
   _printf("type_name_child: %s\n", type_name_child);
   g_assert_cmpstr(type_name_child, ==, "EailIcon");

   eailu_test_code_called = 1;
}

static void
_init_button(Evas_Object *win)
{
   Evas_Object *bg, *icon, *button2;

   bg = elm_bg_add(win);
   evas_object_size_hint_weight_set(bg, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
   elm_win_resize_object_add(win, bg);
   evas_object_show(bg);

   icon = elm_icon_add(win);
   elm_icon_order_lookup_set(icon, ELM_ICON_LOOKUP_THEME_FDO);
   elm_icon_standard_set(icon, "home");
   evas_object_show(icon);

   button2 = elm_button_add(win);
   elm_object_text_set(button2, BUTTON_TEXT2);
   elm_object_part_content_set(button2, "icon", icon);
   evas_object_resize(button2, 140, 60);
   evas_object_show(button2);
}

EAPI_MAIN int
elm_main(int argc, char **argv)
{
   Evas_Object *win;

   win = eailu_create_test_window_with_glib_init(_on_done, _on_focus_in);
   _init_button(win);
   evas_object_show(win);
   elm_run();
   elm_shutdown();

   return 0;
}
ELM_MAIN()
