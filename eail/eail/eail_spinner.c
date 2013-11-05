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
 * @file eail_spinner.c
 * @brief EailSpinner implementation
 */

#include <Elementary.h>

#include "eail_spinner.h"
#include "eail_utils.h"

static void atk_value_interface_init(AtkValueIface *iface);

/**
 * @brief EailSpinner type definition
 */
G_DEFINE_TYPE_WITH_CODE(EailSpinner,
                        eail_spinner,
                        EAIL_TYPE_WIDGET,
                        G_IMPLEMENT_INTERFACE(ATK_TYPE_VALUE,
                                              atk_value_interface_init));

/**
* @brief handler for changed event
*
* @param data passed to callback
* @param obj object that raised event
* @param event_info additional event info
*/

void
_eail_spinner_handle_changed_event(void *data,
                                     Evas_Object *obj,
                                     void *event_info)
{
   eail_emit_atk_signal
                  (ATK_OBJECT(data), "visible-data-changed", ATK_TYPE_OBJECT);
}

/**
 * @brief EailSpinner interface initializer
 *
 * @param spinner EailSpinner instance
 */
static void
eail_spinner_init(EailSpinner *spinner)
{
}

/**
 * @brief EailSpinner initializer
 *
 * @param obj AtkObject instance
 * @param data initialization data
 */
void
eail_spinner_initialize(AtkObject *obj, gpointer data)
{
   Evas_Object *nested_widget = NULL;
   ATK_OBJECT_CLASS(eail_spinner_parent_class)->initialize(obj, data);

   obj->role = ATK_ROLE_SPIN_BUTTON;
   g_return_if_fail(EAIL_IS_WIDGET(obj));

   nested_widget = eail_widget_get_widget(EAIL_WIDGET(obj));
   evas_object_smart_callback_add(nested_widget, "changed",
                                  _eail_spinner_handle_changed_event, obj);
}

/**
 * @brief Gets the accessible name of the accessible
 *
 * @param accessible AtkObject instance
 * @return string representing the accessible name of the accessible
 */
const gchar*
eail_spinner_get_name(AtkObject *accessible)
{
   Evas_Object *widget;
   const gchar *name;

   name = ATK_OBJECT_CLASS(eail_spinner_parent_class)->get_name(accessible);
   if (name) return name;

   widget = eail_widget_get_widget(EAIL_WIDGET(accessible));
   if (widget)
     name = (const gchar*)elm_object_text_get(widget);

   return name;
}

/**
 * @brief EailSpinner class initializer
 *
 * @param klass EailSpinnerClass instance
 */
static void
eail_spinner_class_init(EailSpinnerClass *klass)
{
   AtkObjectClass *atk_class = ATK_OBJECT_CLASS(klass);

   atk_class->initialize = eail_spinner_initialize;
   atk_class->get_name = eail_spinner_get_name;
}

/**
 * @brief Gets the current value of obj
 *
 * @param obj AtkValue instance
 * @param [out] value obj's current value
 */
static void
eail_spinner_get_current_value(AtkValue *obj,
                               GValue   *value)
{
   Evas_Object *widget;

   widget = eail_widget_get_widget(EAIL_WIDGET(obj));
   if (!widget) return;

   memset(value, 0, sizeof(GValue));
   g_value_init(value, G_TYPE_DOUBLE);
   g_value_set_double(value, elm_spinner_value_get(widget));
}

/**
 * @brief Gets the maximum value of obj
 *
 * @param obj AtkValue instance
 * @param [out] value obj's maxiumum value
 */
static void
eail_spinner_get_maximum_value(AtkValue *obj,
                               GValue   *value)
{
   Evas_Object *widget;
   double max;

   widget = eail_widget_get_widget(EAIL_WIDGET(obj));
   if (!widget) return;

   memset(value, 0, sizeof(GValue));
   g_value_init(value, G_TYPE_DOUBLE);
   elm_spinner_min_max_get(widget, NULL, &max);
   g_value_set_double(value, max);
}

/**
 * @brief Gets the minimum value of obj
 *
 * @param obj AtkValue instance
 * @param [out] value obj's minimum value
 */
static void
eail_spinner_get_minimum_value(AtkValue *obj,
                               GValue   *value)
{
   Evas_Object *widget;
   double min;

   widget = eail_widget_get_widget(EAIL_WIDGET(obj));
   if (!widget) return;

   memset(value, 0, sizeof(GValue));
   g_value_init(value, G_TYPE_DOUBLE);
   elm_spinner_min_max_get(widget, &min, NULL);
   g_value_set_double(value, min);
}

/**
 * @brief Sets the current value of obj
 *
 * @param obj AtkValue instance
 * @param value obj's new value
 * @return TRUE if new value was set successfully, FALSE otherwise
 */
static gboolean
eail_spinner_set_current_value(AtkValue     *obj,
                               const GValue *value)
{
   Evas_Object *widget;
   double min, max, val;

   widget = eail_widget_get_widget(EAIL_WIDGET(obj));
   if (!widget) return FALSE;

   val = g_value_get_double(value);
   elm_spinner_min_max_get(widget, &min, &max);

   if (min > val || max < val)
     return FALSE;

   elm_spinner_value_set(widget, val);

   return TRUE;
}

/**
 * @brief Gets the minimum increment of obj
 *
 * @param obj AtkValue instance
 * @param [out] value obj's minimum increment
 */
static void
eail_spinner_get_minimum_increment(AtkValue *obj,
                                   GValue *value)
{
   Evas_Object *widget;

   widget = eail_widget_get_widget(EAIL_WIDGET(obj));
   if (!widget) return;

   memset(value, 0, sizeof(GValue));
   g_value_init(value, G_TYPE_DOUBLE);
   g_value_set_double(value, elm_spinner_step_get(widget));
}

/**
 * @brief AtkValue interface initializer
 *
 * @param iface AtkValueIface instance
 */
static void
atk_value_interface_init(AtkValueIface *iface)
{
   g_return_if_fail(iface != NULL);

   iface->get_current_value = eail_spinner_get_current_value;
   iface->get_maximum_value = eail_spinner_get_maximum_value;
   iface->get_minimum_value = eail_spinner_get_minimum_value;
   iface->set_current_value = eail_spinner_set_current_value;
   iface->get_minimum_increment = eail_spinner_get_minimum_increment;
}
