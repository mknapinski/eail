/*
 * Tested interface: AtkAction
 *
 * Tested AtkObject: EailCtxpopup
 *
 * Description: Test AtkAction interface
 *
 * Test input: accessible object representing EailCtxpopup
 *
 * Expected test result: test should return 0 (success)
 */

#include <Elementary.h>
#include <atk/atk.h>

//#define DEBUG 1

#include "eail_test_utils.h"

#define ACTIONS_NUMBER 1

INIT_TEST("EailCtxpopup")

static void
_do_test(AtkObject *obj)
{
   _printf("Testing ctxpopup instance....\n");

   g_assert(ATK_IS_ACTION(obj));
   g_assert(ACTIONS_NUMBER == atk_action_get_n_actions(ATK_ACTION(obj)));
   eailu_test_action_activate(ATK_ACTION(obj), "dismiss");
   g_assert((eailu_get_action_number(ATK_ACTION(obj), "typo")) == -1);
   eailu_test_action_description_all(ATK_ACTION(obj));

   _printf("Tests passed successfully\n");

   eailu_test_code_called = 1;
}

static void
_ctxpopup_item_cb(void *data, Evas_Object *obj, void *event_info)
{
   _printf("ctxpopup item selected: %s\n", elm_object_item_text_get(event_info));
}

Elm_Object_Item *item_new(Evas_Object *ctxpopup, const char * label, const char *icon)
{
   Evas_Object *ic = elm_icon_add(ctxpopup);
   elm_icon_standard_set(ic, icon);
   elm_image_resizable_set(ic, EINA_FALSE, EINA_FALSE);
   return elm_ctxpopup_item_append(ctxpopup, label, ic, _ctxpopup_item_cb, NULL);
}

static void
_init_ctxpopup(Evas_Object *win)
{
   Evas_Object *ctxpopup, *box;
   Elm_Object_Item *object_it;

   box = elm_box_add(win);
   evas_object_size_hint_weight_set(box, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
   elm_win_resize_object_add(win, box);
   evas_object_show(box);

   ctxpopup = elm_ctxpopup_add(win);
   item_new(ctxpopup, "Go to home folder", "home");
   item_new(ctxpopup, "Save file", "file");
   item_new(ctxpopup, "Delete file", "delete");
   object_it = item_new(ctxpopup, "Navigate to folder", "folder");
   elm_object_item_disabled_set(object_it, EINA_TRUE);
   item_new(ctxpopup, "Edit entry", "edit");
   object_it = item_new(ctxpopup, "Set date and time", "clock");

   evas_object_size_hint_weight_set(ctxpopup, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
   evas_object_show(ctxpopup);
   elm_box_pack_end(box, ctxpopup);
}

EAPI_MAIN int
elm_main(int argc, char **argv)
{
   Evas_Object *win;

   win = eailu_create_test_window_with_glib_init(_on_done, _on_focus_in);
   _init_ctxpopup(win);
   evas_object_show(win);
   elm_run();
   elm_shutdown();

   return 0;
}
ELM_MAIN()
