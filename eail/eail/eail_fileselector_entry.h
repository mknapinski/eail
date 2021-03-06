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
 * @file eail_fileselector_entry.h
 *
 * @brief Header for EailEntry implementation
 */

#ifndef EAIL_FILESELECTOR_ENTRY_H
#define EAIL_FILESELECTOR_ENTRY_H

#include "eail_action_widget.h"
/**
 * @brief Returns a value corresponding to the type of EailFileselectorentry class
 */
#define EAIL_TYPE_FILESELECTOR_ENTRY             (eail_fileselector_entry_get_type())
/**
 * @brief Macro upcasts an instance (obj) of a subclass to the EailFileselectorentry
 * type
 *
 * @param obj AtkObject instance
 */
#define EAIL_FILESELECTOR_ENTRY(obj)             (G_TYPE_CHECK_INSTANCE_CAST((obj), \
                                                  EAIL_TYPE_FILESELECTOR_ENTRY, EailFileselectorEntry))
/**
 * @brief Macro upcasts a subclass (klass) to the EailFileselectorentry class
 *
 * @param klass subclass object
 */
#define EAIL_FILESELECTOR_ENTRY_CLASS(klass)     (G_TYPE_FILESELECTOR_ENTRY_CLASS_CAST((klass), \
                                                  EAIL_TYPE_FILESELECTOR_ENTRY, EailFileselectorEntryClass))
/**
 * @brief Tests whether object (obj) is an instance of EailFileselectorentry class
 *
 * @param obj AtkObject instance
 */
#define EAIL_IS_FILESELECTOR_ENTRY(obj)          (G_TYPE_CHECK_INSTANCE_TYPE((obj), \
                                                  EAIL_TYPE_FILESELECTOR_ENTRY))
/**
 * @brief Tests whether given klass is a subclass of EailFileselectorentry
 *
 * @param klass klass object
 */
#define EAIL_IS_FILESELECTOR_ENTRY_CLASS(klass)  (G_TYPE_CHECK_CLASS_TYPE((klass), \
                                                  EAIL_TYPE_FILESELECTOR_ENTRY))
/**
 * @brief Gets EailFileselectorentry class structure from an obj (class instance)
 *
 * @param obj object instance to get EailEntry class from
 */
#define EAIL_FILESELECTOR_ENTRY_GET_CLASS(obj)   (G_TYPE_INSTANCE_GET_CLASS((obj), \
                                                  EAIL_TYPE_FILESELECTOR_ENTRY, EailFileselectorEntryClass))

#ifdef __cplusplus
extern "C" {
#endif

/** @brief Definition of object structure for Atk EailEntry*/
typedef struct _EailFileselectorEntry      EailFileselectorEntry;
/** @brief Definition of class structure for Atk EailEntry*/
typedef struct _EailFileselectorEntryClass EailFileselectorEntryClass;

/** @brief Definition of object structure for Atk EailEntry*/
struct _EailFileselectorEntry
{
   EailActionWidget parent;/**< @brief Parent widget whose functionality is being extended */
   /** @brief Selection region start */
   gint selection_start;
   /** @brief Selection region end */
   gint selection_end;
};

/** @brief Definition of object structure for Atk EailEntry*/
struct _EailFileselectorEntryClass
{
   EailActionWidgetClass parent_class;/**< @brief parent class that is being extended*/
};

/**
 * \brief Getter for fileselector GType
 * @returns GType for EailFileselector implementation
 */
GType eail_fileselector_entry_get_type(void);

#ifdef __cplusplus
}
#endif

#endif
