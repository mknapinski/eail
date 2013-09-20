/*
 * Tested interface: AtkObject
 *
 * Tested AtkObject: EailFlipselector
 *
 * Description: Test AtkObject interface
 *
 * Test input: accessible object representing EailFlipselector
 *
 * Expected test result: test should return 0 (success)
 */

#include <Elementary.h>

#include "atk/atk.h"
#include "eail_test_utils.h"

#define ITEMS_COUNT 8

INIT_TEST("EailFlipselector")

void
_init_flipselector(Evas_Object *win)
{
   unsigned int i;
   Evas_Object *bg, *bx, *fp;
   static const char *lbl[] =
     {
        "Elementary",
        "Evas",
        "Eina",
        "Edje",
        "Eet",
        "Ecore",
        "Efreet",
        "Edbus"
     };

   bg = elm_bg_add(win);
   elm_win_resize_object_add(win, bg);
   evas_object_size_hint_weight_set(bg, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
   evas_object_show(bg);

   bx = elm_box_add(win);
   evas_object_size_hint_weight_set(bx, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
   elm_win_resize_object_add(win, bx);
   evas_object_show(bx);

   fp = elm_flipselector_add(win);
   evas_object_size_hint_weight_set(fp, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);

   for (i = 0; i < sizeof(lbl) / sizeof(lbl[0]); i++)
     elm_flipselector_item_append(fp, lbl[i], NULL, NULL);

   elm_box_pack_end(bx, fp);
   evas_object_show(fp);
}

void
_do_test(AtkObject *obj)
{
   g_assert(ATK_IS_OBJECT(obj));

   atk_object_set_description(obj, "test");
   g_assert_cmpstr(atk_object_get_description(obj), ==, "test");

   atk_object_set_name(obj, "test name");
   g_assert_cmpstr(atk_object_get_name(obj), ==, "test name");

   g_assert(atk_object_get_n_accessible_children(obj) == ITEMS_COUNT);

   g_assert(atk_object_get_role(obj) == ATK_ROLE_LIST);

   eailu_test_code_called = 1;
}

EAPI_MAIN int
elm_main(int argc, char **argv)
{
   Evas_Object *win = NULL;

   win = eailu_create_test_window_with_glib_init(_on_done, _on_focus_in);
   _init_flipselector(win);
   evas_object_show(win);
   elm_run();
   elm_shutdown();

   return 0;
}
ELM_MAIN()

