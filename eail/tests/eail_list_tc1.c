/*
 * Tested interface: AtkObject
 *
 * Tested AtkObject: EailList
 *
 * Description: Test whether accessible object is successfully registered
 *    in GObject type system and is visible in AtkObject tree. Test
 *    accessible children availability. Test focus.
 *
 * Test input: accessible object representing EailList
 *
 * Expected test result: test should return 0 (success)
 */

#include <Elementary.h>
#include <atk/atk.h>

//#define DEBUG 1

#include "eail_test_utils.h"

INIT_TEST("EailList")

static Evas_Object *glob_elm_list = NULL;

void
_do_test(AtkObject *aobj)
{
   int child_amount = 0, i = 0;

   g_assert(ATK_IS_OBJECT(aobj));

   g_assert(atk_object_get_role(aobj) == ATK_ROLE_LIST);

   child_amount = atk_object_get_n_accessible_children(aobj);
   g_assert(7 == child_amount);

   /* For list widget focusing work */
   eailu_test_atk_focus(aobj, TRUE);

   /* checking list items implementation */
     for (i = 0; i < child_amount; i++)
      {
         AtkObject *child = atk_object_ref_accessible_child(aobj, i);
         _printf("Child amount %d\n", atk_object_get_n_accessible_children(child));
         g_assert(2 == atk_object_get_n_accessible_children(child));

         g_object_unref(child);
       }

   eailu_test_code_called = 1;
}

void
_init_list(Evas_Object *win)
{
   Evas_Object *box, *entry;
   int i = 0;

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

   box = elm_box_add(win);
   evas_object_size_hint_weight_set(box, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
   elm_win_resize_object_add(win, box);
   evas_object_show(box);

   glob_elm_list = elm_list_add(win);
   evas_object_size_hint_weight_set(glob_elm_list, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
   evas_object_size_hint_align_set(glob_elm_list, EVAS_HINT_FILL, EVAS_HINT_FILL);
   elm_object_tree_focus_allow_set(win, EINA_TRUE);

   entry = elm_entry_add(win);
   elm_entry_single_line_set(entry, EINA_TRUE);
   evas_object_size_hint_weight_set(entry, EVAS_HINT_EXPAND, 0.0);
   evas_object_size_hint_align_set(entry, EVAS_HINT_FILL, EVAS_HINT_FILL);
   evas_object_show(entry);

   for (i = 0; i < sizeof(lbl) / sizeof(lbl[0]); i++)
     elm_list_item_append(glob_elm_list, lbl[i], entry, entry, NULL, NULL);

   elm_box_pack_end(box, glob_elm_list);

   elm_list_go(glob_elm_list);
   evas_object_show(glob_elm_list);
}

EAPI_MAIN int
elm_main(int argc, char **argv)
{
   Evas_Object *win;

   win = eailu_create_test_window_with_glib_init(_on_done, _on_focus_in);
   _init_list(win);

   /* and show the window */
   evas_object_show(win);

   elm_run();
   elm_shutdown();

   /* exit code */
   return 0;
}
ELM_MAIN()
