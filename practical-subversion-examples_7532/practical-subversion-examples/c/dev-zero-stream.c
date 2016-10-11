#include <svn_types.h>
#include <svn_error.h>
#include <svn_io.h>

static svn_error_t *
dev_zero_read (void *baton,
               char *buffer,
               apr_size_t *len)
{
  int i;

  for (i = 0; i < *len; ++i)
    {
      buffer[i] = 0;
    }

  return SVN_NO_ERROR;
}

svn_stream_t *
make_dev_zero_stream (apr_pool_t *pool)
{
  /* note that we don't need to store any specific data for use by our read
     callback, so our baton is NULL */
  svn_stream_t *stream = svn_stream_create (NULL, pool);

  svn_stream_set_read (stream, dev_zero_read);

  return stream;
}
