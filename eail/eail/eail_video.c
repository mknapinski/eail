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
 * @file eail_video.c
 * @brief EailVideo implementation
 */
#include <Elementary.h>
#include <Emotion.h>
#include <gio/gio.h>

#include "eail_video.h"
#include "eail_utils.h"
#include "eail_priv.h"

static void atk_action_interface_init(AtkActionIface *iface);
static void atk_stremable_content_iface_init(AtkStreamableContentIface *iface);

/**
 * @brief Define EailVideo type
 */
G_DEFINE_TYPE_WITH_CODE(EailVideo, eail_video, EAIL_TYPE_WIDGET,
                        G_IMPLEMENT_INTERFACE(ATK_TYPE_ACTION,
                                              atk_action_interface_init)
                        G_IMPLEMENT_INTERFACE(ATK_TYPE_STREAMABLE_CONTENT,
                                              atk_stremable_content_iface_init)
);

/**
 * @brief Gets a reference to the state set of the accessible
 *
 * The caller must unreference it when it is no longer needed.
 *
 * Implementation of ref_state_set from AtkObject.
 *
 * @param object AtkObject instance
 *
 * @returns AtkStateSet representing the state set of the accessible
 */
static AtkStateSet*
eail_video_ref_state_set(AtkObject *object)
{
   AtkStateSet *state_set;
   Evas_Object *widget;

   g_return_val_if_fail(EAIL_IS_VIDEO(object), NULL);
   widget = eail_widget_get_widget(EAIL_WIDGET(object));
   if (!widget) return NULL;

   state_set = ATK_OBJECT_CLASS(eail_video_parent_class)->ref_state_set(object);
   atk_state_set_add_state(state_set, ATK_STATE_ANIMATED);

   return state_set;
}

/**
 * @brief Gets the accessible name of the accessible
 *
 * Implementation of get_name from AtkObject.
 *
 * @param object AtkObject instance
 *
 * @returns string representing the title of played video file
 * or NULL if name is not set
 */
static const gchar*
eail_video_name_get(AtkObject *object)
{
   Evas_Object *video;
   const gchar *atk_name;

   g_return_val_if_fail(EAIL_IS_VIDEO(object), NULL);

   atk_name = ATK_OBJECT_CLASS(eail_video_parent_class)->get_name(object);
   if (atk_name) return atk_name;

   video = eail_widget_get_widget(EAIL_WIDGET(object));
   if (!video) return NULL;

   return elm_video_title_get(video);
}

/**
 * @brief Initializes EailVideo object
 *
 * @param object AtkObject instance
 * @param data passed user data
 */
static void
eail_video_initialize(AtkObject *object, gpointer data)
{
   ATK_OBJECT_CLASS(eail_video_parent_class)->initialize(object, data);

   object->role = ATK_ROLE_ANIMATION;
}

/**
 * @brief Initiates EailVideo object
 *
 * @param video EailVideo instance
 */
static void
eail_video_init(EailVideo *video)
{
   video->forward_desc = NULL;
   video->next_desc = NULL;
   video->pause_desc = NULL;
   video->play_desc = NULL;
   video->prev_desc = NULL;
   video->rewind_desc = NULL;
   video->stop_desc = NULL;
   video->uri = NULL;
}

/**
 * @brief Finalizes EailVideo object
 *
 * @param object EailVideo instance
 */
static void
eail_video_finalize(GObject *object)
{
   EailVideo *video = EAIL_VIDEO(object);

   if (video->forward_desc) free(video->forward_desc);
   if (video->next_desc) free(video->next_desc);
   if (video->pause_desc) free(video->pause_desc);
   if (video->play_desc) free(video->play_desc);
   if (video->rewind_desc) free(video->rewind_desc);
   if (video->stop_desc) free(video->stop_desc);
   if (video->uri) free(video->uri);

   G_OBJECT_CLASS(eail_video_parent_class)->finalize(object);
}

/**
 * @brief Initializes EailVideo class
 *
 * @param klass EailVideoClass instance
 */
static void
eail_video_class_init(EailVideoClass *klass)
{
   AtkObjectClass *class = ATK_OBJECT_CLASS(klass);
   GObjectClass *g_object_class = G_OBJECT_CLASS(klass);

   class->initialize = eail_video_initialize;
   class->get_name = eail_video_name_get;
   class->ref_state_set = eail_video_ref_state_set;

   g_object_class->finalize = eail_video_finalize;
}

/**
 * @brief Gets the name of the specified action of the object
 *
 * Implementation of get_action_name from AtkAction interface.
 *
 * @param action AtkAction instance
 * @param i action index
 *
 * @returns string representing the name of the specified action
 */
