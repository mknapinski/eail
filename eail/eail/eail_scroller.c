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
 * @file eail_scroller.c
 * @brief EailScroller implementation
 */

#include <Eina.h>
#include <Evas.h>
#include <Elementary.h>
#include "eail_scroller.h"

/**
 * @brief Definition of EailScroller as GObject
 *
 * EailScroller is extended EAIL_TYPE_SCROLLABLE_WIDGET
 */
G_DEFINE_TYPE(EailScroller, eail_scroller, EAIL_TYPE_SCROLLABLE_WIDGET);

/**
 * @brief Initializer for AtkObjectClass
 *
 * @param obj AtkObject instance
 * @param data initialization data
 */
static void
eail_scroller_initialize(AtkObject *obj, gpointer data)
{
    ATK_OBJECT_CLASS(eail_scroller_parent_class)->initialize(obj, data);

    obj->role = ATK_ROLE_SCROLL_PANE;
}

/**
 * @brief Prepares Eina_List filled with Evas_Object* objects
 * representing content of the widget
 *
 * Call eina_list_free on the returned list when results processing has been finished.
 *
 * @param widget EailWidget instance
 *
 * @return Eina_List representing the list of the widget's children
 */
static Eina_List *
eail_scroller_get_widget_children(EailWidget *widget)
{
    Eina_List *list = NULL;
    Evas_Object *content;
    Evas_Object *obj = eail_widget_get_widget(EAIL_WIDGET(widget));

    if(obj) {
        content = elm_object_part_content_get(obj, "default");
        if(content)
            list = eina_list_append(list, content);
    }

    return list;
}

/**
 * @brief Gets the accessible name of the accessible
 *
 * Implementation of AtkObject->get_name callback.
 *
 * @param obj AtkObject instance
 *
 * @returns string representing the accessible name of
 * the accessible
 */
static const gchar*
eail_scroller_get_name(AtkObject *obj)
{
    const gchar *name;
    Evas_Object *widget = NULL;

    name = ATK_OBJECT_CLASS(eail_scroller_parent_class)->get_name(obj);
    if(name != NULL)
        return name;

    widget = eail_widget_get_widget(EAIL_WIDGET(obj));
    if(widget)
        name = (const gchar*)elm_object_text_get(widget);

    return name;
}

/**
 * @brief EailFileselector GObject instance initializer
 *
 * @param scroller EailScroller instance
 */
static void
eail_scroller_init(EailScroller *scroller)
{
}

/**
 * @brief Initializer for EailScroller GObject class
 *
 * Defines callbacks for base AtkObject.
 *
 * @param klass EailScrollerClass instance
 */
static void
eail_scroller_class_init(EailScrollerClass *klass)
{
    AtkObjectClass *atk_class = ATK_OBJECT_CLASS(klass);
    EailWidgetClass *widget_class = EAIL_WIDGET_CLASS(klass);

    widget_class->get_widget_children = eail_scroller_get_widget_children;

    atk_class->initialize = eail_scroller_initialize;
    atk_class->get_name = eail_scroller_get_name;
}
