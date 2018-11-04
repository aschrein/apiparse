```console
$ cmake -G "Visual Studio 15 2017 Win64" ..
$ build apiparse
$ ./apiparse.exe include_d3d11.cpp -o d3d11api.py  --
$ python d3d11api.py > d3d11wrapper.cpp
$ build injector, injectee_iat
$ ./inject.exe "$PATH_TO_APP"
$ cat log

```