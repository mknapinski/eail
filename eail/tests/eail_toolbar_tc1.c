/*
 * Tested interface: AtkObject
 *
 * Tested AtkObject: EailToolbar
 *
 * Description: Test AtkObject interface
 *
 * Test input: accessible object representing EailToolbar
 *
 * Expected test result: test should return 0 (success)
 */

#include <Elementary.h>
#include <atk/atk.h>

#include "eail_test_utils.h"

#define EAIL_FIRST_TOOLBAR_ITEM_NAME "Print"
#define EAIL_TEST_SUBITEM_TYPE "EailItem"

INIT_TEST("EailToolbar")

static void
_do_test(AtkObject *obj)
{
   int child_amount = 0;
   AtkObject *nested_obj;

   g_assert(eailu_is_object_with_role(obj, ATK_ROLE_TOOL_BAR));
   child_amount = atk_object_get_n_accessible_children(obj);
   g_assert(3 == child_amount);
   atk_object_set_description(obj, "test");
   g_assert_cmpstr(atk_object_get_description(obj), ==, "test");

   atk_object_set_name(obj, "test name");
   g_assert_cmpstr(atk_object_get_name(obj), ==, "test name");

   /* test if toolbar child is EailToolbarItem type*/
   nested_obj = atk_object_ref_accessible_child(obj, 0);
   g_assert(eailu_is_object_with_type(nested_obj, EAIL_TEST_SUBITEM_TYPE));
   g_assert(
          eailu_is_object_with_name(nested_obj, EAIL_FIRST_TOOLBAR_ITEM_NAME));

   g_assert(eailu_is_object_with_role(nested_obj, ATK_ROLE_LIST_ITEM));

   eailu_test_atk_focus(obj, TRUE);
   eailu_test_action_activate(ATK_ACTION(nested_obj), "click");

   eailu_test_code_called = 1;
}

static void
_init_toolbar(Evas_Object *win)
{
   Evas_Object *toolbar, *box, *menu;
   Elm_Object_Item *toolbar_item;

   box = elm_box_add(win);
   evas_object_size_hint_weight_set(box, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
   elm_win_resize_object_add(win, box);
   evas_object_show(box);

   toolbar = elm_toolbar_add(win);
   elm_toolbar_shrink_mode_set(toolbar, ELM_TOOLBAR_SHRINK_SCROLL);
   evas_object_size_hint_weight_set(toolbar, 0.0, 0.0);
   evas_object_size_hint_align_set(toolbar, EVAS_HINT_FILL, 0.0);
   evas_object_show(toolbar);

   elm_toolbar_item_append
         (toolbar, "document-print", EAIL_FIRST_TOOLBAR_ITEM_NAME, NULL, NULL);
   elm_toolbar_item_append(toolbar, "folder-new", "New Folder", NULL, NULL);
   toolbar_item = elm_toolbar_item_append
                  (toolbar, NULL, "Nested menu", NULL, NULL);
   elm_toolbar_item_menu_set(toolbar_item, EINA_TRUE);
   menu = elm_toolbar_item_menu_get(toolbar_item);

   elm_menu_item_add
                  (menu, NULL, "emptytrash", "first menu item", NULL, NULL);
   elm_menu_item_add
               (menu, NULL, "trashcan_full", "second menu item", NULL, NULL);

   elm_box_pack_end(box, toolbar);
}

EAPI_MAIN int
elm_main(int argc, char **argv)
{
   Evas_Object *win;

   win = eailu_create_test_window_with_glib_init(_on_done, _on_focus_in);
   _init_toolbar(win);

   evas_object_show(win);

   elm_run();
   elm_shutdown();

   /* exit code */
   return 0;
}
ELM_MAIN()
