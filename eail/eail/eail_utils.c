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
 * @param item Elm_Object_Item* instance
 *
 * @returns Evas_Object* representing the edje object related to item
 */
Evas_Object *
eail_get_edje_obj_from_item(Elm_Object_Item *item)
{
   Evas_Object *edje = NULL;

   edje = VIEW(item);
   if (!edje)
      DBG("Edje object for item not found.");

   return edje;
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
   g_signal_emit_by_name (parent, "selection_changed");
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

/**
 * @brief Returns the position that is obtained by adding count to the
 * given offset
 *
 * Count may be positive or negative.
 *
 * @param textblock Evas textblock
 * @param offset character offset
 * @param count number of characters to move from offset
 * @returns integer representing the new position
 */
static gint
_eail_move_chars(const Evas_Object *textblock,
                 gint offset,
                 gint count)
{
   Evas_Textblock_Cursor *cur;
   gint dir;
   gboolean res;

   dir = count > 0 ? 1 : -1;
   cur = evas_object_textblock_cursor_new(textblock);
   evas_textblock_cursor_pos_set(cur, offset);

   while(count)
     {
        res = dir > 0 ? evas_textblock_cursor_char_next(cur) :
            evas_textblock_cursor_char_prev(cur);
        if (!res) break;

        count -= dir;
     }

   offset = evas_textblock_cursor_pos_get(cur);
   evas_textblock_cursor_free(cur);

   return offset;
}

/**
 * @brief Gets the utf8 character at offset
 *
 * @param textblock Evas textblock
 * @param offset character offset
 * @returns char representing the utf8 character
 */
static gunichar
_eail_get_unichar_at_offset(const Evas_Object *textblock,
                            int offset)
{
   Evas_Textblock_Cursor *cur;
   gchar *s;
   gunichar c;

   cur = evas_object_textblock_cursor_new(textblock);
   evas_textblock_cursor_pos_set(cur, offset);
   s = evas_textblock_cursor_content_get(cur);
   c = g_utf8_get_char(s);

   evas_textblock_cursor_free(cur);
   g_free(s);

   return c;
}

/**
 * @brief Checks whether the character at offset in textblock is a word's start
 *
 * @param textblock Evas textblock
 * @param offset character offset
 * @returns TRUE on success, FALSE otherwise
 */
static gboolean
_eail_is_word_start(const Evas_Object *textblock,
                    gint offset)
{
   /* first character in a word */
   Evas_Textblock_Cursor *cur = NULL;
   cur = evas_object_textblock_cursor_new(textblock);
   evas_textblock_cursor_pos_set(cur, offset);
   evas_textblock_cursor_word_start(cur);
   gint pos = evas_textblock_cursor_pos_get(cur);
   evas_textblock_cursor_free(cur);

   return pos == offset;
}

/**
 * @brief Checks whether the character at offset in textblock is a word's end
 *
 * @param textblock Evas textblock
 * @param offset character offset
 * @return TRUE on success, FALSE otherwise
 */
static gboolean
_eail_is_word_end(const Evas_Object *textblock,
                  gint offset)
{
   /* is first non-word char after a word */
   Evas_Textblock_Cursor *cur = NULL;
   cur = evas_object_textblock_cursor_new(textblock);
   evas_textblock_cursor_pos_set(cur, offset - 2);
   evas_textblock_cursor_word_end(cur);
   gint pos = evas_textblock_cursor_pos_get(cur);
   evas_textblock_cursor_free(cur);

   return (pos + 1 == offset);
}

/**
 * @brief Checks whether the character at offset in textblock is inside a word
 *
 * @param textblock Evas textblock
 * @param offset character offset
 * @returns TRUE on success, FALSE otherwise
 */
static gboolean
_eail_is_inside_word(const Evas_Object *textblock,
                     gint offset)
{
   Evas_Textblock_Cursor *cur = NULL;
   cur = evas_object_textblock_cursor_new(textblock);
   evas_textblock_cursor_pos_set(cur, offset);
   evas_textblock_cursor_word_start(cur);
   gint pos_start = evas_textblock_cursor_pos_get(cur);
   evas_textblock_cursor_char_prev(cur);
   evas_textblock_cursor_word_end(cur);
   gint pos_end = evas_textblock_cursor_pos_get(cur);
   evas_textblock_cursor_free(cur);

   return (pos_start <= offset && pos_end >= offset);
}

/**
 * @brief Gets the texblock length
 *
 * @param textblock Evas textblock
 * @returns integer representing the textblock length
 */
static gint
_eail_get_len(const Evas_Object *textblock)
{
   Evas_Textblock_Cursor *cur;
   int ctr = 0;

   cur = evas_object_textblock_cursor_new(textblock);
   while (evas_textblock_cursor_char_next(cur))
     ++ctr;

   evas_textblock_cursor_free(cur);

   return ctr;
}

/**
 * @brief Returns the position that is count words from the given offset
 *
 * Count may  be positive or negative. If count is positive, the returned
 * position will be a word end, otherwise it will be a word start.
 *
 * @param textblock Evas textblock
 * @param offset a character offset
 * @param count the number of words to move from offset
 * @returns integer representing the new position
 */
static gint
_eail_move_words(const Evas_Object *textblock,
                 gint offset,
                 gint count)
{
   gint len = _eail_get_len(textblock);

   while (count > 0 && offset < len)
     {
        do
          offset++;
        while (offset < len && !_eail_is_word_end(textblock, offset));

        count--;
     }
   while (count < 0 && offset > 0)
     {
        do
          offset--;
        while (offset > 0 && !_eail_is_word_start(textblock, offset));

        count++;
     }

   return offset;
}

/**
 * @brief Checks whether the character is sentence break
 *
 * @param c character
 * @returns TRUE on success, FALSE otherwise
 */
static gboolean
_eail_is_sentence_break(gunichar c)
{
   if (c == '.' || c == '?' || c == '!') return TRUE;

   return FALSE;
}

/**
 * @brief Checks whether the character at offset in textblock is a sentence's start
 *
 * @param textblock Evas textblock
 * @param offset character offset
 * @returns TRUE on success, FALSE otherwise
 */
static gboolean
_eail_is_sentence_start(const Evas_Object *textblock,
                        gint offset)
{
   gunichar c;
   gint len = _eail_get_len(textblock);

   if (offset > len -1 || offset < 0) return FALSE;

   if (0 == offset) return TRUE;

   c = _eail_get_unichar_at_offset(textblock, offset - 1);
   if (_eail_is_sentence_break(c)) return TRUE;

   return FALSE;
}

/**
 * @brief Checks whether the character at offset in textblock is a sentence's end
 *
 * @param textblock Evas textblock
 * @param offset character offset
 * @return TRUE on success, FALSE otherwise
 */
static gboolean
_eail_is_sentence_end(const Evas_Object *textblock,
                      gint offset)
{
   gunichar c;
   gint len = _eail_get_len(textblock);

   if (offset > len -1 || offset < 0) return FALSE;

   if (offset == len -1) return TRUE;

   c = _eail_get_unichar_at_offset(textblock, offset);
   if (_eail_is_sentence_break(c)) return TRUE;

   return FALSE;
}

/**
 * @brief Checks whether the character at offset in textblock is inside a senetence
 *
 * @param textblock Evas textblock
 * @param offset character offset
 * @returns TRUE on success, FALSE otherwise
 */
static gboolean
_eail_is_inside_sentence(const Evas_Object *textblock,
                         gint offset)
{
   gunichar c;
   gint len = _eail_get_len(textblock);

   if ((offset > len -1 || offset < 0)) return FALSE;

   c = _eail_get_unichar_at_offset(textblock, offset);

   if (!_eail_is_sentence_break(c)) return TRUE;

   return FALSE;
}

/**
 * @brief Returns the position that is count sentences from the given offset
 *
 * Count may  be positive or negative. If count is positive, the returned
 * position will be a sentence end, otherwise it will be a sentence start.
 *
 * @param textblock Evas textblock
 * @param offset a character offset
 * @param count the number of words to move from offset
 * @returns integer representing the new position
 */
static gint
_eail_move_sentences(const Evas_Object *textblock,
                      gint offset,
                      gint count)
{
   gint len = _eail_get_len(textblock);

   while (count > 0 && offset < len)
     {
        do
          offset++;
        while (offset < len && !_eail_is_sentence_end(textblock, offset));

        count--;
     }
   while (count < 0 && offset > 0)
     {
        do
          offset--;
        while (offset > 0 && !_eail_is_sentence_start(textblock, offset));

        count++;
     }

   return offset;
}

/**
 * @brief Checks whether the character at offset in textblock is a line's start
 *
 * @param textblock Evas textblock
 * @param offset character offset
 * @returns TRUE on success, FALSE otherwise
 */
static gboolean
_eail_is_line_start(const Evas_Object *textblock,
                    gint offset)
{
   Evas_Textblock_Cursor *cur = NULL;
   cur = evas_object_textblock_cursor_new(textblock);
   evas_textblock_cursor_pos_set(cur, offset);
   evas_textblock_cursor_line_char_first(cur);
   gint pos = evas_textblock_cursor_pos_get(cur);
   evas_textblock_cursor_free(cur);

   return pos == offset;
}

/**
 * @brief Checks whether the character at offset in textblock is a line's end
 *
 * @param textblock Evas textblock
 * @param offset character offset
 * @return TRUE on success, FALSE otherwise
 */
static gboolean
_eail_is_line_end(const Evas_Object *textblock,
                  gint offset)
{
   /* is first non-word char after a word */
   Evas_Textblock_Cursor *cur = NULL;
   cur = evas_object_textblock_cursor_new(textblock);
   evas_textblock_cursor_pos_set(cur, offset);
   evas_textblock_cursor_line_char_last(cur);
   gint pos = evas_textblock_cursor_pos_get(cur);
   evas_textblock_cursor_free(cur);

   return (pos == offset);
}

/**
 * @brief Checks whether the character at offset in textblock is inside a line
 *
 * @param textblock Evas textblock
 * @param offset character offset
 * @returns TRUE on success, FALSE otherwise
 */
static gboolean
_eail_is_inside_line(const Evas_Object *textblock,
                     gint offset)
{
   Evas_Textblock_Cursor *cur = NULL;
   cur = evas_object_textblock_cursor_new(textblock);
   evas_textblock_cursor_pos_set(cur, offset);
   evas_textblock_cursor_line_char_last(cur);
   gint pos_end = evas_textblock_cursor_pos_get(cur);
   evas_textblock_cursor_free(cur);
   return pos_end != offset;
}

/**
 * @brief Returns the position that is count lines from the given offset
 *
 * Count may  be positive or negative. If count is positive, the returned
 * position will be a line end, otherwise it will be a line start.
 *
 * @param textblock Evas textblock
 * @param offset a character offset
 * @param count the number of words to move from offset
 * @returns integer representing the new position
 */
static gint
_eail_move_lines(const Evas_Object *textblock,
                 gint offset,
                 gint count)
{
   gint len = _eail_get_len(textblock);

   while (count > 0 && offset < len)
     {
        do
          offset++;
        while (offset < len && !_eail_is_line_end(textblock, offset));

        count--;
     }
   while (count < 0 && offset > 0)
     {
        do
          offset--;
        while (offset > 0 && !_eail_is_line_start(textblock, offset));

        count++;
     }

   return offset;
}

/**
 * @brief Gets a slice of the text from textblock after offset
 *
 * Use g_free() to free the returned string.
 *
 * @param textblock Evas textblock
 * @param offset character offset
 * @param boundary_type AtkTextBoundary instance
 * @param [out] start_offset start position of the returned text
 * @param [out] end_offset end position of the returned text
 * @returns newly allocated string containg a slice of text from textblock
 */
gchar *
eail_get_text_after(const Evas_Object *textblock,
                    gint offset,
                    AtkTextBoundary boundary_type,
                    gint *start_offset,
                    gint *end_offset)
{
   const gchar *text;
   int len;
   gint start, end;

   text = evas_textblock_text_markup_to_utf8(
       textblock, evas_object_textblock_text_markup_get(textblock));
   if (!text)
     {
        *start_offset = 0;
        *end_offset = 0;
        return g_strdup("");
     }

   start = offset;
   end = offset;
   len = _eail_get_len(textblock);

   switch (boundary_type)
     {
       case ATK_TEXT_BOUNDARY_CHAR:
           start = _eail_move_chars(textblock, start, 1);
           end = start;
           end = _eail_move_chars(textblock, end, 1);
           break;

       case ATK_TEXT_BOUNDARY_WORD_START:
           if (_eail_is_inside_word(textblock, end))
             end = _eail_move_words(textblock, end, 1);
           while (!_eail_is_word_start(textblock, end) && end < len)
             end = _eail_move_chars(textblock, end, 1);
           start = end;
           if (end < len)
             {
                end = _eail_move_words(textblock, end, 1);
                while (!_eail_is_word_start(textblock, end) && end < len)
                  end = _eail_move_chars(textblock, end, 1);
             }
           break;

       case ATK_TEXT_BOUNDARY_WORD_END:
           end = _eail_move_words(textblock, end, 1);
           start = end;
           if (end < len)
             end = _eail_move_words(textblock, end, 1);
           break;

       case ATK_TEXT_BOUNDARY_SENTENCE_START:
           if (_eail_is_inside_sentence(textblock, end))
             end = _eail_move_sentences(textblock, end, 1);
           while (!_eail_is_sentence_start(textblock, end) && end < len)
             end = _eail_move_chars(textblock, end, 1);
           start = end;
           if (end < len)
             {
                end = _eail_move_sentences(textblock, end, 1);
                while (!_eail_is_sentence_start(textblock, end) && end < len)
                  end = _eail_move_chars(textblock, end, 1);
             }
           break;

       case ATK_TEXT_BOUNDARY_SENTENCE_END:
           end = _eail_move_sentences(textblock, end, 1);
           start = end;
           if (end < len)
             end = _eail_move_sentences(textblock, end, 1);
           break;

       case ATK_TEXT_BOUNDARY_LINE_START:
           if (_eail_is_inside_line(textblock, end))
             end = _eail_move_lines(textblock, end, 1);
           while (!_eail_is_line_start(textblock, end) && end < len)
             end = _eail_move_chars(textblock, end, 1);
           start = end;
           if (end < len)
             {
                end = _eail_move_lines(textblock, end, 1);
                while (!_eail_is_line_start(textblock, end) && end < len)
                  end = _eail_move_chars(textblock, end, 1);
             }
           break;

       case ATK_TEXT_BOUNDARY_LINE_END:
           end = _eail_move_lines(textblock, end, 1);
           start = end;
           if (end < len)
             end = _eail_move_lines(textblock, end, 1);
           break;
     }

   *start_offset = start;
   *end_offset = end;
   g_assert(start <= end);

   return g_utf8_substring(text, start, end);
}

/**
 * @brief Gets a slice of the text from textblock at offset
 *
 * Use g_free() to free the returned string.
 *
 * @param textblock Evas textblock
 * @param offset character offset
 * @param boundary_type AtkTextBoundary instance
 * @param [out] start_offset start position of the returned text
 * @param [out] end_offset end position of the return text
 * @returns newly allocated string containing a slice of text from textblock
 */
gchar *
eail_get_text_at(const Evas_Object *textblock,
                 gint offset,
                 AtkTextBoundary boundary_type,
                 gint *start_offset,
                 gint *end_offset)
{
   const gchar *text;
   int len;
   gint start, end;

   text = evas_textblock_text_markup_to_utf8(
       textblock, evas_object_textblock_text_markup_get(textblock));
   if (!text)
     {
        *start_offset = 0;
        *end_offset = 0;
        return g_strdup("");
     }

   start = offset;
   end = offset;
   len = _eail_get_len(textblock);

   switch (boundary_type)
     {
       case ATK_TEXT_BOUNDARY_CHAR:
           end = _eail_move_chars(textblock, end, 1);
           break;

       case ATK_TEXT_BOUNDARY_WORD_START:
           if (!_eail_is_word_start(textblock, start))
             start = _eail_move_words(textblock, start, -1);
           if (_eail_is_inside_word(textblock, end))
             end = _eail_move_words(textblock, end, 1);
           while (!_eail_is_word_start(textblock, end) && end < len)
             end = _eail_move_chars(textblock, end, 1);
           break;

       case ATK_TEXT_BOUNDARY_WORD_END:
           if (_eail_is_inside_word(textblock, start) &&
               !_eail_is_word_start(textblock, start))
             start = _eail_move_words(textblock, start, -1);
           while (!_eail_is_word_end(textblock, start) && start > 0)
             start = _eail_move_chars(textblock, start, -1);
           end = _eail_move_words(textblock, end, 1);
           break;

       case ATK_TEXT_BOUNDARY_SENTENCE_START:
           if (!_eail_is_sentence_start(textblock, start))
             start = _eail_move_sentences(textblock, start, -1);
           if (_eail_is_inside_sentence(textblock, end))
             end = _eail_move_sentences(textblock, end, 1);
           while (!_eail_is_sentence_start(textblock, end) && end < len)
             end = _eail_move_chars(textblock, end, 1);
           break;

       case ATK_TEXT_BOUNDARY_SENTENCE_END:
           if (_eail_is_inside_sentence(textblock, start) &&
               !_eail_is_sentence_start(textblock, start))
             start = _eail_move_sentences(textblock, start, -1);
           while (!_eail_is_sentence_end(textblock, start) && start > 0)
             start = _eail_move_chars(textblock, start, -1);
           end = _eail_move_sentences(textblock, end, 1);
           break;

       case ATK_TEXT_BOUNDARY_LINE_START:
           if (!_eail_is_line_start(textblock, start))
             start = _eail_move_lines(textblock, start, -1);
           if (_eail_is_inside_line(textblock, end))
             end = _eail_move_lines(textblock, end, 1);
           while (!_eail_is_line_start(textblock, end) && end < len)
             end = _eail_move_chars(textblock, end, 1);
           break;

       case ATK_TEXT_BOUNDARY_LINE_END:
           if (_eail_is_inside_line(textblock, start) &&
               !_eail_is_line_start(textblock, start))
             start = _eail_move_lines(textblock, start, -1);
           while (!_eail_is_line_end(textblock, start) && start > 0)
             start = _eail_move_chars(textblock, start, -1);
           end = _eail_move_lines(textblock, end, 1);
           break;
     }

   *start_offset = start;
   *end_offset = end;
   g_assert(start <= end);

   return g_utf8_substring(text, start, end);
}

/**
 * @brief Gets a slice of the text from textblock before offset
 *
 * Use g_free() to free the returned string.
 *
 * @param textblock Evas textblock
 * @param offset character offset
 * @param boundary_type AtkTextBoundary instance
 * @param [out] start_offset start position of the returned text
 * @param [out] end_offset end position of the returned text
 * @returns newly allocated string containing a slice of text from textblock
 */
gchar *
eail_get_text_before(const Evas_Object *textblock,
                     gint offset,
                     AtkTextBoundary boundary_type,
                     gint *start_offset,
                     gint *end_offset)
{
   const gchar *text;
   gint start, end;

   text = evas_textblock_text_markup_to_utf8(
       textblock, evas_object_textblock_text_markup_get(textblock));
   if (!text)
     {
        *start_offset = 0;
        *end_offset = 0;
        return g_strdup("");
     }

   start = offset;
   end = offset;

   switch (boundary_type)
     {
       case ATK_TEXT_BOUNDARY_CHAR:
           start = _eail_move_chars(textblock, start, -1);
           break;

       case ATK_TEXT_BOUNDARY_WORD_START:
           if (!_eail_is_word_start(textblock, start))
             start = _eail_move_words(textblock, start, -1);
           end = start;
           start = _eail_move_words(textblock, start, -1);
           break;

       case ATK_TEXT_BOUNDARY_WORD_END:
           if (_eail_is_inside_word(textblock, start) &&
               !_eail_is_word_start(textblock, start))
             start = _eail_move_words(textblock, start, -1);
           while (!_eail_is_word_end(textblock, start) && start > 0)
             start = _eail_move_chars(textblock, start, -1);
           end = start;
           start = _eail_move_words(textblock, start, -1);
           while (!_eail_is_word_end(textblock, start) && start > 0)
             start = _eail_move_chars(textblock, start, -1);
           break;

       case ATK_TEXT_BOUNDARY_SENTENCE_START:
           if (!_eail_is_sentence_start(textblock, start))
             start = _eail_move_sentences(textblock, start, -1);
           end = start;
           start = _eail_move_sentences(textblock, start, -1);
           break;

       case ATK_TEXT_BOUNDARY_SENTENCE_END:
           if (_eail_is_inside_sentence(textblock, start) &&
               !_eail_is_sentence_start(textblock, start))
             start = _eail_move_sentences(textblock, start, -1);
           while (!_eail_is_sentence_end(textblock, start) && start > 0)
             start = _eail_move_chars(textblock, start, -1);
           end = start;
           start = _eail_move_sentences(textblock, start, -1);
           while (!_eail_is_sentence_end(textblock, start) && start > 0)
             start = _eail_move_chars(textblock, start, -1);
           break;

       case ATK_TEXT_BOUNDARY_LINE_START:
           if (!_eail_is_line_start(textblock, start))
             start = _eail_move_lines(textblock, start, -1);
           end = start;
           start = _eail_move_lines(textblock, start, -1);
           break;

       case ATK_TEXT_BOUNDARY_LINE_END:
           if (_eail_is_inside_line(textblock, start) &&
               !_eail_is_line_start(textblock, start))
             start = _eail_move_lines(textblock, start, -1);
           while (!_eail_is_line_end(textblock, start) && start > 0)
             start = _eail_move_chars(textblock, start, -1);
           end = start;
           start = _eail_move_lines(textblock, start, -1);
           while (!_eail_is_line_end(textblock, start) && start > 0)
             start = _eail_move_chars(textblock, start, -1);
           break;

     }

   *start_offset = start;
   *end_offset = end;
   g_assert(start <= end);

   return g_utf8_substring(text, start, end);
}

/*
 * @brief Adds attribute to attribute set
 *
 * @param attrib_set AtkAttributeSet to add the attribute to
 * @param attr AtkTextAttrribute to be added
 * @param value attribute value
 *
 * Creates an AtkAttribute from attr and value, and adds it
 * to attrib_set.
 *
 * @returns AtkAttributeSet containing set with added attribute
 **/
AtkAttributeSet*
eail_utils_text_add_attribute(AtkAttributeSet *attrib_set,
                              AtkTextAttribute attr,
                              const gchar     *value)
{
   AtkAttributeSet *return_set;
   AtkAttribute *at = g_malloc (sizeof (AtkAttribute));
   at->name = g_strdup (atk_text_attribute_get_name (attr));
   at->value = g_strdup(value);
   return_set = g_slist_prepend(attrib_set, at);
   return return_set;
}
