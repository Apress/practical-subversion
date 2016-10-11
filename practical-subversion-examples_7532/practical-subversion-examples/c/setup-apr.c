#include <apr_general.h>
#include <stdlib.h>

int
setup_apr (void)
{
  apr_status_t err = apr_initialize ();
  if (err)
    {
      return EXIT_FAILURE;
    }

  if (atexit (apr_terminate) < 0)
    {
      return EXIT_FAILURE;
    }

  return 0;
}
