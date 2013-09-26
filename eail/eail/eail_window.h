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
 * @file eail_window.h
 *
 * @brief Header for EailWindow implementation
 */

#ifndef EAIL_WINDOW_H
#define EAIL_WINDOW_H

#include "eail_action_widget.h"

/**
 * @brief Returns a value corresponding to the type of EailWindow class
 */
#define EAIL_TYPE_WINDOW              (eail_window_get_type())

/**
 * @brief Macro upcasts an instance (obj) of a subclass to the EailWindow
 * type
 *
 * @param obj AtkObject instance
 */
#define EAIL_WINDOW(obj)              (G_TYPE_CHECK_INSTANCE_CAST((obj), \
                                       EAIL_TYPE_WINDOW, EailWindow))

/**
 * @brief Macro upcasts a subclass (klass) to the EailWindow class
 *
 * @param klass subclass object
 */
#define EAIL_WINDOW_CLASS(klass)      (G_TYPE_CHECK_CLASS_CAST((klass), \
                                       EAIL_TYPE_WINDOW, EailWindowClass))

/**
 * @brief Tests whether object (obj) is an instance of EailWindow class
 *
 * @param obj AtkObject instance
 */
#define EAIL_IS_WINDOW(obj)           (G_TYPE_CHECK_INSTANCE_TYPE((obj), \
                                       EAIL_TYPE_WINDOW))

/**
 * @brief Tests whether given klass is a subclass of EailWindow
 *
 * @param klass klass object
 */
#define EAIL_IS_WINDOW_CLASS(klass)   (G_TYPE_CHECK_CLASS_TYPE((klass), \
                                       EAIL_TYPE_WINDOW))

/**
 * @brief Gets EailWindow class structure from an obj (class instance)
 *
 * @param obj object instance to get EailWindow class from
 */
#define EAIL_WINDOW_GET_CLASS(obj)    (G_TYPE_INSTANCE_GET_CLASS((obj), \
                                       EAIL_TYPE_WINDOW, EailWindowClass))
#ifdef __cplusplus
extern "C" {
#endif

/** @brief Definition of object structure for Atk EailWindow*/
typedef struct _EailWindow      EailWindow;

/** @brief Definition of object class for Atk EailWindow*/
typedef struct _EailWindowClass EailWindowClass;

/** @brief Definition of object structure for Atk EailWindow*/
struct _EailWindow
{
    EailActionWidget parent;/*!< @brief Parent widget whose functionality is being extended*/
    gint child_count_last;/**< @brief last reported count of children */
};

/** @brief Definition of object class for Atk EailWindow*/
struct _EailWindowClass
{
    EailActionWidgetClass parent_class;/*!< @brief class that is being extended*/
};

/** @brief Getter for EailWindow GType
 * @returns GType for EailWindow implementation*/
GType   eail_window_get_type  (void);

#ifdef __cplusplus
}
#endif

#endif