static const gchar*
eail_video_action_name_get(AtkAction *action, gint i)
{
   const gchar *action_name;
   g_return_val_if_fail(EAIL_IS_VIDEO(action), NULL);

   switch (i)
     {
       case 0:
           /*"forward,clicked" - the user clicked the forward button.*/
           action_name = "forward";
           break;
       case 1:
           /*"next,clicked" - the user clicked the next button.*/
           action_name = "next";
           break;
       case 2:
           /*"pause,clicked" - the user clicked the pause button.*/
           action_name = "pause";
           break;
       case 3:
           /*"play,clicked" - the user clicked the play button.*/
           action_name = "play";
           break;
       case 4:
           /*"prev,clicked" - the user clicked the prev button*/
           action_name = "prev";
           break;
       case 5:
           /*"rewind,clicked" - the user clicked the rewind button.*/
           action_name = "rewind";
           break;
       case 6:
           /*"stop,clicked" - the user clicked the stop button.*/
           action_name = "stop";
           break;
       default:
           action_name = NULL;
           break;
     }

   return action_name;
}

/**
 * @brief Gets the number of accessible actions available on the object
 *
 * If there are more than one, the first one is considered
 * the "default" action of the object.
 *
 * Implementation of get_n_actions from AtkAction interface.
 *
 * @param action AtkAction instance
 *
 * @returns integer representing the number of actions
 * supported by EailVideo object
 */
static gint
eail_video_n_actions_get(AtkAction *action)
{
   return 7;
}

/**
 * @brief Performs the specified action on the object
 *
 * Implementation of do_action from AtkAction interface.
 *
 * @param action AtkAction instance
 * @param i action index
 *
 * @returns TRUE on success, FALSE otherwise
 */
static gboolean
eail_video_do_action(AtkAction *action, gint i)
{
   Evas_Object *video;
   gboolean result, playing, seekable;
   double position, length;

   g_return_val_if_fail(EAIL_IS_VIDEO(action), FALSE);

   video = eail_widget_get_widget(EAIL_WIDGET(action));
   if (!video) return FALSE;
   if ((elm_object_disabled_get(video)) || (!evas_object_visible_get(video)))
     return FALSE;

   playing = elm_video_is_playing_get(video);
   seekable = elm_video_is_seekable_get(video);
   length = elm_video_play_length_get(video);
   position = elm_video_play_position_get(video);

   switch (i)
     {
       case 0:
           if (!seekable)
             {
                result = FALSE;
                break;
             }
           position += length * 0.3;
           elm_video_play_position_set(video, position);
           eail_emit_atk_signal
               (ATK_OBJECT(action), "visible-data-changed", ATK_TYPE_OBJECT);
           result = TRUE;
           break;
       case 1:
           if (!seekable)
             {
                result = FALSE;
                break;
             }
           position += length * 0.1;
           elm_video_play_position_set(video, position);
           eail_emit_atk_signal
               (ATK_OBJECT(action), "visible-data-changed", ATK_TYPE_OBJECT);
           result = TRUE;
           break;
       case 2:
           if (!playing)
             {
                result = FALSE;
                break;
             }
           elm_video_pause(video);
           result = TRUE;
           break;
       case 3:
           if (playing)
             {
                result = FALSE;
                break;
             }
           elm_video_play(video);
           eail_emit_atk_signal
               (ATK_OBJECT(action), "visible-data-changed", ATK_TYPE_OBJECT);
           result = TRUE;
           break;
       case 4:
           if (!seekable)
             {
                result = FALSE;
                break;
             }
           position -= length * 0.1;
           elm_video_play_position_set(video, position);
           eail_emit_atk_signal
               (ATK_OBJECT(action), "visible-data-changed", ATK_TYPE_OBJECT);
           result = TRUE;
           break;
       case 5:
           if (!seekable)
             {
                result = FALSE;
                break;
             }
           elm_video_play_position_set(video, 0);
           eail_emit_atk_signal
               (ATK_OBJECT(action), "visible-data-changed", ATK_TYPE_OBJECT);
           result = TRUE;
           break;
       case 6:
           elm_video_stop(video);
           elm_video_play_position_set(video, 0);
           eail_emit_atk_signal
               (ATK_OBJECT(action), "visible-data-changed", ATK_TYPE_OBJECT);
           result = TRUE;
           break;
       default:
           result = FALSE;
           break;

     }

   return result;
}

/**
 * @brief Gets the description of the specified action of the object
 *
 * Implementation of get_description from AtkAction interface.
 *
 * @param action AtkAction instance
 * @param i action index
 *
 * @returns string representing the description of the specified action
 */
