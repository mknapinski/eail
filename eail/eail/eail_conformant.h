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
 * @file eail_conformant.h
 *
 * @brief Header for EailConformant implementation
 */

#ifndef EAIL_CONFORMANT_H
#define EAIL_CONFORMANT_H

#include "eail_layout.h"

/**
 * @brief Returns a value corresponding to the type of EailConformant class
 */
#define EAIL_TYPE_CONFORMANT              (eail_conformant_get_type())

/**
 * @brief Macro upcasts an instance (obj) of a subclass to the EailConformant
 * type
 *
 * @param obj AtkObject instance
 */
#define EAIL_CONFORMANT(obj)              (G_TYPE_CHECK_INSTANCE_CAST((obj), \
                                           EAIL_TYPE_CONFORMANT, EailConformant))

/**
 * @brief Macro upcasts a subclass (klass) to the EailConformant class
 *
 * @param klass subclass object
 */
#define EAIL_CONFORMANT_CLASS(klass)      (G_TYPE_CHECK_CLASS_CAST((klass), \
                                           EAIL_TYPE_CONFORMANT, EailConformantClass))

/**
 * @brief Tests whether object (obj) is an instance of EailConformant class
 *
 * @param obj AtkObject instance
 */
#define EAIL_IS_CONFORMANT(obj)           (G_TYPE_CHECK_INSTANCE_TYPE((obj), \
                                           EAIL_TYPE_CONFORMANT))

/**
 * @brief Tests whether given klass is a subclass of EailConformant
 *
 * @param klass klass object
 */
#define EAIL_IS_CONFORMANT_CLASS(klass)   (G_TYPE_CHECK_CLASS_TYPE((klass), \
                                           EAIL_TYPE_CONFORMANT))

/**
 * @brief Gets EailConformant class structure from an obj (class instance)
 *
 * @param obj object instance to get EailConformant class from
 */
#define EAIL_CONFORMANT_GET_CLASS(obj)    (G_TYPE_INSTANCE_GET_CLASS((obj), \
                                           EAIL_TYPE_CONFORMANT, EailConformantClass))

#ifdef __cplusplus
extern "C" {
#endif

/** @brief Definition of object structure for Atk EailConformant*/
typedef struct _EailConformant      EailConformant;

/** @brief Definition of class structure for Atk EailConformant*/
typedef struct _EailConformantClass EailConformantClass;

/** @brief Definition of object structure for Atk EailConformant*/
struct _EailConformant
{
   /** @brief Parent widget whose functionality is being extended */
   EailLayout parent;
};

/** @brief Definition of class structure for Atk EailConformant*/
struct _EailConformantClass
{
   /** @brief parent class whose functionality is being extended */
   EailLayoutClass parent_class;
};

/** @brief Getter for EailConformant GType
 *
 * @returns GType for EailConfomant implementation*/
GType eail_conformant_get_type(void);

#ifdef __cplusplus
}
#endif

#endif
