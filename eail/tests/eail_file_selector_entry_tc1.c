/*
 * Tested interface: AtkAction
 *
 * Tested AtkObject: EailFileselectorEntry
 *
 * Description: Test AtkAction interface
 *
 * Test input: accessible object representing EailFileselectorEntry
 *
 * Expected test result: test should return 0 (success)
 */

#include <Elementary.h>
#include <atk/atk.h>

#include "eail_test_utils.h"

#define EAIL_LABEL_FOR_FSE "Select a file"

INIT_TEST("EailFileselectorEntry")

static void
_do_test(AtkObject *obj)
{
   int actions_num = -1;

   _printf("Testing FileselectorEntry instance....\n");

   g_assert(ATK_IS_OBJECT(obj));
   g_assert(ATK_IS_ACTION(obj));

   actions_num = atk_action_get_n_actions(ATK_ACTION(obj) );
   g_assert(4 == actions_num);

   eailu_test_atk_focus(obj, TRUE);

   /* checking if all needed actions are implemented */
   /* pressed */
   eailu_test_action_activate(ATK_ACTION(obj), "press");

   /* repeated */
   eailu_test_action_activate(ATK_ACTION(obj), "longpress");

   /* unpressed */
   eailu_test_action_activate(ATK_ACTION(obj), "release");

   /* clicked */
   eailu_test_action_activate(ATK_ACTION(obj), "click");

   /* test for non-existing action */
   g_assert((eailu_get_action_number(ATK_ACTION(obj), "typo")) == -1);

   /* test set/get action description */
   eailu_test_action_description_all(ATK_ACTION(obj));

   _printf("DONE. All FileselectorEntry test passed successfully \n");

   eailu_test_code_called = 1;
}

static void
_init_file_selector_button(Evas_Object *win)
{
   Evas_Object *fs_entry, *vbox, *icon;

   vbox = elm_box_add(win);
   elm_win_resize_object_add(win, vbox);
   evas_object_size_hint_weight_set(vbox, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
   evas_object_show(vbox);

   icon = elm_icon_add(win);
   elm_icon_standard_set(icon, "file");
   evas_object_size_hint_aspect_set(icon, EVAS_ASPECT_CONTROL_VERTICAL, 1, 1);

   /* file selector entry */
   fs_entry = elm_fileselector_entry_add(win);
   evas_object_size_hint_align_set(fs_entry, EVAS_HINT_FILL, 0);
   elm_fileselector_entry_path_set(fs_entry, "/tmp");
   elm_object_text_set(fs_entry, EAIL_LABEL_FOR_FSE);
   elm_object_part_content_set(fs_entry, "button icon", icon);

   elm_box_pack_end(vbox, fs_entry);
   evas_object_show(fs_entry);
   evas_object_resize(win, 300, 300);
}

EAPI_MAIN int
elm_main(int argc, char **argv)
{
   Evas_Object *win;

   win = eailu_create_test_window_with_glib_init(_on_done, _on_focus_in);
   _init_file_selector_button(win);

   evas_object_show(win);

   elm_run();
   elm_shutdown();

   return 0;
}
ELM_MAIN()
