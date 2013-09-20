/*
 * Tested interface: AtkObject
 *
 * Tested AtkObject: EailHoversel
 *
 * Description: Test AtkObject interface
 *
 * Test input: accessible object representing EailHoversel
 *
 * Expected test result: test should return 0 (success)
 */

#include <Elementary.h>
#include <atk/atk.h>

#include "eail_test_utils.h"

#define ACTIONS_COUNT 2
#define ACTION_SHRINK 0
#define ACTION_EXPAND 1
#define ITEMS_COUNT 3
#define ITEM1_NAME "Print items"
#define ITEM2_NAME "Option 2"
#define ITEM3_NAME "Clear all items"

INIT_TEST("EailHoversel")

static void
_init_hoversel(Evas_Object *win)
{
   Evas_Object *bg, *rect, *hoversel;

   bg = elm_bg_add(win);
   evas_object_size_hint_weight_set(bg, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
   elm_win_resize_object_add(win, bg);
   evas_object_show(bg);

   rect = evas_object_rectangle_add(evas_object_evas_get(win));
   evas_object_color_set(rect, 255, 0, 0, 255);
   evas_object_show(rect);

   hoversel = elm_hoversel_add(win);
   elm_hoversel_hover_parent_set(hoversel, win);
   elm_hoversel_horizontal_set(hoversel, EINA_FALSE);
   elm_object_text_set(hoversel, "Hoversel");
   elm_object_part_content_set(hoversel, "icon", rect);

   elm_hoversel_item_add(hoversel, ITEM1_NAME, NULL, ELM_ICON_NONE,
                         NULL, NULL);
   elm_hoversel_item_add(hoversel, ITEM2_NAME, "home", ELM_ICON_STANDARD, NULL,
                         NULL);
   elm_hoversel_item_add(hoversel, ITEM3_NAME, "close",
                         ELM_ICON_STANDARD, NULL, NULL);

   evas_object_resize(hoversel, 180, 30);
   evas_object_resize(win, 200, 180);
   evas_object_move(hoversel, 10, 10);
   evas_object_show(hoversel);
   elm_hoversel_hover_begin(hoversel);
}

static void
_do_test(AtkObject *obj)
{
   const char *type_name = g_type_name(G_TYPE_FROM_INSTANCE(obj));
   AtkObject *child;
   char *item_names[] = {ITEM1_NAME, ITEM2_NAME, ITEM3_NAME};
   int n;

   g_assert(ATK_IS_OBJECT(obj));
   g_assert_cmpstr(type_name, ==, "EailHoversel");
   atk_object_set_description(obj, "test");
   g_assert_cmpstr(atk_object_get_description(obj), ==, "test");

   atk_object_set_name(obj, "test name");
   g_assert_cmpstr(atk_object_get_name(obj), ==, "test name");

   g_assert(atk_object_get_n_accessible_children(obj) == ITEMS_COUNT);

   n = ITEMS_COUNT;
   while (n--)
     {
        child = eailu_find_child_with_name(obj, item_names[n]);
        g_assert(child);
        g_object_unref(child);
     }

   eailu_test_code_called = 1;
}

EAPI_MAIN int
elm_main(int argc, char **argv)
{
   Evas_Object *win = NULL;

   win = eailu_create_test_window_with_glib_init(_on_done, _on_focus_in);
   g_assert(win);
   _init_hoversel(win);
   evas_object_show(win);
   elm_run();
   elm_shutdown();

   return 0;
}
ELM_MAIN()
