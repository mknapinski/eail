/*
 * Tested interface: AtkObject
 *
 * Tested AtkObject: EailIndex
 *
 * Description: Test AtkObject interface
 *
 * Test input: accessible object representing EailIndex
 *
 * Expected test result: test should return 0 (success)
 */

#include <Elementary.h>
#include <atk/atk.h>

#include "eail_test_utils.h"

#define INDEX_NAME "index"

INIT_TEST("EailIndex")

static void
_test_index_child_for_object(AtkObject *obj, int index)
{
   g_assert(0 == atk_selection_get_selection_count(ATK_SELECTION(obj)));
   g_assert(atk_selection_add_selection(ATK_SELECTION(obj), index));

   g_assert(atk_selection_is_child_selected(ATK_SELECTION(obj), index));
   g_assert(1 == atk_selection_get_selection_count(ATK_SELECTION(obj)));

   g_assert(atk_selection_clear_selection(ATK_SELECTION(obj)));
   g_assert(0 == atk_selection_get_selection_count(ATK_SELECTION(obj)));
}
static void
_do_test(AtkObject *obj)
{
   int n_children = 0, i = 0;

   g_assert(ATK_IS_OBJECT(obj));
   atk_object_set_name(obj, INDEX_NAME"test");
   g_assert_cmpstr(atk_object_get_name(obj), ==, INDEX_NAME"test");
   atk_object_set_description(obj, "test");
   g_assert_cmpstr(atk_object_get_description(obj), ==, "test");

   g_assert(atk_object_get_role(obj) == ATK_ROLE_LIST);

   eailu_test_atk_focus(obj, FALSE);
   n_children = atk_object_get_n_accessible_children(obj);
   /* there should be 13 index entries */
   g_assert(13 == n_children);
   for (i = 0; i < n_children; ++i)
     {
        _test_index_child_for_object(obj, i);
     }

   eailu_test_code_called = 1;
}

struct _Idx_Data_Type
{
   Evas_Object *id;  /* Pointer to Index */
   Elm_Object_Item *item; /* Item we use for search */
};
typedef struct _Idx_Data_Type Idx_Data_Type;

struct _api_data
{
   unsigned int state;  /* What state we are testing       */
   Idx_Data_Type dt;
};
typedef struct _api_data api_data;

enum _api_state
{
   INDEX_LEVEL_SET,
   INDEX_ACTIVE_SET,
   INDEX_APPEND_RELATIVE,
   INDEX_PREPEND,
   INDEX_ITEM_DEL,
   INDEX_ITEM_FIND,
   INDEX_HORIZONTAL,
   INDEX_INDICATOR_DISABLED,
   INDEX_CLEAR,
   API_STATE_LAST
};
typedef enum _api_state api_state;

static Elm_Genlist_Item_Class itci;
char *gli_text_get(void *data, Evas_Object *obj, const char *part )
{
   char buf[256];
   int j = (uintptr_t)data;
   snprintf(buf, sizeof(buf), "%c%c",
            'A' + ((j >> 4) & 0xf),
            'a' + ((j     ) & 0xf)
            );
   return strdup(buf);
}

void
_index_delay_changed_cb(void *data, Evas_Object *obj, void *event_info)
{
   // called on a change but delayed in case multiple changes happen in a
   // short timespan
   elm_genlist_item_bring_in(elm_object_item_data_get(event_info),
                              ELM_GENLIST_ITEM_SCROLLTO_TOP);
}

void
_index_changed_cb(void *data, Evas_Object *obj, void *event_info)
{
   // this is called on every change, no matter how often
   // elm_genlist_item_bring_in(event_info);
}

void
_index_selected_cb(void *data, Evas_Object *obj, void *event_info)
{
   // called on final select
   const Elm_Object_Item *item = elm_index_selected_item_get(obj, 0);
   _printf("Index selected %s\n", elm_index_item_letter_get(item));
   elm_genlist_item_bring_in(elm_object_item_data_get(event_info),
                              ELM_GENLIST_ITEM_SCROLLTO_TOP);
}

static void
_cleanup_cb(void *data, Evas *e, Evas_Object *obj, void *event_info)
{
   free(data);
}

static void
id_cb(void *data, Evas_Object *obj, void *event_info)
{
   printf("Current Index : %s\n", elm_index_item_letter_get((const Elm_Object_Item *)event_info));
}

static void
_init_index(Evas_Object *win)
{
   Evas_Object *bxx, *gl, *id;
   Elm_Object_Item *glit;
   int i, j;
   api_data *api = calloc(1, sizeof(api_data));

   evas_object_event_callback_add(win, EVAS_CALLBACK_FREE, _cleanup_cb, api);

   bxx = elm_box_add(win);
   elm_win_resize_object_add(win, bxx);
   evas_object_size_hint_weight_set(bxx, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
   evas_object_show(bxx);

   gl = elm_genlist_add(win);
   evas_object_size_hint_weight_set(gl, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
   evas_object_size_hint_align_set(gl, EVAS_HINT_FILL, EVAS_HINT_FILL);
   evas_object_show(gl);

   api->dt.id = id = elm_index_add(win);
   evas_object_size_hint_weight_set(id, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
   evas_object_size_hint_align_set(id, EVAS_HINT_FILL, EVAS_HINT_FILL);
   elm_win_resize_object_add(win, id);

   evas_object_show(id);

   itci.item_style     = "default";
   itci.func.text_get = gli_text_get;
   itci.func.content_get  = NULL;
   itci.func.state_get = NULL;
   itci.func.del       = NULL;

   j = 0;
   for (i = 0; i < 100; i++)
     {
        glit = elm_genlist_item_append(gl, &itci,
                                       (void *)(uintptr_t)j/* item data */,
                                       NULL/* parent */,
                                       ELM_GENLIST_ITEM_NONE,
                                       NULL/* func */, NULL/* func data */);
        if (!(j & 0xf))
          {
             char buf[32];

             snprintf(buf, sizeof(buf), "%c", 'A' + ((j >> 4) & 0xf));
             elm_index_item_append(id, buf, id_cb, glit);
             if (*buf == 'G')  /* Just init dt->item later used in API test */
               api->dt.item = glit;
          }
        j += 2;
     }
   evas_object_smart_callback_add(id, "delay,changed", _index_delay_changed_cb, NULL);
   evas_object_smart_callback_add(id, "changed", _index_changed_cb, NULL);
   evas_object_smart_callback_add(id, "selected", _index_selected_cb, NULL);
   elm_index_level_go(id, 0);

   elm_box_pack_end(bxx, gl);

   evas_object_show(id);

   evas_object_resize(win, 320, 250);
}

EAPI_MAIN int
elm_main(int argc, char **argv)
{
   Evas_Object *win;

   win = eailu_create_test_window_with_glib_init(_on_done, _on_focus_in);
   _init_index(win);

   evas_object_show(win);

   elm_run();
   elm_shutdown();

   /* exit code */
   return 0;
}
ELM_MAIN()
