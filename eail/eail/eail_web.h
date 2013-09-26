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
 * @file eail_web.h
 *
 * @brief Header for EailWeb implementation
 */

#ifndef EAIL_WEB_H
#define EAIL_WEB_H

#include "eail_widget.h"

/**
 * @brief Returns a value corresponding to the type of EailWeb class
 */
#define EAIL_TYPE_WEB             (eail_web_get_type())

/**
 * @brief Macro upcasts an instance (obj) of a subclass to the EailWeb
 * type
 *
 * @param obj AtkObject instance
 */
#define EAIL_WEB(obj)             (G_TYPE_CHECK_INSTANCE_CAST((obj), \
                                            EAIL_TYPE_WEB, \
                                            EailWeb))

/**
 * @brief Macro upcasts a subclass (klass) to the EailWeb class
 *
 * @param klass subclass object
 */
#define EAIL_WEB_CLASS(klass)     (G_TYPE_WEB_CLASS_CAST((klass), \
                                            EAIL_TYPE_WEB, \
                                            EailWebClass))

/**
 * @brief Tests whether object (obj) is an instance of EailWeb class
 *
 * @param obj AtkObject instance
 */
#define EAIL_IS_WEB(obj)          (G_TYPE_CHECK_INSTANCE_TYPE((obj), \
                                            EAIL_TYPE_WEB))

/**
 * @brief Tests whether given klass is a subclass of EailWeb
 *
 * @param klass klass object
 */
#define EAIL_IS_WEB_CLASS(klass)  (G_TYPE_CHECK_CLASS_TYPE((klass), \
                                            EAIL_TYPE_WEB))

/**
 * @brief Gets EailWeb class structure from an obj (class instance)
 *
 * @param obj object instance to get EailWeb class from
 */
#define EAIL_WEB_GET_CLASS(obj)   (G_TYPE_INSTANCE_GET_CLASS((obj), \
                                            EAIL_TYPE_WEB, \
                                            EailWebClass))

#ifdef __cplusplus
extern "C" {
#endif

/** @brief Definition of object structure for Atk EailWeb*/
typedef struct _EailWeb EailWeb;

/** @brief Definition of object class for Atk EailWeb*/
typedef struct _EailWebClass EailWebClass;

/** @brief Definition of object structure for Atk EailWeb*/
struct _EailWeb
{
   EailWidget parent;/**< @brief Parent widget whose functionality is being extended*/
};

/** @brief Definition of object class for Atk EailWeb*/
struct _EailWebClass
{
   EailWidgetClass parent_class;/**< @brief class whose functionality is being extended*/
};

/** @brief Getter for EailWeb GType
 *
 * @returns GType for EailWeb implementation*/
GType eail_web_get_type(void);

#ifdef __cplusplus
}
#endif

#endif

