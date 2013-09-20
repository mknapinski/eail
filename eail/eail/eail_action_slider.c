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
 * @file eail_action_slider.c
 * @brief EailActionSlider implementation
 */

#include <Elementary.h>

#include "eail_action_slider.h"
#include "eail_utils.h"

/**
 * @brief Initialization of AtkValue interface
 *
 * @param iface EailActionSlider object
 */
static void atk_value_interface_init(AtkValueIface *iface);

/**
 * @brief Define EailActionSlider GObject type
 */
G_DEFINE_TYPE_WITH_CODE(EailActionSlider,
                        eail_action_slider,
                        EAIL_TYPE_WIDGET,
                        G_IMPLEMENT_INTERFACE(ATK_TYPE_VALUE,
                                              atk_value_interface_init));

/*
 * Implementation of the *AtkObject* interface
 */

/**
 * @brief handler for pos_changed event
 *
 * @param data passed to callback
 * @param obj object that raised event
 * @param event_info additional event info
 */
void
_eail_action_slider_handle_pos_changed_event(void *data,
                                      Evas_Object *obj,
                                      void *event_info)
{
   eail_emit_atk_signal
                  (ATK_OBJECT(data), "visible-data-changed", ATK_TYPE_OBJECT);
}

/**
 * @brief EailActionSlider object initialization
 *
 * @param obj EailActionSlider object
 * @param data user set additional initialization data
 */
static void
eail_action_slider_initialize(AtkObject *obj, gpointer data)
{
   Evas_Object *nested_widget = NULL;

   ATK_OBJECT_CLASS(eail_action_slider_parent_class)->initialize(obj, data);
   obj->role = ATK_ROLE_SLIDER;

   g_return_if_fail(EAIL_IS_WIDGET(obj));

   nested_widget = eail_widget_get_widget(EAIL_WIDGET(obj));
   evas_object_smart_callback_add(nested_widget, "pos_changed",
                            _eail_action_slider_handle_pos_changed_event, obj);

}

/**
 * @brief EailActionSlider instance initialization
 *
 * @param action_slider EailActionSlider instance
 */
static void
eail_action_slider_init(EailActionSlider *action_slider)
{
}

/**
 * @brief GObject type initialization function
 *
 * @param klass EailActionSlider class
 */
static void
eail_action_slider_class_init(EailActionSliderClass *klass)
{
   AtkObjectClass *class = ATK_OBJECT_CLASS(klass);

   class->initialize = eail_action_slider_initialize;
}

/*
 * Implementation of the *AtkValue* interface
 */

/**
 * @brief Gets obj current value
 *
 * @param obj EailActionSlider instance
 * @param [out] value EailActionSlider current value
 */
static void
eail_action_slider_get_current_value(AtkValue *obj,
                                     GValue   *value)
{
   Evas_Object *widget;

   widget = eail_widget_get_widget(EAIL_WIDGET(obj));
   if (!widget) return;

   memset(value, 0, sizeof(GValue));
   g_value_init(value, G_TYPE_UINT);
   g_value_set_uint(value, elm_actionslider_indicator_pos_get(widget));
}

/**
 * @brief Gets obj maxiumum value
 *
 * @param obj EailActionSlider instance
 * @param [out] value EailActionSlider maxiumum value
 */
static void
eail_action_slider_get_maximum_value(AtkValue *obj,
                                     GValue   *value)
{
   Evas_Object *widget;

   widget = eail_widget_get_widget(EAIL_WIDGET(obj));
   if (!widget) return;

   memset(value, 0, sizeof(GValue));
   g_value_init(value, G_TYPE_UINT);
   g_value_set_uint(value, elm_actionslider_enabled_pos_get(widget));
}

/**
 * @brief Gets obj minimum value
 *
 * @param obj EailActionSlider instance
 * @param [out] value EailActionSlider minimum value
 */
static void
eail_action_slider_get_minimum_value(AtkValue *obj,
                                     GValue   *value)
{
   Evas_Object *widget;

   widget = eail_widget_get_widget(EAIL_WIDGET(obj));
   if (!widget) return;

   memset(value, 0, sizeof(GValue));
   g_value_init(value, G_TYPE_UINT);
   g_value_set_uint(value, ELM_ACTIONSLIDER_NONE);
}

/**
 * @brief Sets obj value
 *
 * @param obj EailActionSlider instance
 * @param value EailActionSlider new value
 * @return TRUE if new value is set successfully, FALSE otherwise
 */
static gboolean
eail_action_slider_set_current_value(AtkValue *obj,
                                     const GValue *value)
{
   Evas_Object *widget;
   guint enum_val;

   widget = eail_widget_get_widget(EAIL_WIDGET(obj));
   if (!widget) return FALSE;

   enum_val = elm_actionslider_enabled_pos_get(widget) &
              g_value_get_uint(value);

   switch (enum_val)
     {
        case ELM_ACTIONSLIDER_LEFT:
        case ELM_ACTIONSLIDER_CENTER:
        case ELM_ACTIONSLIDER_RIGHT:
          elm_actionslider_indicator_pos_set(widget, enum_val);
          break;

        default:
          return FALSE;
          break;
     }

   return TRUE;
}

/**
 * @brief Gets obj minimum increment
 *
 * @param obj EailActionSlider instance
 * @param [out] value EailActionSlider minimum increment
 */
static void
eail_action_slider_get_minimum_increment(AtkValue *obj,
                                         GValue *value)
{
   Evas_Object *widget;

   widget = eail_widget_get_widget(EAIL_WIDGET(obj));
   if (!widget) return;

   memset(value, 0, sizeof(GValue));
   g_value_init(value, G_TYPE_UINT);
   g_value_set_uint(value, ELM_ACTIONSLIDER_NONE);
}

/**
 * @brief Initialization of AtkValue interface
 *
 * Function called upon instance creation. It initializes AtkValue interface
 * implementation i.e hooks method pointers in the interface structure
 * to the implementing class's implementation.
 *
 * @param iface an AtkObject that implements AtkValueInterface
 */
static void
atk_value_interface_init(AtkValueIface *iface)
{
   iface->get_current_value     = eail_action_slider_get_current_value;
   iface->get_maximum_value     = eail_action_slider_get_maximum_value;
   iface->get_minimum_value     = eail_action_slider_get_minimum_value;
   iface->set_current_value     = eail_action_slider_set_current_value;
   iface->get_minimum_increment = eail_action_slider_get_minimum_increment;
}
