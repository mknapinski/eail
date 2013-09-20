/*
 * Tested interface: AtkSelection
 *
 * Tested AtkObject: EailList
 *
 * Description: Test AtkSelection interface
 *
 * Test input: accessible object representing EailList
 *
 * Expected test result: test should return 0 (success)
 */

#include <Elementary.h>
#include <atk/atk.h>

#include "eail_test_utils.h"

#define EAIL_TYPE_FOR_LIST "EailList"
#define EAIL_TYPE_FOR_LIST_ITEM "EailItem"

static Evas_Object *glob_elm_list = NULL;

INIT_TEST("EailList")

void
_test_list_item_interfaces(AtkObject *item_object)
{
   g_assert(ATK_IS_OBJECT(item_object));
   g_assert(eailu_is_object_with_type(item_object, EAIL_TYPE_FOR_LIST_ITEM));

   /**
    * for single list items focusing does NOT work
    */
   eailu_test_atk_focus(item_object, FALSE);

   g_assert(ATK_IS_ACTION(item_object));
}

void
_test_item_selection_single(AtkObject *parent, AtkObject *child, int index)
{
   g_assert(ATK_IS_SELECTION(parent));
   /* in single selection mode it should return FALSE */
   g_assert(!atk_selection_select_all_selection(ATK_SELECTION(parent)));
   g_assert
      (FALSE == atk_selection_is_child_selected(ATK_SELECTION(parent), index));

   /* NOTE: calling click action to select child*/
  eailu_test_action_activate(ATK_ACTION(child), "click");
  g_assert(atk_selection_is_child_selected(ATK_SELECTION(parent), index));
  g_assert(1 == atk_selection_get_selection_count(ATK_SELECTION(parent)));

  /* now removing selection using atk_selection interface */
  g_assert(atk_selection_remove_selection(ATK_SELECTION(parent),index));
  g_assert
     (FALSE == atk_selection_is_child_selected(ATK_SELECTION(parent), index));
  g_assert(0 == atk_selection_get_selection_count(ATK_SELECTION(parent)));
}

void
_test_multiselection_list(AtkObject *atk_list)
{
   elm_list_multi_select_set(glob_elm_list, EINA_TRUE);
   g_assert(atk_selection_select_all_selection(ATK_SELECTION(atk_list)));

   /* all children should be now selected */
   g_assert(7 == atk_selection_get_selection_count(ATK_SELECTION(atk_list)));

   /* now clearing selection */
   g_assert(atk_selection_clear_selection(ATK_SELECTION(atk_list)));
   g_assert(0 == atk_selection_get_selection_count(ATK_SELECTION(atk_list)));
}

void
_do_test(AtkObject *aobj)
{
   int child_amount = 0, i = 0;

   /* checking list items implementation */
     for (i = 0; i < child_amount; i++)
      {
         AtkObject *child = atk_object_ref_accessible_child(aobj, i);
         _test_list_item_interfaces(child);
         _test_item_selection_single(aobj, child, i);
         /* each list item has inside 2 text entry objects */
         printf("Child amount %d\n", atk_object_get_n_accessible_children(child));
         g_assert(2 == atk_object_get_n_accessible_children(child));
         g_object_unref(child);
       }

   /* now quick check if multiselection on list works properly*/
   _test_multiselection_list(aobj);

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
