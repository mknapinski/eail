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
 * @file eail_gengrid.h
 *
 * @brief Header for EailGengrid implementation
 */

#ifndef EAIL_GENGRID_H
#define EAIL_GENGRID_H

#include "eail_widget.h"
/**
 * @brief Returns a value corresponding to the type of EailGengrid class
 */
#define EAIL_TYPE_GENGRID            (eail_gengrid_get_type())
/**
 * @brief Macro upcasts an instance (obj) of a subclass to the EailGengrid
 * type
 *
 * @param obj AtkObject instance
 */
#define EAIL_GENGRID(obj)            (G_TYPE_CHECK_INSTANCE_CAST((obj), \
                                      EAIL_TYPE_GENGRID, EailGengrid))
/**
 * @brief Macro upcasts a subclass (klass) to the EailGengrid class
 *
 * @param klass subclass object
 */
#define EAIL_GENGRID_CLASS(klass)    (G_TYPE_CHECK_CLASS_CAST((klass), \
                                      EAIL_TYPE_GENGRID, EailGengridClass))
/**
 * @brief Tests whether object (obj) is an instance of EailGengrid class
 *
 * @param obj AtkObject instance
 */
#define EAIL_IS_GENGRID(obj)         (G_TYPE_CHECK_INSTANCE_TYPE((obj), \
                                      EAIL_TYPE_GENGRID))
/**
 * @brief Tests whether given klass is a subclass of EailGengrid
 *
 * @param klass klass object
 */
#define EAIL_IS_GENGRID_CLASS(klass) (G_TYPE_CHECK_CLASS_TYPE((klass), \
                                      EAIL_TYPE_GENGRID))
/**
 * @brief Gets EailGengrid class structure from an obj (class instance)
 *
 * @param obj object instance to get EailGengrid class from
 */
#define EAIL_GENGRID_GET_CLASS(obj)  (G_TYPE_INSTANCE_GET_CLASS((obj), \
                                      EAIL_TYPE_GENGRID, EailGengridClass))

#ifdef __cplusplus
extern "C" {
#endif

/** @brief Definition of object structure for Atk EailGengrid*/
typedef struct _EailGengrid      EailGengrid;
/** @brief Definition of class structure for Atk EailGengrid*/
typedef struct _EailGengridClass EailGengridClass;

/** @brief Definition of object structure for Atk EailGengrid*/
struct _EailGengrid
{
   /** @brief Parent widget whose functionality is being extended*/
   EailWidget parent;
};

/** @brief Definition of class structure for Atk EailGengrid*/
struct _EailGengridClass
{
   /** @brief Parent class whose functionality is being extended*/
   EailWidgetClass parent_class;
};

/** @brief Getter for EailGengrid GType
 * @returns GType for EailGengrid implementation*/
GType eail_gengrid_get_type(void);

#ifdef __cplusplus
}
#endif

#endif
