use SVN::Core;
use SVN::Repos;

sub get_logs {
  my ($repos, $path) = @_;

  # open the repository
  my $r = SVN::Repos::open ($repos);

  # call get_logs, just like the RA layer example
  $r->get_logs($path, 0, "HEAD", 1, 0, sub {
    my ($paths, $rev, $author, $date, $msg, $pool) = @_;

    print <<END;
$rev : $date : $author

$msg
END
  });
}
