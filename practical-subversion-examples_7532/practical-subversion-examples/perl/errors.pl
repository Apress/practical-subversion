sub i_want_a_new_error_handler {
  SVN::Error::handler = sub {
    my $err = shift;

    if (SVN::Error::is_error ($err) {
      my $mess = $err->strerror ();

      while ($err) {
        $mess .= " : " . $err->message ();
        $err = $err->child ();
      }

      die $mess;
    } else {
      return undef;
    }
  }
}
