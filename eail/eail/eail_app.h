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
 * @file eail_app.h
 *
 * @brief Header for EailApp implementation
 */

#ifndef EAIL_APP_H
#define EAIL_APP_H

#include <atk/atk.h>

/**
 * @brief Returns a value corresponding to the type of EailApp class
 */
#define EAIL_TYPE_APP             (eail_app_get_type())

/**
 * @brief Macro upcasts an instance (obj) of a subclass to the EailApp
 * type
 *
 * @param obj an AtkObject
 */
#define EAIL_APP(obj)             (G_TYPE_CHECK_INSTANCE_CAST((obj), \
                                   EAIL_TYPE_APP, EailApp))

/**
 * @brief Macro upcasts a subclass (klass) to the EailApp class
 *
 * @param klass subclass object
 */
#define EAIL_APP_CLASS(klass)     (G_TYPE_CHECK_CLASS_CAST((klass), \
                                   EAIL_TYPE_APP, EailAppClass))

/**
 * @brief Tests whether object (obj) is an instance of EailApp class
 *
 * @param obj an AtkObject
 */
#define EAIL_IS_APP(obj)          (G_TYPE_CHECK_INSTANCE_TYPE((obj), \
                                   EAIL_TYPE_APP))

/**
 * @brief Tests whether given klass is a subclass of EailApp
 *
 * @param klass klass object
 */
#define EAIL_IS_APP_CLASS(klass)  (G_TYPE_CHECK_CLASS_TYPE((klass), \
                                   EAIL_TYPE_APP))

/**
 * @brief Gets EailApp class structure from an obj (class instance)
 *
 * @param obj object instance to get EailApp class from
 */
#define EAIL_APP_GET_CLASS(obj)   (G_TYPE_INSTANCE_GET_CLASS((obj), \
                                   EAIL_TYPE_APP, EailAppClass))
#ifdef __cplusplus
extern "C" {
#endif

/** @brief Definition of object structure for Atk EailApp*/
typedef struct _EailApp      EailApp;
/** @brief Definition of class structure for Atk EailApp*/
typedef struct _EailAppClass EailAppClass;

/** @brief Definition of object structure for Atk EailBackground*/
struct _EailApp
{
   /** @brief parent AtkObject that functionality is being extended */
   AtkObject parent;
   gint child_count_last;/**< @brief last reported count of children */
};

/** @brief Definition of class structure for Atk EailBackground*/
struct _EailAppClass
{
   /** @brief parent class whose functionality is being extended */
   AtkObjectClass parent_class;
};

/** @brief Getter for EailApp GType
 *
 * @returns GType for EailApp implementation*/
GType eail_app_get_type(void);

#ifdef __cplusplus
}
#endif

#endif
