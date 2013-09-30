/*
 * Tested interface: AtkObject
 *
 * Tested AtkObject: EailImage
 *
 * Description: Test whether accessible object is successfully registered
 *    in GObject type system and is visible in AtkObject tree.
 *
 * Test input: accessible object representing EailImage
 *
 * Expected test result: test should return 0 (success)
 */

#include <Elementary.h>
#include <atk/atk.h>

#include "eail_test_utils.h"

INIT_TEST("EailImage")

static void
_do_test(AtkObject *obj)
{

   const char *desc_test = "top secret";

   g_assert(ATK_IS_OBJECT(obj));

   g_assert(atk_object_get_role(obj) == ATK_ROLE_IMAGE);

   atk_object_set_description(obj, desc_test);
   g_assert_cmpstr(atk_object_get_description(obj), ==, desc_test);

   atk_object_set_name(obj, "test name");
   g_assert_cmpstr(atk_object_get_name(obj), ==, "test name");

   AtkStateSet *state_set = atk_object_ref_state_set(obj);
   AtkStateType state_types[] = {ATK_STATE_ANIMATED, ATK_STATE_RESIZABLE};
   if (atk_state_set_contains_states(state_set, state_types, 2))
     _printf("atk_ref_state_set: animated and resizable set\n");
   g_object_unref(state_set);

   eailu_test_code_called = 1;
}

static void
_init_image(Evas_Object *win)
{
   Evas_Object *bg, *image;
   const char * const filename = "./data/whiterabbit01.jpg";

   bg = elm_bg_add(win);
   elm_bg_color_set(bg, 255, 255, 255);
   evas_object_size_hint_weight_set(bg, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
   elm_win_resize_object_add(win, bg);
   evas_object_show(bg);

   image = elm_image_add(win);
   g_assert(elm_image_file_set(image, filename, NULL));
   elm_image_no_scale_set(image, EINA_TRUE);
   elm_image_resizable_set(image, EINA_FALSE, EINA_TRUE);
   elm_image_smooth_set(image, EINA_FALSE);
   elm_image_orient_set(image, ELM_IMAGE_FLIP_HORIZONTAL);
   elm_image_aspect_fixed_set(image, EINA_TRUE);
   elm_image_fill_outside_set(image, EINA_TRUE);
   elm_image_editable_set(image, EINA_TRUE);
   evas_object_size_hint_weight_set(image, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
   elm_win_resize_object_add(win, image);
   evas_object_show(image);
}

EAPI_MAIN int
elm_main(int argc, char **argv)
{
   Evas_Object *win;

   win = eailu_create_test_window_with_glib_init(_on_done, _on_focus_in);
   g_assert(win);
   _init_image(win);
   evas_object_show(win);
   elm_run();
   elm_shutdown();

   return 0;
}
ELM_MAIN()
