

## 调优

### 开启profile
```bash
./allocsvr_tool_main -c allocsvr_client.conf -f PHXEcho -s start_profile
```

### 关闭profile
```
./allocsvr_tool_main -c allocsvr_client.conf -f PHXEcho -s end_profile
```

### 查看profile日志

在文本模式下查看， 包括调用堆栈信息
```
/data/install/baserpc/build/bin/pprof --text --stacks ./allocsvr_main profile_allocsvr_main.log
```

```
_/data/install/baserpc/build/bin/pprof --text_  --base=./SectSeqTest.base.heap ./allocSvrTest ./SectSeqTest.0001.heap
```

### 查看二进制
以uint64_t查内容
od -t -u8 max_seq.bin