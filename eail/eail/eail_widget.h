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
 * @file eail_widget.h
 *
 * @brief Header for EailWindow implementation
 */

#ifndef EAIL_WIDGET_H
#define EAIL_WIDGET_H

#include <atk/atk.h>

#include <Eina.h>
#include <Evas.h>

/**
 * @brief Returns a value corresponding to the type of EailWidget class
 */
#define EAIL_TYPE_WIDGET              (eail_widget_get_type())

/**
 * @brief Macro upcasts an instance (obj) of a subclass to the EailWidget
 * type
 *
 * @param obj AtkObject instance
 */
#define EAIL_WIDGET(obj)              (G_TYPE_CHECK_INSTANCE_CAST((obj), \
                                       EAIL_TYPE_WIDGET, EailWidget))

/**
 * @brief Macro upcasts a subclass (klass) to the EailWidget class
 *
 * @param klass subclass object
 */
#define EAIL_WIDGET_CLASS(klass)      (G_TYPE_CHECK_CLASS_CAST((klass), \
                                       EAIL_TYPE_WIDGET, EailWidgetClass))

/**
 * @brief Tests whether object (obj) is an instance of EailWidget class
 *
 * @param obj AtkObject instance
 */
#define EAIL_IS_WIDGET(obj)           (G_TYPE_CHECK_INSTANCE_TYPE((obj), \
                                       EAIL_TYPE_WIDGET))

/**
 * @brief Tests whether given klass is a subclass of EailWidget
 *
 * @param klass klass object
 */
#define EAIL_IS_WIDGET_CLASS(klass)   (G_TYPE_CHECK_CLASS_TYPE((klass), \
                                       EAIL_TYPE_WIDGET))

/**
 * @brief Gets EailWidget class structure from an obj (class instance)
 *
 * @param obj object instance to get EailWidget class from
 */
#define EAIL_WIDGET_GET_CLASS(obj)    (G_TYPE_INSTANCE_GET_CLASS((obj), \
                                       EAIL_TYPE_WIDGET, EailWidgetClass))
#ifdef __cplusplus
extern "C" {
#endif

/** @brief Definition of object structure for Atk EailWidget*/
typedef struct _EailWidget      EailWidget;

/** @brief Definition of object class for Atk EailWidget*/
typedef struct _EailWidgetClass EailWidgetClass;

/** @brief Definition of object structure for Atk EailWidget*/
struct _EailWidget
{
   AtkObject parent;/**< @brief Parent AtkObject whose functionality is being extended*/

   Evas_Object *widget;/**< @brief Internal widget that is represented by EailWidget */
   AtkLayer layer;/**< @brief Describes layer of a component (eg. ATK_LAYER_WIDGET) */
};

/** @brief Definition of object class for Atk EailWidget*/
struct _EailWidgetClass
{
   AtkObjectClass parent_class;/**< @brief class that is being extended*/

   /** @brief callback definition for eail_widget_get_widget_children func*/
   Eina_List * (*get_widget_children)  (EailWidget *widget);
};

/**
 * @brief Getter for widget GType
 *
 * @returns GType for EailWidget implementation
 */
GType           eail_widget_get_type              (void);
Evas_Object *   eail_widget_get_widget            (EailWidget *widget);
Eina_List *     eail_widget_get_widget_children   (EailWidget *widget);

#ifdef __cplusplus
}
#endif

#endif
