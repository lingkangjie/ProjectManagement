# tmux cheatsheet

# tmux and vim config
If you install [install gpakosz tmux configuration](https://github.com/gpakosz/.tmux#enabling-the-powerline-look), and want to enable vim copy
please two commands to .tmux.config

```console
set -g status-keys vi
set -g mode-keys vi
```
<ctrl> + b, and 'r' to reload configuration

# tmux Basics
1. start new:

    ```console
    $ tmux
    ```

2. start new with session name:

    ```console
    $ tmux new -s myname
    ```

3. attach to named:

    ```console
    $ tmux a -t myname
    ```

4. list sessions:

    ```console
    $ tmux ls
    ```
5. kill session:

    ```console
    $ tmux kill-session -t myname
    ```

In tmux, hit the prefix `ctrl+b` and then:
Features comes from [install gpakosz tmux configuration](https://github.com/gpakosz/.tmux#enabling-the-powerline-look) will be noted as (#)

## Sessions

    :new<CR>  new session
    s  list sessions
    $  name session

## Windows (tabs)

    `c`         new window
    ,           name window
    w           list windows
    f           find window
    &           kill window
    -          splits the current windows vertically (#)
    _           splits the current 
    .           move window - prompted for a new number
    :movew<CR>  move window to the next unused number
    - `<prefix> C-h` and `<prefix> C-l` let you navigate windows (default
    - `<prefix> Tab` brings you to the last active window

## Panes (splits)

    %  horizontal split
    "  vertical split
    
    o  swap panes
    q  show pane numbers
    x  kill pane
    h, j, k, l goes to left, bottom, up, and right pane
    H, J, K, L move pane left, bottom, up and right a bit
    ⍽  space - toggle between layouts

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

Notes:

* You can cmd+click URLs to open in iTerm.

TODO:

* Conf copy mode to use system clipboard. See PragProg book.
