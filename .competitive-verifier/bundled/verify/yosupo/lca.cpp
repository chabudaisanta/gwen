Traceback (most recent call last):
  File "/home/chabudaisanta/.local/share/pipx/venvs/competitive-verifier/lib/python3.13/site-packages/competitive_verifier/oj/resolver.py", line 290, in resolve
    bundled_code = language.bundle(path, basedir=basedir)
  File "/home/chabudaisanta/.local/share/pipx/venvs/competitive-verifier/lib/python3.13/site-packages/competitive_verifier/oj/languages/cplusplus.py", line 243, in bundle
    bundler.update(path)
    ~~~~~~~~~~~~~~^^^^^^
  File "/home/chabudaisanta/.local/share/pipx/venvs/competitive-verifier/lib/python3.13/site-packages/competitive_verifier/oj/languages/cplusplus_bundle.py", line 322, in update
    assert len(lines) == len(uncommented_lines)
           ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
AssertionError
