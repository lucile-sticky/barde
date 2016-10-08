# barde

[CppCms](http://cppcms.com) web-based application.
Depending on your configuration file (default is config.js) once compiled you can execute as a standalone server or fast-CGI to execute using web-server (may be Nginx, Apache ...).
Statics files (in www directory) must be served on a real web-server.

## Install
### Dependencies
#### Servers

mysql

#### Libraries

* libbooster.so.0
* libcppcms.so.1
* libcppdb.so.0

* linux-vdso.so.1
* libm.so.6
* libpthread.so.0
* libpcre.so.3
* libdl.so.2
* libgcrypt.so.11
* libz.so.1

The 3 first libraries can be found there:
https://sourceforge.net/projects/cppcms/files/

### Build

```
$ mkdir build
$ cd build
$ cmake ..
$ make
```

### Execute

```
$ ./playlists --c ../config.js

Then open localhost:8080/playlists in your browser.
```

