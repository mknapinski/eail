/*
 * Tested interface: AtkAction
 *
 * Tested AtkObject: EailGenlist
 *
 * Description: Test AtkAction interface
 *
 * Test input: accessible object representing EailGenlist
 *
 * Expected test result: test should return 0 (success)
 */

#include <Elementary.h>
#include <atk/atk.h>

#include "eail_test_utils.h"

INIT_TEST("EailGenlist")

#define N_ITEMS 18

static Evas_Object *glob_genlist = NULL;
static Elm_Genlist_Item_Class *_itc_group = NULL;
static int nitems = 0;

Elm_Object_Item *glob_test_item = NULL;

static void
_do_test(AtkObject *aobj)
{
   int n_actions = 0;
   const gchar *test_desc = "test description for scroll up";
   const gchar *result_desc;

   g_assert(ATK_IS_ACTION(aobj));
   n_actions = atk_action_get_n_actions(ATK_ACTION(aobj));
   g_assert(4 == n_actions);

   /* scroll up*/
   eailu_test_action_activate(ATK_ACTION(aobj), "scroll_up");

   /* scroll_down */
   eailu_test_action_activate(ATK_ACTION(aobj), "scroll_down");

   /* scroll_left */
   eailu_test_action_activate(ATK_ACTION(aobj), "scroll_left");

   /* scroll_right*/
   eailu_test_action_activate(ATK_ACTION(aobj), "scroll_right");

   /* description test */
   g_assert(!atk_action_get_description(ATK_ACTION(aobj), 0));
   g_assert(atk_action_set_description(ATK_ACTION(aobj), 0, test_desc));

   result_desc = atk_action_get_description(ATK_ACTION(aobj), 0);
   g_assert(0 == g_strcmp0(result_desc, test_desc));
   eailu_test_code_called = 1;
}

static char *
_item_label_get(void *data, Evas_Object *obj, const char *part)
{
   time_t t = (time_t)ecore_time_unix_get();
   char buf[256];
   int i = (int)(long)data;

   if (!strcmp(part, "elm.text"))
     snprintf(buf, sizeof(buf), "Item # %i", i);
   else
     {
        int n;
        snprintf(buf, sizeof(buf), "realized at %s", ctime(&t));
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

   snprintf(buf, sizeof(buf), "Group %d (item #%d)", i / 7, i);

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
