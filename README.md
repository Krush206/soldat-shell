# Soldat Shell
Shell (Bash) scripts for Soldat dedicated servers.
#### How to use
Change the password inside the scripts, change their execution permissions and run the following:
`./rcon_announce | nc <server ip> <port> > /dev/null &`
`./rcon_commands | nc <server ip> <port> | dos2unix >> /tmp/cmds &`
`./rcon_kills | nc <server ip> <port> | dos2unix >> /tmp/kill_log &`

It is recommended to have a `tmpfs` mount on `/tmp`.
