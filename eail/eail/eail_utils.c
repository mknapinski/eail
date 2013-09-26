/*
 * Copyright (c) 2013 Samsung Electronics Co., Ltd.
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this library; see the file COPYING.LIB.  If not, write to
 * the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
 * Boston, MA 02110-1301, USA.
 */

/**
 * @file eail_utils.c
 * @brief Implementation of Utility Functions
 */

/* enabling beta API support for Eo parts*/
#define EFL_BETA_API_SUPPORT

#include <atk/atk.h>
#include <Elementary.h>
#include <Eina.h>
#include <Evas.h>

/* internal elm_widget api for listening EDJE parts */

#define ELM_INTERNAL_API_ARGESFSDFEFC
#include <elm_widget.h>

#include "eail_utils.h"
#include "eail_factory.h"
#include "eail_dynamic_content.h"
#include "eail_priv.h"

/**
 * @param string base string to get substring from
 * @param start_offset beginning offset
 * @param end_offset end offset
 *
 * @returns newly allocated substring
 */
gchar *
eail_get_substring(const gchar *string,
                   gint         start_offset,
                   gint         end_offset)
{
   gchar *substring = NULL;
   gint len = 0;
   gint sub_len = 0;

   if (!string) return NULL;

   len = g_utf8_strlen(string, -1);

   if ((start_offset <  0) ||
      (start_offset > len - 1) ||
      (end_offset   <  -1))
    return NULL;
   else if (end_offset == -1 ||
           end_offset >= len - 1)
     sub_len = len - start_offset + 1;
   else
     sub_len = end_offset - start_offset;

   substring = g_malloc0(sub_len + 1);

   return g_utf8_strncpy(substring, &string[start_offset], sub_len);
}

/**
 * @param widget Evas_Object instance for getting state set
 * @param state_set current state set taken from object's parent
 *
 * @returns AtkStateSet representing the given Evas_Object
 */
AtkStateSet *
eail_evas_obj_ref_state_set(Evas_Object *widget, AtkStateSet *state_set)
{
   if (!widget) {
       atk_state_set_add_state(state_set, ATK_STATE_DEFUNCT);
       return state_set;
   }

   if (!elm_object_disabled_get(widget)) {
       atk_state_set_add_state(state_set, ATK_STATE_SENSITIVE);
       atk_state_set_add_state(state_set, ATK_STATE_ENABLED);
   }

   if (evas_object_visible_get(widget)) {
       int x, y, width, height;
       int vp_x, vp_y, vp_width, vp_height;

       atk_state_set_add_state(state_set, ATK_STATE_VISIBLE);

       evas_object_geometry_get(widget, &x, &y, &width, &height);
       evas_output_viewport_get(evas_object_evas_get(widget),
                                &vp_x, &vp_y, &vp_width, &vp_height);

       if ((x + width) >= vp_x && (y + height) >= vp_y &&
           (vp_x + vp_width) >= x && (vp_y + vp_height) >= y) {
           atk_state_set_add_state(state_set, ATK_STATE_SHOWING);
       }
   }

   if (elm_object_focus_allow_get(widget)) {
       atk_state_set_add_state(state_set, ATK_STATE_FOCUSABLE);

       if (elm_object_focus_get(widget)) {
           atk_state_set_add_state(state_set, ATK_STATE_FOCUSED);
       }
   }

   return state_set;
}

/**
 * @param widget Evas_Object instance
 * @return TRUE if grabbing focus was successfull, FALSE otherwise
 */
gboolean
eail_evas_obj_grab_focus(Evas_Object *widget)
{
   Ecore_Evas *ee = ecore_evas_ecore_evas_get(evas_object_evas_get(widget));

   if (!widget || !elm_object_focus_allow_get(widget)) {
         return FALSE;
   }

   ecore_evas_activate(ee);
   elm_object_focus_set(widget, EINA_TRUE);

   return TRUE;
}

/**
 * @param widget Evas_Object instance to press on
 * @param x x coordinate
 * @param y y coordinate
 */
