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
 * @file eail_route.c
 * @brief EailRoute implementation
 */

#include <Elementary.h>

#include "eail_route.h"

static void atk_value_interface_init(AtkValueIface *iface);

/**
 * @brief Define EailRoute GObject type
 */
G_DEFINE_TYPE_WITH_CODE(EailRoute,
                        eail_route,
                        EAIL_TYPE_WIDGET,
                        G_IMPLEMENT_INTERFACE(ATK_TYPE_VALUE,
                                              atk_value_interface_init));

/**
 * @brief EailRoute object initialization
 *
 * @param obj EailRoute object
 * @param data user set additional initialization data
 */
static void
eail_route_initialize(AtkObject *obj, gpointer data)
{
   ATK_OBJECT_CLASS(eail_route_parent_class)->initialize(obj, data);

   obj->role = ATK_ROLE_IMAGE_MAP;
}

/**
 * @brief EailRoute instance initialization
 *
 * @param route EailRoute instance
 */
static void
eail_route_init(EailRoute *route)
{
}

/**
 * @brief GObject type initialization function
 *
 * @param klass EailRoute class
 */
static void
eail_route_class_init(EailRouteClass *klass)
{
   AtkObjectClass *class = ATK_OBJECT_CLASS(klass);

   class->initialize = eail_route_initialize;
}

/*
 * Implementation of the *AtkValue* interface
 */

/**
 * @brief Gets obj current value
 *
 * @param obj EailRoute instance
 * @param value EailRoute current value
 */
static void
eail_route_get_current_value(AtkValue *obj,
                             GValue   *value)
{
   double longitude_min, longitude_max;
   double latitude_min, latitude_max;
   Evas_Object *widget;
   gchar buf[200];

   widget = eail_widget_get_widget(EAIL_WIDGET(obj));
   if (!widget) return;

   elm_route_longitude_min_max_get(widget, &longitude_min, &longitude_max);
   elm_route_latitude_min_max_get(widget, &latitude_min, &latitude_max);

   g_snprintf(buf, sizeof(buf), "<longitude><min>%0.6f</min><max>%0.6f</max></longitude>\n"
                                "<latitude><min>%0.6f</min><max>%0.6f</max></latitude>",
                                 longitude_min, longitude_max, latitude_min, latitude_max);

   memset(value, 0, sizeof(GValue));
   g_value_init(value, G_TYPE_STRING);
   g_value_set_string(value, buf);
}

/**
 * @brief AtkValue interface initialization
 *
 * @param iface an AtkValue interface
 */
static void
atk_value_interface_init(AtkValueIface *iface)
{
   g_return_if_fail(iface != NULL);

   iface->get_current_value = eail_route_get_current_value;
}
