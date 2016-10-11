#include <svn_ra.h>
#include <svn_config.h>

svn_error_t *open_tmp_file (apr_file_t ** fp, void *cb, apr_pool_t * p);

svn_error_t *
print_head_revnum (svn_ra_plugin_t * ra_plugin,
		   const char *url, apr_pool_t * pool)
{
  svn_ra_session_t *session;
  svn_ra_callbacks2_t *ra_callbacks;
  svn_revnum_t revnum;
  apr_hash_t *config;

  SVN_ERR (svn_ra_create_callbacks (&ra_callbacks, pool));
  ra_callbacks->open_tmp_file = open_tmp_file;

  SVN_ERR (svn_config_get_config (&config, NULL, pool));

  SVN_ERR (svn_ra_open2 (&session, url, ra_callbacks, NULL, config, pool));

  SVN_ERR (svn_ra_get_latest_revnum (session, &revnum, pool));

  printf ("HEAD is currently revision %" SVN_REVNUM_T_FMT "\n", revnum);

  return SVN_NO_ERROR;
}
