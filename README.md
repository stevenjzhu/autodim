# autodim
Simple daemon to adjust brightness base on whether a power supply is connected

# dependencies
Currently depends on xbacklight for functionality, but future versions will ideally include that functionality

# compilation
    make

# running
    ./autodim

# behavior
When started, autodim will save you current brightness.  When a power supply is connected, the saved brightness will be restored.  On the other hand, autodim will save current brightness and adjust screen brightness to 30% when a power supply is disconnected.
