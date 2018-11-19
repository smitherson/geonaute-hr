## Geonaute bluetooth heart rate monitor
So I bouth a heart rate monitor for running and it has bluetooth so I'm bound to play with it.

###### The goals of this project so far
- play with bluetooth 
- play with heart rate data

###### Current state of the project
- For now it just reads the data, if you have such a monitor - feel free do download and play with it as well.

###### Future plans
- rewrite the reading in Python, for educational purposes
- create a good format for saving and tagging the data
- visualise the data
- add scanning so anyone can easily add their HR monitor, right now you have to change the physical address in the code

Running it:
Note you have to change the physical address used in `gattlib_connect`
```
git clone git@github.com:smitherson/geonaute-hr.git
cd geonaute-hr
mkdir build
cd build
cmake ..
make
./read_write
```
