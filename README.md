# Soldat Shell
Shell (Bash) scripts for Soldat dedicated servers.

#### Scripts descriptions
*rcon_kills*: Count the kills for each player and display a message after each defined row has been reached.\
*rcon_announce*: Periodically display a set of three messages after five minutes + two other messages, after 30 seconds, counted from the five minutes.\
*rcon_commands*: Execute commands requested by a player; currently, the commands are: !date (*date* Unix command), !info (*uname -a* Unix command), !script (script information; credits), !red or !1 (change the player's team to Alpha), !blue or !2 (change the player's team to Bravo), !spec or !5 (spectate; become a spectator).\
*rcon_balancer*: Check for inconsistency in Alpha and Bravo teams and automatically switch the team of the last player who joined.

#### How to use
Install [socat](http://www.dest-unreach.org/socat), change the password inside the scripts, change their execution permissions and run the following:\
`./rcon_announce | nc <server ip> <port> > /dev/null &`\
`socat exec:"./rcon_commands" system:"nc <server ip> <port> | dos2unix" &`\
`socat exec:"./rcon_kills" system:"nc <server ip> <port> | dos2unix" &`\
`socat exec:"./rcon_balancer" system:"nc <server ip> <port> | dos2unix" &`

#### Notes
Since I made them on GNU/Linux, it is unlikely that Unix or other Unix-like operating systems (including macOS) be able to run them, because GNU programs tend to be and act different than these found in Unix and other Unix-like operating systems. However, *BSD and illumos users may already have or can install GNU programs, so they just need to adapt their environment.

Windows users may use [Cygwin](https://cygwin.com) to run the scripts (but they haven't been tested on this platform; testing is required).

On Unix and Unix-like operating systems, it is recommended to have a `tmpfs` mount on `/tmp`.
