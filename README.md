# libcr-test

This is the testing and benchmark tool for [libcr](https://github.com/sm2coin/libcr "libcr on Github").

## License

libcr-test is released under the GNU Affero General Public License, version 3 or newer (AGPLv3+).


## Installing

You need to have CMake installed.
Simply clone the repository, and execute:
	
	cmake .

You can also pass `-DRELEASE=ON` or `-DRELEASE=OFF` to build for release (or debug, respectively).
Pass `-DCOMPACT_IP=ON` (or OFF) to control the compact instruction pointer feature.