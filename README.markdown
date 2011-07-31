goldeen
=======

goldeen is an implementation of Bruce Schneier's [Blowfish algorithm][0], a 64-bit block cipher designed in 1994.
You can find all the details about the way the encryption is done in the previous link.

Installation
------------

It is pretty simple to get goldeen up and running. All it takes to build is

	$ make

And optionally run the random test cases:

	$ make test

If all tests pass, you can install it typing:

	# make install

Usage
-----

	$ goldeen [actions] [options]

### Actions

Two basic functions:

* `encrypt`		-		encrypt the input with the Blowfish algorithm
* `decrypt`		- 		decrypt the input with the Blowfish algorithm

### Options

* `-h` | `--help`					-		prints help message and finishes
* `-i` | `--input` `<inputfile>`	-		takes `inputfile` as the content to be encrypted/decrypted. Otherwise, you will be prompted.
* `-o` | `--output` `<outputfile>`	-		writes the output of the algorithm to `<outputfile>`. Otherwise, `stdout` will be used.
* `-k` | `--key-file` `<keyfile>`	-		If this option is passed, goldeen will use the content of `<keyfile>` as the key to the algorithm (56 bytes at most). 
											If the file is bigger, only the first 56 bytes will be considered

Caveats
-------

* goldeen acts the same way for both little endian and big endian architectures. I have no time to work on this right now, so if you want help, feel free to send a pull request. :-)
 
License
-------

goldeen is licensed under the GNU Public License (version 3). For more details, refer to the LICENSE file or contact me at renato.mascosta@gmail.com

Author
------

Renato Mascarenhas - [@rmascarenhasc][1]

[0]: http://www.schneier.com/paper-blowfish-fse.html
[1]: http://twitter.com/rmascarenhasc
