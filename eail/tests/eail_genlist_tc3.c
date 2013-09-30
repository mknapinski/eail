/*
 * Tested interface: AtkSelection
 *
 * Tested AtkObject: EailGenlist
 *
 * Description: Test AtkSelection interface
 *
 * Test input: accessible object representing EailGenlist
 *
 * Expected test result: test should return 0 (success)
 */

#include <Elementary.h>
#include <atk/atk.h>

#include "eail_test_utils.h"

INIT_TEST("EailGenlist")

#define EAIL_TYPE_FOR_LIST_ITEM "EailItem"
#define N_ITEMS 18

static Evas_Object *glob_genlist = NULL;
static Elm_Genlist_Item_Class *_itc_group = NULL;
static int nitems = 0;

Elm_Object_Item *glob_test_item = NULL;

static void
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

static void
_test_item_selection_single(AtkObject *parent, AtkObject *child, int index)
{
   g_assert(ATK_IS_SELECTION(parent));
   /* in single selection mode it should return FALSE */
   g_assert(!atk_selection_select_all_selection(ATK_SELECTION(parent)));
   g_assert
      (FALSE == atk_selection_is_child_selected(ATK_SELECTION(parent), index));

   g_assert(atk_selection_add_selection(ATK_SELECTION(parent), index));
   g_assert(atk_selection_is_child_selected(ATK_SELECTION(parent), index));
   g_assert(1 == atk_selection_get_selection_count(ATK_SELECTION(parent)));

   /* now removing selection using atk_selection interface */
   g_assert(atk_selection_remove_selection(ATK_SELECTION(parent),index));
   g_assert
     (FALSE == atk_selection_is_child_selected(ATK_SELECTION(parent), index));
   g_assert(0 == atk_selection_get_selection_count(ATK_SELECTION(parent)));
}

static void
_test_multiselection_list(AtkObject *atk_list)
{
   elm_genlist_multi_select_set(glob_genlist, EINA_TRUE);
   g_assert(atk_selection_select_all_selection(ATK_SELECTION(atk_list)));

   /* all children should be now selected */
   g_assert
      (N_ITEMS == atk_selection_get_selection_count(ATK_SELECTION(atk_list)));

   /* now clearing selection */
   g_assert(atk_selection_clear_selection(ATK_SELECTION(atk_list)));
   g_assert(0 == atk_selection_get_selection_count(ATK_SELECTION(atk_list)));
}

static void
_do_test(AtkObject *aobj)
{
   int child_amount = 0, i = 0;

   child_amount = atk_object_get_n_accessible_children(aobj);
   g_assert(N_ITEMS == child_amount);

   /* checking list items implementation */
   for (i = 0; i < child_amount; i++)
     {
        AtkObject *child = atk_object_ref_accessible_child(aobj, i);
        /* each list item has inside 2 text entry objects */

        _test_list_item_interfaces(child);
        _test_item_selection_single(aobj, child, i);

        g_object_unref(child);
     }

   /* now quick check if multiselection on list works properly*/
   _test_multiselection_list(aobj);
   eailu_test_code_called = 1;
}

static char *
_item_label_get(void *data, Evas_Object *obj, const char *part)
{
   time_t t = (time_t)ecore_time_unix_get();
   char buf[256];
   int i = (int)(long)data;
   struct tm timeinfo;
   localtime_r(&t, &timeinfo);

   if (!strcmp(part, "elm.text"))
     g_snprintf(buf, sizeof(buf), "Item # %i", i);
   else
     {
        int n;
        strftime(buf, sizeof(buf), "realized at %c", &timeinfo);
        n = strlen(buf);
        buf[n - 1] = '\0';
     }

   return strdup(buf);
}

static Evas_Object *
_item_content_get(void *data, Evas_Object *obj, const char *part)
{
   Evas_Object *ic = elm_icon_add(obj);

   if (!strcmp(part, "elm.swallow.icon"))
     elm_icon_standard_set(ic, "clock");

   evas_object_size_hint_aspect_set(ic, EVAS_ASPECT_CONTROL_VERTICAL, 1, 1);
   return ic;
}

