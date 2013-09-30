/*
 * Tested interface: AtkAction
 *
 * Tested AtkObject: EailSlideshow
 *
 * Description: Test AtkSelection interface
 *
 * Test input: accessible object representing EailSlideshow
 *
 * Expected test result: test should return 0 (success)
 */

#include <Elementary.h>
#include <atk/atk.h>

#include "eail_test_utils.h"

INIT_TEST("EailSlideshow")

static Evas_Object *slideshow;
static Elm_Slideshow_Item_Class itc;

static char img1[256], img2[256], img3[256], img4[256], img5[256], img6[256],
            img7[256], img8[256], img9[256];

/* get our images to make slideshow items */
static Evas_Object *
_get(void        *data,
     Evas_Object *obj)
{
   Evas_Object *photo = elm_photo_add(obj);
   elm_photo_file_set(photo, data);
   elm_photo_fill_inside_set(photo, EINA_TRUE);
   elm_object_style_set(photo, "shadow");

   return photo;
}

/* ordering alphabetically */
static int
_cmp_func(const void *data1,
          const void *data2)
{
   const char *img_path1, *img_path2;

   const Elm_Object_Item *slide_it1 = data1;
   const Elm_Object_Item *slide_it2 = data2;

   img_path1 = elm_object_item_data_get(slide_it1);
   img_path2 = elm_object_item_data_get(slide_it2);

   return strcasecmp(img_path1, img_path2);
}

static void
_do_test(AtkObject *obj)
{
   int child_count, selection_count, action_count;
   AtkObject *child, *selection;
   gboolean result;
   const gchar *action_name, *action_description;
   /*sometimes this is called several times*/
   static int tested = 0;
   if (tested > 0) return;
   tested++;

   g_object_ref(obj);

   g_assert(eailu_is_object_with_role(obj, ATK_ROLE_DOCUMENT_PRESENTATION));

   child_count = atk_object_get_n_accessible_children(obj);
   g_assert(child_count == 9);

   for (int i = 0; i < child_count; i++)
     {
        child = atk_object_ref_accessible_child(obj, i);
        g_assert(eailu_is_object_with_role(child, ATK_ROLE_IMAGE));
        g_object_unref(child);
     }

   /*test AtkSelectionIface*/
   g_assert(ATK_IS_SELECTION(obj));
   selection = atk_selection_ref_selection(ATK_SELECTION(obj), 0);
   /*this may fail due to a problem with  slideshow cache*/
   g_assert(ATK_IS_OBJECT(selection));
   g_assert(eailu_is_object_with_role(selection, ATK_ROLE_IMAGE));
   g_object_unref(selection);
   selection_count = atk_selection_get_selection_count(ATK_SELECTION(obj));
   g_assert(selection_count == 1);
   result = atk_selection_add_selection(ATK_SELECTION(obj), 5);
   g_assert(result);
   result = atk_selection_is_child_selected(ATK_SELECTION(obj), 5);
   g_assert(result);

   /*test AtkActionIface*/
   g_assert(ATK_IS_ACTION(obj));
   /* test set/get action description */
   eailu_test_action_description_all(ATK_ACTION(obj));
   action_count = atk_action_get_n_actions(ATK_ACTION(obj));
   g_assert(4 == action_count);
   action_name = atk_action_get_name(ATK_ACTION(obj), 0);
   g_assert(!strcmp("next", action_name));
   action_name = atk_action_get_name(ATK_ACTION(obj), 1);
   g_assert(!strcmp("previous", action_name));
   result = atk_action_set_description(ATK_ACTION(obj), 2,
                                       "start the slideshow");
   g_assert(result);
   action_description = atk_action_get_description(ATK_ACTION(obj), 2);
   g_assert(!strcmp("start the slideshow", action_description));
   result = atk_action_do_action(ATK_ACTION(obj), 0);
   g_assert(result);
   result = atk_selection_is_child_selected(ATK_SELECTION(obj), 6);
   g_assert(result);

   eailu_test_atk_focus(obj, TRUE);

   g_object_unref(obj);
   eailu_test_code_called = 1;
}

static void
_init_slideshow(Evas_Object *win)
{
   Evas_Object *bg;
   Elm_Object_Item *it;
   const char *data_dir;

   data_dir = "./data";
   g_snprintf(img1, sizeof(img1), "%s/01.jpg", data_dir);
   g_snprintf(img2, sizeof(img2), "%s/02.jpg", data_dir);
   g_snprintf(img3, sizeof(img3), "%s/03.jpg", data_dir);
   g_snprintf(img4, sizeof(img4), "%s/04.jpg", data_dir);
   g_snprintf(img5, sizeof(img5), "%s/05.jpg", data_dir);
   g_snprintf(img6, sizeof(img6), "%s/06.jpg", data_dir);
   g_snprintf(img7, sizeof(img7), "%s/07.jpg", data_dir);
   g_snprintf(img8, sizeof(img8), "%s/08.jpg", data_dir);
   g_snprintf(img9, sizeof(img9), "%s/09.jpg", data_dir);

   evas_object_resize(win, 600, 400);

   bg = elm_bg_add(win);
   evas_object_size_hint_weight_set(bg, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
   elm_win_resize_object_add(win, bg);
   evas_object_show(bg);

   slideshow = elm_slideshow_add(win);
   elm_slideshow_loop_set(slideshow, EINA_TRUE);
   elm_slideshow_cache_before_set(slideshow, 8);
   elm_slideshow_cache_after_set(slideshow, 8);
   evas_object_size_hint_weight_set(slideshow,
                                    EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
   elm_win_resize_object_add(win, slideshow);
   evas_object_show(slideshow);

   itc.func.get = _get;
   itc.func.del = NULL;

   elm_slideshow_item_sorted_insert(slideshow, &itc, img1, _cmp_func);
   elm_slideshow_item_sorted_insert(slideshow, &itc, img2, _cmp_func);
   elm_slideshow_item_sorted_insert(slideshow, &itc, img3, _cmp_func);
   elm_slideshow_item_sorted_insert(slideshow, &itc, img4, _cmp_func);
   elm_slideshow_item_sorted_insert(slideshow, &itc, img5, _cmp_func);
   elm_slideshow_item_sorted_insert(slideshow, &itc, img6, _cmp_func);
   elm_slideshow_item_sorted_insert(slideshow, &itc, img7, _cmp_func);
   elm_slideshow_item_sorted_insert(slideshow, &itc, img8, _cmp_func);
   elm_slideshow_item_sorted_insert(slideshow, &itc, img9, _cmp_func);

   /*show last item, otherwise tests fail*/
   it = elm_slideshow_item_nth_get(slideshow, 8);
   elm_slideshow_item_show(it);
}

EAPI_MAIN int
elm_main(int    argc,
         char **argv)
{

   Evas_Object *win;
   win = eailu_create_test_window_with_glib_init(_on_done, _on_focus_in);
   _init_slideshow(win);

   evas_object_show(win);
   elm_run();
   elm_shutdown();

   return 0;
}
ELM_MAIN()
