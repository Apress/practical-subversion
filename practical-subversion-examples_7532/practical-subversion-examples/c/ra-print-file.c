#include <svn_ra.h>
#include <svn_config.h>

svn_error_t *open_tmp_file (apr_file_t ** fp, void *cb, apr_pool_t * p);
svn_error_t *
cat_file (const char *repos_url, const char *file_path, apr_pool_t * pool)
{
  svn_ra_session_t *session;
  apr_hash_t *config;
  svn_stream_t *stream;
  svn_ra_callbacks2_t *ra_callbacks;

  SVN_ERR (svn_ra_create_callbacks (&ra_callbacks, pool));
  ra_callbacks->open_tmp_file = open_tmp_file;

  SVN_ERR (svn_config_get_config (&config, NULL, pool));

  SVN_ERR (svn_ra_open2 (&session, repos_url, ra_callbacks, NULL,
			 config, pool));

  SVN_ERR (svn_stream_for_stdout (&stream, pool));

  SVN_ERR (svn_ra_get_file (session, file_path, SVN_INVALID_REVNUM,
			    stream, NULL, NULL, pool));

  return SVN_NO_ERROR;
}
