/*
 * Tested interface: AtkObject
 *
 * Tested AtkObject: EailMenu
 *
 * Description: Test whether accessible object is successfully registered
 *    in GObject type system and is visible in AtkObject tree. Test
 *    accessible children availability.
 *
 * Test input: accessible object representing EailMenu
 *
 * Expected test result: test should return 0 (success)
 */

#include <Elementary.h>
#include <atk/atk.h>

#include "eail_test_utils.h"

#define EAIL_TYPE_FOR_MENU "EailMenu"
#define EAIL_TYPE_FOR_MENU_ITEM "EailMenuItem"
#define T_MENU_ITEM_WITH_NO_CHILD_NAME "first item"
#define T_SUBM_WITH_ICON_NAME "RJ Menu 1 with icon"
#define T_NESTED_BUTTON_NAME "button - delete items (2nd level)"
#define T_MENU_WITH_SUB_AND_ICON_NAME "second item"
#define T_CUSTOM_NAME_FOR_ITEM "Custom name set to nested button"
#define T_CUSTOM_DESCRIPTION "Description for testing"

INIT_TEST("EailMenu")

static void
_show(void *data, Evas *e, Evas_Object *obj, void *event_info)
{
   Evas_Event_Mouse_Down *ev = event_info;
   elm_menu_move(data, ev->canvas.x, ev->canvas.y);
   evas_object_show(data);
}

static void
_do_test(AtkObject *aobj)
{
   AtkObject *found_obj = NULL;
   int child_amount = 0;

   found_obj = eailu_find_child_with_name(aobj, T_SUBM_WITH_ICON_NAME);
   atk_object_set_description(aobj, "test");
   g_assert_cmpstr(atk_object_get_description(aobj), ==, "test");

   g_assert(found_obj);

   /* checking if nested button can be found */
   found_obj = eailu_find_child_with_name(aobj, T_NESTED_BUTTON_NAME);
   g_assert(found_obj);

   /* checking if menu with submenus AND nested icon can be found */
   found_obj = eailu_find_child_with_name(aobj, T_MENU_WITH_SUB_AND_ICON_NAME);
   g_assert(found_obj);
   eailu_test_atk_focus(found_obj, FALSE);

   /* cheking if name/description change can be done for item */
   atk_object_set_name(found_obj, T_CUSTOM_NAME_FOR_ITEM);
   g_assert(0 == g_strcmp0
                    (T_CUSTOM_NAME_FOR_ITEM, atk_object_get_name(found_obj)));
   atk_object_set_description(found_obj, T_CUSTOM_DESCRIPTION);
   g_assert(0 == g_strcmp0
               (T_CUSTOM_DESCRIPTION, atk_object_get_description(found_obj)));

   /* submenu with icons should have 7 children - one for each of 5 submenus,
    * plus 1 for separator, which is shown as MENU ITEM with empty name
    * plus 1 for nested ICON */
   child_amount = atk_object_get_n_accessible_children(found_obj);
   g_assert(7 == child_amount);

   eailu_test_code_called = TRUE;
}


static void
_init_menu(Evas_Object *win)
{
   Evas_Object *menu, *button;
   Elm_Object_Item *menu_it, *menu_it1;

   menu = elm_menu_add(win);
   elm_object_tree_focus_allow_set(win, EINA_TRUE);

   elm_menu_item_add
            (menu, NULL, NULL, T_MENU_ITEM_WITH_NO_CHILD_NAME, NULL, NULL);
   menu_it = elm_menu_item_add
               (menu, NULL, "mail-reply-all", T_MENU_WITH_SUB_AND_ICON_NAME,
                NULL, NULL );
   elm_menu_item_add
      (menu, menu_it, "object-rotate-left", T_SUBM_WITH_ICON_NAME, NULL, NULL);

   button = elm_button_add(win);
   elm_object_text_set(button, T_NESTED_BUTTON_NAME);
   menu_it1 = elm_menu_item_add(menu, menu_it, NULL, NULL, NULL, NULL );
   elm_object_item_content_set(menu_it1, button);

   /* separator by atk is treated as empty MENU ITEM */
   elm_menu_item_separator_add(menu, menu_it);
   elm_menu_item_add(menu, menu_it, NULL, "third item (2nd lev)", NULL, NULL);
   elm_menu_item_add(menu, menu_it, NULL, "fourth item (2nd lev)", NULL, NULL);
   elm_menu_item_add
         (menu, menu_it, "window-new", "sub menu (2nd lev)", NULL, NULL );

   menu_it = elm_menu_item_add(menu, NULL, NULL, "third item", NULL, NULL );
   elm_object_item_disabled_set(menu_it, EINA_TRUE );

   evas_object_event_callback_add(win, EVAS_CALLBACK_MOUSE_DOWN, _show, menu);
   evas_object_show(menu);
}

EAPI_MAIN int
elm_main(int argc, char **argv)
{
   Evas_Object *win;

   win = eailu_create_test_window_with_glib_init(_on_done, _on_focus_in);
   _init_menu(win);

   evas_object_show(win);
   elm_run();
   elm_shutdown();

   return 0;
}
ELM_MAIN()
