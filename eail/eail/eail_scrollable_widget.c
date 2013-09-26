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
 * @file eail_scrollable_widget.c
 * @brief Implementation of widget that can scroll its content
 */

#include <atk/atk.h>
#include <Elementary.h>

#include "eail_scrollable_widget.h"
#include "eail_priv.h"
#include "eail_utils.h"

#define EAIL_STR_SCROLL_UP "scroll_up" /**< 'scroll_up' action name */
#define EAIL_STR_SCROLL_DOWN "scroll_down" /**< 'scroll_down' action name */
#define EAIL_STR_SCROLL_LEFT "scroll_left"/**< 'scroll_left' action name */
#define EAIL_STR_SCROLL_RIGHT "scroll_right"/**< 'scroll_right' action name */

/**
 * @brief Definition of EailScrollableWidget as GObject
 *
 * NOTE: this class extends EAIL_TYPE_ACTION_WIDGET, which implements
 * all needed functions for ATK_ACTION interface so additional implementing of
 * ATK_ACTION interface here is not needed.
 * Only thing that is really needed is to call:
 * eail_action_widget_action_append(...) to register all needed additional
 * actions
 */
G_DEFINE_TYPE(EailScrollableWidget, eail_scrollable_widget,
              EAIL_TYPE_ACTION_WIDGET);

/**
 * @brief Handler for 'scroll' event
 *
 * @param data data passed to callback
 * @param obj Evas_Object that raised event
 * @param event_info additional event info
 */
static void
_eail_scrollable_handle_scroll_event(void *data,
                                     Evas_Object *obj,
                                     void *event_info)
{
   eail_emit_atk_signal
                  (ATK_OBJECT(data), "visible-data-changed", ATK_TYPE_OBJECT);
}

/**
 * @brief Initializer for GObject class
 *
 * @param scrollable_widget EailScrollableWidget instance
 */
static void
eail_scrollable_widget_init(EailScrollableWidget *scrollable_widget)
{
}

/**
 * @brief Finalize function for GObject
 *
 * @param object GObject instance to finalize
 */
static void
eail_scrollable_widget_finalize(GObject *object)
{
   G_OBJECT_CLASS(eail_scrollable_widget_parent_class)->finalize(object);
}



/**
 * @brief 'scroll up' action callback
 *
 * @param action AtkAction instance
 * @param data additional callback data
 * @returns TRUE on success, FALSE otherwise
 */
static gboolean
_action_scroll_up(AtkAction *action,
                  void *data)
{
   Evas_Object *widget;

   g_return_val_if_fail(EAIL_IS_SCROLLABLE_WIDGET(action), FALSE);

   widget = eail_widget_get_widget(EAIL_WIDGET(action));
   _eail_scrollable_handle_scroll_event(action, widget, NULL);
   return eail_action_scroll_up(widget, NULL);
}

/**
 * @brief 'scroll down' action callback
 *
 * @param action AtkAction instance
 * @param data additional callback data
 * @returns TRUE on success, FALSE otherwise
 */
static gboolean
_action_scroll_down(AtkAction *action,
                    void *data)
{
   Evas_Object *widget;

   g_return_val_if_fail(EAIL_IS_SCROLLABLE_WIDGET(action), FALSE);

   widget = eail_widget_get_widget(EAIL_WIDGET(action));
   _eail_scrollable_handle_scroll_event(action, widget, NULL);
   return eail_action_scroll_down(widget, NULL);
}

/**
 * @brief 'scroll left' action callback
 *
 * @param action AtkAction instance
 * @param data additional callback data
 * @returns TRUE on success, FALSE otherwise
 */
static gboolean
_action_scroll_left(AtkAction *action,
                    void *data)
{
   Evas_Object *widget;

   g_return_val_if_fail(EAIL_IS_SCROLLABLE_WIDGET(action), FALSE);

   widget = eail_widget_get_widget(EAIL_WIDGET(action));
   _eail_scrollable_handle_scroll_event(action, widget, NULL);
   return eail_action_scroll_left(widget, NULL);
}

