use SVN::Core;
use SVN::Ra;

sub print_log {
  my $url = shift;

  # open the RA library
  my $ra = SVN::Ra->new (url => $url);

  # call get_log over revisions from 0 to HEAD, passing it a callback that
  # simply prints out the revision, date, author, and message
  $ra->get_log ([''], 0, "HEAD", 1, 0, sub {
    my ($paths, $rev, $author, $date, $msg, $pool) = @_;

    print <<END;
$rev : $date : $author

$msg
END
  });
}
