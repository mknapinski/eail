/*
 * Tested interface: AtkObject
 *
 * Tested AtkObject: EailBackground
 *
 * Description: Test AtkObject interface
 *
 * Test input: accessible object representing EailBackground
 *
 * Expected test result: test should return 0 (success)
 */

#include <Elementary.h>
#include <Ecore_X.h>
#include <atk/atk.h>

#include "eail_test_utils.h"

#define TEST_STR "bg-test"
#define IMG_PATH "./data/08.jpg"
#define X 50
#define Y 60
#define WIDTH 320
#define HEIGHT 280

INIT_TEST("EailBackground")

static void
_do_test(AtkObject *obj)
{
   AtkAttributeSet *attr_set, *node;
   AtkAttribute *p;

   g_assert(ATK_IS_OBJECT(obj));
   atk_object_set_description(obj, "test");
   g_assert_cmpstr(atk_object_get_description(obj), ==, "test");
   g_assert(atk_object_get_role(obj) == ATK_ROLE_IMAGE);

   atk_object_set_name(obj, "test name");
   g_assert_cmpstr(atk_object_get_name(obj), ==, "test name");

   attr_set = atk_object_get_attributes(obj);
   g_assert(g_slist_length(attr_set) == 3);

   node = g_slist_nth(attr_set, 0);
   p = (AtkAttribute *)node->data;
   g_assert_cmpstr(p->name, ==, "type");
   g_assert_cmpstr(p->value, ==, "elm_bg");

   node = g_slist_next(node);
   p = (AtkAttribute *)node->data;
   g_assert_cmpstr(p->name, ==, "file");
   g_assert_cmpstr(p->value, ==, IMG_PATH);

   node = g_slist_next(node);
   p = (AtkAttribute *)node->data;
   g_assert_cmpstr(p->name, ==, "display mode");
   g_assert_cmpstr(p->value, ==, "stretch");

   g_slist_free(attr_set);

   eailu_test_code_called = 1;
}

static void
_init_background(Evas_Object *win)
{
   Evas_Object *bg;
   char buf[PATH_MAX];

   bg = elm_bg_add(win);
   elm_bg_load_size_set(bg, 20, 20);
   elm_bg_option_set(bg, ELM_BG_OPTION_STRETCH);
   snprintf(buf, sizeof(buf), IMG_PATH);
   elm_bg_file_set(bg, buf, NULL);
   evas_object_size_hint_weight_set(bg, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
   elm_win_resize_object_add(win, bg);
   evas_object_show(bg);
   evas_object_resize(win, WIDTH, HEIGHT);
}

EAPI_MAIN int
elm_main(int argc, char **argv)
{
   Evas_Object *win;

   win = eailu_create_test_window_with_glib_init(_on_done, _on_focus_in);
   _init_background(win);
   evas_object_show(win);
   elm_run();
   elm_shutdown();

   return 0;
}
ELM_MAIN()
