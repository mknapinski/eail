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
 * @file eail_check.h
 *
 * @brief Header for EailCheck implementation
 */

#ifndef EAIL_CHECK_H
#define EAIL_CHECK_H

#include "eail_action_widget.h"
/**
 * @brief Returns a value corresponding to the type of EailCheck class
 */
#define EAIL_TYPE_CHECK             (eail_check_get_type())
/**
 * @brief Macro upcasts an instance (obj) of a subclass to the EailCheck
 * type
 *
 * @param obj AtkObject instance
 */
#define EAIL_CHECK(obj)             (G_TYPE_CHECK_INSTANCE_CAST((obj), \
                                     EAIL_TYPE_CHECK, EailCheck))
/**
 * @brief Macro upcasts a subclass (klass) to the EailCheck class
 *
 * @param klass subclass object
 */
#define EAIL_CHECK_CLASS(klass)     (G_TYPE_CHECK_CLASS_CAST((klass), \
                                     EAIL_TYPE_CHECK, EailCheckClass))
/**
 * @brief Tests whether object (obj) is an instance of EailCheck class
 *
 * @param obj AtkObject instance
 */
#define EAIL_IS_CHECK(obj)          (G_TYPE_CHECK_INSTANCE_TYPE((obj), \
                                     EAIL_TYPE_CHECK))
/**
 * @brief Tests whether given klass is a subclass of EailCheck
 *
 * @param klass klass object
 */
#define EAIL_IS_CHECK_CLASS(klass)  (G_TYPE_CHECK_CLASS_TYPE((klass), \
                                     EAIL_TYPE_CHECK))
/**
 * @brief Gets EailCheck class structure from an obj (class instance)
 *
 * @param obj object instance to get EailCheck class from
 */
#define EAIL_CHECK_GET_CLASS(obj)   (G_TYPE_INSTANCE_GET_CLASS((obj), \
                                     EAIL_TYPE_CHECK, EailCheckClass))

#ifdef __cplusplus
extern "C" {
#endif

/** @brief Definition of object structure for Atk EailCheck*/
typedef struct _EailCheck      EailCheck;
/** @brief Definition of class structure for Atk EailCheck*/
typedef struct _EailCheckClass EailCheckClass;

/** @brief Definition of object structure for Atk EailCheck*/
struct _EailCheck
{
   EailActionWidget parent; /*!< @brief Parent widget whose functionality is being extended*/
};

/** @brief Definition of class structure for Atk EailCheck*/
struct _EailCheckClass
{
   /** @brief parent class whose functionality is being extended */
   EailActionWidgetClass parent_class;
};

/**
 * @brief Getter for EailCheck GType
 *
 * @returns GType for EailCheck implementation
 */
GType eail_check_get_type(void);

#ifdef __cplusplus
}
#endif

#endif