void
eail_mouse_press_on_coords(Evas_Object *widget, int x, int y)
{
   Evas* evas = NULL;
   evas =  evas_object_evas_get(widget);

   evas_event_feed_mouse_move(evas, x, y, 0, NULL);
   evas_event_feed_mouse_down(evas, 1, EVAS_BUTTON_NONE, 0, NULL);
}

/**
 * @param widget Evas_Object instance to press on
 * @param x x coordinate
 * @param y y coordinate
 */
void
eail_mouse_release_on_coords(Evas_Object *widget, int x, int y)
{
   Evas* evas = NULL;
   evas =  evas_object_evas_get(widget);

   evas_event_feed_mouse_move(evas, x, y, 0, NULL);
   evas_event_feed_mouse_up(evas, 1, EVAS_BUTTON_NONE, 0, NULL);
}

/**
 * @param widget Evas_Object instance to click on
 * @param x x coordinate
 * @param y y coordinate
 */
void
eail_mouse_click_on_coords(Evas_Object *widget, int x, int y)
{
   Evas* evas = NULL;
   evas =  evas_object_evas_get(widget);

   evas_event_feed_mouse_move(evas, x, y, 0, NULL);
   evas_event_feed_mouse_down(evas, 1, EVAS_BUTTON_NONE, 0, NULL);
   evas_event_feed_mouse_up(evas, 1, EVAS_BUTTON_NONE, 0, NULL);
}

/**
 * @param widget Evas_Object instance to get values for
 * @param [out] x x coordinate
 * @param [out] y Y coordinate
 */
void
eail_get_coords_widget_center(Evas_Object *widget, int *x, int *y)
{
   int w, h;
   evas_object_geometry_get(widget, x, y, &w, &h);

   *x = *x + (w / 2);
   *y = *y + (h / 2);
}

/**
 * @param item Elm_Object_Item* instance
 *
 * @returns Eina_List * representing the list of raw Evas_Object*
 * (not every returned Evas_Object is a widget - widget-only content
 * is returned by eail_get_edje_parts_for_item function)
 */
Eina_List *
eail_get_raw_evas_obj_list_from_item(Elm_Object_Item *item)
{
   Evas_Object *edje = NULL;

   edje = VIEW(item);
   if (!edje)
     {
        DBG("Edje object for item not found. Returning empty list");\
        return NULL;
     }

    return evas_object_smart_members_get(edje);
}

/**
 *
 * The returned list has to be freed when no longer needed but DO NOT
 * FREE CONTENT STRINGS.
 *
 * @param item Elm_Object_Item * to get strings from
 * @returns Eina_List representing the list of string that represent text
 * content of item
 */
Eina_List *
eail_item_get_content_strings(Elm_Object_Item *item)
{
   Eina_List *strings_list = NULL;
   Eina_List *edje_parts = NULL;
   Evas_Object *part = NULL;
   Eina_List *l;


   edje_parts = eail_get_raw_evas_obj_list_from_item(item);
   EINA_LIST_FOREACH(edje_parts, l, part)
     {
      const gchar *type_name = evas_object_type_get(part);

      if (0 == strcmp(type_name, "text"))
        {
           const gchar *text = evas_object_text_text_get(part);

           if (text)
             strings_list = eina_list_append(strings_list, text);
        }
     }

   return strings_list;
}

/**
 * It does filtering inside and returs only parts that can be used later in
 * eail factory (only widgets will be returned).
 *
 * @param item Elm_Object_Item instance to get objects from
 *
 * @returns an Eina_List filled with Evas_Object* objects representing content
 * found inside item
 */
Eina_List *
eail_get_edje_parts_for_item(Elm_Object_Item *item)
{
   Eina_List *edje_parts = NULL;
   Eina_List *usable_parts = NULL;
   int i = 0;

   edje_parts = eail_get_raw_evas_obj_list_from_item(item);
   for (i = 0; i < eina_list_count(edje_parts); ++i)
     {
        Evas_Object *obj = eina_list_nth(edje_parts, i);

        /* adding only parts that can be used by eail_factory later */
        if (elm_object_widget_check(obj))
          usable_parts = eina_list_append(usable_parts, obj);
     }

   eina_list_free(edje_parts);

   return usable_parts;
}

/**
 *
 * @param widget Evas_Object instance
 * @param type type of scroll action
 *
 * @returns TRUE if scroll action was successful, FALSE otherwise
 */
