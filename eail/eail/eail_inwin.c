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
 * @file eail_inwin.c
 * @brief EailInwin implementation
 */

#include <Elementary.h>

#include "eail_inwin.h"
#include "eail_priv.h"

/**
 * @brief Define EailInwin GObject type
 */
G_DEFINE_TYPE(EailInwin, eail_inwin, EAIL_TYPE_WIDGET);

/**
 * @brief EailInwin object initialization
 *
 * @param obj EailInwin object
 * @param data user set additional initialization data
 */
static void
eail_inwin_initialize(AtkObject *obj, gpointer data)
{
   ATK_OBJECT_CLASS(eail_inwin_parent_class)->initialize(obj, data);

   obj->role = ATK_ROLE_GLASS_PANE;
}

/**
 * @brief Implementation of get_widget_children from EailWidget
 *
 * @param widget EailInwin instance
 *
 * @return list of widgets, NULL if no children
 */
static Eina_List *
eail_inwin_get_widget_children(EailWidget *widget)
{
   Eina_List *list = NULL;
   Evas_Object *obj = eail_widget_get_widget(widget);

   if (obj)
     list = eina_list_append(list, elm_win_inwin_content_get(obj));

   return list;
}

/**
 * @brief EailInwin instance initialization
 *
 * @param inwin EailInwin instance
 */
static void
eail_inwin_init(EailInwin *inwin)
{
}

/**
 * @brief GObject type initialization function
 *
 * @param klass EailInwin class
 */
static void
eail_inwin_class_init(EailInwinClass *klass)
{
   AtkObjectClass *class = ATK_OBJECT_CLASS(klass);
   EailWidgetClass *widget_class = EAIL_WIDGET_CLASS(klass);

   widget_class->get_widget_children = eail_inwin_get_widget_children;
   class->initialize = eail_inwin_initialize;
}
