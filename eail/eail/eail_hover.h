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
 * @file eail_hover.h
 *
 * @brief Header for EailHoverimplementation
 */

#ifndef EAIL_HOVER_H
#define EAIL_HOVER_H

#include "eail_widget.h"
/**
 * @brief Returns a value corresponding to the type of EailHover class
 */
#define EAIL_TYPE_HOVER            (eail_hover_get_type())
/**
 * @brief Macro upcasts an instance (obj) of a subclass to the EailHover
 * type
 *
 * @param obj AtkObject instance
 */
#define EAIL_HOVER(obj)            (G_TYPE_CHECK_INSTANCE_CAST((obj), \
                                    EAIL_TYPE_HOVER, EailHover))
/**
 * @brief Macro upcasts a subclass (klass) to the EailHover class
 *
 * @param klass subclass object
 */
#define EAIL_HOVER_CLASS(klass)    (G_TYPE_CHECK_CLASS_CAST((klass), \
                                    EAIL_TYPE_HOVER, EailHoverClass))
/**
 * @brief Tests whether object (obj) is an instance of EailHover class
 *
 * @param obj AtkObject instance
 */
#define EAIL_IS_HOVER(obj)         (G_TYPE_CHECK_INSTANCE_TYPE((obj), \
                                    EAIL_TYPE_HOVER))
/**
 * @brief Tests whether given klass is a subclass of EailHover
 *
 * @param klass klass object
 */
#define EAIL_IS_HOVER_CLASS(klass) (G_TYPE_CHECK_CLASS_TYPE((klass), \
                                    EAIL_TYPE_HOVER))
/**
 * @brief Gets EailHover class structure from an obj (class instance)
 *
 * @param obj object instance to get EailHover class from
 */
#define EAIL_HOVER_GET_CLASS(obj)  (G_TYPE_INSTANCE_GET_CLASS((obj), \
                                    EAIL_TYPE_HOVER, EailHoverClass))

#ifdef __cplusplus
extern "C" {
#endif

/** @brief Definition of object structure for Atk EailHover*/
typedef struct _EailHover      EailHover;
/** @brief Definition of class structure for Atk EailHover*/
typedef struct _EailHoverClass EailHoverClass;

/** @brief Definition of object structure for Atk EailHover*/
struct _EailHover
{
   EailWidget parent;/**< @brief Parent widget whose functionality is being extended*/

   char *click_description;/**< @brief 'click' action description string */
};

/** @brief Definition of class structure for Atk EailHover*/
struct _EailHoverClass
{
   EailWidgetClass parent_class;/**< @brief class that is being extended*/
};

/**
 * @brief Getter for EailHover GType
 * @returns GType for EailHover implementation
 */
GType eail_hover_get_type(void);

#ifdef __cplusplus
}
#endif

#endif
