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
 * @file eail_background.h
 *
 * @brief Header for EailBackground implementation
 */

#ifndef EAIL_BACKGROUND_H
#define EAIL_BACKGROUND_H

#include "eail_image.h"
/**
 * @brief Returns a value corresponding to the type of EailBackground class
 */
#define EAIL_TYPE_BACKGROUND             (eail_background_get_type())
/**
 * @brief Macro upcasts an instance (obj) of a subclass to the EailBackground
 * type
 *
 * @param obj AtkObject instance
 */
#define EAIL_BACKGROUND(obj)             (G_TYPE_CHECK_INSTANCE_CAST((obj), \
                                          EAIL_TYPE_BACKGROUND, EailBackground))
/**
 * @brief Macro upcasts a subclass (klass) to the EailBackground class
 *
 * @param klass subclass object
 */
#define EAIL_BACKGROUND_CLASS(klass)     (G_TYPE_CHECK_CLASS_CAST((klass), \
                                          EAIL_TYPE_BACKGROUND, EailBackgroundClass))
/**
 * @brief Tests whether object (obj) is an instance of EailBackground class
 *
 * @param obj AtkObject instance
 */
#define EAIL_IS_BACKGROUND(obj)          (G_TYPE_CHECK_INSTANCE_TYPE((obj), \
                                          EAIL_TYPE_BACKGROUND))
/**
 * @brief Tests whether given klass is a subclass of EailBackground
 *
 * @param klass klass object
 */
#define EAIL_IS_BACKGROUND_CLASS(klass)  (G_TYPE_CHECK_CLASS_TYPE((klass), \
                                          EAIL_TYPE_BACKGROUND))
/**
 * @brief Gets EailBackground class structure from an obj (class instance)
 *
 * @param obj object instance to get EailBackground class from
 */
#define EAIL_BACKGROUND_GET_CLASS(obj)   (G_TYPE_INSTANCE_GET_CLASS((obj), \
                                          EAIL_TYPE_BACKGROUND, EailBackgroundClass))

#ifdef __cplusplus
extern "C" {
#endif

/** @brief Definition of object structure for Atk EailBackground*/
typedef struct _EailBackground      EailBackground;
/** @brief Definition of class structure for Atk EailBackground*/
typedef struct _EailBackgroundClass EailBackgroundClass;

/** @brief Definition of object structure for Atk EailBackground*/
struct _EailBackground
{
   EailWidget parent;/*!< @brief Parent widget whose functionality is being extended*/

   char *description;/*!< @brief Background description */
};

/** @brief Definition of class structure for Atk EailBackground*/
struct _EailBackgroundClass
{
   EailWidgetClass parent_class;/*!< parent class that is extended*/
};

/**
 * @brief Getter for EailBackground GType object
 *
 * @returns GType for EailBackground implementation
 */
GType eail_background_get_type(void);

#ifdef __cplusplus
}
#endif

#endif
