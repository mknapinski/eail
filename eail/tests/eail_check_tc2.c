/*
 * Tested interface: AtkObject
 *
 * Tested AtkObject: EailCheck
 *
 * Description: Test whether accessible object is successfully registered
 *    in GObject type system and is visible in AtkObject tree.
 *
 * Test input: accessible object representing EailCheck
 *
 * Expected test result: test should return 0 (success)
 */

#include <Elementary.h>
#include <atk/atk.h>

#include "eail_test_utils.h"

#define CHECK_TEXT2 "checkbox2"

INIT_TEST("EailCheck")

static void
_init_check(Evas_Object *win)
{
   Evas_Object *bg, *cb2, *icon;
   Eina_Bool value2;

   bg = elm_bg_add(win);
   evas_object_size_hint_weight_set(bg, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
   elm_win_resize_object_add(win, bg);
   evas_object_show(bg);

   icon = elm_icon_add(win);
   elm_icon_order_lookup_set(icon, ELM_ICON_LOOKUP_THEME_FDO);
   elm_icon_standard_set(icon, "home");
   evas_object_show(icon);

   cb2 = elm_check_add(win);
   elm_object_text_set(cb2, CHECK_TEXT2);
   elm_check_state_pointer_set(cb2, &value2);
   elm_check_state_set(cb2, EINA_TRUE);
   elm_object_part_content_set(cb2, "icon", icon);
   evas_object_move(cb2, 10, 50);
   evas_object_resize(cb2, 200, 30);
   evas_object_show(cb2);
}

static void
_do_test(AtkObject *obj)
{
   const char *name = atk_object_get_name(ATK_OBJECT(obj));
   const char *type_name_child;
   AtkStateSet *state_set;
   AtkObject *xobj;

   g_assert(ATK_IS_OBJECT(obj));

   atk_object_set_description(obj, "test");
   g_assert_cmpstr(atk_object_get_description(obj), ==, "test");

   g_assert(atk_object_get_role(obj) == ATK_ROLE_CHECK_BOX);
   g_assert_cmpstr(name, ==, CHECK_TEXT2);

   state_set = atk_object_ref_state_set(obj);
   g_assert(atk_state_set_contains_state(state_set, ATK_STATE_CHECKED));
   g_object_unref(state_set);

   atk_object_set_name(obj, CHECK_TEXT2"test");
   g_assert_cmpstr(atk_object_get_name(obj), ==, CHECK_TEXT2"test");

   // test children
   g_assert(1 == atk_object_get_n_accessible_children(obj));

   xobj = atk_object_ref_accessible_child(obj, 0);
   g_assert(NULL != xobj);
   type_name_child = g_type_name(G_TYPE_FROM_INSTANCE(xobj));
   _printf("type_name_child: %s\n", type_name_child);
   g_assert_cmpstr(type_name_child, ==, "EailIcon");
   g_object_unref(xobj);

   eailu_test_code_called = 1;
}

EAPI_MAIN int
elm_main(int argc, char **argv)
{
   Evas_Object *win;

   win = eailu_create_test_window_with_glib_init(_on_done, _on_focus_in);
   g_assert(win);
   _init_check(win);
   evas_object_show(win);
   elm_run();
   elm_shutdown();

   return 0;
}
ELM_MAIN()
