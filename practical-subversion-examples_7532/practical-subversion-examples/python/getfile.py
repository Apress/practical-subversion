import sys

from svn import core
from svn import fs

def main_func(pool, path, filename, rev=None):
  fsobj = fs.open(path, None, pool)

  if rev is None:
    rev = fs.youngest_rev(fsobj, pool)

  root = fs.revision_root(fsobj, rev, pool)
  filestream = fsobj.file_contents(root, filename, pool)
  while 1:
    data = core.svn_stream_read(file, 1024)
    if not data:
      break
    sys.stdout.write(data)

  fs.close(fsobj)

if __name__ == '__main__':
  main_func(core.svn_pool_create(), sys.argv[1], sys.argv[2])
