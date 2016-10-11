#include <svn_utf.h>

svn_error_t *some_svn_function (const char **res,
                                const char *path,
                                apr_pool_t *p);

svn_error_t *
call_some_subversion_function(const char *native_path, apr_pool_t *pool)
{
  const char *utf8_path, *result_of_operation, *native_result;

  SVN_ERR (svn_utf_cstring_to_utf8 (&utf8_path, native_path, pool));

  SVN_ERR (some_svn_function (&result_of_operation, utf8_path, pool));

  SVN_ERR (svn_utf_cstring_from_utf8 (&native_result,
                                      result_of_operation, pool));

  printf ("the result is %s\n", native_result);

  return SVN_NO_ERROR;
}
