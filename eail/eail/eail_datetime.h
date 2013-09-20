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
 * @file eail_datetime.h
 *
 * @brief Header for EailDatetime implementation
 */

#ifndef EAIL_DATETIME_H
#define EAIL_DATETIME_H

#include "eail_widget.h"
/**
 * @brief Returns a value corresponding to the type of EailDatetime class
 */
#define EAIL_TYPE_DATETIME             (eail_datetime_get_type())
/**
 * @brief Macro upcasts an instance (obj) of a subclass to the EailDatetime
 * type
 *
 * @param obj an AtkObject
 */
#define EAIL_DATETIME(obj)             (G_TYPE_CHECK_INSTANCE_CAST((obj), \
                                        EAIL_TYPE_DATETIME, EailDatetime))
/**
 * @brief Macro upcasts a subclass (klass) to the EailDatetime class
 *
 * @param klass subclass object
 */
#define EAIL_DATETIME_CLASS(klass)     (G_TYPE_CHECK_CLASS_CAST((klass), \
                                        EAIL_TYPE_DATETIME, EailDatetimeClass))
/**
 * @brief Tests whether object (obj) is an instance of EailDatetime class
 *
 * @param obj an AtkObject
 */
#define EAIL_IS_DATETIME(obj)          (G_TYPE_CHECK_INSTANCE_TYPE((obj), \
                                        EAIL_TYPE_DATETIME))
/**
 * @brief Tests whether given klass is a subclass of EailDatetime
 *
 * @param klass klass object
 */
#define EAIL_IS_DATETIME_CLASS(klass)  (G_TYPE_CHECK_CLASS_TYPE((klass), \
                                        EAIL_TYPE_DATETIME))
/**
 * @brief Gets EailDatetime class structure from an obj (class instance)
 *
 * @param obj object instance to get EailDatetime class from
 */
#define EAIL_DATETIME_GET_CLASS(obj)   (G_TYPE_INSTANCE_GET_CLASS((obj), \
                                        EAIL_TYPE_DATETIME, EailDatetimeClass))

#ifdef __cplusplus
extern "C" {
#endif

/** @brief Definition of object structure for Atk EailDatetime*/
typedef struct _EailDatetime      EailDatetime;

/** @brief Definition of class structure for Atk EailDatetime*/
typedef struct _EailDatetimeClass EailDatetimeClass;

/** @brief Definition of object structure for Atk EailDatetime*/
struct _EailDatetime
{
   /** @brief Parent object that functionality is being extended*/
   EailWidget parent;
};

/** @brief Definition of class structure for Atk EailDatetime*/
struct _EailDatetimeClass
{
   /** @brief Parent class whose functionality is being extended*/
   EailWidgetClass parent_class;
};

/** @brief Getter for EailDatetime GType
 * @returns GType for EailDatetime implementation*/
GType eail_datetime_get_type(void);

#ifdef __cplusplus
}
#endif

#endif
