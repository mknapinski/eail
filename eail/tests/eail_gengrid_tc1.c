/*
 * Tested interface: AtkObject
 *
 * Tested AtkObject: EailGengrid
 *
 * Description: Test AtkObject interface
 *
 * Test input: accessible object representing EailGengrid
 *
 * Expected test result: test should return 0 (success)
 */

#include <Elementary.h>
#include <atk/atk.h>

#include "eail_test_utils.h"

INIT_TEST("EailGengrid")

typedef struct _Example_Item
{
   const char *path;
} Example_Item;

static Elm_Gengrid_Item_Class *gic = NULL;
static Evas_Object *grid;

static const char *imgs[9] =
{
   "./data/01.jpg",
   "./data/02.jpg",
   "./data/03.jpg",
   "./data/04.jpg",
   "./data/05.jpg",
   "./data/06.jpg",
   "./data/07.jpg",
   "./data/08.jpg",
   "./data/09.jpg"
};

static char *
_grid_label_get(void *data, Evas_Object *obj, const char *part)
{
   const Example_Item *it = data;
   char buf[256];
   g_snprintf(buf, sizeof(buf), "Photo %s", it->path);
   return strdup(buf);
}

static Evas_Object *
_grid_content_get(void *data, Evas_Object *obj, const char *part)
{
   const Example_Item *it = data;
   if (!strcmp(part, "elm.swallow.icon"))
     {
        Evas_Object *icon = elm_bg_add(obj);
        char buf[256];
        g_snprintf(buf, sizeof(buf),"%s", it->path);
        elm_bg_file_set(icon, buf, NULL);
        evas_object_size_hint_aspect_set(icon, EVAS_ASPECT_CONTROL_VERTICAL, 1, 1);
        evas_object_show(icon);
        return icon;
     }
   return NULL;

}

static Eina_Bool
_grid_state_get(void *data, Evas_Object *obj, const char *part)
{
   return EINA_FALSE;
}

static void
_grid_del(void *data, Evas_Object *obj)
{
   Example_Item *it = data;
   eina_stringshare_del(it->path);
   free(it);
}

static Example_Item *
_item_new(unsigned int index)
{
   Example_Item *it = malloc(sizeof(*it));
   if (index <= 8)
     it->path = eina_stringshare_add(imgs[index]);
   else
     it->path = eina_stringshare_add(imgs[0]);
   return it;
}

void _on_selected(void *data, Evas_Object *obj, void *event_info)
{
   _printf("selected item with handle: %p\n", event_info);
}

static void
_print_object_info(AtkObject *obj)
{
   AtkRole atk_role = atk_object_get_role(obj);
   const char *name = atk_object_get_name(obj);
   const char *type_name = g_type_name(G_TYPE_FROM_INSTANCE(obj));
   const char *role_name = atk_role_get_name(atk_role);

   _printf("atk_object_get_name: %s\n", name ? name : "NULL");
   _printf("atk_object_get_role: %s\n", role_name ? role_name : "NULL");
   _printf("atk_object_get_type_name: %s\n", type_name ? type_name : "NULL");
}

static void
_do_test(AtkObject *obj)
{
   int child_count = atk_object_get_n_accessible_children(obj);

   g_assert(ATK_IS_OBJECT(obj));

   atk_object_set_description(obj, "test");
   g_assert_cmpstr(atk_object_get_description(obj), ==, "test");

   atk_object_set_name(obj, "test name");
   g_assert_cmpstr(atk_object_get_name(obj), ==, "test name");

   g_assert(atk_object_get_role(obj) == ATK_ROLE_TABLE);

   for (int i = 0; i < child_count; i++)
     {
        AtkObject *child = atk_object_ref_accessible_child(obj, i);
        _print_object_info(child);
        g_assert(child);
        g_object_unref(child);
     }

   eailu_test_code_called = 1;
}
static Ecore_Timer *timer_delay;

static Eina_Bool delay_test_run(void *data)
{
   Eina_List *l, *list = elm_gengrid_realized_items_get(data);
   Elm_Object_Item *it;
   int i = 0;
   Evas_Object *win = elm_object_parent_widget_get(data);
   EINA_LIST_FOREACH(list, l, it)
     {
        unsigned int x, y;
        elm_gengrid_item_pos_get(it, &x, &y);
        i++;
     }
   _on_focus_in(NULL, win, NULL);
   return EINA_FALSE;
}

static void
_init_gengrid(Evas_Object *win)
{
   Evas_Object *bg;

   bg = elm_bg_add(win);
   evas_object_size_hint_weight_set(bg, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
   elm_win_resize_object_add(win, bg);
   evas_object_show(bg);
   grid = elm_gengrid_add(win);

   elm_gengrid_item_size_set(grid, 200, 200);
   evas_object_size_hint_weight_set(grid, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
   elm_win_resize_object_add(win, grid);
   evas_object_smart_callback_add(grid, "selected", _on_selected, NULL);
   elm_gengrid_horizontal_set(grid, EINA_FALSE);
   elm_gengrid_multi_select_set(grid, EINA_TRUE);
   evas_object_show(grid);

   if (!gic)
     {
        gic = elm_gengrid_item_class_new();
        gic->item_style = "default";
        gic->func.text_get = _grid_label_get;
        gic->func.content_get = _grid_content_get;
        gic->func.state_get = _grid_state_get;
        gic->func.del = _grid_del;
     } // we only create the first time its needed. we dont unref/free

   elm_gengrid_item_append(grid, gic, _item_new(0), NULL, NULL);
   elm_gengrid_item_append(grid, gic, _item_new(1), NULL, NULL);
   elm_gengrid_item_append(grid, gic, _item_new(2), NULL, NULL);
   elm_gengrid_item_append(grid, gic, _item_new(3), NULL, NULL);
   elm_gengrid_item_append(grid, gic, _item_new(4), NULL, NULL);
   elm_gengrid_item_append(grid, gic, _item_new(5), NULL, NULL);
   elm_gengrid_item_append(grid, gic, _item_new(6), NULL, NULL);
   elm_gengrid_item_append(grid, gic, _item_new(7), NULL, NULL);
   elm_gengrid_item_append(grid, gic, _item_new(8), NULL, NULL);
   evas_object_resize(win, 800, 600);
}

EAPI_MAIN int
elm_main(int argc, char **argv)
{
   Evas_Object *win;

   win = eailu_create_test_window_with_glib_init(_on_done, NULL);
   _init_gengrid(win);
   evas_object_show(win);

   timer_delay = ecore_timer_add(1, delay_test_run, grid);

   elm_run();
   elm_shutdown();
   return 0;
}
ELM_MAIN()
