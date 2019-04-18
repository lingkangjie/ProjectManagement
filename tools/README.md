# tmux cheatsheet

# tmux and vim config
If you has installed [gpakosz tmux configuration](https://github.com/gpakosz/.tmux#enabling-the-powerline-look), and want to enable vim copy
please two commands to `~/.tmux.conf.local`

## gpakosz tmux configuration install
```
$ cd
$ git clone https://github.com/gpakosz/.tmux.git
$ ln -s -f .tmux/.tmux.conf
$ cp .tmux/.tmux.conf.local .
```
If you want to use `Ctrl + a` in vim, not in tmux, please command two lines in:  
```
$ vim ~/.tmux.conf
#set -g prefix2 C-a                        # GNU-Screen compatible prefix
#bind C-a send-prefix -2
```
  

## tmux setting to enable vim
```console
set -g status-keys vi
set -g mode-keys vi
```
`<prefix>` means you have to either hit <kbd>Ctrl</kbd> + <kbd>a</kbd> or <kbd>Ctrl</kbd> + <kbd>b</kbd>
Using `<prefix> r`, to reload configuration

# tmux Basics
1. start new:

    ```console
    $ tmux
    ```

2. start new with session name:

    ```console
    $ tmux new -s my-session-name
    $ :new -s my-session-name
    ```

3. attach to named:

    ```console
    $ tmux a -t myname
    ```

4. list sessions:

    ```console
    $ tmux ls
    ```
5. kill myname session:

    ```console
    $ tmux kill-session -t myname
    $ tmux kill-session -a # kill all sessions but the current
    $ tmux kill-session -a -t myname # kill all sessions buti *myname*
    ```
6. attach to last session
    ```
    $ tmux a
    $ tmux a -t mysession # attach to last session
    ```
In tmux, hit the prefix `ctrl+b` and then:
Features comes from [install gpakosz tmux configuration](https://github.com/gpakosz/.tmux#enabling-the-powerline-look) will be noted as (#)

## Sessions

    :new<CR>  new session
    s  list sessions
    $  name session

## Windows (tabs)

    c           new window
    ,           name window
    w           list windows
    p           previous window
    n           next window
    f           find window
    0,1,...,9   switch window by number
    &           close current window
    -           splits the current windows vertically (#)
    _           splits the current 
    .           move window - prompted for a new number
    :movew<CR>  move window to the next unused number
    `<prefix> C-h` and `<prefix> C-l` let you navigate windows (default
    `<prefix> Tab` brings you to the last active window

## Panes (splits)

    %  horizontal split
    "  vertical split
    
    o  swap panes
    q  show pane numbers
    x  kill pane
    h, j, k, l goes to left, bottom, up, and right pane
    H, J, K, L move pane left, bottom, up and right a bit
    ⍽  space - toggle between layouts
## copy mode

    - `<prefix> Enter` enters copy-mode
    - `<prefix> b` lists the paste-buffers
    - `<prefix> p` pastes from the top paste-buffer
    - `<prefix> P` lets you choose the paste-buffer to paste from

    Bindings for `copy-mode-vi`:

    - `v` begins selection / visual mode
    - `C-v` toggles between blockwise visual mode and visual mode
    - `H` jumps to the start of line
    - `L` jumps to the end of line
    - `y` copies the selection to the top paste-buffer
    - `Escape` cancels the current operation

## Window/pane surgery

    :joinp -s :2<CR>  move window 2 into a new pane in the current window
    :joinp -t :1<CR>  move the current pane into a new pane in window 1

* [Move window to pane](http://unix.stackexchange.com/questions/14300/tmux-move-window-to-pane)
* [How to reorder windows](http://superuser.com/questions/343572/tmux-how-do-i-reorder-my-windows)

## Misc

    d  detach
    t  big clock
    ?  list shortcuts
    :  prompt

Resources:

* [cheat sheet](http://cheat.errtheblog.com/s/tmux/)
* [tmux cheat](https://tmuxcheatsheet.com/?q=&hPP=100&idx=tmux_cheats&p=0&is_v=1)

Notes:

* You can cmd+click URLs to open in iTerm.

TODO:

* Conf copy mode to use system clipboard. See PragProg book.
