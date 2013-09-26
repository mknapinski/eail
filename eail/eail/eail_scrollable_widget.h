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
 * @file eail_scrollable_widget.h
 *
 * @brief Header for EailScrollableWidget implementation
 */

#ifndef EAIL_SCROLLABLE_WIDGET_H
#define EAIL_SCROLLABLE_WIDGET_H

#include "eail_action_widget.h"
/**
 * @brief Returns a value corresponding to the type of EailScrollableWidget class
 */
#define EAIL_TYPE_SCROLLABLE_WIDGET      (eail_scrollable_widget_get_type())
/**
 * @brief Macro upcasts an instance (obj) of a subclass to the EailScrollableWidget
 * type
 *
 * @param obj AtkObject instance
 */
#define EAIL_SCROLLABLE_WIDGET(obj)      (G_TYPE_CHECK_INSTANCE_CAST((obj), \
                                         EAIL_TYPE_SCROLLABLE_WIDGET, \
                                         EailScrollableWidget))
/**
 * @brief Macro upcasts a subclass (klass) to the EailScrollableWidget class
 *
 * @param klass subclass object
 */
#define EAIL_SCROLLABLE_WIDGET_CLASS(klass) \
                              (G_TYPE_SCROLLABLE_WIDGET_CLASS_CAST((klass), \
                                            EAIL_TYPE_SCROLLABLE_WIDGET, \
                                            EailScrollableWidgetClass))
/**
 * @brief Tests whether object (obj) is an instance of EailScrollableWidget class
 *
 * @param obj AtkObject instance
 */
#define EAIL_IS_SCROLLABLE_WIDGET(obj) \
                                       (G_TYPE_CHECK_INSTANCE_TYPE((obj), \
                                        EAIL_TYPE_SCROLLABLE_WIDGET))
/**
 * @brief Tests whether given klass is a subclass of EailScrollableWidget
 *
 * @param klass klass object
 */
#define EAIL_IS_SCROLLABLE_WIDGET_CLASS(klass)(G_TYPE_CHECK_CLASS_TYPE((klass),\
                                               EAIL_TYPE_SCROLLABLE_WIDGET))
/**
 * @brief Gets EailScrollableWidget class structure from an obj (class instance)
 *
 * @param obj object instance to get EailScrollableWidget class from
 */
#define EAIL_SCROLLABLE_WIDGET_GET_CLASS(obj) \
                                            (G_TYPE_INSTANCE_GET_CLASS((obj), \
                                            EAIL_TYPE_SCROLLABLE_WIDGET, \
                                            EailScrollableWidgetClass))

#ifdef __cplusplus
extern "C" {
#endif

/** @brief Definition of object structure for Atk EailScrollableWidget*/
typedef struct _EailScrollableWidget EailScrollableWidget;
/** @brief Definition of object class for Atk EailScrollableWidget*/
typedef struct _EailScrollableWidgetClass EailScrollableWidgetClass;

/** @brief Definition of object structure for Atk EailScrollableWidget*/
struct _EailScrollableWidget
{
   /** @brief Parent widget whose functionality is being extended*/
   EailActionWidget parent;
};

/** @brief Definition of object class for Atk EailScrollableWidget*/
struct _EailScrollableWidgetClass
{
   /** @brief Parent class whose functionality is being extended*/
   EailActionWidgetClass parent_class;
};

/** @brief Getter for EailScrollable GType
 *
 * @returns GType for EailScrollable implementation*/
GType eail_scrollable_widget_get_type(void);

#ifdef __cplusplus
}
#endif

#endif

