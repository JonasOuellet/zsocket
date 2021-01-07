# zsocket
Simple dll for zbrush to send command to Maya or other DCC through socket communication.

Compile with visual studio.

Open a port in Maya

```python
cmds.commandPort(name="127.0.0.1:6001", sourceType='python')
```

Call the dll with zscript to send command to Maya.

```python
[RoutineCall,CheckSystem]
[MemCreate, output, 256, 0]

[VarSet, err, [FileExecute,[Var,dllPath],"SocketSend", "cmds.createNode('transform')",,,output]]
[VarSet, outStr, ""]
[MemReadString, output, outStr]
[MemDelete, output]
[If, err,
  [Note, [StrMerge, "Error Occured: ", outStr, err]]
,
  [Note, outStr]
]
```

See `Test_zscript.txt` for more info
