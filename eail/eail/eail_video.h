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
 * @file eail_video.h
 *
 * @brief Header for EailVideo implementation
 */
#ifndef EAIL_VIDEO_H
#define EAIL_VIDEO_H

#include "eail_widget.h"

/**
 * @brief Returns a value corresponding to the type of EailVideo class
 */
#define EAIL_TYPE_VIDEO             (eail_video_get_type ())

/**
 * @brief Macro upcasts an instance (obj) of a subclass to the EailVideo
 * type
 *
 * @param obj an AtkObject
 */
#define EAIL_VIDEO(obj)             (G_TYPE_CHECK_INSTANCE_CAST ((obj), \
                                     EAIL_TYPE_VIDEO, EailVideo))

/**
 * @brief Macro upcasts a subclass (klass) to the EailVideo class
 *
 * @param klass subclass object
 */
#define EAIL_VIDEO_CLASS(klass)     (G_TYPE_CHECK_CLASS_CAST ((klass), \
                                     EAIL_TYPE_VIDEO, EailVideoClass))

/**
 * @brief Tests whether object (obj) is an instance of EailVideo class
 *
 * @param obj an AtkObject
 */
#define EAIL_IS_VIDEO(obj)          (G_TYPE_CHECK_INSTANCE_TYPE ((obj), \
                                     EAIL_TYPE_VIDEO))

/**
 * @brief Tests whether given klass is a subclass of EailVideo
 *
 * @param klass klass object
 */
#define EAIL_IS_VIDEO_CLASS(klass)  (G_TYPE_CHECK_CLASS_TYPE ((klass), \
                                     EAIL_TYPE_VIDEO))

/**
 * @brief Gets EailVideo class structure from an obj (class instance)
 *
 * @param obj object instance to get EailVideo class from
 */
#define EAIL_VIDEO_GET_CLASS(obj)   (G_TYPE_INSTANCE_GET_CLASS ((obj), \
                                     EAIL_TYPE_VIDEO, EailVideoClass))
#ifdef __cplusplus
extern "C" {
#endif

/** @brief Definition of object structure for Atk EailVideo*/
typedef struct _EailVideo       EailVideo;

/** @brief Definition of object structure for Atk EailVideo*/
typedef struct _EailVideoClass  EailVideoClass;

/** @brief Definition of object structure for Atk EailVideo*/
struct _EailVideo
{
    /** @brief Parent widget whose functionality is being extended*/
    EailWidget parent;
    /** @brief 'forward' action description*/
    char *forward_desc;
    /** @brief 'next' action descritpion*/
    char *next_desc;
    /** @brief 'pause' action descirption*/
    char *pause_desc;
    /** @brief 'play' action description*/
    char *play_desc;
    /** @brief 'prev' action description*/
    char *prev_desc;
    /** @brief 'rewind' action description*/
    char *rewind_desc;
    /** @brief 'stop' action description*/
    char *stop_desc;
    /** @brief Video file URI*/
    char *uri;
};

/** @brief Definition of object structure for Atk EailVideo*/
struct _EailVideoClass
{
    EailWidgetClass parent_class;/*!< class that is being extended*/
};

/** @brief Getter for EailVideo GType
 *
 * @returns GType for EailVideo implementation*/
GType eail_video_get_type (void);

#ifdef __cplusplus
}
#endif

#endif
