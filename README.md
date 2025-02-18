# echo-server

Only notable thing is I wrote this using the [ed](https://www.gnu.org/software/ed/) text editor.

Other than that its a normal echo server that echos a message back to its client. Connects to port `8080`.
## Building 

```
make all
```

## TODO

1. [X] Implement Basic echo functionality on both client and server
2. [ ] Concurrently handle multiple clients with multi-threading
3. [ ] Use semaphore to assign to threads to track maximum client connections
