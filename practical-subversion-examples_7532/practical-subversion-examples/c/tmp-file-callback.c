#include <svn_io.h>
#include <svn_path.h>

svn_error_t *
open_tmp_file (apr_file_t **fp,
               void *callback_baton,
               apr_pool_t *pool)
{
  const char *tmpdir, *ignored_filename;

  SVN_ERR (svn_io_temp_dir (&tmpdir, pool));

  SVN_ERR (svn_io_open_unique_file (fp,
                                    &ignored_filename,
                                    svn_path_join (tmpdir, "tempfile", pool),
                                    ".tmp",
                                    TRUE, /* delete on close */
                                    pool));

  return SVN_NO_ERROR;
}