static const gchar*
eail_video_description_get(AtkAction *action, gint i)
{
   const gchar *action_description;
   EailVideo *video;

   g_return_val_if_fail(EAIL_IS_VIDEO(action), NULL);
   video = EAIL_VIDEO(action);

   switch (i)
     {
       case 0:
           action_description = video->forward_desc;
           break;
       case 1:
           action_description = video->next_desc;
           break;
       case 2:
           action_description = video->pause_desc;
           break;
       case 3:
           action_description = video->play_desc;
           break;
       case 4:
           action_description = video->prev_desc;
           break;
       case 5:
           action_description = video->rewind_desc;
           break;
       case 6:
           action_description = video->stop_desc;
           break;
       default:
           action_description = NULL;
           break;
     }

   return action_description;
}

/**
 * @brief Sets a description of the specified action of the object
 *
 * Implementation of set_description from AtkAction interface.
 *
 * @param action AtkAction instance
 * @param i action index
 * @param description action descritpion
 *
 * @returns TRUE on success, FALSE otherwise
 */
static gboolean
eail_video_description_set(AtkAction *action, gint i, const char *description)
{
   EailVideo *video;
   gchar **value;

   g_return_val_if_fail(EAIL_IS_VIDEO(action), FALSE);
   video = EAIL_VIDEO(action);
   switch (i)
     {
       case 0:
           value = &video->forward_desc;
           break;
       case 1:
           value = &video->next_desc;
           break;
       case 2:
           value = &video->pause_desc;
           break;
       case 3:
           value = &video->play_desc;
           break;
       case 4:
           value = &video->prev_desc;
           break;
       case 5:
           value = &video->rewind_desc;
           break;
       case 6:
           value = &video->stop_desc;
           break;
       default:
           value = NULL;
           break;
     }
   if (value)
     {
        free(*value);
        *value = g_strdup(description);
        return TRUE;
     }
   return FALSE;
}

/**
 * @brief AtkAction interface initializer
 *
 * Function called upon instance creation. It initializes AtkAction
 * interface implementation i.e hooks method pointers in the interface structure
 * to the implementing class's implementation.
 *
 * @param iface AtkActionIface instance
 */
static void
atk_action_interface_init(AtkActionIface *iface)
{
   g_return_if_fail(iface != NULL);

   iface->do_action = eail_video_do_action;
   iface->get_n_actions = eail_video_n_actions_get;
   iface->get_name = eail_video_action_name_get;
   iface->get_description = eail_video_description_get;
   iface->set_description = eail_video_description_set;
}

/**
 * @brief Helper function for matching file extension with a mime type
 *
 * @param ext file extension
 *
 * @returns string representing the mime type
 */
static const char *
_match_mime_type(const char *ext)
{
   if (!strcmp(ext, ".264"))
     return "video/h261";
   if (!strcmp(ext, ".3g2"))
     return "video/3gpp2";
   if (!strcmp(ext, ".3gp"))
     return "video/3gpp";
   if (!strcmp(ext, ".asf"))
     return "video/x-ms-asf";
   if (!strcmp(ext, ".avi"))
     return "video/x-msvideo";
   if (!strcmp(ext, ".clp"))
     return "application/x-msclip";
   if (!strcmp(ext, "flv"))
     return "video/x-flv";
   if (!strcmp(ext, ".m4v"))
     return "video/x-m4v";
   if (!strcmp(ext, ".mkv"))
     return "video/x-matroska";
   if (!strcmp(ext, ".mov"))
     return "video/quicktime";
   if (!strcmp(ext, ".mp2"))
     return "audio/mpeg";
   if (!strcmp(ext, ".mp4"))
     return "video/mp4";
   if (!strcmp(ext, ".mpe"))
     return "video/mpeg";
   if (!strcmp(ext, ".mpeg"))
     return "video/mpeg";
   if (!strcmp(ext, ".mpg"))
     return "video/mpeg";
   if (!strcmp(ext, ".mts"))
     return "model/vnd.mts";
   if (!strcmp(ext, ".mxf"))
     return "application/mxf";
   if (!strcmp(ext, ".ogg"))
     return "audio/ogg";
   if (!strcmp(ext, ".ogv"))
     return "video/ogg";
   if (!strcmp(ext, ".rm"))
     return "audio/x-pn-realaudio";
   if (!strcmp(ext, ".swf"))
     return "application/x-shockwave-flash";
   if (!strcmp(ext, ".ts"))
     return "video/MP2T";
   if (!strcmp(ext, ".weba"))
     return "audio/webm";
   if (!strcmp(ext, ".webm"))
     return "video/webm";
   if (!strcmp(ext, ".wmv"))
     return "video/x-ms-wmv";

   return NULL;
}

/**
 * @brief Helper function for getting extension from a filename
 *
 * @param filename target filename
 *
 * @returns string representing the file extension
 */
static const char *
_get_file_ext(const char *filename) {
     const char *ext = strrchr(filename, '.');

     if(!ext) return "";
     return ext;
}

