/*
 * Tested interface: AtkObject
 *
 * Tested AtkObject: EailPrefs
 *
 * Description: Test AtkObject interface
 *
 * Test input: accessible object representing EailPrefs
 *
 * Expected test result: test should return 0 (success)
 */

#include <Elementary.h>

#include "eail_test_utils.h"

#define CHILDREN_COUNT 9

Elm_Prefs_Data *prefs_data;

INIT_TEST("EailPrefs")

static void
_do_test(AtkObject *obj)
{
   AtkObject *child;
   int i;

   g_assert(ATK_IS_OBJECT(obj));
   atk_object_set_description(obj, "test");
   g_assert_cmpstr(atk_object_get_description(obj), ==, "test");

   atk_object_set_name(obj, "test name");
   g_assert_cmpstr(atk_object_get_name(obj), ==, "test name");

   g_assert(atk_object_get_role(obj) == ATK_ROLE_REDUNDANT_OBJECT);

   g_assert(atk_object_get_n_accessible_children(obj) == CHILDREN_COUNT);

   for (i = 0; i < atk_object_get_n_accessible_children(obj); ++i)
     {
        child = atk_object_ref_accessible_child(obj, i);
        g_assert(child);
        g_object_unref(child);
     }

   eailu_test_code_called = 1;
}

static void
_init_prefs(Evas_Object *win)
{
   Evas_Object *bg, *prefs;

   elm_policy_set(ELM_POLICY_QUIT, ELM_POLICY_QUIT_LAST_WINDOW_CLOSED);
   elm_win_autodel_set(win, EINA_TRUE);

   bg = elm_bg_add(win);
   elm_win_resize_object_add(win, bg);
   evas_object_size_hint_weight_set(bg, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
   evas_object_show(bg);

   prefs = elm_prefs_add(win);
   evas_object_size_hint_weight_set(prefs, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
   elm_win_resize_object_add(win, prefs);
   evas_object_show(prefs);
   elm_prefs_autosave_set(prefs, EINA_TRUE);
   prefs_data = elm_prefs_data_new("./data/prefs_example_02.cfg", NULL,
                                   EET_FILE_MODE_READ_WRITE);
   elm_prefs_file_set(prefs, "./data/prefs_example_02.epb", NULL);
   elm_prefs_data_set(prefs, prefs_data);

   evas_object_resize(win, 320, 320);
}

EAPI_MAIN int
elm_main(int argc, char **argv)
{
   Evas_Object *win;

   win = eailu_create_test_window_with_glib_init(_on_done, _on_focus_in);
   _init_prefs(win);
   evas_object_show(win);
   elm_run();
   elm_prefs_data_unref(prefs_data);
   elm_shutdown();

   return 0;
}

ELM_MAIN()
