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
 * @file eail_text.h
 *
 * @brief Header for EailText implementation
 */

#ifndef EAIL_TEXT_H
#define EAIL_TEXT_H

#include "eail_widget.h"

/**
 * @brief Returns a value corresponding to the type of EailText class
 */
#define EAIL_TYPE_TEXT             (eail_text_get_type())

/**
 * @brief Macro upcasts an instance (obj) of a subclass to the EailText
 * type
 *
 * @param obj AtkObject instance
 */
#define EAIL_TEXT(obj)             (G_TYPE_CHECK_INSTANCE_CAST((obj), \
                                    EAIL_TYPE_TEXT, EailText))

/**
 * @brief Macro upcasts a subclass (klass) to the EailText class
 *
 * @param klass subclass object
 */
#define EAIL_TEXT_CLASS(klass)     (G_TYPE_CHECK_CLASS_CAST((klass), \
                                    EAIL_TYPE_TEXT, EailTextClass))

/**
 * @brief Tests whether object (obj) is an instance of EailText class
 *
 * @param obj AtkObject instance
 */
#define EAIL_IS_TEXT(obj)          (G_TYPE_CHECK_INSTANCE_TYPE((obj), \
                                    EAIL_TYPE_TEXT))

/**
 * @brief Tests whether given klass is a subclass of EailText
 *
 * @param klass klass object
 */
#define EAIL_IS_TEXT_CLASS(klass)  (G_TYPE_CHECK_CLASS_TYPE((klass), \
                                    EAIL_TYPE_TEXT))

/**
 * @brief Gets EailText class structure from an obj (class instance)
 *
 * @param obj object instance to get EailText class from
 */
#define EAIL_TEXT_GET_CLASS(obj)   (G_TYPE_INSTANCE_GET_CLASS((obj), \
                                    EAIL_TYPE_TEXT, EailTextClass))

#ifdef __cplusplus
extern "C" {
#endif
/** @brief Definition of object structure for Atk EailActionSlider*/
typedef struct _EailText      EailText;
/** @brief Definition of object class for Atk EailActionSlider*/
typedef struct _EailTextClass EailTextClass;

/** @brief Definition of object structure for Atk EailActionSlider*/
struct _EailText
{
   /** @brief Parent widget whose functionality is being extended*/
   EailWidget parent;
};

/** @brief Definition of object class for Atk EailActionSlider*/
struct _EailTextClass
{
   EailWidgetClass parent_class;/*!< @brief parent class that is being extended*/
};

/** @brief Getter for EailText GType
 * @returns GType for EailText implementation*/
GType eail_text_get_type(void);

#ifdef __cplusplus
}
#endif

#endif