/**
 * @brief 'scroll right' action callback
 *
 * @param action AtkAction instance
 * @param data additional callback data
 * @returns TRUE on success, FALSE otherwise
 */
static gboolean
_action_scroll_right(AtkAction *action,
                     void *data)
{
   Evas_Object *widget;

   g_return_val_if_fail(EAIL_IS_SCROLLABLE_WIDGET(action), FALSE);

   widget = eail_widget_get_widget(EAIL_WIDGET(action));
   _eail_scrollable_handle_scroll_event(action, widget, NULL);
   return eail_action_scroll_right(widget, NULL);
}

/**
 * @brief Adds actions for scrolling to actions table
 * @param widget widget that implements EailScrollable interface
 */
static void
eail_scrollable_widget_actions_table_init(EailScrollableWidget *widget)
{
   eail_action_widget_action_append(EAIL_ACTION_WIDGET(widget),
                                    EAIL_STR_SCROLL_UP, NULL,
                                    _action_scroll_up);
   eail_action_widget_action_append(EAIL_ACTION_WIDGET(widget),
                                    EAIL_STR_SCROLL_DOWN, NULL,
                                    _action_scroll_down);
   eail_action_widget_action_append(EAIL_ACTION_WIDGET(widget),
                                    EAIL_STR_SCROLL_LEFT, NULL,
                                    _action_scroll_left);
   eail_action_widget_action_append(EAIL_ACTION_WIDGET(widget),
                                    EAIL_STR_SCROLL_RIGHT, NULL,
                                    _action_scroll_right);
}

/**
 * @brief Initializer for AtkObjectClass
 *
 * @param obj AtkObject instance
 * @param data initialization data
 */
static void
eail_scrollable_widget_initialize(AtkObject *obj, gpointer data)
{
   ATK_OBJECT_CLASS(eail_scrollable_widget_parent_class)->initialize(obj, data);
   Evas_Object *evas_obj = NULL;
   EailScrollableWidget *widget = EAIL_SCROLLABLE_WIDGET(obj);

   /* Initializing possible widget actions table*/
   eail_scrollable_widget_actions_table_init(widget);

   evas_obj = eail_widget_get_widget(EAIL_WIDGET(widget));
   if (!evas_obj)
     {
         ERR("No evas object for ScrollableWidget");
         return;
     }

   evas_object_smart_callback_add
       (evas_obj, "scroll", _eail_scrollable_handle_scroll_event, widget);
   evas_object_smart_callback_add
       (evas_obj, "scroll,left", _eail_scrollable_handle_scroll_event, widget);
   evas_object_smart_callback_add
       (evas_obj, "scroll,right", _eail_scrollable_handle_scroll_event, widget);
   evas_object_smart_callback_add
        (evas_obj, "scroll,up", _eail_scrollable_handle_scroll_event, widget);
   evas_object_smart_callback_add
        (evas_obj, "scroll,down", _eail_scrollable_handle_scroll_event, widget);
   evas_object_smart_callback_add(evas_obj,
         "scroll,page,changed", _eail_scrollable_handle_scroll_event, widget);
   evas_object_smart_callback_add(evas_obj,
              "scroll,anim,stop", _eail_scrollable_handle_scroll_event, widget);
   evas_object_smart_callback_add(evas_obj,
                        "swipe", _eail_scrollable_handle_scroll_event, widget);
   evas_object_smart_callback_add(evas_obj,
             "scroll,drag,stop", _eail_scrollable_handle_scroll_event, widget);
}

/**
 * @brief Initializer for GObject
 *
 * @param klass EailScrollableWidgetClass instance
 */
static void
eail_scrollable_widget_class_init(EailScrollableWidgetClass *klass)
{
   AtkObjectClass *class = ATK_OBJECT_CLASS(klass);
   GObjectClass *gobject_class = G_OBJECT_CLASS(klass);
   class->initialize = eail_scrollable_widget_initialize;
   gobject_class->finalize = eail_scrollable_widget_finalize;
}
