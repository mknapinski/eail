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
 * @file eail_grid.c
 * @brief EailGrid implementation
 */

#include <Elementary.h>

#include "eail_grid.h"

/**
 * @brief Definition of EailGrid as GObject
 *
 * EailGrid is extended EAIL_TYPE_WIDGET
 */
G_DEFINE_TYPE(EailGrid, eail_grid, EAIL_TYPE_WIDGET);

/**
 * @brief Gets EvasObject from EailWidget.
 *
 * Implementation of eail_widget_get_widget_children callback from
 * EailWidget.
 *
 * @param widget EailWidget instance
 *
 * @returns Eina_List filled with Evas_Object* objects contained in grid widget
 */
static Eina_List*
eail_grid_children_get(EailWidget *widget)
{
   Evas_Object *obj;
   Eina_List *list = NULL;

   obj = eail_widget_get_widget(widget);
   if (obj)
     list = elm_grid_children_get(obj);

   return list;
}

/**
 * @brief Initializer for AtkObjectClass
 *
 * @param obj AtkObject instance
 * @param data initialization data
 */
static void
eail_grid_initialize(AtkObject *obj, gpointer data)
{
   ATK_OBJECT_CLASS(eail_grid_parent_class)->initialize(obj, data);
   obj->role = ATK_ROLE_FILLER;
}

/**
 * @brief Initializer for GObject class
 *
 * @param grid EailGrid instance
 */
static void
eail_grid_init(EailGrid *grid)
{
}

/**
 * @brief Initializer for GObject grid class
 *
 * Defines callbacks for base AtkObject.
 *
 * @param klass EailGridClass class
 */
static void
eail_grid_class_init(EailGridClass *klass)
{
   AtkObjectClass *class = ATK_OBJECT_CLASS(klass);
   EailWidgetClass *widget_class = EAIL_WIDGET_CLASS(klass);

   widget_class->get_widget_children = eail_grid_children_get;

   class->initialize = eail_grid_initialize;
}
