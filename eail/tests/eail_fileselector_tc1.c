/*
 * Tested interface: AtkObject
 *
 * Tested AtkObject: EailFileselector
 *
 * Description: Test AtkObject interface
 *
 * Test input: accessible object representing EailFileselector
 *
 * Expected test result: test should return 0 (success)
 */

#include <Elementary.h>
#include <atk/atk.h>

#include "eail_test_utils.h"

#define FILESELECTOR_NAME "fileselector name"

INIT_TEST("EailFileselector")

static void
_test_atk_focus(AtkObject *obj)
{
   AtkStateSet *state_set = NULL;
   state_set = atk_object_ref_state_set(obj);

   g_assert(atk_state_set_contains_state(state_set, ATK_STATE_FOCUSABLE));
   /* in fileselector focus always gets to one of internal children after
    * marking as focused*/
   g_assert(atk_component_grab_focus(ATK_COMPONENT(obj)));
   g_assert(!atk_state_set_contains_state(state_set, ATK_STATE_FOCUSED));
}

static void
_do_test(AtkObject *obj)
{
   int child_amount = 0;

   g_assert(ATK_IS_OBJECT(obj));

   atk_object_set_name(obj, FILESELECTOR_NAME"test");
   g_assert_cmpstr(atk_object_get_name(obj), ==, FILESELECTOR_NAME"test");

   atk_object_set_name(obj, "test name");
   g_assert_cmpstr(atk_object_get_name(obj), ==, "test name");

   g_assert(eailu_is_object_with_role(obj, ATK_ROLE_FILE_CHOOSER));
   child_amount = atk_object_get_n_accessible_children(obj);

   /*
    * up button, home button, list, ok button, cancel button, entry and list -
    * overall 7 child objects
    */
   g_assert(7 == child_amount);

   _test_atk_focus(obj);

   eailu_test_code_called = 1;
}

static void
_init_fileselector(Evas_Object *win)
{
   Evas_Object *fileselector, *box;

   box = elm_box_add(win);
   evas_object_size_hint_weight_set(box, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
   elm_win_resize_object_add(win, box);
   evas_object_show(box);

   fileselector = elm_fileselector_add(win);
   elm_fileselector_path_set(fileselector, "/tmp");
   elm_fileselector_expandable_set(fileselector, EINA_FALSE);

   evas_object_size_hint_weight_set(fileselector, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
   evas_object_size_hint_align_set(fileselector, EVAS_HINT_FILL, EVAS_HINT_FILL);
   evas_object_show(fileselector);

   elm_box_pack_end(box, fileselector);
}

EAPI_MAIN int
elm_main(int argc, char **argv)
{
   Evas_Object *win;

   win = eailu_create_test_window_with_glib_init(_on_done, _on_focus_in);
   _init_fileselector(win);

   evas_object_show(win);

   elm_run();
   elm_shutdown();

   /* exit code */
   return 0;
}
ELM_MAIN()
