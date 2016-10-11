import os
import sys

import svn.core
import svn.wc

def main_func(pool, files):
  for f in files:
    # the paths passed in to subversion must be absolute
    dirpath = fullpath = os.path.abspath(f)

    if not os.path.isdir(dirpath):
      dirpath = os.path.dirname(dirpath)
    adm_baton = svn.wc.svn_wc_adm_open(None, dirpath, 1, 1, pool)

    try:
      entry = svn.wc.svn_wc_entry(fullpath, adm_baton, 0, pool)
      print entry.url
    finally:
      svn.wc.svn_wc_adm_close(adm_baton)

if __name__ == '__main__':
  main_func(svn.core.svn_pool_create(), sys.argv[1:])
