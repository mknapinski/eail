/*
 * Tested interface: AtkAction
 *
 * Tested AtkObject: EailImage
 *
 * Description: Test AtkAction interface
 *
 * Test input: accessible object representing EailImage
 *
 * Expected test result: test should return 0 (success)
 */

#include <Elementary.h>
#include <atk/atk.h>

#include "eail_test_utils.h"

#define ACTIONS_NUMBER 1

static int action_test_passed;

INIT_TEST("EailImage")

static void
_on_image_clicked(void *data, Evas_Object *obj, void *ev)
{
   _printf("photo clicked called\n");
   ++action_test_passed;
}

static void
_do_test(AtkObject *obj)
{
   g_assert(ATK_IS_ACTION(obj));

   g_assert(ACTIONS_NUMBER == atk_action_get_n_actions(ATK_ACTION(obj)));

   eailu_test_action_activate(ATK_ACTION(obj), "click");

   g_assert((eailu_get_action_number(ATK_ACTION(obj), "typo")) == -1);

   eailu_test_action_description_all(ATK_ACTION(obj));

   g_assert(action_test_passed);

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
   evas_object_smart_callback_add(image, "clicked", _on_image_clicked, NULL);
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
