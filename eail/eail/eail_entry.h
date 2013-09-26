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
 * @file eail_entry.h
 *
 * @brief Header for EailEntry implementation
 */

#ifndef EAIL_ENTRY_H
#define EAIL_ENTRY_H

#include "eail_text.h"
/**
 * @brief Returns a value corresponding to the type of EailEntry class
 */
#define EAIL_TYPE_ENTRY              (eail_entry_get_type())
/**
 * @brief Macro upcasts an instance (obj) of a subclass to the EailEntry
 * type
 *
 * @param obj AtkObject instance
 */
#define EAIL_ENTRY(obj)              (G_TYPE_CHECK_INSTANCE_CAST((obj), \
                                      EAIL_TYPE_ENTRY, EailEntry))
/**
 * @brief Macro upcasts a subclass (klass) to the EailEntry class
 *
 * @param klass subclass object
 */
#define EAIL_ENTRY_CLASS(klass)      (G_TYPE_CHECK_CLASS_CAST((klass), \
                                      EAIL_TYPE_ENTRY, EailEntryClass))
/**
 * @brief Tests whether object (obj) is an instance of EailEntry class
 *
 * @param obj AtkObject instance
 */
#define EAIL_IS_ENTRY(obj)           (G_TYPE_CHECK_INSTANCE_TYPE((obj), \
                                      EAIL_TYPE_ENTRY))
/**
 * @brief Tests whether given klass is a subclass of EailEntry
 *
 * @param klass klass object
 */
#define EAIL_IS_ENTRY_CLASS(klass)   (G_TYPE_CHECK_CLASS_TYPE((klass), \
                                      EAIL_TYPE_ENTRY))
/**
 * @brief Gets EailEntry class structure from an obj (class instance)
 *
 * @param obj object instance to get EailEntry class from
 */
#define EAIL_ENTRY_GET_CLASS(obj)    (G_TYPE_INSTANCE_GET_CLASS((obj), \
                                      EAIL_TYPE_ENTRY, EailEntryClass))

#ifdef __cplusplus
extern "C" {
#endif

/** @brief Definition of object structure for Atk EailEntry*/
typedef struct _EailEntry      EailEntry;
/** @brief Definition of class structure for Atk EailEntry*/
typedef struct _EailEntryClass EailEntryClass;

/** @brief Definition of object structure for Atk EailEntry*/
struct _EailEntry
{
   /** @brief Parent object whose functionality is being extended*/
   EailWidget parent;
   /** @brief Selection region start */
   gint selection_start;
   /** @brief Selection regiond end */
   gint selection_end;
};

/** @brief Definition of class structure for Atk EailEntry*/
struct _EailEntryClass
{
   /** @brief Parent class whose functionality is being extended*/
   EailWidgetClass parent_class;
};

/** @brief Getter for EailEntry GType
 * @returns GType for EailEntry implementation*/
GType eail_entry_get_type(void);

#ifdef __cplusplus
}
#endif

#endif
