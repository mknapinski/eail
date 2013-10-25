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
 * @file eail_utils.h
 *
 * @brief Header for Eail Utility functions
 */

#ifndef EAIL_UTILS_H
#define EAIL_UTILS_H

#include <glib.h>

#ifdef __cplusplus
extern "C" {
#endif

#define EAIL_STR_SCROLL_UP "scroll_up" /**< @brief String for 'scroll up'*/
#define EAIL_STR_SCROLL_DOWN "scroll_down" /**< @brief String for 'scroll down'*/
#define EAIL_STR_SCROLL_LEFT "scroll_left" /**< @brief String for 'scroll left'*/
#define EAIL_STR_SCROLL_RIGHT "scroll_right" /**< @brief String for 'scroll right'*/

#define float_epsilon 0.00001
#define float_equal(a,b) (fabs((a) - (b)) < float_epsilon)

/**
 * @enum EAIL_SCROLL_TYPE Type of scroll direction
 */
enum EAIL_SCROLL_TYPE
{
   EAIL_SCROLL_TYPE_UP,/**< scroll 'up' direction */
   EAIL_SCROLL_TYPE_DOWN,/**< scroll 'down' direction */
   EAIL_SCROLL_TYPE_LEFT,/**< scroll 'left' direction */
   EAIL_SCROLL_TYPE_RIGHT/**< scroll 'right' direction */
};

/**
 * @brief Helper function gets substring from string
 */
gchar *eail_get_substring(const gchar* string, gint start_offset, gint end_offset);

/**
 * @brief Generic function for generating ref_state for Evas_Objects stored in
 * ATK objects
 *
 */
AtkStateSet *eail_evas_obj_ref_state_set(Evas_Object *widget,
                                         AtkStateSet *state_set);

/**
 * @brief Generic function for grabbing focus on Evas_Object stored in ATK object
 */
gboolean eail_evas_obj_grab_focus(Evas_Object *widget);

/**
 * @brief Emulates mouse 'press' event at given coordinates
 */
void eail_mouse_press_on_coords(Evas_Object *widget, int x, int y);

/**
 * @brief Emulates mouse 'release' event at given coordinates
 */
void eail_mouse_release_on_coords(Evas_Object *widget, int x, int y);

/**
 * @brief Emulates mouse 'click' event at given coordinates
 */
void eail_mouse_click_on_coords(Evas_Object *widget, int x, int y);

/**
 * @brief Gets coordinates of center of given widget
 */
void eail_get_coords_widget_center(Evas_Object *widget, int *x, int *y);

/**
 * @brief Gets edje parts list for item
 *
 */
Eina_List *eail_get_edje_parts_for_item(Elm_Object_Item *item);

/**
 * @brief Gets edje object for item
 *
 */
Evas_Object *eail_get_edje_obj_from_item(Elm_Object_Item *item);

/**
 * @brief Gets text content from item (each text is put into field in Eina_List
 * as const gchar*)
 */
Eina_List *eail_item_get_content_strings(Elm_Object_Item *item);

/**
 * @brief Scrolls screen to given direction
 */
gboolean eail_handle_scroll(Evas_Object *widget,
                            enum EAIL_SCROLL_TYPE type);

/**
 * @brief Scrolls screen up
 */
gboolean eail_action_scroll_up(Evas_Object *widget,
                               void *data);

/**
 * @brief Scrolls screen down
 */
gboolean eail_action_scroll_down(Evas_Object *widget,
                                 void *data);

/**
 * @brief Scrolls screen left
 */
gboolean eail_action_scroll_left(Evas_Object *widget,
                                 void *data);

/**
 * @brief Scrolls screen right
 */
gboolean eail_action_scroll_right(Evas_Object *widget,
                                  void *data);

/**
 * @brief Emits signal for ATK Object
 */
void eail_emit_atk_signal(AtkObject * atk_obj,
                          const gchar *signal_name,
                          GType object_type);

/**
 * @brief Handler for selected event in for list-based content
 */
void eail_list_item_handle_selected_event(void *data,
                                          Evas_Object *obj,
                                          void *event_info);

/**
 * @brief Handler for unselected event for list-based content
 */
void eail_list_item_handle_unselected_event(void *data,
                                            Evas_Object *obj,
                                            void *event_info);


/**
 * @brief Gets raw evas object list for eail item
 */
Eina_List *eail_get_raw_evas_obj_list_from_item(Elm_Object_Item *item);

/**
 * @brief Helper function for emitting 'children changed' signal when needed
 */
void eail_emit_children_changed(gboolean added,
                                AtkObject *atk_obj,
                                gint child_number);

/**
 * @brief Helper function for emit children changed signal when needed (func
 * version that takes object instead of index-number)
 */
void eail_emit_children_changed_obj(gboolean added,
                                    AtkObject *atk_obj,
                                    AtkObject *changed_obj);

/**
 * @brief Helper function to notify windows about focus changes of its children
 */
void eail_notify_child_focus_changes(void);

#ifdef __cplusplus
}
#endif

#endif
