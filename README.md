# Oberon Ceres Disk Utilities
This repository contains tools to list, extract, and convert files in an original harddisk dump of an [ETHZ Oberon Ceres Machine](https://people.inf.ethz.ch/wirth/).

Currently, one tool is implemented: **obdisk** for listing and extracting files.

I plan to implement a further tool to convert text files from the proprietary Ceres rich text format to standard ASCII.

## Compiling And Installation
1. Download the .tar.gz packages from the "[Releases](https://github.com/good-sushi/obdisk/releases)" page.
2. Extract and build:
    ```
    $ tar xzf obdisk-x.y.tar.gz
    $ cd obdisk-x.y
    $ ./configure
    $ make && make install
    ```

## Getting A Ceres Dump File For Testing
A reference original dump file can be found here: http://www.bitsavers.org/bits/eth/Ceres/ceres_xt2190.zip

The archive contains three files. The **ceres_xt2190.dat** file is the actual dump file to use.

## Usage
### obdisk
List and extract files on a Ceres harddisk dump.

```
USAGE: obdisk [-lxhfvV] [-d dest_dir] image_file [file_arg]

-l	List directory of image_file
	If file_arg is omitted: list all entries
	otherwise, list files matching regex in file_arg

-x	Extract files on image_file

-d	Extract into destination dir 'dest_dir' (must already exist)

-f	Brute force mode (ignores disk structure; use with caution)

-v	Verbose output

-h	Show this help information

-V	Show version information

image_file is the filename of a disk image of an
original Oberon Ceres machine.
```
#### Examples

* ```obdisk -l ceres.dat```

  List all files in the disk image **ceres.dat**

* ```obdisk -l ceres.dat '*.Text'```

  List all files matching the specified wild card pattern `*.Text`.<br> 
  *(note the single quotes to avoid shell expansion)*.

* ```obdisk -lf ceres.dat````

  List all files in the disk image **ceres.dat** using brute force mode.
  
* ```obdisk -xv ceres.dat```
 
  Extract *all* files in the disk image into the *current directory* and display progress.

* ```obdisk -xvf ceres.dat````

  Same as above, but extract in brute force mode. Duplicate files are only exported once
  (i.e. the last instance found overwrites previous ones with the same name).
  
* ```obdisk -x -d ../path/filedir ceres.dat 'Write.*'```

  Extract all files matching the wildcard search pattern `Write.*` into the destination directory `../path/filedir`.<br>
  *(note: the specified directory must already exist)*.

#### Notes On "Brute Force" Mode

In "normal" mode (i.e. without the `-f` option), **obdisk** expects to find an intact directory structure and will take the files to be listed/extracted from there.

If normal mode produces unexpected results (e.g. no files found, or error messages about missing file headers), running with the `-f` option will scan the entire disk image for sectors which look like directory entries, and will try to find files there.

Please be aware that the files found by brute force may have been corrupted/truncated/deleted etc. on the original Ceres disk. It is recommended to check and validate the extracted files manually.
  
## References
Portions of this code have been derived from the official [Project Oberon](https://github.com/Project-Oberon/Source-Code/tree/main/CERES%20Oberon%20V4) source code and documentation, in particular from the logical filesystem implementation in the `FileDir` and `Files` modules.

## License
This code is made available under GPL v3.
