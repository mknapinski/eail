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
 * @file eail.c
 * @brief EAIL initialization
 */

#include <Elementary.h>

#include <gmodule.h>
#include <atk-bridge.h>
#include "eail.h"
#include "eail_app.h"
#include "eail_priv.h"
#include "eail_clipboard.h"

/** @brief Struct definition for listener info*/
typedef struct _EailUtilListenerInfo EailUtilListenerInfo;
/** @brief Struct definition for event info*/
typedef struct _EailKeyEventInfo EailKeyEventInfo;

/** @brief Struct definition for listener info*/
struct _EailUtilListenerInfo
{
   gint key; /**< @brief key of entry */
   guint signal_id;/**< @brief id of the signal */
   gulong hook_id;/**< @brief emit hook value returned on signal registration*/
};

/** @brief Struct definition for event info*/
struct _EailKeyEventInfo
{
  AtkKeyEventStruct *key_event;/**< @brief key of event */
  gpointer func_data;/**< @brief additional data passed to the event */
};

/** @brief Cache of objects used for listening and propagating signals */
static GHashTable *listener_list = NULL;
/** @brief Last added id of a listener */
static gint listener_idx = 1;

/**
 * @brief Domain index that will be used as the DOMAIN parameter on EINA log macros
 *
 * A negative value means a log occurred.
 */
int _eail_log_dom = -1;

/**
 * @brief A variable for tracking the last focused AtkObject
 */
static AtkObject *eail_atk_last_focused_obj = NULL;

/**
 * @brief Gets the name of the toolkit
 * @return string representing the name of the toolkit
 */
static const gchar * eail_get_toolkit_name(void)
{
   return "elementary";
}

/**
 * @brief Gets the version of the toolkit
 *
 * @return string representing the version of the toolkit
 */
static const gchar * eail_get_toolkit_version(void)
{
   return g_strdup_printf("%i.%i.%i", elm_version->major,
                          elm_version->minor,
                          elm_version->micro);
}

/**
 * @brief Gets the accessible root container for the current application
 *
 * @return AtkObject representing the accessible root container
 */
static AtkObject * eail_get_root(void)
{
   static AtkObject *root = NULL;

   if (!root)
     {
        root = g_object_new(EAIL_TYPE_APP, NULL);
        atk_object_initialize(root, NULL);
   }

   return root;
}

/**
 * @brief Callback to be called when an object receives focus
 *
 * @param current_focused_obj AtkObject instance
 */
static void
eail_focus_listener_cb(AtkObject *current_focused_obj)
{
   if (current_focused_obj == eail_atk_last_focused_obj)
     return;

   if (eail_atk_last_focused_obj)
     {
        atk_object_notify_state_change
                    (eail_atk_last_focused_obj, ATK_STATE_FOCUSED, FALSE);
        g_object_unref(eail_atk_last_focused_obj);
     }

   g_object_ref(current_focused_obj);

   /* already notifying about focus in widget implementation so do not need
    * to notify here for new focus*/
   eail_atk_last_focused_obj = current_focused_obj;
}

/**
 * @brief Initializes object focus tracking
 */
static void
eail_app_focus_listener_init()
{
   atk_add_focus_tracker(eail_focus_listener_cb);
}

/**
 * @brief Creates and adds a listener for the given object type
 *
 * @param listener GObject Emission Hook (a simple function pointer to get
 * invoked when the signal is emitted)
 * @param object_type name string representing object's type
 * @param signal name string representing the signal to listen
 * @param hook_data GObject 'hook' info
 *
 * @returns integer representing the id of the newly added listener
 */
static guint
add_listener (GSignalEmissionHook listener,
              const gchar         *object_type,
              const gchar         *signal,
              const gchar         *hook_data)
{
  GType type;
  guint signal_id;
  gint  rc = 0;

  type = g_type_from_name (object_type);
  if (type)
    {
      signal_id  = g_signal_lookup (signal, type);
      if (signal_id > 0)
        {
          EailUtilListenerInfo *listener_info;

          rc = listener_idx;

          listener_info = g_malloc(sizeof(EailUtilListenerInfo));
          listener_info->key = listener_idx;
          listener_info->hook_id =
                          g_signal_add_emission_hook (signal_id, 0, listener,
                                                      g_strdup (hook_data),
                                                      (GDestroyNotify) g_free);
          listener_info->signal_id = signal_id;

          g_hash_table_insert(listener_list, &(listener_info->key), listener_info);
          listener_idx++;
        }
      else
        {
           DBG("Invalid signal type %s\n", signal);
        }
    }
  else
    {
       DBG("Invalid object type %s\n", object_type);
    }
  return rc;
}

