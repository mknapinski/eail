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
 * @file eail_mapbuf.h
 *
 * @brief Header for EailMapbuf implementation
 */

#ifndef EAIL_MAPBUF_H
#define EAIL_MAPBUF_H

#include "eail_widget.h"
/**
 * @brief Returns a value corresponding to the type of EailMapbuf class
 */
#define EAIL_TYPE_MAPBUF            (eail_mapbuf_get_type())
/**
 * @brief Macro upcasts an instance (obj) of a subclass to the EailMapbuf
 * type
 *
 * @param obj an AtkObject
 */
#define EAIL_MAPBUF(obj)            (G_TYPE_CHECK_INSTANCE_CAST((obj), \
                                     EAIL_TYPE_MAPBUF, EailMapbuf))
/**
 * @brief Macro upcasts a subclass (klass) to the EailMapbuf class
 *
 * @param klass subclass object
 */
#define EAIL_MAPBUF_CLASS(klass)    (G_TYPE_CHECK_CLASS_CAST((klass), \
                                     EAIL_TYPE_MAPBUF, EailMapbufClass))
/**
 * @brief Tests whether object (obj) is an instance of EailMapbuf class
 *
 * @param obj an AtkObject
 */
#define EAIL_IS_MAPBUF(obj)         (G_TYPE_CHECK_INSTANCE_TYPE((obj), \
                                     EAIL_TYPE_MAPBUF))
/**
 * @brief Tests whether given klass is a subclass of EailMapbuf
 *
 * @param klass klass object
 */
#define EAIL_IS_MAPBUF_CLASS(klass) (G_TYPE_CHECK_CLASS_TYPE((klass), \
                                     EAIL_TYPE_MAPBUF))
/**
 * @brief Gets EailMapbuf class structure from an obj (class instance)
 *
 * @param obj object instance to get EailMapbuf class from
 */
#define EAIL_MAPBUF_GET_CLASS(obj)  (G_TYPE_INSTANCE_GET_CLASS((obj), \
                                     EAIL_TYPE_MAPBUF, EailMapbufClass))

#ifdef __cplusplus
extern "C" {
#endif

/** @brief Definition of object structure for Atk EailMapbuf*/
typedef struct _EailMapbuf      EailMapbuf;
/** @brief Definition of object class for Atk EailMapbuf*/
typedef struct _EailMapbufClass EailMapbufClass;

/** @brief Definition of object structure for Atk EailMapbuf*/
struct _EailMapbuf
{
   /** @brief Parent object that functionality is being extended*/
   EailWidget parent;
};

/** @brief Definition of object class for Atk EailMapbuf*/
struct _EailMapbufClass
{
   /** @brief Parent class whose functionality is being extended*/
   EailWidgetClass parent_class;
};

/**@brief Getter for EailMapbuf GType
 * @returns GType for EailMapbuf implementation */
GType eail_mapbuf_get_type(void);

#ifdef __cplusplus
}
#endif

#endif
