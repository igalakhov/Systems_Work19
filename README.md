# Systems_Work19 - Nevertheless, She Persisted

## Directions:

Starting with working handshake code from the previous assignment:
  - ***COPY*** your files, do not modify the files in your current handshake repository.
  - Modify there server and client programs:
    - Client
      1. Handshake
      2. Prompt user for input
      3. Send input to server.
      4. Get response from server and display it to user
      5. Repeat steps 2-4 until interrupted (ctrl-c).
    - Server
      1. Handshake
      2. Get data from client
      3. "Process" data - do something interesting to it
      4. Send processed data back to client
      5. Repeat 2-4 until the client exits
         1. When the client exits, the pipes will close automatically
      6. Reset for a new handshake
      7. Include a signal handler for SIGINT that will remove the WKP
