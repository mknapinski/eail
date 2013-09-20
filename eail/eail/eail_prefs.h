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
 * @file eail_prefs.h
 *
 * @brief Header for EailPrefs implementation
 */

#ifndef EAIL_PREFS_H
#define EAIL_PREFS_H

#include "eail_widget.h"
/**
 * @brief Returns a value corresponding to the type of EailPrefs class
 */
#define EAIL_TYPE_PREFS             (eail_prefs_get_type())
/**
 * @brief Macro upcasts an instance (obj) of a subclass to the EailPrefs
 * type
 *
 * @param obj an AtkObject
 */
#define EAIL_PREFS(obj)             (G_TYPE_CHECK_INSTANCE_CAST((obj), \
                                    EAIL_TYPE_PREFS, EailPrefs))
/**
 * @brief Macro upcasts a subclass (klass) to the EailPrefs class
 *
 * @param klass subclass object
 */
#define EAIL_PREFS_CLASS(klass)     (G_TYPE_CHECK_CLASS_CAST((klass), \
                                    EAIL_TYPE_PREFS, EailPrefsClass))
/**
 * @brief Tests whether object (obj) is an instance of EailPrefs class
 *
 * @param obj an AtkObject
 */
#define EAIL_IS_PREFS(obj)          (G_TYPE_CHECK_INSTANCE_TYPE((obj), \
                                    EAIL_TYPE_PREFS))
/**
 * @brief Tests whether given klass is a subclass of EailPrefs
 *
 * @param klass klass object
 */
#define EAIL_IS_PREFS_CLASS(klass)  (G_TYPE_CHECK_CLASS_TYPE((klass), \
                                    EAIL_TYPE_PREFS))
/**
 * @brief Gets EailPrefs class structure from an obj (class instance)
 *
 * @param obj object instance to get EailPrefs class from
 */
#define EAIL_PREFS_GET_CLASS(obj)   (G_TYPE_INSTANCE_GET_CLASS((obj), \
                                    EAIL_TYPE_PREFS, EailPrefsClass))

#ifdef __cplusplus
extern "C" {
#endif

/** @brief Definition of object structure for Atk EailPrefs*/
typedef struct _EailPrefs      EailPrefs;
/** @brief Definition of object class for Atk EailPrefs*/
typedef struct _EailPrefsClass EailPrefsClass;

/** @brief Definition of object structure for Atk EailPrefs*/
struct _EailPrefs
{
   /** @brief Parent object that functionality is being extended*/
   EailWidget parent;
};

/** @brief Definition of object class for Atk EailPrefs*/
struct _EailPrefsClass
{
   /** @brief Parent object that functionality is being extended*/
   EailWidgetClass parent_class;
};

/** @brief Getter for EailPrefs GType
 *
 * @returns GType for EailPrefs implementation*/
GType eail_prefs_get_type(void);

#ifdef __cplusplus
}
#endif

#endif