/**
 * @brief Helper function for getting the video's file path
 *
 * @param widget Evas_Object instance
 *
 * @returns string representing the path to video file
 */
static const char *
_get_video_path(const Evas_Object *widget)
{
   Evas_Object *emotion;

   emotion = elm_video_emotion_get(widget);

   return emotion_object_file_get(emotion);
}

/**
 * @brief Gets the character string of the specified mime type
 *
 * The first mime type is at position 0, the second at position 1, and so on.
 *
 * Implementation of get_mime_type from AtkStreamableContent interface.
 *
 * @param streamable AtkStreamableContent instance
 * @param i index of supported mime type
 *
 * @returns string representing the specified mime type
 */
static const gchar*
eail_video_mime_type_get(AtkStreamableContent *streamable, gint i)
{
   Evas_Object *widget;
   const char *path, *ext;

   g_return_val_if_fail(EAIL_IS_VIDEO(streamable), NULL);
   if (i != 0) return NULL;

   widget = eail_widget_get_widget(EAIL_WIDGET(streamable));
   path = _get_video_path(widget);
   ext = _get_file_ext(path);

   return _match_mime_type(ext);
}

/**
 * @brief Gets the number of mime types supported by this object
 *
 * For video widget implementation by ATK-EAIL, there will always be maximum one
 * supported content type at a time for the given file,
 * so this function will always return 1 or 0.
 *
 * Implementation of get_n_mime_types from AtkStreamableContent interface.
 *
 * @param streamable AtkStreamableContent instance
 *
 * @returns integer representing the number of mime-types for the given
 * streamable content ('0' in case when no mime-type was matched for content file)
 */
static gint
eail_video_n_mime_types_get(AtkStreamableContent *streamable)
{
   const gchar *mime_type = NULL;

   mime_type = eail_video_mime_type_get(streamable, 0);
   if (mime_type) return 1;

   /* no mime type for content*/
   return 0;
}

/**
 * @brief Gets a string representing an URI
 *
 * Gets a string representing a URI in IETF standard format
 * (see http://www.ietf.org/rfc/rfc2396.txt) from which the object's
 * content may be streamed in the specified mime-type, if one is available.
 * If mime_type is NULL, the URI for the default (and possibly only)
 * mime-type is returned.
 *
 * Implementation of get_uri from AtkStreamableContent.
 *
 * @param streamable AtkStreamableContent instance
 * @param mime_type requested mime type
 *
 * @returns string representing the video file's URI
 */
static const char *
eail_video_get_uri(AtkStreamableContent *streamable, const gchar *mime_type)
{
   Evas_Object *widget;
   EailVideo *video;
   const char *mime, *path;
   GFile *file;

   g_return_val_if_fail(EAIL_IS_VIDEO(streamable), NULL);

   widget = eail_widget_get_widget(EAIL_WIDGET(streamable));
   video = EAIL_VIDEO(streamable);
   mime =  eail_video_mime_type_get(streamable, 0);
   if ((mime) && (mime_type) && (strcmp(mime, mime_type))) return NULL;

   path = _get_video_path(widget);
   file = g_file_new_for_path(path);
   if (video->uri) free(video->uri);
   video->uri = g_file_get_uri(file);
   g_object_unref(file);

   return video->uri;
}

/**
 * @brief Gets the content in the specified mime type
 *
 * Implementation of get_stream from AtkStreamableContent.
 *
 * @param streamable AtkStreamableContent instance
 * @param mime_type requested mime type
 *
 * @returns GIOChannel to video file
 */
static GIOChannel *
eail_video_get_stream(AtkStreamableContent *streamable,
                      const gchar *mime_type)
{
   Evas_Object *widget;
   GError *error;
   GIOChannel *channel;
   const char *path, *mime;

   g_return_val_if_fail(EAIL_IS_VIDEO(streamable), NULL);

   widget = eail_widget_get_widget(EAIL_WIDGET(streamable));
   mime = eail_video_mime_type_get(streamable, 0);
   if ((mime) && (strcmp(mime, mime_type))) return NULL;
   path = _get_video_path(widget);

   error = NULL;
   channel = g_io_channel_new_file(path, "r", &error);
   if (error)
     {
        WRN("cannot open GIOChannel %s", error->message);
        g_error_free(error);
     }
   return channel;
}

/**
 * @brief Initializes AtkStreamableContent interface
 *
 * @param iface EailVideo instance
 */
static void
atk_stremable_content_iface_init(AtkStreamableContentIface *iface)
{
   g_return_if_fail(iface != NULL);

   iface->get_n_mime_types = eail_video_n_mime_types_get;
   iface->get_mime_type = eail_video_mime_type_get;
   iface->get_stream = eail_video_get_stream;
   iface->get_uri = eail_video_get_uri;
}

