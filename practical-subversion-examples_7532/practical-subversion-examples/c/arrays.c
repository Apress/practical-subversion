#include <apr.h>
#include <apr_tables.h>

void
demonstrate_apr_arrays (apr_pool_t *pool)
{
  int i;

  /* make an array with 2 slots in it */
  apr_array_header_t *array = apr_array_make (pool, 2, sizeof (char *));

  /* push two strings onto the array */
  (*(char **) apr_array_push (array)) = "foo";
  (*(char **) apr_array_push (array)) = "bar";

  /* push another one on, this causes the extra spot to be allocated */
  (*(char **) apr_array_push (array)) = "baz";

  /* iterate over them */
  for (i = 0; i < array->nelts; ++i)
    {
      printf ("%s\n", ((char **) array->elts)[i]);
    }

  {
    /* try to pop off the first item */
    char *item = apr_array_pop (array);
    if (item)
      {
        /* use it */
      }
  }
}
