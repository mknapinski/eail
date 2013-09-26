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
 * @file eail_separator.h
 *
 * @brief Header for EailSeparator implementation
 */

#ifndef EAIL_SEPARATOR_H
#define EAIL_SEPARATOR_H

#include "eail_widget.h"
/**
 * @brief Returns a value corresponding to the type of EailSeparator class
 */
#define EAIL_TYPE_SEPARATOR            (eail_separator_get_type())
/**
 * @brief Macro upcasts an instance (obj) of a subclass to the EailSeparator
 * type
 *
 * @param obj AtkObject instance
 */
#define EAIL_SEPARATOR(obj)            (G_TYPE_CHECK_INSTANCE_CAST((obj), \
                                        EAIL_TYPE_SEPARATOR, EailSeparator))
/**
 * @brief Macro upcasts a subclass (klass) to the EailSeparator class
 *
 * @param klass subclass object
 */
#define EAIL_SEPARATOR_CLASS(klass)    (G_TYPE_CHECK_CLASS_CAST((klass), \
                                        EAIL_TYPE_SEPARATOR, EailSeparatorClass))
/**
 * @brief Tests whether object (obj) is an instance of EailSeparator class
 *
 * @param obj AtkObject instance
 */
#define EAIL_IS_SEPARATOR(obj)         (G_TYPE_CHECK_INSTANCE_TYPE((obj), \
                                        EAIL_TYPE_SEPARATOR))
/**
 * @brief Tests whether given klass is a subclass of EailSeparator
 *
 * @param klass klass object
 */
#define EAIL_IS_SEPARATOR_CLASS(klass) (G_TYPE_CHECK_CLASS_TYPE((klass), \
                                        EAIL_TYPE_SEPARATOR))
/**
 * @brief Gets EailSeparator class structure from an obj (class instance)
 *
 * @param obj object instance to get EailActionSlider class from
 */
#define EAIL_SEPARATOR_GET_CLASS(obj)  (G_TYPE_INSTANCE_GET_CLASS((obj), \
                                        EAIL_TYPE_SEPARATOR, EailSeparatorClass))

#ifdef __cplusplus
extern "C" {
#endif

/** @brief Definition of object structure for Atk EailActionSlider*/
typedef struct _EailSeparator      EailSeparator;
/** @brief Definition of object class for Atk EailActionSlider*/
typedef struct _EailSeparatorClass EailSeparatorClass;

/** @brief Definition of object structure for Atk EailActionSlider*/
struct _EailSeparator
{
   /** @brief Parent widget whose functionality is being extended*/
   EailWidget parent;
};

/** @brief Definition of object class for Atk EailActionSlider*/
struct _EailSeparatorClass
{
   /** @brief Parent class whose functionality is being extended*/
   EailWidgetClass parent_class;
};

/** @brief Getter for EailSeparator GType
 *
 * @returns GType for EailSeparator implementation
 * */
GType eail_separator_get_type(void);

#ifdef __cplusplus
}
#endif

#endif
