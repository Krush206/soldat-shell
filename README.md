# Soldat C Shell
C Shell scripts for Soldat dedicated servers.

#### Scripts descriptions
*rcon_kills*: Count the kills for each player and display a message after each defined row has been reached.\
*rcon_announce*: Periodically display a set of three messages after five minutes + two other messages, after 30 seconds, counted from the five minutes.\
*rcon_commands*: Execute commands requested by a player; currently, the commands are: !date (*date* Unix command), !info (*uname -a* Unix command), !script (script information; credits), !red or !1 (change the player's team to Alpha), !blue or !2 (change the player's team to Bravo), !spec or !5 (spectate; become a spectator).\
*rcon_balancer*: Check for inconsistency in Alpha and Bravo teams and automatically switch the team of the last player who joined.

#### How to use
Install [socat](http://www.dest-unreach.org/socat), change the password inside the scripts, change their execution permissions and run the following:\
`./rcon_announce | nc <server ip> <port> > /dev/null &`\
`socat exec:"./rcon_commands" tcp:<server ip>:<port>,crnl &`\
`socat exec:"./rcon_kills" tcp:<server ip>:<port>,crnl &`\
`socat exec:"./rcon_balancer" tcp:<server ip>:<port>,crnl &`

#### Notes
The scripts were made in FreeBSD. *BSD programs are different than these from GNU/Linux systems.

Windows users may use [Cygwin](https://cygwin.com) to run the scripts (but they haven't been tested on this platform; testing is required).

On Unix and Unix-like operating systems, it is recommended to have a `tmpfs` mount on `/tmp`.
