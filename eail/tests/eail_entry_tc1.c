/*
 * Tested interface: AtkObject
 *
 * Tested AtkObject: EailEntry
 *
 * Description: Test AtkObject interface
 *
 * Test input: accessible object representing EailEntry
 *
 * Expected test result: test should return 0 (success)
 */

#include <Elementary.h>
#include <atk/atk.h>
#include "eail_test_utils.h"

INIT_TEST("EailEntry")

static void
_do_test(AtkObject *obj)
{
   AtkRole role;
   AtkStateSet *state_set;
   const char *type_name = g_type_name(G_TYPE_FROM_INSTANCE(obj));

   g_assert(ATK_IS_OBJECT(obj));
   g_assert_cmpstr(type_name, ==, "EailEntry");

   atk_object_set_description(obj, "test");
   g_assert_cmpstr(atk_object_get_description(obj), ==, "test");

   atk_object_set_name(obj, "test name");
   g_assert_cmpstr(atk_object_get_name(obj), ==, "test name");

   role = atk_object_get_role(obj);
   g_assert(role == ATK_ROLE_ENTRY);
   state_set = atk_object_ref_state_set(obj);
   g_assert(atk_state_set_contains_state(state_set, ATK_STATE_MULTI_LINE));
   g_object_unref(state_set);

   eailu_test_code_called = 1;
}

static void
_init_entry(Evas_Object *win)
{
   Evas_Object *box, *tb, *bg, *en;

   elm_policy_set(ELM_POLICY_QUIT, ELM_POLICY_QUIT_LAST_WINDOW_CLOSED);
   elm_win_autodel_set(win, EINA_TRUE);

   bg = elm_bg_add(win);
   elm_win_resize_object_add(win, bg);
   evas_object_size_hint_weight_set(bg, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
   evas_object_show(bg);

   box = elm_box_add(win);
   evas_object_size_hint_weight_set(box, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
   elm_win_resize_object_add(win, box);
   evas_object_show(box);

   tb = elm_box_add(win);
   elm_box_horizontal_set(tb, EINA_TRUE);
   evas_object_size_hint_weight_set(tb, EVAS_HINT_EXPAND, 0.0);
   evas_object_size_hint_weight_set(tb, EVAS_HINT_FILL, EVAS_HINT_FILL);
   elm_box_pack_end(box, tb);
   evas_object_show(tb);

   en = elm_entry_add(win);
   elm_entry_line_wrap_set(en, ELM_WRAP_WORD);
   evas_object_size_hint_weight_set(en, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
   evas_object_size_hint_align_set(en, EVAS_HINT_FILL, EVAS_HINT_FILL);
   elm_box_pack_end(box, en);
   evas_object_show(en);

   evas_object_resize(win, 200, 300);
}

EAPI_MAIN int
elm_main(int argc, char *argv[])
{
   Evas_Object *win;

   win = eailu_create_test_window_with_glib_init(_on_done, _on_focus_in);
   _init_entry(win);
   evas_object_show(win);
   elm_run();
   elm_shutdown();

   return 0;
}
ELM_MAIN()
