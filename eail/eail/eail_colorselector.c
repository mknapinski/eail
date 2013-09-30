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
 * @file eail_colorselector.c
 * @brief EailColorselector implementation.
 */

#include <Elementary.h>

#include "eail_colorselector.h"

static void atk_value_interface_init(AtkValueIface *iface);

/**
 * @brief EailColorselector type definition
 */
G_DEFINE_TYPE_WITH_CODE(EailColorselector,
                        eail_colorselector,
                        EAIL_TYPE_WIDGET,
                        G_IMPLEMENT_INTERFACE(ATK_TYPE_VALUE,
                                              atk_value_interface_init));

/**
 * @brief EailColorselector initializer
 *
 * @param obj AtkObject instance
 * @param data initialization data
 */
static void
eail_colorselector_initialize(AtkObject *obj, gpointer data)
{
   ATK_OBJECT_CLASS(eail_colorselector_parent_class)->initialize(obj, data);
   obj->role = ATK_ROLE_COLOR_CHOOSER;
}

/**
 * @brief EailColorselector instance initializer
 *
 * @param selector EailColorselector instance
 */
static void
eail_colorselector_init(EailColorselector *selector)
{
}

/**
 * @brief EailColorselector class initializer
 *
 * @param klass EailColorselectorClass instance
 */
static void
eail_colorselector_class_init(EailColorselectorClass *klass)
{
   AtkObjectClass *atk_class = ATK_OBJECT_CLASS(klass);
   atk_class->initialize = eail_colorselector_initialize;
}

/**
 * @brief Gets obj's current value
 *
 * @param obj AtkValue instance
 * @param [out] value obj's current value
 */
static void
eail_colorselector_get_current_value(AtkValue *obj,
                                     GValue   *value)
{
   gint r = 0;
   gint g = 0;
   gint b = 0;
   gint a = 0;
   gint color = 0;
   Evas_Object *widget = NULL;

   widget = eail_widget_get_widget(EAIL_WIDGET(obj));
   if (!widget) return;

   elm_colorselector_color_get(widget, &r, &g, &b, &a);
   color = (a << 24) | (r << 16) | (g << 8) | b;

   memset(value, 0, sizeof(GValue));
   g_value_init(value, G_TYPE_INT);
   g_value_set_int(value, color);
}

/**
 * @brief Gets obj's maxiumum value
 *
 * @param obj AtkValue instance
 * @param [out] value obj's maximum value
 */
static void
eail_colorselector_get_maximum_value(AtkValue *obj,
                                     GValue   *value)
{
   gint max = 0xFFFFFFFF;
   Evas_Object *widget = NULL;

   widget = eail_widget_get_widget(EAIL_WIDGET(obj));
   if (!widget) return;

   memset(value, 0, sizeof(GValue));
   g_value_init(value, G_TYPE_INT);
   g_value_set_int(value, max);
}

/**
 * @brief Gets obj's minimum value
 *
 * @param obj AtkValue instance
 * @param [out] value obj's minimum value
 */
static void
eail_colorselector_get_minimum_value(AtkValue *obj,
                                     GValue   *value)
{
   gint min = 0;
   Evas_Object *widget = NULL;

   widget = eail_widget_get_widget(EAIL_WIDGET(obj));
   if (!widget) return;

   memset(value, 0, sizeof(GValue));
   g_value_init(value, G_TYPE_INT);
   g_value_set_int(value, min);
}

/**
 * @brief Sets obj's current value
 *
 * @param obj AtkValue instance
 * @param value obj's current value
 * @return TRUE if value was set successfully, FALSE otherwise
 */
static gboolean
eail_colorselector_set_current_value(AtkValue *obj,
                                     const GValue *value)
{
   gint r = 0xFFFFFFFF;
   gint g = 0xFFFFFFFF;
   gint b = 0xFFFFFFFF;
   gint a = 0xFFFFFFFF;
   gint color = 0xFFFFFFFF;
   Evas_Object *widget = NULL;

   widget = eail_widget_get_widget(EAIL_WIDGET(obj));
   if (!widget) return FALSE;

   color = g_value_get_int(value);
   a = (0xFF000000 & (guint)color) >> 24;
   r = (0x00FF0000 & color) >> 16;
   g = (0x0000FF00 & color) >> 8;
   b = 0x000000FF & color;
   elm_colorselector_color_set(widget, r, g, b, a);

   return TRUE;
}

/**
 * @brief Gets obj's minimum increment
 *
 * @param obj AtkValue instance
 * @param [out] value obj's minimum increment
 */
static void
eail_colorselector_get_minimum_increment(AtkValue *obj,
                                         GValue *value)
{
   gint inc = 0x01010101;

   memset(value,  0, sizeof(GValue));
   g_value_init(value, G_TYPE_INT);
   g_value_set_int(value, inc);
}

/**
 * @brief AtkValue interface initializer
 *
 * @param iface AtkValueIface instance
 */
static void atk_value_interface_init(AtkValueIface *iface)
{
   iface->get_current_value     = eail_colorselector_get_current_value;
   iface->get_maximum_value     = eail_colorselector_get_maximum_value;
   iface->get_minimum_value     = eail_colorselector_get_minimum_value;
   iface->set_current_value     = eail_colorselector_set_current_value;
   iface->get_minimum_increment = eail_colorselector_get_minimum_increment;
}

