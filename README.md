# dumps all available keyevents to csv

It's like getevent -lp, but dumps the output as CSV 

Tested against some Android Emulators and Kali Linux on Virtual Box. 

## Compile it and use it 

```sh
g++ -std=c++2a -O3 -g0 ./newgetevent.cpp &&  ./a.out > keyeventskaliexample.txt
```



