#include <svn_client.h>
#include <svn_config.h>

svn_error_t *
run_diff_with_external_program (const char *diff_cmd,
                                apr_array_header_t *diff_options,
                                const char *source_url,
                                const char *dest_url,
                                apr_file_t *outfile,
                                apr_file_t *errfile,
                                svn_client_ctx_t *ctx,
                                apr_pool_t *pool)
{
  svn_opt_revision_t revision;

  revision.kind = svn_opt_revision_head;

  /* this assumes that diff_cmd is something like "/path/to/my/diff_program" */
  svn_config_set (apr_hash_get (ctx->config,
                                SVN_CONFIG_CATEGORY_CONFIG,
                                APR_HASH_KEY_STRING),
                  SVN_CONFIG_SECTION_HELPERS,
                  SVN_CONFIG_OPTION_DIFF_CMD, diff_cmd);

  /* diff_options is an array of const char * options to pass to the diff
     command */
  SVN_ERR (svn_client_diff (diff_options,
                            source_url,
                            &revision, /* HEAD */
                            dest_url,
                            &revision, /* HEAD */
                            TRUE,
                            TRUE,
                            FALSE,
                            outfile,
                            errfile,
                            ctx,
                            pool));

  return SVN_NO_ERROR;
}
