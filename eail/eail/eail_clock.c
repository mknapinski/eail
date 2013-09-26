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
 * @file eail_clock.c
 * @brief EailClock implementation
 */

#include <Elementary.h>

#include "eail_clock.h"

static void atk_value_interface_init(AtkValueIface *iface);

/**
 * @brief Define EailClock GObject type
 */
G_DEFINE_TYPE_WITH_CODE(EailClock,
                        eail_clock,
                        EAIL_TYPE_WIDGET,
                        G_IMPLEMENT_INTERFACE(ATK_TYPE_VALUE,
                                              atk_value_interface_init));

/*
 * Implementation of the *AtkObject* interface
 */

/**
 * @brief EailClock object initialization
 *
 * @param obj EailClock object
 * @param data additional initialization data
 */
static void
eail_clock_initialize(AtkObject *obj, gpointer data)
{
   ATK_OBJECT_CLASS(eail_clock_parent_class)->initialize(obj, data);

   obj->role = ATK_ROLE_TEXT;
}

/**
 * @brief EailClock instance initialization
 *
 * @param clock EailClock instance
 */
static void
eail_clock_init(EailClock *clock)
{
}

/**
 * @brief GObject type initialization function
 *
 * @param klass EailClockClass instance
 */
static void
eail_clock_class_init(EailClockClass *klass)
{
   AtkObjectClass *class = ATK_OBJECT_CLASS(klass);

   class->initialize = eail_clock_initialize;
}

/*
 * Implementation of the *AtkValue* interface
 */

/**
 * @brief Gets obj's current value
 *
 * @param obj EailClock instance
 * @param [out] value obj's current value
 */
static void
eail_clock_get_current_value(AtkValue *obj,
                             GValue   *value)
{
   Evas_Object *widget;
   int hour, minute, second;
   int current_second_day;

   widget = eail_widget_get_widget(EAIL_WIDGET(obj));
   if (!widget) return;

   elm_clock_time_get(widget, &hour, &minute, &second);

   current_second_day = (3600 * hour) + (60 * minute) + second;

   memset(value, 0, sizeof(GValue));
   g_value_init(value, G_TYPE_INT);
   g_value_set_int(value, current_second_day);
}

/**
 * @brief Gets obj's maxiumum value
 *
 * @param obj EailClock instance
 * @param [out] value obj's maxiumum value
 */
static void
eail_clock_get_maximum_value(AtkValue *obj,
                             GValue   *value)
{
   Evas_Object *widget;

   widget = eail_widget_get_widget(EAIL_WIDGET(obj));
   if (!widget) return;

   memset(value, 0, sizeof(GValue));
   g_value_init(value, G_TYPE_INT);
   g_value_set_int(value, 24 * 60 * 60 - 1);  // 24h - 1s
}

/**
 * @brief Gets obj's minimum value
 *
 * @param obj EailClock instance
 * @param [out] value obj's minimum value
 */
static void
eail_clock_get_minimum_value(AtkValue *obj,
                             GValue   *value)
{
   Evas_Object *widget;

   widget = eail_widget_get_widget(EAIL_WIDGET(obj));
   if (!widget) return;

   memset(value, 0, sizeof(GValue));
   g_value_init(value, G_TYPE_INT);
   g_value_set_int(value, 0);                 // 0 - seconds
}

/**
 * @brief Sets obj's value
 *
 * @param obj EailClock instance
 * @param value obj's new value
 * @return TRUE if new value was set successfully, FALSE otherwise
 */
static gboolean
eail_clock_set_current_value(AtkValue     *obj,
                             const GValue *value)
{
   Evas_Object *widget;
   int hour, minute, second;
   int current_second_day;

   widget = eail_widget_get_widget(EAIL_WIDGET(obj));
   if (!widget) return FALSE;

   current_second_day = g_value_get_int(value);

   if (0 > current_second_day)
     return FALSE;

   hour = current_second_day / 3600;
   minute = (current_second_day % 3600) / 60;
   second = current_second_day % 60;

   if (23 < hour || 59 < minute || 59 < second)
     return FALSE;

   elm_clock_time_set(widget, hour, minute, second);

   return TRUE;
}

/**
 * @brief Gets obj's minimum increment
 *
 * @param obj EailClock instance
 * @param [out] value obj's minimum increment
 */
static void
eail_clock_get_minimum_increment(AtkValue *obj,
                                 GValue   *value)
{
   Evas_Object *widget;

   widget = eail_widget_get_widget(EAIL_WIDGET(obj));
   if (!widget) return;

   memset(value, 0, sizeof(GValue));
   g_value_init(value, G_TYPE_INT);
   g_value_set_int(value, 1);
}

/**
 * @brief Initialization of AtkValue interface
 *
 * @brief Function called upon instance creation.
 *
 * It initializes AtkValue interface
 * implementation i.e hooks method pointers in the interface structure
 * to the implementing class's implementation.
 *
 * @param iface AtkObject that implements AtkValueInterface
 */
static void
atk_value_interface_init(AtkValueIface *iface)
{
   g_return_if_fail(iface != NULL);

   iface->get_current_value     = eail_clock_get_current_value;
   iface->get_maximum_value     = eail_clock_get_maximum_value;
   iface->get_minimum_value     = eail_clock_get_minimum_value;
   iface->set_current_value     = eail_clock_set_current_value;
   iface->get_minimum_increment = eail_clock_get_minimum_increment;
}