gboolean
eail_handle_scroll(Evas_Object *widget,
                   enum EAIL_SCROLL_TYPE type)
{
   int x, y, w, h;

   elm_scroller_region_get(widget, &x, &y, &w, &h);

   if (EAIL_SCROLL_TYPE_UP == type)
      y = y - h;
   else if (EAIL_SCROLL_TYPE_DOWN == type)
      y = y + h;
   else if (EAIL_SCROLL_TYPE_RIGHT == type)
      x = x + w;
   else if (EAIL_SCROLL_TYPE_LEFT == type)
      x = x - w;

   elm_scroller_region_bring_in(widget, x, y, w, h);

   return TRUE;
}

/**
 *
 * @param widget Evas_Object* instance
 * @param data additional action data
 * @returns TRUE if scroll action was successful, FALSE otherwise
 */
gboolean
eail_action_scroll_up(Evas_Object *widget,
                      void *data)
{
   return eail_handle_scroll(widget, EAIL_SCROLL_TYPE_UP);
}

/**
 *
 * @param widget Evas_Object* instance
 * @param data additional action data
 *
 * @returns TRUE if scroll action was successful, FALSE otherwise
 */
gboolean
eail_action_scroll_down(Evas_Object *widget,
                        void *data)
{
   return eail_handle_scroll(widget, EAIL_SCROLL_TYPE_DOWN);
}

/**
 *
 * @param widget Evas_Object* instance
 * @param data additional action data
 *
 * @returns TRUE if scroll action was successful, FALSE otherwise
 */
gboolean
eail_action_scroll_left(Evas_Object *widget,
                        void *data)
{
   return eail_handle_scroll(widget, EAIL_SCROLL_TYPE_LEFT);
}

/**
 *
 * @param widget Evas_Object* instance
 * @param data additional action data
 *
 * @returns TRUE if scroll action was successful, FALSE otherwise
 */
gboolean
eail_action_scroll_right(Evas_Object *widget,
                         void *data)
{
   return eail_handle_scroll(widget, EAIL_SCROLL_TYPE_RIGHT);
}

/**
 * @param atk_obj AtkObject instance that emits the signal
 * @param signal_name name of signal
 * @param object_type GType of object
 */
void
eail_emit_atk_signal(AtkObject *atk_obj, const gchar *signal_name,
                     GType object_type)
{
   guint signal = g_signal_lookup (signal_name,object_type);
   if (!signal)
     {
        ERR("No signal with name %s was found", signal_name);
        return;
     }

   DBG("Raising %s signal", signal_name);
   g_signal_emit (atk_obj, signal, 0);
}

/**
 * @param added boolean used for marking if child is added. TRUE if child was
 * added, FALSE when child was removed
 * @param atk_obj AtkObject instance that emits the signal
 * @param child_number index number of changed child
 */
void
eail_emit_children_changed(gboolean added, AtkObject *atk_obj, gint child_number)
{

   DBG("Emitting child-changed for index %d. ADDED: %d", child_number, added);

   if (added)
     g_signal_emit_by_name
                 (atk_obj, "children_changed::add", child_number, NULL, NULL);
   else
     g_signal_emit_by_name
               (atk_obj, "children_changed::remove", child_number, NULL, NULL);

}

/**
 * @param added boolean used for marking if child is added. TRUE if child was
 * added, FALSE when child was removed
 * @param atk_obj an AtkObject that emits the signal
 * @param changed_obj pointer to object that has been added/removed
 */
void
eail_emit_children_changed_obj(gboolean added,
                               AtkObject *atk_obj,
                               AtkObject *changed_obj)
{

   DBG("Emitting child-changed for obj. ADDED: %d", added);

   if (added)
     g_signal_emit_by_name
                 (atk_obj, "children_changed::add", NULL, changed_obj, NULL);
   else
     g_signal_emit_by_name
               (atk_obj, "children_changed::remove", NULL, changed_obj, NULL);

}
/**
 * @brief Handles 'selected' state changes for item
 *
 * @param item Elm_Object instance
 * @param selected value of 'selected' state
 * @param parent AtkObject that holds item inside
 * @param role role of item
 */
