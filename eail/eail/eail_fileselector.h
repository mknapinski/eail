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
 * @file eail_fileselector.h
 *
 * @brief Header for EailFileselector implementation
 */

#ifndef EAIL_FILESELECTOR_H
#define EAIL_FILESELECTOR_H

#include "eail_layout.h"
/**
 * @brief Returns a value corresponding to the type of EailFileselector class
 */
#define EAIL_TYPE_FILESELECTOR            (eail_fileselector_get_type())
/**
 * @brief Macro upcasts an instance (obj) of a subclass to the EailFileselector
 * type
 *
 * @param obj an AtkObject
 */
#define EAIL_FILESELECTOR(obj)            (G_TYPE_CHECK_INSTANCE_CAST((obj), \
                                           EAIL_TYPE_FILESELECTOR, EailFileselector))
/**
 * @brief Macro upcasts a subclass (klass) to the EailFileselector class
 *
 * @param klass subclass object
 */
#define EAIL_FILESELECTOR_CLASS(klass)    (G_TYPE_FILESELECTOR_CLASS_CAST((klass), \
                                           EAIL_TYPE_FILESELECTOR, EailFileselectorClass))
/**
 * @brief Tests whether object (obj) is an instance of EailFileselector class
 *
 * @param obj an AtkObject
 */
#define EAIL_IS_FILESELECTOR(obj)         (G_TYPE_CHECK_INSTANCE_TYPE((obj), \
                                           EAIL_TYPE_FILESELECTOR))
/**
 * @brief Tests whether given klass is a subclass of EailFileselector
 *
 * @param klass klass object
 */
#define EAIL_IS_FILESELECTOR_CLASS(klass) (G_TYPE_CHECK_CLASS_TYPE((klass), \
                                           EAIL_TYPE_FILESELECTOR))
/**
 * @brief Gets EailFileselector class structure from an obj (class instance)
 *
 * @param obj object instance to get EailFileselector class from
 */
#define EAIL_FILESELECTOR_GET_CLASS(obj)  (G_TYPE_INSTANCE_GET_CLASS((obj), \
                                           EAIL_TYPE_FILESELECTOR, EailFileselectorClass))

#ifdef __cplusplus
extern "C" {
#endif

/** @brief Definition of object structure for Atk EailFileselector*/
typedef struct _EailFileselector      EailFileselector;
/** @brief Definition of class structure for Atk EailFileselector*/
typedef struct _EailFileselectorClass EailFileselectorClass;

/** @brief Definition of object structure for Atk EailFileselector*/
struct _EailFileselector
{
   /** @brief Parent object that functionality is being extended*/
   EailLayout parent;
};

/** @brief Definition of class structure for Atk EailFileselector*/
struct _EailFileselectorClass
{
   /** @brief Parent class whose functionality is being extended*/
   EailLayoutClass parent_class;
};

/** @brief Getter for EailFileselector GType
 * @returns GType for EailFileselector implementation*/
GType eail_fileselector_get_type(void);

#ifdef __cplusplus
}
#endif

#endif
