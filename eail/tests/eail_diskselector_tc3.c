/*
 * Tested interface: AtkSelection
 *
 * Tested AtkObject: EailDiskselector
 *
 * Description: Test AtkSelection interface
 *
 * Test input: accessible object representing EailDiskselector
 *
 * Expected test result: test should return 0 (success)
 */

#include <Elementary.h>
#include <atk/atk.h>

#include "eail_test_utils.h"

INIT_TEST("EailDiskselector")

static void
_do_test(AtkObject *obj)
{
   int i = 0;

   g_assert(ATK_IS_SELECTION(obj));

   g_assert(atk_selection_is_child_selected(ATK_SELECTION(obj), 0));

   for (i = 1; i < 7; ++i)
      g_assert(FALSE == atk_selection_is_child_selected(ATK_SELECTION(obj), i));

   eailu_test_code_called = 1;
}

static void
_init_diskselector(Evas_Object *win)
{
   Evas_Object  *bx, *ds, *bg;
   unsigned int i;
   static const char *lbl[] =
     {
        "Sunday",
        "Monday",
        "Tuesday",
        "Wednesday",
        "Thursday",
        "Friday",
        "Saturday"
     };
   bg = elm_bg_add(win);
   elm_win_resize_object_add(win, bg);
   evas_object_size_hint_weight_set(bg, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
   evas_object_show(bg);

   bx = elm_box_add(win);
   evas_object_size_hint_weight_set(bx, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
   evas_object_size_hint_align_set(bx, EVAS_HINT_FILL, EVAS_HINT_FILL);
   elm_win_resize_object_add(win, bx);
   evas_object_show(bx);

   /* default */
   ds = elm_diskselector_add(win);
   evas_object_size_hint_weight_set(ds, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
   evas_object_size_hint_align_set(ds, EVAS_HINT_FILL, EVAS_HINT_FILL);

   elm_box_pack_end(bx, ds);

   for (i = 0; i < sizeof(lbl) / sizeof(lbl[0]); i++)
     elm_diskselector_item_append(ds, lbl[i], NULL, NULL, NULL);
   evas_object_show(ds);
   evas_object_resize(win, 800, 200);
}

EAPI_MAIN int
elm_main(int argc, char **argv)
{
   Evas_Object *win;

   win = eailu_create_test_window_with_glib_init(_on_done, _on_focus_in);
   _init_diskselector(win);

   evas_object_show(win);

   elm_run();
   elm_shutdown();

   /* exit code */
   return 0;
}
ELM_MAIN()
