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
 * @file eail_colorselector.h
 *
 * @brief Header for EailColorselector implementation
 */

#ifndef EAIL_COLORSELECTOR_H
#define EAIL_COLORSELECTOR_H

#include "eail_widget.h"
/**
 * @brief Returns a value corresponding to the type of EailColorselector class
 */
#define EAIL_TYPE_COLORSELECTOR              (eail_colorselector_get_type())
/**
 * @brief Macro upcasts an instance (obj) of a subclass to the EailColorselector
 * type
 *
 * @param obj an AtkObject
 */
#define EAIL_COLORSELECTOR(obj)              (G_TYPE_CHECK_INSTANCE_CAST((obj), \
                                              EAIL_TYPE_COLORSELECTOR, EailColorselector))
/**
 * @brief Macro upcasts a subclass (klass) to the EailColorselector class
 *
 * @param klass subclass object
 */
#define EAIL_COLORSELECTOR_CLASS(klass)      (G_TYPE_CHECK_CLASS_CAST((klass), \
                                              EAIL_TYPE_COLORSELECTOR, EailColorselectorClass))
/**
 * @brief Tests whether object (obj) is an instance of EailColorselector class
 *
 * @param obj an AtkObject
 */
#define EAIL_IS_COLORSELECTOR(obj)           (G_TYPE_CHECK_INSTANCE_TYPE((obj), \
                                              EAIL_TYPE_COLORSELECTOR))
/**
 * @brief Tests whether given klass is a subclass of EailColorselector
 *
 * @param klass klass object
 */
#define EAIL_IS_COLORSELECTOR_CLASS(klass)   (G_TYPE_CHECK_CLASS_TYPE((klass), \
                                              EAIL_TYPE_COLORSELECTOR))
/**
 * @brief Gets EailColorselector class structure from an obj (class instance)
 *
 * @param obj object instance to get EailColorselector class from
 */
#define EAIL_COLORSELECTOR_GET_CLASS(obj)    (G_TYPE_INSTANCE_GET_CLASS((obj), \
                                              EAIL_TYPE_COLORSELECTOR, EailColorselectorClass))

#ifdef __cplusplus
extern "C" {
#endif

/** @brief Definition of object structure for Atk EailColorselector*/
typedef struct _EailColorselector      EailColorselector;
/** @brief Definition of class structure for Atk EailColorselector*/
typedef struct _EailColorselectorClass EailColorselectorClass;

/** @brief Definition of object structure for Atk EailColorselector*/
struct _EailColorselector
{
   /** @brief parent object that functionality is being extended */
   EailWidget parent;
};

/** @brief Definition of class structure for Atk EailColorselector*/
struct _EailColorselectorClass
{
   /** @brief parent class whose functionality is being extended */
   EailWidgetClass parent_class;
};

/**
 * @brief Getter for EailColorselector GType
 *
 * @returns GType for EailColorselector implementation
 * */
GType eail_colorselector_get_type(void);

#ifdef __cplusplus
}
#endif

#endif
