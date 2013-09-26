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
 * @file eail_diskselector.h
 *
 * @brief Header for EailDiskselector implementation
 */

#ifndef EAIL_DISKSELECTOR_H
#define EAIL_DISKSELECTOR_H

#include "eail_scrollable_widget.h"
/**
 * @brief Returns a value corresponding to the type of EailDiskselector class
 */
#define EAIL_TYPE_DISKSELECTOR             (eail_diskselector_get_type())
/**
 * @brief Macro upcasts an instance (obj) of a subclass to the EailDiskselector
 * type
 *
 * @param obj AtkObject instance
 */
#define EAIL_DISKSELECTOR(obj)             (G_TYPE_CHECK_INSTANCE_CAST ((obj), \
                                           EAIL_TYPE_DISKSELECTOR, EailDiskselector))
/**
 * @brief Macro upcasts a subclass (klass) to the EailDiskselector class
 *
 * @param klass subclass object
 */
#define EAIL_DISKSELECTOR_CLASS(klass)     (G_TYPE_CHECK_CLASS_CAST ((klass), \
                                           EAIL_TYPE_DISKSELECTOR, EailDiskselectorClass))
/**
 * @brief Tests whether object (obj) is an instance of EailDiskselector class
 *
 * @param obj AtkObject instance
 */
#define EAIL_IS_DISKSELECTOR(obj)          (G_TYPE_CHECK_INSTANCE_TYPE ((obj), \
                                           EAIL_TYPE_DISKSELECTOR))
/**
 * @brief Tests whether given klass is a subclass of EailDiskselector
 *
 * @param klass klass object
 */
#define EAIL_IS_DISKSELECTOR_CLASS(klass)  (G_TYPE_CHECK_CLASS_TYPE ((klass), \
                                           EAIL_TYPE_DISKSELECTOR))
/**
 * @brief Gets EailDiskselector class structure from an obj (class instance)
 *
 * @param obj object instance to get EailDiskselector class from
 */
#define EAIL_DISKSELECTOR_GET_CLASS(obj)   (G_TYPE_INSTANCE_GET_CLASS ((obj), \
                                           EAIL_TYPE_DISKSELECTOR, EailDiskselectorClass))

#ifdef __cplusplus
extern "C" {
#endif

/** @brief Definition of object structure for Atk EailDiskselector*/
typedef struct _EailDiskselector      EailDiskselector;
/** @brief Definition of class structure for Atk EailDiskselector*/
typedef struct _EailDiskselectorClass EailDiskselectorClass;

/** @brief Definition of object structure for Atk EailDiskselector*/
struct _EailDiskselector
{
   /** @brief Parent widget whose functionality is being extended*/
   EailScrollableWidget parent;
};

/** @brief Definition of class structure for Atk EailDiskselector*/
struct _EailDiskselectorClass
{
   /** @brief Parent class whose functionality is being extended*/
   EailScrollableWidgetClass parent_class;
};

/** @brief Getter for EailDiskselector GType
 * @returns GType for EailDiskselector implementation*/
GType eail_diskselector_get_type(void);

#ifdef __cplusplus
}
#endif

#endif
