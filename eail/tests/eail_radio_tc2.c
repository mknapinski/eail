/*
 * Tested interface: AtkObject
 *
 * Tested AtkObject: EailRadio
 *
 * Description: Test whether accessible object is successfully registered
 *    in GObject type system and is visible in AtkObject tree.
 *
 * Test input: accessible object representing EailRadio
 *
 * Expected test result: test should return 0 (success)
 */

#include <Elementary.h>
#include <atk/atk.h>

#include "eail_test_utils.h"

#define RADIO1_SEL 1
#define RADIO2_SEL 2
#define RADIO3_SEL 3
#define RADIO1 "Radio 1"
#define RADIO2 "Radio 2"
#define RADIO3 "Radio 3"
#define TEST_NAME "Test Name"

int val = 1;
int name_test_passed;

static void _init_radio(Evas_Object *win);

INIT_TEST("EailRadioButton")

EAPI_MAIN int
elm_main(int argc, char **argv)
{
   Evas_Object *win;

   win = eailu_create_test_window_with_glib_init(_on_done, _on_focus_in);
   _init_radio(win);

   evas_object_show(win);
   elm_run();
   elm_shutdown();

   return 0;
}
ELM_MAIN()

static void
_init_radio(Evas_Object *win)
{
   Evas_Object *bg, *radio, *group, *bx, *ic;

   elm_policy_set(ELM_POLICY_QUIT, ELM_POLICY_QUIT_LAST_WINDOW_CLOSED);
   elm_win_autodel_set(win, EINA_TRUE);

   bg = elm_bg_add(win);
   elm_win_resize_object_add(win, bg);
   evas_object_show(bg);

   bx = elm_box_add(win);
   elm_box_horizontal_set(bx, EINA_TRUE);
   evas_object_size_hint_weight_set(bx, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
   elm_win_resize_object_add(win, bx);
   evas_object_show(bx);

   group = radio = elm_radio_add(win);
   elm_object_text_set(radio, RADIO1);
   elm_radio_state_value_set(radio, 1);
   elm_radio_value_pointer_set(radio, &val);
   ic = elm_icon_add(win);
   elm_icon_standard_set(ic, "home");
   elm_object_part_content_set(radio, "icon", ic);
   elm_box_pack_end(bx, radio);
   evas_object_size_hint_weight_set(radio, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
   evas_object_size_hint_align_set(radio, EVAS_HINT_FILL, EVAS_HINT_FILL);
   evas_object_show(radio);

   radio = elm_radio_add(win);
   elm_object_text_set(radio, RADIO2);
   elm_radio_state_value_set(radio, 2);
   elm_radio_value_pointer_set(radio, &val);
   elm_radio_group_add(radio, group);
   ic = elm_icon_add(win);
   elm_icon_standard_set(ic, "file");
   elm_object_part_content_set(radio, "icon", ic);
   elm_box_pack_end(bx, radio);
   evas_object_size_hint_weight_set(radio, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
   evas_object_size_hint_align_set(radio, EVAS_HINT_FILL, EVAS_HINT_FILL);
   evas_object_show(radio);

   radio = elm_radio_add(win);
   elm_object_text_set(radio, RADIO3);
   elm_radio_state_value_set(radio, 3);
   elm_radio_value_pointer_set(radio, &val);
   elm_radio_group_add(radio, group);
   elm_box_pack_end(bx, radio);
   evas_object_size_hint_weight_set(radio, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
   evas_object_size_hint_align_set(radio, EVAS_HINT_FILL, EVAS_HINT_FILL);
   evas_object_show(radio);

   elm_radio_value_set(radio, RADIO2_SEL);
}

static void
_do_test(AtkObject *obj)
{
   AtkStateSet *state_set = atk_object_ref_state_set(obj);
   const char *name;

   g_assert(ATK_IS_OBJECT(obj));
   g_assert(atk_object_get_role(obj) == ATK_ROLE_RADIO_BUTTON);

   atk_object_set_description(obj, "test");
   g_assert_cmpstr(atk_object_get_description(obj), ==, "test");

   name = atk_object_get_name(obj);

   if (!strcmp(name, RADIO1))
     {
        state_set = atk_object_ref_state_set(obj);
        g_assert(!atk_state_set_contains_state(state_set, ATK_STATE_CHECKED));
        g_object_unref(state_set);
        ++name_test_passed;

        atk_object_set_name(obj, TEST_NAME);
        g_assert_cmpstr(atk_object_get_name(obj), ==, TEST_NAME);
     }
   else if (!strcmp(name, RADIO2))
     {
        state_set = atk_object_ref_state_set(obj);
        g_assert(atk_state_set_contains_state(state_set, ATK_STATE_CHECKED));
        g_object_unref(state_set);
        ++name_test_passed;
        g_assert(name_test_passed == 2);

        atk_object_set_name(obj, TEST_NAME);
        g_assert_cmpstr(atk_object_get_name(obj), ==, TEST_NAME);
     }
   else if (!strcmp(name, RADIO3))
     {
        state_set = atk_object_ref_state_set(obj);
        g_assert(!atk_state_set_contains_state(state_set, ATK_STATE_CHECKED));
        g_object_unref(state_set);
        ++name_test_passed;
        g_assert(name_test_passed == 3);

        atk_object_set_name(obj, TEST_NAME);
        g_assert_cmpstr(atk_object_get_name(obj), ==, TEST_NAME);
     }
   else
     {
        g_assert(0);
     }

   eailu_test_code_called = 1;
}
