#include <svn_client.h>

void
status_callback (void *baton, const char *path, svn_wc_status2_t * status)
{
  char value;

  /* a real status callback would handle many more cases here */
  switch (status->text_status)
    {
    case svn_wc_status_added:
      value = 'A';
      break;

    case svn_wc_status_deleted:
      value = 'D';
      break;

    case svn_wc_status_replaced:
      value = 'R';
      break;

    case svn_wc_status_modified:
      value = 'M';
      break;

    case svn_wc_status_merged:
      value = 'G';
      break;

    case svn_wc_status_conflicted:
      value = 'C';
      break;

    default:
      value = '?';
    }

  printf ("%c %s\n", value, path);
}
