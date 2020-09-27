# PA2
### Contact information
 - Name: Leyen Qian</br>
 - Email: jiqi2811@colorado.edu</br>
 - CU Identity: 108384916</br>

 ### Files Description
 - lakka_hello_module.c (./hello/lakka_hello_module.c)</br>
 the implementation of simple loadable kernel module (for part 1)

 - lakka_character_module.c ( ./character/lakka_character_module.c )</br>
 the implementation of character device driver (for part 2)

 - test.c (./test/test.c)
 the implementation of test program (for part 3)

 ### Building Instructions (for loadable kernel modules)
 - There are Makefile config files in both character and hello module.</br>
 - Change the current working directory to character or hello.</br>
 - Then type \<make all\> for building or \<make clean\> for cleaning.</br>
 - We shall get corresponding *.ko file for kernel module installing.</br>

 ### Building Instructions (for test program)
 - Change the current working directory to test.</br>
 - Then type \<gcc test.c\>.</br>
 - We shall get an executable file called a.out</br>

 ### Installing Instructions (for character module)
 - cd ./character 
 - sudo su
 - make clean
 - make all
 - insmod lakka_hello_module.ko
 - mknod -m 777 /dev/lakka_character_device_file c 440 0</br>
 Please check the final major and minor number through "dmesg".</br>
 Numbers may change due to conflict, default are 440 and 0.

### Running Instructions (for test program)
 - cd ./test
 - ./a.out
 - type "?" for showing support commands, sample code as following:
 - w | 22 | hello_from_user_space</br>
 write "hello_from_user_space" to device buffer with the buff size of 22 bytes.
 - s | 10 | 0</br>
 set the device file IO offset to 10 (bytes) relative to the beginning of the device buffer.
 - r | 21</br>
 read content from device buffer with the buff size of 21 bytes.