static void
_eail_handle_selected_for_item(Elm_Object_Item *item,
                               gboolean selected,
                               AtkObject *parent,
                               AtkRole role)
{
   AtkObject *atk_item_obj = NULL;
   if (!item && !parent)
     {
        ERR("Wrong parameters passed. Ignoring event");
        return;
     }

   atk_item_obj = eail_factory_get_item_atk_obj(item, role, parent);
   if (!atk_item_obj)
     {
        DBG("Couldn't find factory obj for item. Ignoring 'selected' event");
        return;
     }

   atk_object_notify_state_change(atk_item_obj, ATK_STATE_SELECTED, selected);
}

/**
 * @brief Function that calls apropriate handling for selected event
 *
 * @param data passed to callback
 * @param obj Evas_Object that raised event
 * @param event_info additional event info
 * @param selected selected state
 */
static void
_eail_list_item_do_handle_selected_event(void *data,
                                         Evas_Object *obj,
                                         void *event_info,
                                         gboolean selected)
{
   Elm_Object_Item *obj_item = (Elm_Object_Item*)event_info;
   if (!obj_item)
     {
        DBG("obj_item not found in event_info. Ignoring event..");
        return;
     }

   eail_notify_child_focus_changes();
   _eail_handle_selected_for_item
                     (obj_item, selected, ATK_OBJECT(data), ATK_ROLE_LIST_ITEM);
}

/**
 *
 * @param data passed to callback
 * @param obj Evas_Object that raised event
 * @param event_info additional event info
 */
void
eail_list_item_handle_selected_event(void *data,
                                      Evas_Object *obj,
                                      void *event_info)
{
   _eail_list_item_do_handle_selected_event(data, obj, event_info, TRUE);
}

/**
 *
 * @param data passed to callback
 * @param obj Evas_Object that raised event
 * @param event_info additional event info
 */
void
eail_list_item_handle_unselected_event(void *data,
                                       Evas_Object *obj,
                                       void *event_info)
{
   _eail_list_item_do_handle_selected_event(data, obj, event_info, FALSE);
}

/**
 * @brief Notifies objects with given role about possible hierarchy changes
 *
 * @param data an AtkRole
 *
 * @returns always FALSE
 */
static gboolean
_notifiy_content_holders_by_type(gpointer data)
{
   Eina_List *objs = NULL, *l = NULL;
   AtkObject *atk_obj = NULL;
   AtkRole role = ATK_ROLE_INVALID;

   role = (AtkRole)data;

   if (ATK_ROLE_APPLICATION == role)
     {
         atk_obj = atk_get_root();
         if (atk_obj && EAIL_IS_DYNAMIC_CONTENT(atk_obj))
            eail_dynamic_content_update_hierarchy(EAIL_DYNAMIC_CONTENT(atk_obj));

         return FALSE;
     }
   else
     {
        objs = eail_factory_find_objects_with_role(role);
        EINA_LIST_FOREACH(objs, l, atk_obj)
          {
            if (EAIL_IS_DYNAMIC_CONTENT(atk_obj))
              {
                   eail_dynamic_content_update_hierarchy
                                               (EAIL_DYNAMIC_CONTENT(atk_obj));
              }
          }
        eina_list_free(objs);
     }

   return FALSE;
}

/**
 * @brief Sends notification about possible hierarchy changes to objects
 * with dynamic content interface
 *
 * @param delay value in miliseconds, objects will be notified after this
 * timeout
 */
static void
_eail_notify_focus_listeners_delayed(guint delay)
{
   g_timeout_add
      (delay, _notifiy_content_holders_by_type, (gpointer)ATK_ROLE_APPLICATION);
   g_timeout_add
      (delay, _notifiy_content_holders_by_type, (gpointer)ATK_ROLE_WINDOW);
   g_timeout_add
      (delay, _notifiy_content_holders_by_type, (gpointer)ATK_ROLE_FILLER);
}

void
eail_notify_child_focus_changes(void)
{
   _eail_notify_focus_listeners_delayed(0);
   /* sending delayed notifications to objects after hierarchy has
    * been fully established*/
   _eail_notify_focus_listeners_delayed(1600);
}
