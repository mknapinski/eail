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
 * @file eail_multibuttonentry.h
 *
 * @brief Header for EailMultibuttonentry implementation
 */

#ifndef EAIL_MULTIBUTTONENTRY_H
#define EAIL_MULTIBUTTONENTRY_H

#include "eail_action_widget.h"
/**
 * @brief Returns a value corresponding to the type of EailMultibuttonentry class
 */
#define EAIL_TYPE_MULTIBUTTONENTRY             (eail_multibuttonentry_get_type())
/**
 * @brief Macro upcasts an instance (obj) of a subclass to the EailMultibuttonentry
 * type
 *
 * @param obj AtkObject instance
 */
#define EAIL_MULTIBUTTONENTRY(obj)             (G_TYPE_CHECK_INSTANCE_CAST((obj), \
                                                EAIL_TYPE_MULTIBUTTONENTRY, EailMultibuttonentry))
/**
 * @brief Macro upcasts a subclass (klass) to the EailMultibuttonentry class
 *
 * @param klass subclass object
 */
#define EAIL_MULTIBUTTONENTRY_CLASS(klass)     (G_TYPE_MULTIBUTTONENTRY_CLASS_CAST((klass), \
                                                EAIL_TYPE_MULTIBUTTONENTRY, EailMultibuttonentryClass))
/**
 * @brief Tests whether object (obj) is an instance of EailMultibuttonentry class
 *
 * @param obj AtkObject instance
 */
#define EAIL_IS_MULTIBUTTONENTRY(obj)          (G_TYPE_CHECK_INSTANCE_TYPE((obj), \
                                                EAIL_TYPE_MULTIBUTTONENTRY))
/**
 * @brief Tests whether given klass is a subclass of EailMultibuttonentry
 *
 * @param klass klass object
 */
#define EAIL_IS_MULTIBUTTONENTRY_CLASS(klass)  (G_TYPE_CHECK_CLASS_TYPE((klass), \
                                                EAIL_TYPE_MULTIBUTTONENTRY))
/**
 * @brief Gets EailMultibuttonentry class structure from an obj (class instance)
 *
 * @param obj object instance to get EailMultibuttonentry class from
 */
#define EAIL_MULTIBUTTONENTRY_GET_CLASS(obj)   (G_TYPE_INSTANCE_GET_CLASS((obj), \
                                                EAIL_TYPE_MULTIBUTTONENTRY, EailMultibuttonentryClass))

#ifdef __cplusplus
extern "C" {
#endif

/** @brief Definition of object structure for Atk EailMultibuttonentry*/
typedef struct _EailMultibuttonentry      EailMultibuttonentry;
/** @brief Definition of object class for Atk EailMultibuttonentry*/
typedef struct _EailMultibuttonentryClass EailMultibuttonentryClass;

/** @brief Definition of object structure for Atk EailMultibuttonentry*/
struct _EailMultibuttonentry
{
   /** @brief Parent widget whose functionality is being extended*/
   EailActionWidget parent;
};

/** @brief Definition of object class for Atk EailMultibuttonentry*/
struct _EailMultibuttonentryClass
{
   /** @brief class that is being extended*/
   EailActionWidgetClass parent_class;
};

/**
 * @brief Getter for EailMultibuttonentry GType
 * @returns GType for EailMultibuttonentry implementation
 */
GType eail_multibuttonentry_get_type(void);

#ifdef __cplusplus
}
#endif

#endif
