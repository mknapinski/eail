/*
 * Tested interface: AtkAction
 *
 * Tested AtkObject: EailFileselector
 *
 * Description: Test AtkAction interface
 *
 * Test input: accessible object representing EailFileselector
 *
 * Expected test result: test should return 0 (success)
 */

#include <Elementary.h>
#include <atk/atk.h>

//#define DEBUG 1

#include "eail_test_utils.h"

#define ACTIONS_NUMBER 3
#define EAIL_LABEL_FOR_FSB "File selector button"

static int action_test_passed;

INIT_TEST("EailButton")

static void
_on_click(void *data, Evas_Object *obj, void *event_info)
{
   _printf("fileselector button click\n");
   action_test_passed++;
}

static void
_on_press(void *data, Evas_Object *obj, void *event_info)
{
   _printf("fileselector button press\n");
   action_test_passed++;
}

static void
_on_release(void *data, Evas_Object *obj, void *event_info)
{
   _printf("fileselector button release\n");
   action_test_passed++;
}

static void
_do_test(AtkObject *obj)
{
   _printf("Testing FileSelectorButton instance....\n");

   g_assert(ATK_IS_OBJECT(obj));
   g_assert(ATK_IS_ACTION(obj));

   g_assert(ACTIONS_NUMBER == atk_action_get_n_actions(ATK_ACTION(obj)));
   eailu_test_action_activate(ATK_ACTION(obj), "click");
   eailu_test_action_activate(ATK_ACTION(obj), "press");
   eailu_test_action_activate(ATK_ACTION(obj), "release");
   g_assert((eailu_get_action_number(ATK_ACTION(obj), "typo")) == -1);

   eailu_test_atk_focus(obj, TRUE);

   _printf("DONE. All FileSelectorButton test passed successfully \n");

   g_assert(action_test_passed == 3);

   eailu_test_code_called = 1;
}

static void
_init_file_selector_button(Evas_Object *win)
{
   Evas_Object *fs_button, *vbox, *icon;

   vbox = elm_box_add(win);
   elm_win_resize_object_add(win, vbox);
   evas_object_size_hint_weight_set(vbox, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
   evas_object_show(vbox);

   icon = elm_icon_add(win);
   elm_icon_standard_set(icon, "file");
   evas_object_size_hint_aspect_set(icon, EVAS_ASPECT_CONTROL_VERTICAL, 1, 1);

   /* file selector button */
   fs_button = elm_fileselector_button_add(win);
   elm_fileselector_button_path_set(fs_button, "/tmp");
   elm_object_text_set(fs_button, EAIL_LABEL_FOR_FSB);
   elm_object_part_content_set(fs_button, "icon", icon);
   evas_object_smart_callback_add(fs_button, "clicked", _on_click, NULL);
   evas_object_smart_callback_add(fs_button, "pressed", _on_press, NULL);
   evas_object_smart_callback_add(fs_button, "unpressed", _on_release, NULL);

   elm_box_pack_end(vbox, fs_button);
   evas_object_show(fs_button);
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