/**
 * @brief Removes a listener for given object type
 *
 * @param remove_listener id of the listener to remove
 */
static void
eail_remove_global_event_listener (guint remove_listener)
{
  if (remove_listener > 0)
  {
    EailUtilListenerInfo *listener_info;
    gint tmp_idx = remove_listener;

    listener_info = (EailUtilListenerInfo *)
                                 g_hash_table_lookup(listener_list, &tmp_idx);

    if (listener_info != NULL)
      {
        /* Hook id of 0 and signal id of 0 are invalid */
        if (listener_info->hook_id != 0 && listener_info->signal_id != 0)
          {
            /* Remove the emission hook */
            g_signal_remove_emission_hook(listener_info->signal_id,
                                                listener_info->hook_id);

            /* Remove the element from the hash */
            g_hash_table_remove(listener_list, &tmp_idx);
          }
        else
          {
             DBG("Invalid listener hook_id %ld or signal_id %d\n",
                            listener_info->hook_id, listener_info->signal_id);
          }
      }
    else
      {
         DBG("No listener with the specified listener id %d", remove_listener);
      }
  }
  else
  {
     DBG("Invalid listener_id %d", remove_listener);
  }
}

/**
 * @brief Initialization for global event listener
 *
 * @param listener GSignalEmissionHook (GObject 'signal invocation hint')
 * @param event_type string representing the event's type
 *
 * @return integer representing the id of the added listener
 */
static guint
eail_add_global_event_listener(GSignalEmissionHook listener,
                               const gchar *event_type)
{
   guint rc = 0;
   gchar **split_string;

   split_string = g_strsplit(event_type, ":", 3);

   if (split_string)
     {
       if (!strcmp("window", split_string[0]))
         rc = add_listener /* window event handling */
                       (listener, "AtkWindow", split_string[1], event_type);
       else
         rc = add_listener /* regular event handling */
                    (listener, split_string[1], split_string[2], event_type);

       g_strfreev(split_string);
     }

   return rc;
}

/**
 * @brief Destructor for listener info object
 *
 * @param data data to be freed
 */
static void
eail_listener_info_destroy(gpointer data)
{
   g_free(data);
}

/**
 * @brief AtkUtil class initialization
 */
static void atk_util_install(void)
{
   AtkUtilClass *uclass;

   uclass = ATK_UTIL_CLASS(g_type_class_ref(ATK_TYPE_UTIL));
   uclass->get_toolkit_name = eail_get_toolkit_name;
   uclass->get_toolkit_version = eail_get_toolkit_version;
   uclass->get_root = eail_get_root;

   eail_app_focus_listener_init();

   uclass->add_global_event_listener = eail_add_global_event_listener;
   uclass->remove_global_event_listener = eail_remove_global_event_listener;
   uclass->add_key_event_listener = NULL;
   uclass->remove_key_event_listener = NULL;
}

/**
 * @brief Function to be executed by Elementary when EAIL module is loaded
 *
 * @param m Elm_Module data
 * @return 1 on success, 0 otherwise
 */
int
elm_modapi_init(void *m)
{
   static gboolean initialized = FALSE;
   if (initialized) return 1;

   _eail_log_dom = eina_log_domain_register("eail", EAIL_LOG_COLOR);
   if (!_eail_log_dom)
     {
        EINA_LOG_ERR("could not register eail log domain.");
        _eail_log_dom = EINA_LOG_DOMAIN_GLOBAL;
     }

   if (!ecore_main_loop_glib_integrate())
     {
        ERR("Cannot integrate with glib main loop");
        return 0;
     }

   initialized = TRUE;

#if !GLIB_CHECK_VERSION(2,35,0)
   g_type_init();
#endif

   listener_list = g_hash_table_new_full
                  (g_int_hash, g_int_equal, NULL, eail_listener_info_destroy);

   atk_util_install();

   atk_misc_instance = g_object_new(ATK_TYPE_MISC, NULL);

   atk_bridge_adaptor_init(NULL, NULL);

   return 1;
}

/**
 * @brief Function to be executed by Elementary when EAIL module is unloaded
 *
 * @param m Elm_Module data
 * @return always 1 - notifying success
 */
int
elm_modapi_shutdown(void *m)
{
   eail_clipboard_free();

   if ((_eail_log_dom > -1) && (_eail_log_dom != EINA_LOG_DOMAIN_GLOBAL))
     {
        eina_log_domain_unregister(_eail_log_dom);
        _eail_log_dom = -1;
     }
   /*always succeed*/
   return 1;
}

