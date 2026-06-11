Traceback (most recent call last):
  File "/home/chabudaisanta/.local/share/pipx/venvs/competitive-verifier/lib/python3.13/site-packages/competitive_verifier/oj/resolver.py", line 290, in resolve
    bundled_code = language.bundle(path, basedir=basedir)
  File "/home/chabudaisanta/.local/share/pipx/venvs/competitive-verifier/lib/python3.13/site-packages/competitive_verifier/oj/languages/cplusplus.py", line 243, in bundle
    bundler.update(path)
    ~~~~~~~~~~~~~~^^^^^^
  File "/home/chabudaisanta/.local/share/pipx/venvs/competitive-verifier/lib/python3.13/site-packages/competitive_verifier/oj/languages/cplusplus_bundle.py", line 479, in update
    self._resolve(pathlib.Path(included), included_from=path)
    ~~~~~~~~~~~~~^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
  File "/home/chabudaisanta/.local/share/pipx/venvs/competitive-verifier/lib/python3.13/site-packages/competitive_verifier/oj/languages/cplusplus_bundle.py", line 286, in _resolve
    raise BundleErrorAt(path, -1, "no such header")
competitive_verifier.oj.languages.cplusplus_bundle.BundleErrorAt: gwen/hash/rolling_hash.hpp: line -1: no such header
