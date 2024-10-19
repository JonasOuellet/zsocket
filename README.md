# zsocket
Simple dll for zbrush to send command to Maya or other DCC through socket communication.


## Build

Build we cmake using visual studio build tools.

```
cmake -Bbuild
cmake --build build --config Release
```

## Test

To test the dll, open ZBrush and load the `Test_zscript.txt` zscript.  Make sure to open a command port either in maya or python.  Click on the `SocketSend` buttont to send a message.

* Open a port in Maya
    ```python
    cmds.commandPort(name="127.0.0.1:6001", sourceType='python')
    ```
* Start python socket server
    ```bash
    python open_test_socket.py
    ```
