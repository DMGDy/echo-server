# echo-server

Only notable thing is I wrote this using the [ed](https://www.gnu.org/software/ed/) text editor.

Other than that its a normal echo server that echos a message back to its client. Connects to port `8080`.

Also uses `pthreads` for multithreading to connect to multiple clients.
Places a `mutex` lock on a shared counter that counts how many
messages are received from all the clients across different threads.
Semaphores used to only allow 8 seperate connections to clients.


# Building 

```
make all
```

## TODO

1. [X] Implement Basic echo functionality on both client and server
2. [X] Concurrently handle multiple clients with multi-threading
3. [X] Use semaphore to assign to threads to track maximum client connections
