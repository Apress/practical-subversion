#include <apr.h>
#include <apr_file_io.h>

void
parse_file (const char *filename, apr_pool_t *pool)
{
  apr_file_t *file;
  apr_status_t err;

  err = apr_file_open (&file, filename, 0, APR_OS_DEFAULT, pool);
  if (err) {
    /* couldn't open the file, so handle the error somehow */
  } else {
    /* it succeeded, so we continue on parsing the contents of the file */
  }
}
