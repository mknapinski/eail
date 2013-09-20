/*
 * Tested interface: AtkObject
 *
 * Tested AtkObject: EailMap
 *
 * Description: Test AtkObject interface
 *
 * Test input: accessible object representing EailMap
 *
 * Expected test result: test should return 0 (success)
 */

#include <Elementary.h>
#include <atk/atk.h>

#include "eail_test_utils.h"

static int test_passed;

static void
_on_done(void *data, Evas_Object *object, void *event_info)
{
   elm_exit();
}

static void
_do_test(AtkObject *object)
{
   int w,h;
   gboolean result;
   const gchar *description;

   g_assert(eailu_is_object_with_role(object, ATK_ROLE_IMAGE_MAP));
   g_assert(atk_object_get_name(object) != NULL);
   atk_image_get_image_size(ATK_IMAGE(object), &w, &h);
   g_assert(w == 512);
   g_assert(h == 512);
   result = atk_image_set_image_description(ATK_IMAGE(object),
                                            "This is a map");
   g_assert(result);
   description = atk_image_get_image_description(ATK_IMAGE(object));
   g_assert(!strcmp(description, "This is a map"));

   test_passed++;
}

static void
_on_map_loaded(void *data, Evas_Object *obj, void *event_info)
{
   AtkObject *aobj = atk_get_root();
   if (aobj)
     eailu_traverse_children_for_type_call_cb(aobj, "EailMap", _do_test);

   g_assert(test_passed);

   _on_done(NULL, obj, NULL);
}

static void
_init_map(Evas_Object *win)
{
   Evas_Object *bg, *map;

   bg = elm_bg_add(win);
   elm_win_resize_object_add(win, bg);
   evas_object_size_hint_weight_set(bg, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
   evas_object_show(bg);

   map = elm_map_add(win);
   elm_win_resize_object_add(win, map);
   evas_object_size_hint_weight_set(map, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
   evas_object_show(map);

   evas_object_smart_callback_add(map, "loaded", _on_map_loaded, NULL);
   elm_map_zoom_set(map, 6);
   elm_map_region_show(map, 19.45, 51.75);
   evas_object_resize(win, 512, 512);
}

EAPI_MAIN int
elm_main(int argc, char **argv)
{
   Evas_Object *win;

   win = eailu_create_test_window_with_glib_init(NULL, NULL);
   _init_map(win);
   evas_object_show(win);

   elm_run();
   elm_shutdown();

   return 0;
}
ELM_MAIN()
