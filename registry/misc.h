/*
 *   See file ../COPYRIGHT for copying and redistribution conditions.
 *
 *   This header-file specifies the API for miscellaneous registry functions.
 *   The methods of this module are thread-compatible but not thread-safe.
 */

#ifndef LDM_MISC_H
#define LDM_MISC_H

#include "registry.h"

#ifdef __cplusplus
extern "C" {
#endif

/*
 * Allocates memory.
 *
 * Arguments:
 *      nbytes          The number of bytes to allocate
 *      status          Pointer to the status variable to be set.  Shall not be
 *                      NULL.  Upon successful return, "*status" will be 0;
 *                      otherwise it will be ENOMEM.
 * Returns:
 *      NULL            System error.  "log_start()" called.  "*status" is
 *                      ENOMEM.
 *      else            A pointer to the allocated memory.  "*status" is 0.
 */
void* reg_malloc(
    const size_t        nbytes,
    RegStatus* const    status);

/*
 * Clones the prefix of a string.  Logs a message if an error occurs.
 *
 * ARGUMENTS:
 *      clone           Pointer to a pointer to the clone.  Set upon successful
 *                      return.  Shall not be NULL.  The client should call
 *                      "free(*clone)" when the clone is no longer needed.
 *      string          Pointer to the string to clone.  Shall not be NULL.
 *      nbytes          The number of bytes of prefix to clone
 * RETURNS:
 *      0               Success.  "*clone" is not NULL.
 *      ENOMEM          System error.  "log_start()" called.
 */
RegStatus reg_clonePrefix(
    char** const        clone,
    const char* const   string,
    size_t              nbytes);

/*
 * Clones a string.  Logs a message if an error occurs.
 *
 * ARGUMENTS:
 *      clone           Pointer to a pointer to the clone.  Set upon successful
 *                      return.  Shall not be NULL.  The client should call
 *                      "free(*clone)" when the clone is no longer needed.
 *      src             The string to clone.  Shall not be NULL.
 * RETURNS:
 *      0               Success.  "*clone" is not NULL.
 *      ENOMEM          System error.  "log_start()" called.
 */
RegStatus reg_cloneString(
    char** const        clone,
    const char* const   src);

/*
 * Indicates if a path is absolute or not.
 *
 * Arguments:
 *      path            The path to be checked.  Shall not be NULL.
 * Returns:
 *      0               The path is not absolute
 *      else            The path is absolute
 */
int reg_isAbsPath(const char* const path);

/*
 * Indicates if a path is the absolute path name of the root node.
 *
 * Arguments:
 *      path            The path to be checked.  Shall not be NULL.
 * Returns:
 *      0               The path is not the absolute pathname of the root node
 *      else            The path is the absolute pathname of the root node
 */
int reg_isAbsRootPath(const char* const path);

/*
 * Ensures that a path name is absolute.
 *
 * Arguments:
 *      path            The path name to be vetted.  Shall not be NULL.
 * Returns:
 *      0               The path name is absolute
 *      EINVAL     The path name isn't absolute.  "log_start()" called.
 */
RegStatus reg_vetAbsPath(
    const char* const   path);

/*
 * Splits an absolute path name into relative path name and value-name
 * components.
 *
 * Arguments:
 *      path            Pointer to the absolute path name.  Shall not be
 *                      NULL.  Client may free upon return.
 *      absPath         Pointer to the absolute path name on which to base the
 *                      returned relative pathname.  Shall not be NULL.
 *      relPath         Pointer to a pointer to the path name of "path"
 *                      relative to "absPath" without the value-name component.
 *                      Shall not be NULL.  Set upon successful return.  The
 *                      client should call "free(*relPath)" when the path is no
 *                      longer needed.
 *      valueName       Pointer to a pointer to the name of the corresponding
 *                      value.  Shall not be NULL.  Set upon successful return.
 *                      The client should call "free(*valueName)" when the name
 *                      is no longer needed.
 * Returns:
 *      0               Success.  "*relPath" and "*valueName" are set.
 *      EINVAL          "path" isn't valid.  "log_start()" called.
 *      ENOMEM          System error.  "log_start()" called.
 */
RegStatus reg_splitAbsPath(
    const char* const   path,
    const char* const   absPath,
    char** const        relPath,
    char** const        valueName);

/*
 * Returns the parent pathname of a child pathname.  The child pathname may
 * be absolute or relative.
 *
 * Arguments:
 *      child           Pointer to the child pathname whose parent pathname is
 *                      to be returned.  Shall not be NULL.  May point to an
 *                      empty string, in which case ENOENT will be
 *                      returned.
 *      parent          Pointer to a pointer to the parent pathname.  Shall not
 *                      be NULL.  Set upon successful return.  The client
 *                      should call "free(*parent)" when the parent pathname is
 *                      no longer needed.
 * Returns:
 *      0               Success.  "*parent" is not NULL.
 *      ENOENT          The child pathname has no parent pathname
 *      ENOMEM          System error.  "log_start()" called.
 */
RegStatus reg_getParentPath(
    const char* const   child,
    char** const        parent);

#ifdef __cplusplus
}
#endif

#endif