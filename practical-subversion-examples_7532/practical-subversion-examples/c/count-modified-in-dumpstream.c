#include <svn_repos.h>

svn_error_t *empty_uuid_record_function (const char *uuid,
                                         void *parse_baton,
                                         apr_pool_t *pool);
svn_error_t *empty_set_revision_property_function (void *revision_baton,
                                                   const char *name,
                                                   const svn_string_t *value);
svn_error_t *empty_set_node_property_function (void *node_baton,
                                               const char *name,
                                               const svn_string_t *value);
svn_error_t *empty_remove_node_props (void *node_baton);
svn_error_t *empty_close_node_function (void *node_baton);
svn_error_t *empty_close_revision_function (void *node_baton);
svn_error_t *empty_delete_node_property (void *node_baton, const char *name);

/* A baton to hold the path we're looking for and the current count. */
struct count_baton {
  const char *path;
  int count;
};

/* callback to be called at the start of each revision */
svn_error_t *
new_revision_record (void **revision_baton,
                     apr_hash_t *headers,
                     void *parse_baton,
                     apr_pool_t *pool)
{
  /* the only state we need to keep is in the parse_baton, so we'll treat the
     revision baton and the parse baton as the same thing. */
  *revision_baton = parse_baton;

  return SVN_NO_ERROR;
}

/* callback to be called for each new node */
svn_error_t *
new_node_record (void **node_baton,
                 apr_hash_t *headers,
                 void *revision_baton,
                 apr_pool_t *pool)
{
  struct count_baton *cb = revision_baton;

  /* grab the filename out of the headers */
  const char *filename = apr_hash_get (headers, SVN_REPOS_DUMPFILE_NODE_PATH,
                                       APR_HASH_KEY_STRING);

  /* if it matches the path we're looking for, increment our count */
  if (filename && strcmp (filename, cb->path) == 0)
    {
      cb->count++;
    }

  return SVN_NO_ERROR;
}

/* callback to be called when we get to the fulltext of each file */
svn_error_t *
set_fulltext (svn_stream_t **stream,
              void *node_baton)
{
  /* we don't care about the content of the file, so we return a NULL stream */
  *stream = NULL;

  return SVN_NO_ERROR;
}

/* count the number of times a particular file is modified in a dumpfile */
svn_error_t *
count_times_modified (const char *path,
                      svn_stream_t *dumpfile,
                      apr_pool_t *pool)
{
  struct count_baton cb;
  struct svn_repos_parse_fns_t parser_fns;

  /* initialize our count baton */
  cb.path = path;
  cb.count = 0;

  /* fill in the parser function callbacks.
   *
   * Note:  The empty functions used for uninteresting callbacks have not been 
   * included to save space.  Just assume all they do is return SVN_NO_ERROR.
   */
  parser_fns.new_revision_record = new_revision_record;
  parser_fns.uuid_record = empty_uuid_record_function;
  parser_fns.new_node_record = new_node_record;
  parser_fns.set_revision_property = empty_set_revision_property_function;
  parser_fns.set_node_property = empty_set_node_property_function;
  parser_fns.delete_node_property = empty_delete_node_property;
  parser_fns.remove_node_props = empty_remove_node_props;
  parser_fns.set_fulltext = set_fulltext;
  parser_fns.apply_textdelta = NULL;
  parser_fns.close_node = empty_close_node_function;
  parser_fns.close_revision = empty_close_revision_function;

  /* parse the dumpstream using our callback functions and count baton */
  SVN_ERR (svn_repos_parse_dumpstream (dumpfile, &parser_fns, &cb,
                                       NULL, NULL, pool));

  printf ("%s changed %d times in the dumpfile\n", cb.path, cb.count);

  return SVN_NO_ERROR;
}
