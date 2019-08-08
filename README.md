# Trojan

Simple trojan program

When you launch it, it copies itself to /bin/ then launch this copy and exit
The copy transform itself into a daemon and listen on a port (default 4242) and add a cron file that start this copy at OS launch

You can connect to this port with netcat and the daemon will ask of you a password to connect (sha3 verification) contained in the credential file
Once you are in you can create a remote shell with root permissions (type help to see commands)

The daemon also keylog when it can, it opens the keyboard device and reads from it (does not work on ssh connexions) and split it into 3 keylog files with each its own particularity


**System**

Linux only

### To start
```
make && sudo ./Durex
```

### To remove
```
nc localhost 4242

then:
type passwd
(for password)

then:
type remove
type stop
```

It will remove every file created and stop the current daemon