static char *
_group_label_get(void *data, Evas_Object *obj, const char *part)
{
   char buf[256];
   int i = (int)(long)data;

   g_snprintf(buf, sizeof(buf), "Group %d (item #%d)", i / 7, i);

   return strdup(buf);
}

static Evas_Object *
_group_content_get(void *data, Evas_Object *obj, const char *part)
{
   Evas_Object *ic = elm_icon_add(obj);

   if (!strcmp(part, "elm.swallow.icon"))
     elm_icon_standard_set(ic, "home");

   evas_object_size_hint_aspect_set(ic, EVAS_ASPECT_CONTROL_VERTICAL, 1, 1);
   return ic;
}

static void
_item_sel_cb(void *data, Evas_Object *obj, void *event_info)
{
//   printf("sel item data [%p] on genlist obj [%p], item pointer [%p]\n",
//          data, obj, event_info);
}

void
_init_genlist(Evas_Object *win)
{
   Evas_Object *box, *entry;
   static Elm_Genlist_Item_Class *_itc = NULL;
   int i = 0;

   if (!_itc)
     {
        _itc = elm_genlist_item_class_new();
        _itc->item_style = "default";
        _itc->func.text_get = _item_label_get;
        _itc->func.content_get = _item_content_get;
        _itc->func.state_get = NULL;
        _itc->func.del = NULL;
     }

   if (!_itc_group)
     {
        _itc_group = elm_genlist_item_class_new();
        _itc_group->item_style = "group_index";
        _itc_group->func.text_get = _group_label_get;
        _itc_group->func.content_get = _group_content_get;
        _itc_group->func.state_get = NULL;
        _itc_group->func.del = NULL;
     }

   box = elm_box_add(win);
   evas_object_size_hint_weight_set(box, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
   elm_win_resize_object_add(win, box);
   evas_object_show(box);

   glob_genlist = elm_genlist_add(win);

   evas_object_size_hint_weight_set
                           (glob_genlist, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
   evas_object_size_hint_align_set
                                (glob_genlist, EVAS_HINT_FILL, EVAS_HINT_FILL);
   elm_box_pack_end(box, glob_genlist);
   evas_object_show(glob_genlist);

   entry = elm_entry_add(win);
   elm_entry_single_line_set(entry, EINA_TRUE);
   evas_object_size_hint_weight_set(entry, EVAS_HINT_EXPAND, 0.0);
   evas_object_size_hint_align_set(entry, EVAS_HINT_FILL, EVAS_HINT_FILL);
   evas_object_show(entry);


   /* filling genlist */
   for (i = 0; i < N_ITEMS; i++)
     {
        Elm_Object_Item *gli = NULL, *glg = NULL;

         if (i % 7 == 0)
           {
               glg = gli = elm_genlist_item_append(glob_genlist, _itc_group,
                                                   (void *) (long) nitems++,
                                                   NULL, ELM_GENLIST_ITEM_GROUP,
                                                   _item_sel_cb, NULL );
               elm_genlist_item_select_mode_set(
                        gli, ELM_OBJECT_SELECT_MODE_DISPLAY_ONLY);
           }
         else
           {
               gli = elm_genlist_item_append(glob_genlist, _itc,
                                             (void *) (long) nitems++, glg,
                                             ELM_GENLIST_ITEM_NONE,
                                             _item_sel_cb, NULL );
           }

         if (i == 1)
           glob_test_item = gli;
     }

}

EAPI_MAIN int
elm_main(int argc, char **argv)
{
   Evas_Object *win;

   win = eailu_create_test_window_with_glib_init(_on_done, _on_focus_in);
   _init_genlist(win);

   /* and show the window */
   evas_object_show(win);

   elm_run();
   elm_shutdown();

   /* exit code */
   return 0;
}
ELM_MAIN()
