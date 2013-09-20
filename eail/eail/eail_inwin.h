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
 * @file eail_inwin.h
 *
 * @brief Header for EailInwin implementation
 */

#ifndef EAIL_INWIN_H
#define EAIL_INWIN_H

#include "eail_widget.h"
/**
 * @brief Returns a value corresponding to the type of EailInwin class
 */
#define EAIL_TYPE_INWIN             (eail_inwin_get_type())
/**
 * @brief Macro upcasts an instance (obj) of a subclass to the EailInwin
 * type
 *
 * @param obj an AtkObject
 */
#define EAIL_INWIN(obj)             (G_TYPE_CHECK_INSTANCE_CAST((obj), \
                                     EAIL_TYPE_INWIN, EailInwin))
/**
 * @brief Macro upcasts a subclass (klass) to the EailInwin class
 *
 * @param klass subclass object
 */
#define EAIL_INWIN_CLASS(klass)     (G_TYPE_CHECK_CLASS_CAST((klass), \
                                     EAIL_TYPE_INWIN, EailInwinClass))
/**
 * @brief Tests whether object (obj) is an instance of EailInwin class
 *
 * @param obj an AtkObject
 */
#define EAIL_IS_INWIN(obj)          (G_TYPE_CHECK_INSTANCE_TYPE((obj), \
                                     EAIL_TYPE_INWIN))
/**
 * @brief Tests whether given klass is a subclass of EailInwin
 *
 * @param klass klass object
 */
#define EAIL_IS_INWIN_CLASS(klass)  (G_TYPE_CHECK_CLASS_TYPE((klass), \
                                     EAIL_TYPE_INWIN))
/**
 * @brief Gets EailInwin class structure from an obj (class instance)
 *
 * @param obj object instance to get EailInwin class from
 */
#define EAIL_INWIN_GET_CLASS(obj)   (G_TYPE_INSTANCE_GET_CLASS((obj), \
                                     EAIL_TYPE_WINDOW, EailInwinClass))

#ifdef __cplusplus
extern "C" {
#endif

/** @brief Definition of object structure for Atk EailInwin*/
typedef struct _EailInwin      EailInwin;
/** @brief Definition of class structure for Atk EailInwin*/
typedef struct _EailInwinClass EailInwinClass;

/** @brief Definition of object structure for Atk EailInwin*/
struct _EailInwin
{
   /** @brief Parent object that functionality is being extended*/
   EailWidget parent;
};

/** @brief Definition of class structure for Atk EailInwin*/
struct _EailInwinClass
{
   /** @brief Parent class whose functionality is being extended*/
   EailWidgetClass parent_class;
};

/** @brief Getter for EailInwin GType
 * @returns GType for EailInwin implementation*/
GType eail_inwin_get_type(void);

#ifdef __cplusplus
}
#endif

#endif
