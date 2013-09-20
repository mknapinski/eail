/*
 * Tested interface: AtkImage
 *
 * Tested AtkObject: EailImage
 *
 * Description: Test AtkImage interface implementation
 *
 * Test input: accessible object representing EailImage
 *
 * Expected test result: test should return 0 (success)
 */

#include <Elementary.h>
#include <atk/atk.h>

//#define DEBUG 1

#include "eail_test_utils.h"

#define ACTIONS_NUMBER 1

INIT_TEST("EailImage")

static void
_on_image_clicked(void *data, Evas_Object *obj, void *ev)
{
   _printf("photo clicked called\n");
}

static void
_do_test(AtkObject *obj)
{
   const char *desc_test = "top secret";
   const char *desc;
   int height = 0, width  = 0;
   int x = -1, y = -1;

   g_assert(ATK_IS_IMAGE(obj));

   atk_image_get_image_position(ATK_IMAGE(obj), &x, &y, ATK_XY_SCREEN);
   _printf("atk_image_get_image_position on screen: x: %d y %d\n", x, y);

   g_assert(NULL == atk_image_get_image_description(ATK_IMAGE(obj)));
   g_assert(TRUE == atk_image_set_image_description(ATK_IMAGE(obj), desc_test));
   desc = atk_image_get_image_description(ATK_IMAGE(obj));
   _printf("atk_image_get_image_description: %s\n", desc ? desc : "NULL");
   g_assert(NULL != desc);
   g_assert_cmpstr(desc_test, ==, desc);

   atk_image_get_image_size(ATK_IMAGE(obj), &height, &width);
   _printf("atk_image_get_image_size: height %d width %d\n", height, width);

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
